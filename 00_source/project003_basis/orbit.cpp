//============================================================
//
//	�O�Տ��� [orbit.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "orbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "scene.h"
#include "sceneGame.h"
#include "pause.h"

//************************************************************
//	�q�N���X [COrbit] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
COrbit::COrbit() : CObject(LABEL_NONE, DIM_3D, object::DEFAULT_PRIO),
	m_pVtxBuff		(nullptr),		// ���_�o�b�t�@
	m_pRenderState	(nullptr),		// �����_�[�X�e�[�g�̏��
	m_state			(STATE_NORMAL),	// ���
	m_nNumVtx		(0),			// �K�v���_��
	m_nCounterState	(0),			// ��ԊǗ��J�E���^�[
	m_nTextureID	(0)				// �e�N�X�`���C���f�b�N�X
{
	// �����o�ϐ����N���A
	memset(&m_orbit, 0, sizeof(m_orbit));	// �O�Ղ̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
COrbit::~COrbit()
{

}

//============================================================
//	����������
//============================================================
HRESULT COrbit::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff		= nullptr;		// ���_�o�b�t�@
	m_pRenderState	= nullptr;		// �����_�[�X�e�[�g�̏��
	m_state			= STATE_NORMAL;	// ���
	m_nNumVtx		= 0;			// �K�v���_��
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nTextureID	= NONE_IDX;		// �e�N�X�`���C���f�b�N�X

	// �O�Ղ̏���������
	m_orbit.offset = SOffset(VEC3_ZERO, VEC3_ZERO, XCOL_WHITE);	// �I�t�Z�b�g���
	D3DXMatrixIdentity(&m_orbit.mtxVanish);			// �����J�n���̐e�̃}�g���b�N�X
	D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[0]);	// ���[�̃��[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[1]);	// ���[�̃��[���h�}�g���b�N�X
	m_orbit.pMtxParent	= nullptr;	// �e�̃}�g���b�N�X
	m_orbit.pPosPoint	= nullptr;	// �e���_���W
	m_orbit.pColPoint	= nullptr;	// �e���_�J���[
	m_orbit.nPart		= 1;		// ������
	m_orbit.nTexPart	= 1;		// �e�N�X�`��������
	m_orbit.bAlpha		= false;	// ��������
	m_orbit.bInit		= false;	// ��������

	// ������ݒ�
	if (FAILED(SetLength(1)))
	{ // �����̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̐���
	m_pRenderState = CRenderState::Create();
	if (m_pRenderState == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}
	
	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�

	// �|���S���̗��ʂ�\����Ԃɂ���
	pRenderState->SetCulling(D3DCULL_NONE);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void COrbit::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// �e���_���W�̔j��
	SAFE_DEL_ARRAY(m_orbit.pPosPoint);

	// �e���_�J���[�̔j��
	SAFE_DEL_ARRAY(m_orbit.pColPoint);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void COrbit::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void COrbit::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxIdent;	// �P�ʃ}�g���b�N�X�ݒ�p
	D3DXMATRIX mtxParent;	// �e�̃}�g���b�N�X
	bool bUpdate = true;	// �X�V��

	// �P�ʃ}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxIdent);

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	if (GET_MANAGER->GetScene()->GetMode() == CScene::MODE_GAME)
	{ // ���[�h���Q�[���̏ꍇ

		if (CSceneGame::GetPause()->IsPause())
		{ // �|�[�Y���̏ꍇ

			// �X�V���Ȃ��󋵂ɂ���
			bUpdate = false;
		}
	}
	
	if (m_state != STATE_NONE)
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		if (bUpdate)
		{ // �X�V����󋵂̏ꍇ

			//------------------------------------------------
			//	��Ԃ��Ƃ̐ݒ�
			//------------------------------------------------
			switch (m_state)
			{ // ��Ԃ��Ƃ̏���
			case STATE_NORMAL:	// �ʏ���

				// �e�}�g���b�N�X��ݒ�
				mtxParent = *m_orbit.pMtxParent;

				break;

			case STATE_VANISH:	// �������

				// �e�}�g���b�N�X��ݒ�
				mtxParent = m_orbit.mtxVanish;

				// �J�E���^�[�����Z
				if (m_nCounterState < (m_nNumVtx / orbit::MAX_OFFSET) + 1)
				{ // �J�E���^�[���O�Ղ��L�ѐ؂鎞�Ԃ�菬�����ꍇ

					// �J�E���^�[�����Z
					m_nCounterState++;
				}
				else
				{ // �J�E���^�[���O�Ղ��L�ѐ؂鎞�Ԉȏ�̏ꍇ

					// �J�E���^�[��␳
					m_nCounterState = 0;

					// ��Ԃ�ݒ�
					m_state = STATE_NONE;	// �������Ȃ����
				}

				break;

			default:	// ��O���
				assert(false);
				break;
			}

			//------------------------------------------------
			//	�I�t�Z�b�g�̏�����
			//------------------------------------------------
			for (int nCntOff = 0; nCntOff < orbit::MAX_OFFSET; nCntOff++)
			{ // �I�t�Z�b�g�̐����J��Ԃ�

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[nCntOff]);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&m_orbit.aMtxWorldPoint[nCntOff], m_orbit.offset.aOffset[nCntOff].x, m_orbit.offset.aOffset[nCntOff].y, m_orbit.offset.aOffset[nCntOff].z);

				// �e�̃}�g���b�N�X�Ɗ|�����킹��
				D3DXMatrixMultiply(&m_orbit.aMtxWorldPoint[nCntOff], &m_orbit.aMtxWorldPoint[nCntOff], &mtxParent);
			}

			//------------------------------------------------
			//	���_���W�ƒ��_�J���[�̏������炷
			//------------------------------------------------
			for (int nCntVtx = m_nNumVtx - 1; nCntVtx >= orbit::MAX_OFFSET; nCntVtx--)
			{ // �ێ����钸�_�̍ő吔���J��Ԃ� (�I�t�Z�b�g���͊܂܂Ȃ�)

				// ���_�������炷
				m_orbit.pPosPoint[nCntVtx] = m_orbit.pPosPoint[nCntVtx - orbit::MAX_OFFSET];
				m_orbit.pColPoint[nCntVtx] = m_orbit.pColPoint[nCntVtx - orbit::MAX_OFFSET];
			}

			//------------------------------------------------
			//	�ŐV�̒��_���W�ƒ��_�J���[�̏���ݒ�
			//------------------------------------------------
			for (int nCntOff = 0; nCntOff < orbit::MAX_OFFSET; nCntOff++)
			{ // �I�t�Z�b�g�̐����J��Ԃ�

				// ���_���W�̐ݒ�
				m_orbit.pPosPoint[nCntOff] = D3DXVECTOR3
				( // ����
					m_orbit.aMtxWorldPoint[nCntOff]._41,	// x
					m_orbit.aMtxWorldPoint[nCntOff]._42,	// y
					m_orbit.aMtxWorldPoint[nCntOff]._43		// z
				);

				// ���_�J���[�̐ݒ�
				m_orbit.pColPoint[nCntOff] = m_orbit.offset.aCol[nCntOff];
			}
		}

		//----------------------------------------------------
		//	���_���W�ƒ��_�J���[�̏���������
		//----------------------------------------------------
		if (!m_orbit.bInit)
		{ // �������ς݂ł͂Ȃ��ꍇ

			for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
			{ // �ێ����钸�_�̍ő吔���J��Ԃ�

				// ���_���W�̐ݒ�
				m_orbit.pPosPoint[nCntVtx] = D3DXVECTOR3
				( // ����
					m_orbit.aMtxWorldPoint[nCntVtx % orbit::MAX_OFFSET]._41,	// x
					m_orbit.aMtxWorldPoint[nCntVtx % orbit::MAX_OFFSET]._42,	// y
					m_orbit.aMtxWorldPoint[nCntVtx % orbit::MAX_OFFSET]._43		// z
				);

				// ���_�J���[�̐ݒ�
				m_orbit.pColPoint[nCntVtx] = m_orbit.offset.aCol[nCntVtx % orbit::MAX_OFFSET];
			}

			// �������ς݂ɂ���
			m_orbit.bInit = true;
		}

		//----------------------------------------------------
		//	�|���S���̕`��
		//----------------------------------------------------
		// �����_�[�X�e�[�g��ݒ�
		m_pRenderState->Set();

		// ���_���̐ݒ�
		SetVtx();

		// �P�ʃ}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxIdent);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(object::FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetTexture(m_nTextureID));

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

		// �����_�[�X�e�[�g���Đݒ�
		m_pRenderState->Reset();
	}
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void COrbit::BindTexture(const int nTextureID)
{
	if (nTextureID >= NONE_IDX)
	{ // �e�N�X�`���C���f�b�N�X���g�p�\�ȏꍇ

		// �e�N�X�`���C���f�b�N�X����
		m_nTextureID = nTextureID;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void COrbit::BindTexture(const char *pTexturePass)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	if (pTexturePass != nullptr)
	{ // ���蓖�Ă�e�N�X�`���p�X������ꍇ

		// �e�N�X�`���C���f�b�N�X��ݒ�
		m_nTextureID = pTexture->Regist(pTexturePass);
	}
	else
	{ // ���蓖�Ă�e�N�X�`���p�X���Ȃ��ꍇ

		// �e�N�X�`���Ȃ��C���f�b�N�X��ݒ�
		m_nTextureID = NONE_IDX;
	}
}

//============================================================
//	��Ԏ擾����
//============================================================
int COrbit::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
COrbit *COrbit::Create
(
	D3DXMATRIX *pMtxParent,				// �e�}�g���b�N�X
	const SOffset offset,				// �I�t�Z�b�g���
	const int nPart,					// ������
	const int nTexPart,					// �e�N�X�`��������
	const bool bAlpha,					// ��������
	const CRenderState::EBlend blend	// ���u�����h��
)
{
	// �O�Ղ̐���
	COrbit *pOrbit = new COrbit;
	if (pOrbit == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �O�Ղ̏�����
		if (FAILED(pOrbit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �O�Ղ̔j��
			SAFE_DELETE(pOrbit);
			return nullptr;
		}

		// �e�̃}�g���b�N�X��ݒ�
		pOrbit->SetMatrixParent(pMtxParent);

		// �I�t�Z�b�g����ݒ�
		pOrbit->SetOffset(offset);

		// �e�N�X�`����������ݒ�
		pOrbit->SetTexPart(nTexPart);

		// �������󋵂�ݒ�
		pOrbit->SetEnableAlpha(bAlpha);

		// ���u�����h��ݒ�
		pOrbit->SetAlphaBlend(blend);

		// ������ݒ�
		if (FAILED(pOrbit->SetLength(nPart)))
		{ // �����̐ݒ�Ɏ��s�����ꍇ

			// �O�Ղ̔j��
			SAFE_DELETE(pOrbit);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pOrbit;
	}
}

//============================================================
//	�e�}�g���b�N�X�폜����
//============================================================
void COrbit::DeleteMatrixParent(void)
{
	// ������Ԃɂ���
	SetState(STATE_VANISH);

	// �e�}�g���b�N�X��nullptr�ɂ���
	m_orbit.pMtxParent = nullptr;
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void COrbit::SetState(const EState state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // �ݒ肷���Ԃ����݂̏�Ԋ��A�ݒ肷���Ԃ��ʏ��Ԃ̏ꍇ

		// �����𔲂���
		return;
	}

	// �����̏�Ԃ�ݒ�
	m_state = state;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_NORMAL:	// �ʏ���

		// ���������Ă��Ȃ���Ԃɂ���
		m_orbit.bInit = false;

		break;

	case STATE_VANISH:	// �������

		// ���݂̐e�}�g���b�N�X����������}�g���b�N�X�ɐݒ�
		m_orbit.mtxVanish = *m_orbit.pMtxParent;

		break;

	default:	// ��O���
		assert(false);
		break;
	}
}

//============================================================
//	�e�̃}�g���b�N�X�̐ݒ菈��
//============================================================
void COrbit::SetMatrixParent(D3DXMATRIX *pMtxParent)
{
	// �����̐e�}�g���b�N�X��ݒ�
	m_orbit.pMtxParent = pMtxParent;
}

//============================================================
//	�I�t�Z�b�g���̐ݒ菈��
//============================================================
void COrbit::SetOffset(const SOffset offset)
{
	// �����̃I�t�Z�b�g����ݒ�
	m_orbit.offset = offset;
}

//============================================================
//	�e�N�X�`���������̐ݒ菈��
//============================================================
void COrbit::SetTexPart(const int nTexPart)
{
	// ��O����
	assert(nTexPart > 0);	// 0�ȉ��G���[

	// �����̃e�N�X�`����������ݒ�
	m_orbit.nTexPart = nTexPart;
}

//============================================================
//	���u�����h�̐ݒ菈��
//============================================================
void COrbit::SetAlphaBlend(const CRenderState::EBlend blend)
{
	// �����̃��u�����h��ݒ�
	m_orbit.blend = blend;

	// ���u�����h��ύX
	GetRenderState()->SetAlphaBlend(blend);
}

//============================================================
//	�������󋵂̐ݒ菈��
//============================================================
void COrbit::SetEnableAlpha(const bool bAlpha)
{
	// �����̓������󋵂�ݒ�
	m_orbit.bAlpha = bAlpha;
}

//============================================================
//	�������󋵂̐ݒ菈��
//============================================================
void COrbit::SetEnableInit(const bool bInit)
{
	// �����̏������󋵂�ݒ�
	m_orbit.bInit = bInit;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
HRESULT COrbit::SetLength(const int nPart)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �K�v���_�������߂�
	m_nNumVtx = nPart * orbit::MAX_OFFSET;

	//--------------------------------------------------------
	//	�e���_���W�̔j���E����
	//--------------------------------------------------------
	// �e���_���W�̔j��
	SAFE_DEL_ARRAY(m_orbit.pPosPoint);
	assert(m_orbit.pPosPoint == nullptr);

	// �e���_���W�̃������m��
	m_orbit.pPosPoint = new D3DXVECTOR3[m_nNumVtx];
	if (m_orbit.pPosPoint != nullptr)
	{ // �m�ۂɐ��������ꍇ

		// �������N���A
		memset(m_orbit.pPosPoint, 0, sizeof(D3DXVECTOR3) * m_nNumVtx);
	}
	else
	{ // �m�ۂɎ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�e���_�J���[�̔j���E����
	//--------------------------------------------------------
	// �e���_�J���[�̔j��
	SAFE_DEL_ARRAY(m_orbit.pColPoint);
	assert(m_orbit.pColPoint == nullptr);

	// �e���_�J���[�̃������m��
	m_orbit.pColPoint = new D3DXCOLOR[m_nNumVtx];
	if (m_orbit.pColPoint != nullptr)
	{ // �m�ۂɐ��������ꍇ

		// �������N���A
		memset(m_orbit.pColPoint, 0, sizeof(D3DXCOLOR) * m_nNumVtx);
	}
	else
	{ // �m�ۂɎ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	���_�o�b�t�@�̔j���E����
	//--------------------------------------------------------
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);
	assert(m_pVtxBuff == nullptr);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * m_nNumVtx,	// �K�v���_��
		D3DUSAGE_WRITEONLY,		// �g�p���@
		object::FVF_VERTEX_3D,	// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// �������̎w��
		&m_pVtxBuff,			// ���_�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_���̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *COrbit::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void COrbit::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �O�Ղ̒��_�o�b�t�@���g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
		{ // �ێ����钸�_�̍ő吔���J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = m_orbit.pPosPoint[nCntVtx];

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = VEC3_ZERO;

			// ���_�J���[�̐ݒ�
			if (m_orbit.bAlpha)
			{ // �����ɂ��Ă����ꍇ

				pVtx[0].col = D3DXCOLOR
				( // ����
					m_orbit.pColPoint[nCntVtx].r,	// r
					m_orbit.pColPoint[nCntVtx].g,	// g
					m_orbit.pColPoint[nCntVtx].b,	// b
					m_orbit.pColPoint[nCntVtx].a - (m_orbit.pColPoint[nCntVtx].a / (m_nNumVtx * 0.5f)) * (nCntVtx / 2)	// a
				);
			}
			else
			{ // �����x��ς��Ȃ��ꍇ

				pVtx[0].col = m_orbit.pColPoint[nCntVtx];
			}

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			( // ����
				(1.0f / (float)m_orbit.nTexPart) * (nCntVtx / 2),	// u
				1.0f * -((nCntVtx % 2) - 1)							// v
			);

			// ���_�f�[�^�̃|�C���^��i�߂�
			pVtx += 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�j������
//============================================================
void COrbit::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//
//	�I�u�W�F�N�g���b�V�������O���� [objectMeshRing.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshRing.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float CALC_TEXDIR[] = { -1.0f, 1.0f };	// �e�N�X�`�������̌v�Z�W��
	const POSGRID2 MIN_PART		= GRID2_ONE;		// �������̍ŏ��l
	const POSGRID2 MIN_TEXPART	= GRID2_ONE;		// �e�N�X�`���������̍ŏ��l
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(CALC_TEXDIR) == CObjectMeshRing::TEXDIR_MAX, "ERROR : Array Count Mismatch");

//************************************************************
//	�q�N���X [CObjectMeshRing] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectMeshRing::CObjectMeshRing(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pVtxBuff		(nullptr),		// ���_�o�b�t�@
	m_pIdxBuff		(nullptr),		// �C���f�b�N�X�o�b�t�@
	m_pRenderState	(nullptr),		// �����_�[�X�e�[�g�̏��
	m_part			(GRID2_ZERO),	// ������
	m_texPart		(GRID2_ZERO),	// �e�N�X�`��������
	m_nNumVtx		(0),			// �K�v���_��
	m_nNumIdx		(0),			// �K�v�C���f�b�N�X��
	m_nTextureID	(0)				// �e�N�X�`���C���f�b�N�X
{
	// �����o�ϐ����N���A
	memset(&m_meshRing, 0, sizeof(m_meshRing));	// ���b�V�������O�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectMeshRing::~CObjectMeshRing()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectMeshRing::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff		= nullptr;		// ���_�o�b�t�@
	m_pIdxBuff		= nullptr;		// �C���f�b�N�X�o�b�t�@
	m_pRenderState	= nullptr;		// �����_�[�X�e�[�g�̏��
	m_part			= MIN_PART;		// ������
	m_texPart		= GRID2_ONE;	// �e�N�X�`��������
	m_nNumVtx		= 0;			// �K�v���_��
	m_nNumIdx		= 0;			// �K�v�C���f�b�N�X��
	m_nTextureID	= NONE_IDX;		// �e�N�X�`���C���f�b�N�X

	m_meshRing.pos			= VEC3_ZERO;		// �ʒu
	m_meshRing.rot			= VEC3_ZERO;		// ����
	m_meshRing.col			= XCOL_WHITE;		// �F
	m_meshRing.texDir		= TEXDIR_OUTSIDE;	// �e�N�X�`������
	m_meshRing.fHoleRadius	= 0.0f;				// ���̔��a
	m_meshRing.fThickness	= 0.0f;				// ����
	m_meshRing.fOuterPlusY	= 0.0f;				// �O����Y���W���Z��

	// ��������ݒ�
	if (FAILED(SetPattern(MIN_PART)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

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

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectMeshRing::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	SAFE_RELEASE(m_pIdxBuff);

	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// �I�u�W�F�N�g���b�V�������O��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectMeshRing::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectMeshRing::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����_�[�X�e�[�g��ݒ�
	m_pRenderState->Set();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshRing.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshRing.rot.y, m_meshRing.rot.x, m_meshRing.rot.z);
	D3DXMatrixMultiply(&m_meshRing.mtxWorld, &m_meshRing.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshRing.pos.x, m_meshRing.pos.y, m_meshRing.pos.z);
	D3DXMatrixMultiply(&m_meshRing.mtxWorld, &m_meshRing.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshRing.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(object::FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive
	( // ����
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nNumVtx,		// �g�p���钸�_��
		0,				// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
		m_nNumIdx - 2	// �v���~�e�B�u (�|���S��) ��
	);

	// �����_�[�X�e�[�g���Đݒ�
	m_pRenderState->Reset();
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectMeshRing::BindTexture(const int nTextureID)
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
void CObjectMeshRing::BindTexture(const char *pTexturePass)
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
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectMeshRing::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshRing.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectMeshRing::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_meshRing.pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshRing::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshRing.rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_meshRing.rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectMeshRing::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_meshRing.rot;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectMeshRing::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_meshRing.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectMeshRing::GetColor(void) const
{
	// �F��Ԃ�
	return m_meshRing.col;
}

//============================================================
//	��������
//============================================================
CObjectMeshRing *CObjectMeshRing::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart,	// �e�N�X�`��������
	const float fHoleRadius,	// ���̔��a
	const float fThickness,		// ����
	const float fOuterPlusY		// �O����Y���W���Z��
)
{
	// �I�u�W�F�N�g���b�V�������O�̐���
	CObjectMeshRing *pMeshRing = new CObjectMeshRing;
	if (pMeshRing == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g���b�V�������O�̏�����
		if (FAILED(pMeshRing->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g���b�V�������O�̔j��
			SAFE_DELETE(pMeshRing);
			return nullptr;
		}

		// �ʒu��ݒ�
		pMeshRing->SetVec3Position(rPos);

		// ������ݒ�
		pMeshRing->SetVec3Rotation(rRot);

		// �F��ݒ�
		pMeshRing->SetColor(rCol);

		// ���̔��a��ݒ�
		pMeshRing->SetHoleRadius(fHoleRadius);

		// ������ݒ�
		pMeshRing->SetThickness(fThickness);

		// �O����Y���W���Z�ʂ�ݒ�
		pMeshRing->SetOuterPlusY(fOuterPlusY);

		// ��������ݒ�
		if (FAILED(pMeshRing->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �I�u�W�F�N�g���b�V�������O�̔j��
			SAFE_DELETE(pMeshRing);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pMeshRing->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pMeshRing;
	}
}

//============================================================
//	�e�N�X�`�������̐ݒ菈��
//============================================================
void CObjectMeshRing::SetTexDir(const ETexDir texDir)
{
	// �e�N�X�`��������ݒ�
	m_meshRing.texDir = texDir;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�e�N�X�`�������擾����
//============================================================
CObjectMeshRing::ETexDir CObjectMeshRing::GetTexDir(void) const
{
	// �e�N�X�`��������Ԃ�
	return m_meshRing.texDir;
}

//============================================================
//	���̔��a�̐ݒ菈��
//============================================================
void CObjectMeshRing::SetHoleRadius(const float fHoleRadius)
{
	// �����̌��̔��a��ݒ�
	m_meshRing.fHoleRadius = fHoleRadius;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���̔��a�擾����
//============================================================
float CObjectMeshRing::GetHoleRadius(void) const
{
	// ���̔��a��Ԃ�
	return m_meshRing.fHoleRadius;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshRing::SetThickness(const float fThickness)
{
	// �����̑�����ݒ�
	m_meshRing.fThickness = fThickness;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����擾����
//============================================================
float CObjectMeshRing::GetThickness(void) const
{
	// ������Ԃ�
	return m_meshRing.fThickness;
}

//============================================================
//	�|���S���O����Y���W���Z�ʂ̐ݒ菈��
//============================================================
void CObjectMeshRing::SetOuterPlusY(const float fOuterPlusY)
{
	// �����̊O����Y���W���Z�ʂ�ݒ�
	m_meshRing.fOuterPlusY = fOuterPlusY;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�|���S���O����Y���W���Z�ʂ̎擾����
//============================================================
float CObjectMeshRing::GetOuterPlusY(void) const
{
	// �O����Y���W���Z�ʂ�Ԃ�
	return m_meshRing.fOuterPlusY;
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CObjectMeshRing::SetPattern(const POSGRID2& rPart)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �������̐ݒ�s��
	assert(rPart.x >= MIN_PART.x);
	assert(rPart.y >= MIN_PART.y);

	// �����̕�������ݒ�
	m_part = rPart;

	// �K�v���_�E�C���f�b�N�X�������߂�
	m_nNumVtx = (m_part.x + 1) * (m_part.y + 1); // �K�v���_��
	m_nNumIdx = (m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2; // �K�v�C���f�b�N�X��

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

	// �C���f�b�N�X�o�b�t�@�̔j��
	SAFE_RELEASE(m_pIdxBuff);
	assert(m_pIdxBuff == nullptr);

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer
	( // ����
		sizeof(WORD) * m_nNumIdx,	// �K�v�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,	// �g�p���@
		D3DFMT_INDEX16,		// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,	// �������̎w��
		&m_pIdxBuff,		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // �C���f�b�N�X�o�b�t�@�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_�E�C���f�b�N�X���̐ݒ�
	SetVtx();
	SetIdx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�������擾����
//============================================================
POSGRID2 CObjectMeshRing::GetPattern(void) const
{
	// ��������Ԃ�
	return m_part;
}

//============================================================
//	�e�N�X�`���������̐ݒ菈��
//============================================================
void CObjectMeshRing::SetTexPattern(const POSGRID2& rTexPart)
{
	if (rTexPart.x >= MIN_TEXPART.x
	&&  rTexPart.y >= MIN_TEXPART.y)
	{ // �e�N�X�`�����������Œ�l�ȏ�̏ꍇ

		// �����̃e�N�X�`����������ݒ�
		m_texPart = rTexPart;

		// ���_���̐ݒ�
		SetVtx();
	}
	else { assert(false); }	// �Œ�l����
}

//============================================================
//	�e�N�X�`���������擾����
//============================================================
POSGRID2 CObjectMeshRing::GetTexPattern(void) const
{
	// �e�N�X�`����������Ԃ�
	return m_texPart;
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *CObjectMeshRing::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectMeshRing::SetVtx(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;	// ���_�ʒu�̌v�Z�p
	D3DXVECTOR3 vecNor;	// �@���x�N�g���̌v�Z�p
	float fRateWidth  = ((float)(m_texPart.x) / (float)(m_part.x));	// �e�N�X�`�����������̊���
	float fRateHeight = ((float)(m_texPart.y) / (float)(m_part.y));	// �e�N�X�`���c�������̊���

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				// ���_���W�̕�����ݒ�
				vecPos = D3DXVECTOR3
				( // ����
					sinf(D3DXToRadian(nCntWidth * (360 / (float)m_part.x))) * ((nCntHeight * (m_meshRing.fThickness / (float)m_part.y)) + m_meshRing.fHoleRadius),	// x
					nCntHeight * (m_meshRing.fOuterPlusY / (float)m_part.y),																						// y
					cosf(D3DXToRadian(nCntWidth * (360 / (float)m_part.x))) * ((nCntHeight * (m_meshRing.fThickness / (float)m_part.y)) + m_meshRing.fHoleRadius)	// z
				);

				// ���_���W�̐ݒ�
				pVtx[0].pos = vecPos;

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshRing.col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				( // ����
					fRateWidth  * (nCntWidth  - m_part.x) * CALC_TEXDIR[m_meshRing.texDir],	// u
					fRateHeight * (nCntHeight - m_part.y) * -1.0f							// v
				);

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�C���f�b�N�X���̐ݒ菈��
//============================================================
void CObjectMeshRing::SetIdx(void)
{
	// �|�C���^��錾
	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	if (m_pIdxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				pIdx[0] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth);

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 1)
			{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

				pIdx[0] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth - 1);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 1));

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}
		}

		// �C���f�b�N�X�o�b�t�@���A�����b�N����
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	�X�N���[���̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CObjectMeshRing::SetScrollTex(const float fTexU, const float fTexV)
{
	// �ϐ���錾
	float fRateWidth  = ((float)(m_texPart.x) / (float)(m_part.x));	// �e�N�X�`�����������̊���
	float fRateHeight = ((float)(m_texPart.y) / (float)(m_part.y));	// �e�N�X�`���c�������̊���

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				( // ����
					fTexU + fRateWidth  * (nCntWidth  - m_part.x) * CALC_TEXDIR[m_meshRing.texDir],	// u
					fTexV + fRateHeight * (nCntHeight - m_part.y) * -1.0f							// v
				);

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�j������
//============================================================
void CObjectMeshRing::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//
//	�I�u�W�F�N�g���b�V���V�����_�[���� [objectMeshCylinder.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshCylinder.h"
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
static_assert(NUM_ARRAY(CALC_TEXDIR) == CObjectMeshCylinder::TEXDIR_MAX, "ERROR : Array Count Mismatch");

//************************************************************
//	�q�N���X [CObjectMeshCylinder] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectMeshCylinder::CObjectMeshCylinder(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
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
	memset(&m_meshCylinder, 0, sizeof(m_meshCylinder));	// ���b�V���V�����_�[�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectMeshCylinder::~CObjectMeshCylinder()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectMeshCylinder::Init(void)
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

	m_meshCylinder.pos		= VEC3_ZERO;		// �ʒu
	m_meshCylinder.rot		= VEC3_ZERO;		// ����
	m_meshCylinder.col		= XCOL_WHITE;		// �F
	m_meshCylinder.texDir	= TEXDIR_OUTSIDE;	// �e�N�X�`������
	m_meshCylinder.fRadius	= 0.0f;				// ���a
	m_meshCylinder.fHeight	= 0.0f;				// �c��

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
void CObjectMeshCylinder::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	SAFE_RELEASE(m_pIdxBuff);

	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// �I�u�W�F�N�g���b�V���V�����_�[��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectMeshCylinder::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectMeshCylinder::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����_�[�X�e�[�g��ݒ�
	m_pRenderState->Set();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshCylinder.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshCylinder.rot.y, m_meshCylinder.rot.x, m_meshCylinder.rot.z);
	D3DXMatrixMultiply(&m_meshCylinder.mtxWorld, &m_meshCylinder.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshCylinder.pos.x, m_meshCylinder.pos.y, m_meshCylinder.pos.z);
	D3DXMatrixMultiply(&m_meshCylinder.mtxWorld, &m_meshCylinder.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshCylinder.mtxWorld);

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
void CObjectMeshCylinder::BindTexture(const int nTextureID)
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
void CObjectMeshCylinder::BindTexture(const char *pTexturePass)
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
void CObjectMeshCylinder::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshCylinder.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectMeshCylinder::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_meshCylinder.pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshCylinder.rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_meshCylinder.rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectMeshCylinder::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_meshCylinder.rot;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_meshCylinder.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectMeshCylinder::GetColor(void) const
{
	// �F��Ԃ�
	return m_meshCylinder.col;
}

//============================================================
//	���a�̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetRadius(const float fRadius)
{
	// �����̔��a��ݒ�
	m_meshCylinder.fRadius = fRadius;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���a�擾����
//============================================================
float CObjectMeshCylinder::GetRadius(void) const
{
	// ���a��Ԃ�
	return m_meshCylinder.fRadius;
}

//============================================================
//	�c���̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetHeight(const float fHeight)
{
	// �����̏c����ݒ�
	m_meshCylinder.fHeight = fHeight;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�c���擾����
//============================================================
float CObjectMeshCylinder::GetHeight(void) const
{
	// �c����Ԃ�
	return m_meshCylinder.fHeight;
}

//============================================================
//	��������
//============================================================
CObjectMeshCylinder *CObjectMeshCylinder::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart,	// �e�N�X�`��������
	const float fRadius,		// ���a
	const float fHeight			// �c��
)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̐���
	CObjectMeshCylinder *pMeshCylinder = new CObjectMeshCylinder;
	if (pMeshCylinder == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g���b�V���V�����_�[�̏�����
		if (FAILED(pMeshCylinder->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g���b�V���V�����_�[�̔j��
			SAFE_DELETE(pMeshCylinder);
			return nullptr;
		}

		// �ʒu��ݒ�
		pMeshCylinder->SetVec3Position(rPos);

		// ������ݒ�
		pMeshCylinder->SetVec3Rotation(rRot);

		// �F��ݒ�
		pMeshCylinder->SetColor(rCol);

		// ���a��ݒ�
		pMeshCylinder->SetRadius(fRadius);

		// �c����ݒ�
		pMeshCylinder->SetHeight(fHeight);

		// ��������ݒ�
		if (FAILED(pMeshCylinder->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �I�u�W�F�N�g���b�V���V�����_�[�̔j��
			SAFE_DELETE(pMeshCylinder);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pMeshCylinder->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pMeshCylinder;
	}
}

//============================================================
//	�e�N�X�`�������̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetTexDir(const ETexDir texDir)
{
	// �e�N�X�`��������ݒ�
	m_meshCylinder.texDir = texDir;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�e�N�X�`�������擾����
//============================================================
CObjectMeshCylinder::ETexDir CObjectMeshCylinder::GetTexDir(void) const
{
	// �e�N�X�`��������Ԃ�
	return m_meshCylinder.texDir;
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CObjectMeshCylinder::SetPattern(const POSGRID2& rPart)
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
POSGRID2 CObjectMeshCylinder::GetPattern(void) const
{
	// ��������Ԃ�
	return m_part;
}

//============================================================
//	�e�N�X�`���������̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetTexPattern(const POSGRID2& rTexPart)
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
POSGRID2 CObjectMeshCylinder::GetTexPattern(void) const
{
	// �e�N�X�`����������Ԃ�
	return m_texPart;
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *CObjectMeshCylinder::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetVtx(void)
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
					sinf(D3DXToRadian(nCntWidth * (360 / (float)m_part.x))) * m_meshCylinder.fRadius,	// x
					(nCntHeight * (m_meshCylinder.fHeight / (float)m_part.y)),							// y
					cosf(D3DXToRadian(nCntWidth * (360 / (float)m_part.x))) * m_meshCylinder.fRadius	// z
				);

				// ���_���W�̐ݒ�
				pVtx[0].pos = vecPos;

				// �@���x�N�g���̕�����ݒ�
				vecNor = D3DXVECTOR3(vecPos.x, 0.0f, vecPos.z);

				// �x�N�g���𐳋K��
				D3DXVec3Normalize(&vecNor, &vecNor);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = vecNor;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshCylinder.col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				( // ����
					fRateWidth  * (nCntWidth  - m_part.x) * CALC_TEXDIR[m_meshCylinder.texDir],	// u
					fRateHeight * (nCntHeight - m_part.y) * -1.0f								// v
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
void CObjectMeshCylinder::SetIdx(void)
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

				pIdx[0] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth);
				pIdx[1] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth);

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 1)
			{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

				pIdx[0] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth - 1);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 2));

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
void CObjectMeshCylinder::SetScrollTex(const float fTexU, const float fTexV)
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
					fTexU + fRateWidth  * (nCntWidth  - m_part.x) * CALC_TEXDIR[m_meshCylinder.texDir],	// u
					fTexV + fRateHeight * (nCntHeight - m_part.y) * -1.0f								// v
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
void CObjectMeshCylinder::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

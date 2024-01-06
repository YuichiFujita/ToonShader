//============================================================
//
//	�I�u�W�F�N�g���f������ [objectModel.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�q�N���X [CObjectModel] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectModel::CObjectModel(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pRenderState	(nullptr),		// �����_�[�X�e�[�g�̏��
	m_pMat			(nullptr),		// �}�e���A���ւ̃|�C���^
	m_pos			(VEC3_ZERO),	// �ʒu
	m_rot			(VEC3_ZERO),	// ����
	m_scale			(VEC3_ZERO),	// �g�嗦
	m_nModelID		(0)				// ���f���C���f�b�N�X
{
	// �����o�ϐ����N���A
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	D3DXMatrixIdentity(&m_mtxWorld);				// ���[���h�}�g���b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectModel::~CObjectModel()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectModel::Init(void)
{
	// �����o�ϐ���������
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	D3DXMatrixIdentity(&m_mtxWorld);				// ���[���h�}�g���b�N�X
	m_pRenderState	= nullptr;		// �����_�[�X�e�[�g�̏��
	m_pMat			= nullptr;		// �}�e���A���ւ̃|�C���^
	m_pos			= VEC3_ZERO;	// �ʒu
	m_rot			= VEC3_ZERO;	// ����
	m_scale			= VEC3_ONE;		// �g�嗦
	m_nModelID		= NONE_IDX;		// ���f���C���f�b�N�X

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
void CObjectModel::Uninit(void)
{
	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// �}�e���A���̔j��
	SAFE_DEL_ARRAY(m_pMat);

	// �I�u�W�F�N�g���f����j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectModel::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectModel::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����_�[�X�e�[�g��ݒ�
	m_pRenderState->Set();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetTexture(m_modelData.pTextureID[nCntMat]));

		if (m_scale != VEC3_ONE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_modelData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �����_�[�X�e�[�g���Đݒ�
	m_pRenderState->Reset();
}

//============================================================
//	���f���������� (�C���f�b�N�X)
//============================================================
void CObjectModel::BindModel(const int nModelID)
{
	// �|�C���^��錾
	CModel *pModel = GET_MANAGER->GetModel();	// ���f���ւ̃|�C���^

	if (nModelID > NONE_IDX)
	{ // ���f���C���f�b�N�X���g�p�\�ȏꍇ

		// ���f���C���f�b�N�X����
		m_nModelID = nModelID;

		// ���f�������蓖��
		m_modelData = *pModel->GetModel(nModelID);

		// ���}�e���A���̐ݒ�
		if (FAILED(SetOriginMaterial(m_modelData.pBuffMat, (int)m_modelData.dwNumMat)))
		{ assert(false); }	// �ݒ莸�s
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	���f���������� (�p�X)
//============================================================
void CObjectModel::BindModel(const char *pModelPass)
{
	// �|�C���^��錾
	CModel *pModel = GET_MANAGER->GetModel();	// ���f���ւ̃|�C���^

	if (pModelPass != nullptr)
	{ // ���蓖�Ă郂�f���p�X�����݂���ꍇ

		// ���f���C���f�b�N�X����
		m_nModelID = pModel->Regist(pModelPass);

		// ���f�������蓖��
		m_modelData = *pModel->GetModel(m_nModelID);

		// ���}�e���A���̐ݒ�
		if (FAILED(SetOriginMaterial(m_modelData.pBuffMat, (int)m_modelData.dwNumMat)))
		{ assert(false); }	// �ݒ莸�s
	}
	else { assert(false); }	// ���f���p�X����
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectModel::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectModel::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�g�嗦�̐ݒ菈��
//============================================================
void CObjectModel::SetVec3Scaling(const D3DXVECTOR3& rScale)
{
	// �����̊g�嗦����
	m_scale = rScale;
}

//============================================================
//	�g�嗦�擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetVec3Scaling(void) const
{
	// �g�嗦��Ԃ�
	return m_scale;
}

//============================================================
//	���a�擾����
//============================================================
float CObjectModel::GetRadius(void) const
{
	// ���f���̔��a��Ԃ�
	return m_modelData.fRadius;
}

//============================================================
//	�}�e���A���̑S�ݒ菈��
//============================================================
void CObjectModel::SetAllMaterial(const D3DXMATERIAL& rMat)
{
	// �����̃}�e���A����S�}�e���A���ɐݒ�
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		m_pMat[nCntMat] = rMat;
	}
}

//============================================================
//	�}�e���A���̍Đݒ菈��
//============================================================
void CObjectModel::ResetMaterial(void)
{
	if (IsDeath())
	{ // ����ł���ꍇ

		return;
	}

	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	// �S�}�e���A���ɏ����}�e���A�����Đݒ�
	if (m_pMat != nullptr)
	{ // �g�p����Ă���ꍇ

		for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			m_pMat[nCntMat] = pOriginMat[nCntMat];
		}
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CObjectModel::GetPtrMtxWorld(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CObjectModel::GetMtxWorld(void) const
{
	// ���[���h�}�g���b�N�X��Ԃ�
	return m_mtxWorld;
}

//============================================================
//	��������
//============================================================
CObjectModel *CObjectModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rScale)
{
	// �I�u�W�F�N�g���f���̐���
	CObjectModel *pModel = new CObjectModel;
	if (pModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g���f���̏�����
		if (FAILED(pModel->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g���f���̔j��
			SAFE_DELETE(pModel);
			return nullptr;
		}

		// �ʒu��ݒ�
		pModel->SetVec3Position(rPos);

		// ������ݒ�
		pModel->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pModel->SetVec3Scaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pModel;
	}
}

//============================================================
//	�}�e���A���ݒ菈��
//============================================================
void CObjectModel::SetMaterial(const D3DXMATERIAL& rMat, const int nID)
{
	if (nID > NONE_IDX && nID < (int)m_modelData.dwNumMat)
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă��Ȃ��ꍇ

		// �����C���f�b�N�X�̃}�e���A����ݒ�
		m_pMat[nID] = rMat;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�}�e���A���擾����
//============================================================
D3DXMATERIAL CObjectModel::GetMaterial(const int nID) const
{
	if (nID > NONE_IDX && nID < (int)m_modelData.dwNumMat)
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă��Ȃ��ꍇ

		// �����C���f�b�N�X�̃}�e���A����Ԃ�
		return m_pMat[nID];
	}
	else
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă���ꍇ

		// �ϐ���錾
		D3DXMATERIAL mat;	// ��O���̃}�e���A��

		// �}�e���A���̃��������N���A
		ZeroMemory(&mat, sizeof(mat));

		// ��O����
		assert(false);

		// ��O���̃}�e���A����Ԃ�
		return mat;
	}
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CObjectModel::SetAlpha(const float fAlpha)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �ϐ���錾
		float fSetAlpha = fAlpha;	// �ݒ肷�铧���x

		// �����x�̕␳
		useful::LimitNum(fSetAlpha, 0.0f, pOriginMat[nCntMat].MatD3D.Diffuse.a);

		// �����x��ݒ�
		m_pMat[nCntMat].MatD3D.Diffuse.a = fSetAlpha;
	}
}

//============================================================
//	�����x�擾����
//============================================================
float CObjectModel::GetAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (m_pMat[nCntMat].MatD3D.Diffuse.a > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = m_pMat[nCntMat].MatD3D.Diffuse.a;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�ő哧���x�擾����
//============================================================
float CObjectModel::GetMaxAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (pOriginMat[nCntMat].MatD3D.Diffuse.a > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = pOriginMat[nCntMat].MatD3D.Diffuse.a;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CObjectModel::SetModelData(const CModel::SModel& rModel)
{
	// �����̃��f��������
	m_modelData = rModel;
}

//============================================================
//	���f�����擾����
//============================================================
CModel::SModel CObjectModel::GetModelData(void) const
{
	// ���f������Ԃ�
	return m_modelData;
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectModel::SetMtxWorld(const D3DXMATRIX& rMtxWorld)
{
	// �����̃}�g���b�N�X��ݒ�
	m_mtxWorld = rMtxWorld;
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *CObjectModel::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	�}�e���A���|�C���^�擾����
//============================================================
D3DXMATERIAL *CObjectModel::GetPtrMaterial(const int nID) const
{
	if (nID > NONE_IDX && nID < (int)m_modelData.dwNumMat)
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă��Ȃ��ꍇ

		// �����C���f�b�N�X�̃}�e���A����Ԃ�
		return &m_pMat[nID];
	}
	else
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă���ꍇ

		// nullptr��Ԃ�
		return nullptr;
	}
}

//============================================================
//	�j������
//============================================================
void CObjectModel::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���}�e���A���̐ݒ菈��
//============================================================
HRESULT CObjectModel::SetOriginMaterial(const LPD3DXBUFFER pBuffMat, const int nNumMat)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	//--------------------------------------------------------
	//	�������J���E�m��
	//--------------------------------------------------------
	// �}�e���A���̔j��
	SAFE_DEL_ARRAY(m_pMat);

	if (m_pMat == nullptr)
	{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

		// �}�e���A�������̃������m��
		m_pMat = new D3DXMATERIAL[nNumMat];

		if (m_pMat != nullptr)
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_pMat, 0, sizeof(D3DXMATERIAL) * nNumMat);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	�}�e���A������ݒ�
	//--------------------------------------------------------
	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���f�[�^���R�s�[
		m_pMat[nCntMat] = pOriginMat[nCntMat];
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//
//	���f������ [model.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\TXT\\model.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

	const D3DXVECTOR3 INIT_VTXMIN = D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f);	// ���f���̍ŏ��̒��_���W�̏����l
	const D3DXVECTOR3 INIT_VTXMAX = D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f);	// ���f���̍ő�̒��_���W�̏����l
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
int CModel::m_nNumAll = 0;	// ���f���̑���

//************************************************************
//	�e�N���X [CModel] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CModel::CModel()
{
	// ���f���ւ̃|�C���^���N���A
	memset(&m_aModel[0], 0, sizeof(m_aModel));

	// �S�t�@�C�������폜
	m_sFileName.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CModel::~CModel()
{

}

//============================================================
//	���f����������
//============================================================
HRESULT CModel::Load(void)
{
	// ���f���ւ̃|�C���^��������
	memset(&m_aModel[0], 0, sizeof(m_aModel));

	// �S�t�@�C�������폜
	m_sFileName.clear();

	// �Z�b�g�A�b�v�̓Ǎ�
	LoadSetup();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���f���j������
//============================================================
void CModel::Unload(void)
{
	// ���f���̔j��
	for (int nCntModel = 0; nCntModel < model::MAX_NUM; nCntModel++)
	{ // ���f���̍ő吔���J��Ԃ�

		// �e�N�X�`���C���f�b�N�X�̔j��
		SAFE_FREE(m_aModel[nCntModel].pTextureID);

		// ���b�V���̔j��
		SAFE_RELEASE(m_aModel[nCntModel].pMesh);

		// �}�e���A���̔j��
		SAFE_RELEASE(m_aModel[nCntModel].pBuffMat);
	}

	// �S�t�@�C�������폜
	m_sFileName.clear();
}

//============================================================
//	���f���o�^����
//============================================================
int CModel::Regist(const char *pFileName)
{
	// �ϐ���錾
	int nID = m_nNumAll;	// ���f���Ǎ��ԍ�

	if (pFileName != nullptr)
	{ // �|�C���^���g�p����Ă���ꍇ

		int nCntModel = 0;	// ���f���ԍ�
		for (auto sFileName : m_sFileName)
		{ // �Ō���܂ŌJ��Ԃ�

			if (sFileName.compare(pFileName) == 0)
			{ // �����񂪈�v�����ꍇ

				// ���łɓǂݍ���ł��郂�f���̔z��ԍ���Ԃ�
				return nCntModel;
			}

			// ���̃��f���ԍ��ɂ���
			nCntModel++;
		}

		if (m_nNumAll >= model::MAX_NUM)
		{ // ���f���I�[�o�[�̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}

		// x�t�@�C���̓ǂݍ���
		if (FAILED(LoadXFileModel(nID, pFileName)))
		{ // x�t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}

		// �e�N�X�`���̓ǂݍ���
		if (FAILED(LoadTextureModel(nID)))
		{ // �e�N�X�`���̓ǂݍ��݂Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}

		// �����蔻��̍쐬
		SetCollisionModel(nID);

		// ���f���̃t�@�C������ۑ�
		m_sFileName.push_back(pFileName);

		// ���f���̑��������Z
		m_nNumAll++;

		// �ǂݍ��񂾃��f���̔z��ԍ���Ԃ�
		return nID;
	}
	else
	{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return NONE_IDX;
	}
}

//============================================================
//	���f�����擾����
//============================================================
CModel::SModel *CModel::GetModel(const int nID)
{
	if (nID > NONE_IDX && nID < m_nNumAll)
	{ // �����̃C���f�b�N�X���͈͓��̏ꍇ

		// �����̃��f���A�h���X��Ԃ�
		return &m_aModel[nID];
	}
	else { assert(false); return nullptr; }	// �͈͊O
}

//============================================================
//	��������
//============================================================
CModel *CModel::Create(void)
{
	// ���f���̐���
	CModel *pModel = new CModel;
	if (pModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���f���̓Ǎ�
		if (FAILED(pModel->Load()))
		{ // ���f���Ǎ��Ɏ��s�����ꍇ

			// ���f���̔j��
			SAFE_DELETE(pModel);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pModel;
	}
}

//============================================================
//	�j������
//============================================================
void CModel::Release(CModel *&prModel)
{
	// ���f���̔j��
	assert(prModel != nullptr);
	prModel->Unload();

	// �������J��
	SAFE_DELETE(prModel);
}

//============================================================
//	x�t�@�C���̓ǂݍ���
//============================================================
HRESULT CModel::LoadXFileModel(const int nID, const char *pFileName)
{
	// �ϐ���錾
	HRESULT hr;	// �ُ�I���̊m�F�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// x�t�@�C���̓ǂݍ���
	hr = D3DXLoadMeshFromX
	( // ����
		pFileName,					// ���f���̑��΃p�X
		D3DXMESH_SYSTEMMEM,			// ���b�V���쐬�p�I�v�V����
		pDevice,					// �f�o�C�X�ւ̃|�C���^
		nullptr,					// �אڐ��f�[�^
		&m_aModel[nID].pBuffMat,	// �}�e���A���ւ̃|�C���^
		nullptr,					// �G�t�F�N�g�f�[�^
		&m_aModel[nID].dwNumMat,	// �}�e���A���̐�
		&m_aModel[nID].pMesh		// ���b�V�� (���_���) �ւ̃|�C���^
	);

	if (FAILED(hr))
	{ // x�t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "x�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	if (m_aModel[nID].pTextureID == nullptr)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �m�ۂ����������̃A�h���X���擾
		m_aModel[nID].pTextureID = (int*)malloc(m_aModel[nID].dwNumMat * sizeof(int*));
	}

	if (m_aModel[nID].pTextureID == nullptr)
	{ // ���I�m�ۂɎ��s�����ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "���I�m�ۂɎ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�e�N�X�`���̓ǂݍ���
//============================================================
HRESULT CModel::LoadTextureModel(const int nID)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	D3DXMATERIAL *pMat;	// �}�e���A���ւ̃|�C���^

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_aModel[nID].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aModel[nID].dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{ // �e�N�X�`���t�@�C�������݂���ꍇ

			// �e�N�X�`����o�^
			m_aModel[nID].pTextureID[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
		}
		else
		{ // �e�N�X�`���t�@�C�������݂��Ȃ��ꍇ

			// �e�N�X�`����o�^
			m_aModel[nID].pTextureID[nCntMat] = NONE_IDX;	// �e�N�X�`���Ȃ�
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����蔻��̍쐬
//============================================================
void CModel::SetCollisionModel(const int nID)
{
	// �ϐ���錾
	int			nNumVtx;	// ���f���̒��_��
	DWORD		dwSizeFVF;	// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y
	BYTE		*pVtxBuff;	// ���f���̒��_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3	vtx;		// ���f���̒��_���W

	// ���f���̒��_�����擾
	nNumVtx = m_aModel[nID].pMesh->GetNumVertices();

	// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(m_aModel[nID].pMesh->GetFVF());

	// ���f���̒��_�o�b�t�@�����b�N
	m_aModel[nID].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{ // ���f���̒��_�����J��Ԃ�

		// ���f���̒��_���W����
		vtx = *(D3DXVECTOR3*)pVtxBuff;

		// x���_���W�̐ݒ�
		if (vtx.x < m_aModel[nID].vtxMin.x)
		{ // ����� x���_���W�����������ꍇ

			// x���_������
			m_aModel[nID].vtxMin.x = vtx.x;
		}
		else if (vtx.x > m_aModel[nID].vtxMax.x)
		{ // ����� x���_���W�����傫���ꍇ

			// x���_������
			m_aModel[nID].vtxMax.x = vtx.x;
		}

		// y���_���W�̐ݒ�
		if (vtx.y < m_aModel[nID].vtxMin.y)
		{ // ����� y���_���W�����������ꍇ

			// y���_������
			m_aModel[nID].vtxMin.y = vtx.y;
		}
		else if (vtx.y > m_aModel[nID].vtxMax.y)
		{ // ����� y���_���W�����傫���ꍇ

			// y���_������
			m_aModel[nID].vtxMax.y = vtx.y;
		}

		// z���_���W�̐ݒ�
		if (vtx.z < m_aModel[nID].vtxMin.z)
		{ // ����� z���_���W�����������ꍇ

			// z���_������
			m_aModel[nID].vtxMin.z = vtx.z;
		}
		else if (vtx.z > m_aModel[nID].vtxMax.z)
		{ // ����� z���_���W�����傫���ꍇ

			// z���_������
			m_aModel[nID].vtxMax.z = vtx.z;
		}

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	// ���f���̒��_�o�b�t�@���A�����b�N
	m_aModel[nID].pMesh->UnlockVertexBuffer();

	// ���f���T�C�Y�����߂�
	m_aModel[nID].size = m_aModel[nID].vtxMax - m_aModel[nID].vtxMin;

	// ���f���̉~�̓����蔻����쐬
	m_aModel[nID].fRadius = ((m_aModel[nID].size.x * 0.5f) + (m_aModel[nID].size.z * 0.5f)) * 0.5f;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CModel::LoadSetup(void)
{
	// �ϐ���錾
	int nEnd = 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "FILENAME") == 0)
			{ // �ǂݍ��񂾕����� FILENAME �̏ꍇ

				// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%s", &aString[0]);

				// �t�@�C���p�X��ǂݍ���
				fscanf(pFile, "%s", &aString[0]);

				// ���f����o�^
				Regist(&aString[0]);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "���f���Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

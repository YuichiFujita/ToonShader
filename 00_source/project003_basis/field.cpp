//============================================================
//
//	�n�ʏ��� [field.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		nullptr,	// �����e�N�X�`��
	};

	const char *SETUP_TXT = "data\\TXT\\field.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

	const int PRIORITY = 0;	// �n�ʂ̗D�揇��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CField::STerrainInfo CField::m_aTerrainInfo[TERRAIN_MAX] = {};	// �n�`���

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CField::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CField::CField() : CObjectMeshField(CObject::LABEL_FIELD, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CField::~CField()
{

}

//============================================================
//	����������
//============================================================
HRESULT CField::Init(void)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̏�����
	if (FAILED(CObjectMeshField::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Z�b�g�A�b�v�̓Ǎ�
	LoadSetup();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CField::Uninit(void)
{
	for (int nCntField = 0; nCntField < TERRAIN_MAX; nCntField++)
	{ // �n�`���̍ő吔���J��Ԃ�

		// �n�`���̔j��
		SAFE_DEL_ARRAY(m_aTerrainInfo[nCntField].pPosGap);
	}

	// �I�u�W�F�N�g���b�V���t�B�[���h�̏I��
	CObjectMeshField::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CField::Update(void)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̍X�V
	CObjectMeshField::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CField::Draw(void)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̕`��
	CObjectMeshField::Draw();
}

//============================================================
//	��������
//============================================================
CField *CField::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart		// ������
)
{
	// �n�ʂ̐���
	CField *pField = new CField;
	if (pField == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �n�ʂ̏�����
		if (FAILED(pField->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �n�ʂ̔j��
			SAFE_DELETE(pField);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pField->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[texture]));

		// �ʒu��ݒ�
		pField->SetVec3Position(rPos);

		// ������ݒ�
		pField->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pField->SetVec2Sizing(rSize);

		// �F��ݒ�
		pField->SetColor(rCol);

		// ��������ݒ�
		if (FAILED(pField->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �n�ʂ̔j��
			SAFE_DELETE(pField);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pField;
	}
}

//============================================================
//	�n�`�̐ݒ菈��
//============================================================
void CField::SetTerrain(const ETerrain terrain)
{
	if (terrain < TERRAIN_MAX)
	{ // �l���͈͓��̏ꍇ

		// �n�`��ݒ�
		CObjectMeshField::SetTerrain(m_aTerrainInfo[terrain].part, m_aTerrainInfo[terrain].pPosGap);
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CField::LoadSetup(void)
{
	// �ϐ���錾
	int nID = 0;		// �C���f�b�N�X�̑���p
	int nNumVtx = 0;	// ���_���̑���p
	int nEnd = 0;		// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	for (int nCntField = 0; nCntField < TERRAIN_MAX; nCntField++)
	{ // �n�`���̍ő吔���J��Ԃ�

		// �n�`���̔j��
		SAFE_DEL_ARRAY(m_aTerrainInfo[nCntField].pPosGap);
	}

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_aTerrainInfo[0], 0, sizeof(m_aTerrainInfo));	// �n�`���

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �n�`�̐ݒ�
			if (strcmp(&aString[0], "TERRAINSET") == 0)
			{ // �ǂݍ��񂾕����� TERRAINSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_TERRAINSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "FIELDSET") == 0)
					{ // �ǂݍ��񂾕����� FIELDSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "PART") == 0)
							{ // �ǂݍ��񂾕����� PART �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aTerrainInfo[nID].part.x);	// ������X��ǂݍ���
								fscanf(pFile, "%d", &m_aTerrainInfo[nID].part.y);	// ������Y��ǂݍ���

								// ���_����ݒ�
								nNumVtx = (m_aTerrainInfo[nID].part.x + 1) * (m_aTerrainInfo[nID].part.y + 1);

								if (m_aTerrainInfo[nID].pPosGap == nullptr)
								{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

									// ���_�����������m��
									m_aTerrainInfo[nID].pPosGap = new D3DXVECTOR3[nNumVtx];

									// ��O����
									assert(m_aTerrainInfo[nID].pPosGap != nullptr);	// ��g�p��
								}
								else { assert(false); }	// �g�p��
							}
							else if (strcmp(&aString[0], "GAPSET") == 0)
							{ // �ǂݍ��񂾕����� GAPSET �̏ꍇ

								for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
								{ // ���_�����J��Ԃ�

									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].x);	// ���_���W�̂����X��ǂݍ���
									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].y);	// ���_���W�̂����Y��ǂݍ���
									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].z);	// ���_���W�̂����Z��ǂݍ���
									fscanf(pFile, "%s", &aString[0]);								// , ��ǂݍ��� (�s�v)
								}
							}
						} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v

						// �C���f�b�N�X�����Z
						nID++;
					}
				} while (strcmp(&aString[0], "END_TERRAINSET") != 0);	// �ǂݍ��񂾕����� END_TERRAINSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�n�ʃZ�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

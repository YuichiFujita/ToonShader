//============================================================
//
//	�X�e�[�W���� [stage.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "collision.h"
#include "retentionManager.h"

#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "liquid.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\TXT\\stage.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
}

//************************************************************
//	�e�N���X [CStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStage::CStage()
{
	// �����o�ϐ����N���A
	memset(&m_stageLimit,	0, sizeof(m_stageLimit));	// �͈�
	memset(&m_field,		0, sizeof(m_field));		// �n�ʏ��
	memset(&m_wall,			0, sizeof(m_wall));			// �Ǐ��
	memset(&m_scenery,		0, sizeof(m_scenery));		// �i�F���
	memset(&m_sky,			0, sizeof(m_sky));			// ����
	memset(&m_liquid,		0, sizeof(m_liquid));		// �t�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CStage::~CStage()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStage::Init(void)
{
	// �����o�ϐ���������
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// �͈�

	// �n�ʂ̏���������
	m_field.ppField = nullptr;	// �n�ʂ̏��
	m_field.nNum = 0;			// �n�ʂ̑���

	// �ǂ̏���������
	m_wall.ppWall = nullptr;	// �ǂ̏��
	m_wall.nNum = 0;			// �ǂ̑���

	// �i�F�̏���������
	m_scenery.ppScenery = nullptr;	// �i�F�̏��
	m_scenery.nNum = 0;				// �i�F�̑���

	// ��̏���������
	m_sky.ppSky = nullptr;	// ��̏��
	m_sky.nNum = 0;			// ��̑���

	// �t�̂̏���������
	m_liquid.ppLiquid = nullptr;	// �t�̂̏��
	m_liquid.nNum = 0;				// �t�̂̑���

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CStage::Uninit(void)
{
	// �n�ʂ̔j��
	SAFE_DEL_ARRAY(m_field.ppField);

	// �ǂ̔j��
	SAFE_DEL_ARRAY(m_wall.ppWall);

	// �i�F�̔j��
	SAFE_DEL_ARRAY(m_scenery.ppScenery);

	// ��̔j��
	SAFE_DEL_ARRAY(m_sky.ppSky);

	// �t�̂̔j��
	SAFE_DEL_ARRAY(m_liquid.ppLiquid);
}

//============================================================
//	�X�V����
//============================================================
void CStage::Update(void)
{

}

//============================================================
//	�X�e�[�W�͈͂̐ݒ菈��
//============================================================
void CStage::SetStageLimit(const SStageLimit& rLimit)
{
	// �����̃X�e�[�W�͈͂�ݒ�
	m_stageLimit = rLimit;
}

//============================================================
//	�X�e�[�W�͈͎擾����
//============================================================
CStage::SStageLimit CStage::GetStageLimit(void) const
{
	// �X�e�[�W�͈͂�Ԃ�
	return m_stageLimit;
}

//============================================================
//	�ʒu�̕␳����
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	switch (m_stageLimit.mode)
	{ // �������[�h���Ƃ̏���
	case LIMIT_BOX:		// ��`�͈�

		// �p���̓�������
		collision::InBoxPillar
		( // ����
			rPos,				// ����ʒu
			VEC3_ZERO,			// ���茴�_�ʒu
			VEC3_ALL(fRadius),	// ����T�C�Y(�E�E��E��)
			VEC3_ALL(fRadius),	// ����T�C�Y(���E���E�O)
			D3DXVECTOR3(fabsf(m_stageLimit.fRight), 0.0f, fabsf(m_stageLimit.fFar)),	// ���茴�_�T�C�Y(�E�E��E��)
			D3DXVECTOR3(fabsf(m_stageLimit.fLeft),  0.0f, fabsf(m_stageLimit.fNear))	// ���茴�_�T�C�Y(���E���E�O)
		);

		break;

	case LIMIT_CIRCLE:	// �~�͈�

		// �~���̓�������
		collision::InCirclePillar
		( // ����
			rPos,					// ����ʒu
			VEC3_ZERO,				// ���茴�_�ʒu
			fRadius,				// ���蔼�a
			m_stageLimit.fRadius	// ���茴�_���a
		);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�͈͊O�̒��n����
//============================================================
bool CStage::LandLimitPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_stageLimit.fField)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		rPos.y = m_stageLimit.fField + fHeight;

		// �ړ��ʂ�������
		rMove.y = 0.0f;

		// ���n���Ă���󋵂�Ԃ�
		return true;
	}

	// ���n���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	�L��Y���W�Ƃ̓����蔻��
//============================================================
bool CStage::CollisionKillY(const D3DXVECTOR3& rPos)
{
	if (rPos.y < m_stageLimit.fKillY)
	{ // �ʒu���폜�̐����ʒu��艺�̏ꍇ

		// ���S���Ă���󋵂�Ԃ�
		return true;
	}

	// ���S���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	�n�ʂ̒��n���� (��]�l��)
//============================================================
bool CStage::LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove)
{
	// �ϐ���錾
	CField *pCurrentField = nullptr;			// ���n�\��̒n��
	float fCurrentPos = m_stageLimit.fField;	// ���n�\���Y���W

	for (int nCntField = 0; nCntField < m_field.nNum; nCntField++)
	{ // �n�ʂ̑������J��Ԃ�

		if (m_field.ppField[nCntField] != nullptr)
		{ // �n�ʂ��g�p����Ă���ꍇ

			if (m_field.ppField[nCntField]->IsPositionRange(rPos))
			{ // �n�ʂ͈͓̔��̏ꍇ

				// �ϐ���錾
				float fPosHeight = m_field.ppField[nCntField]->GetPositionHeight(rPos);	// ���nY���W

				if (fCurrentPos <= fPosHeight)
				{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

					// ���n�\��̒n�ʂ��X�V
					pCurrentField = m_field.ppField[nCntField];

					// ���n�\���Y���W���X�V
					fCurrentPos = fPosHeight;
				}
			}
		}
		else { assert(false); }	// ��g�p��
	}

	if (pCurrentField != nullptr)
	{ // ���n�\��̒n�ʂ����݂���ꍇ

		// ���b�V���̒��n
		return pCurrentField->LandPosition(rPos, rMove);
	}
	else
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ

		// ���n���Ă��Ȃ���Ԃ�Ԃ�
		return false;
	}
}

//============================================================
//	�n�ʂ͈͓̔��̎擾���� (��]�l��)
//============================================================
bool CStage::IsFieldPositionRange(const D3DXVECTOR3&rPos)
{
	for (int nCntField = 0; nCntField < m_field.nNum; nCntField++)
	{ // �n�ʂ̑������J��Ԃ�

		if (m_field.ppField[nCntField] != nullptr)
		{ // �n�ʂ��g�p����Ă���ꍇ

			if (m_field.ppField[nCntField]->IsPositionRange(rPos))
			{ // �n�ʂ͈͓̔��̏ꍇ

				// �͈͓��ɂ����Ԃ�Ԃ�
				return true;
			}
		}
		else { assert(false); }	// ��g�p��
	}

	// �͈͓��ɂ��Ȃ���Ԃ�Ԃ�
	return false;
}

//============================================================
//	�n�ʂ̒��n�ʒu�̎擾���� (��]�l��)
//============================================================
float CStage::GetFieldPositionHeight(const D3DXVECTOR3&rPos)
{
	// �ϐ���錾
	CField *pCurrentField = nullptr;			// ���n�\��̒n��
	float fCurrentPos = m_stageLimit.fField;	// ���n�\���Y���W

	for (int nCntField = 0; nCntField < m_field.nNum; nCntField++)
	{ // �n�ʂ̑������J��Ԃ�

		if (m_field.ppField[nCntField] != nullptr)
		{ // �n�ʂ��g�p����Ă���ꍇ

			if (m_field.ppField[nCntField]->IsPositionRange(rPos))
			{ // �n�ʂ͈͓̔��̏ꍇ

				// �ϐ���錾
				float fPosHeight = m_field.ppField[nCntField]->GetPositionHeight(rPos);	// ���nY���W

				if (fCurrentPos <= fPosHeight)
				{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

					// ���n�\��̒n�ʂ��X�V
					pCurrentField = m_field.ppField[nCntField];

					// ���n�\���Y���W���X�V
					fCurrentPos = fPosHeight;
				}
			}
		}
		else { assert(false); }	// ��g�p��
	}

	if (pCurrentField != nullptr)
	{ // ���n�\��̒n�ʂ����݂���ꍇ

		// ���n�ʒu��Ԃ�
		return fCurrentPos;
	}
	else
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ

		// �����ʒu��Ԃ�
		return rPos.y;
	}
}

//============================================================
//	��������
//============================================================
CStage *CStage::Create(const CScene::EMode mode)
{
	// �X�e�[�W�̐���
	CStage *pStage = new CStage;
	if (pStage == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�e�[�W�̏�����
		if (FAILED(pStage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�e�[�W�̔j��
			SAFE_DELETE(pStage);
			return nullptr;
		}

		// �Z�b�g�A�b�v�̓Ǎ�
		if (FAILED(LoadSetup(mode, pStage)))
		{ // �ǂݍ��݂Ɏ��s�����ꍇ

			// �X�e�[�W�̔j��
			SAFE_DELETE(pStage);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pStage;
	}
}

//============================================================
//	�j������
//============================================================
void CStage::Release(CStage *&prStage)
{
	// �X�e�[�W�̏I��
	assert(prStage != nullptr);
	prStage->Uninit();

	// �������J��
	SAFE_DELETE(prStage);
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CStage::LoadSetup(const CScene::EMode /*mode*/, CStage *pStage)
{
	// �ϐ���錾
	int nEnd = 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �`���[�g���A���t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		while (1)
		{ // EOF�܂Ŗ������[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �I�[�̏ꍇ EOF��Ԃ�
			if (nEnd == EOF)
			{ // �t�@�C�����I�[�̏ꍇ

				// �����𔲂���
				break;
			}

			// �͈͏��̓Ǎ�
			if (FAILED(LoadLimit(&aString[0], pFile, pStage)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �n�ʂ̓Ǎ�
			else if (FAILED(LoadField(&aString[0], pFile, pStage)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �ǂ̓Ǎ�
			else if (FAILED(LoadWall(&aString[0], pFile, pStage)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �i�F�̓Ǎ�
			else if (FAILED(LoadScenery(&aString[0], pFile, pStage)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// ��̓Ǎ�
			else if (FAILED(LoadSky(&aString[0], pFile, pStage)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �t�̂̓Ǎ�
			else if (FAILED(LoadLiquid(&aString[0], pFile, pStage)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}
		}
		
		// �t�@�C�������
		fclose(pFile);

		// ������Ԃ�
		return S_OK;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�e�[�W�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//============================================================
//	�͈͏��̓Ǎ�����
//============================================================
HRESULT CStage::LoadLimit(const char* pString, FILE *pFile, CStage *pStage)
{
	// �ϐ���錾
	SStageLimit stageLimit;		// �X�e�[�W�͈͂̑���p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr || pStage == nullptr)
	{ // ������E�t�@�C���E�X�e�[�W�����݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�e�[�W�͈͂̐ݒ�
	if (strcmp(pString, "LIMITSET") == 0)
	{ // �ǂݍ��񂾕����� LIMITSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "CENTER") == 0)
			{ // �ǂݍ��񂾕����� CENTER �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &stageLimit.center.x);	// ���S���WX��ǂݍ���
				fscanf(pFile, "%f", &stageLimit.center.y);	// ���S���WY��ǂݍ���
				fscanf(pFile, "%f", &stageLimit.center.z);	// ���S���WZ��ǂݍ���
			}
			else if (strcmp(&aString[0], "NEAR") == 0)
			{ // �ǂݍ��񂾕����� NEAR �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &stageLimit.fNear);		// �O�ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				stageLimit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "FAR") == 0)
			{ // �ǂݍ��񂾕����� FAR �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &stageLimit.fFar);		// ��ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				stageLimit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RIGHT") == 0)
			{ // �ǂݍ��񂾕����� RIGHT �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &stageLimit.fRight);	// �E�ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				stageLimit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "LEFT") == 0)
			{ // �ǂݍ��񂾕����� LEFT �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &stageLimit.fLeft);		// ���ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				stageLimit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RADIUS") == 0)
			{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &stageLimit.fRadius);	// ���a��ǂݍ���

				// �������[�h���~�͈͂ɐݒ�
				stageLimit.mode = LIMIT_CIRCLE;
			}
			else if (strcmp(&aString[0], "FIELD") == 0)
			{ // �ǂݍ��񂾕����� FIELD �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &stageLimit.fField);	// �n�ʈʒu��ǂݍ���
			}
			else if (strcmp(&aString[0], "KILLY") == 0)
			{ // �ǂݍ��񂾕����� KILLY �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &stageLimit.fKillY);	// �폜�ʒu��ǂݍ���
			}
		} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

		// �X�e�[�W�͈͂̐ݒ�
		pStage->SetStageLimit(stageLimit);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�n�ʏ��̓Ǎ�����
//============================================================
HRESULT CStage::LoadField(const char* pString, FILE *pFile, CStage *pStage)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXVECTOR2 size = VEC3_ZERO;	// �傫���̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p

	int nCurrentID = 0;	// ���݂̓ǂݍ��ݐ��̕ۑ��p
	int nTextureID = 0;	// �e�N�X�`���C���f�b�N�X�̑���p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr || pStage == nullptr)
	{ // ������E�t�@�C���E�X�e�[�W�����݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�ʂ̐ݒ�
	if (strcmp(pString, "STAGE_FIELDSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_FIELDSET �̏ꍇ

		// ���݂̓ǂݍ��ݐ���������
		nCurrentID = 0;

		do
		{ // �ǂݍ��񂾕����� END_STAGE_FIELDSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // �ǂݍ��񂾕����� NUM �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%d", &pStage->m_field.nNum);	// �ǂݍ��ݐ���ǂݍ���

				if (pStage->m_field.nNum > 0)
				{ // �ǂݍ��ނ��̂�����ꍇ

					if (pStage->m_field.ppField == nullptr)
					{ // �n�ʂ��g�p����Ă��Ȃ��ꍇ

						// �n�ʂ̓ǂݍ��ݐ����������m��
						pStage->m_field.ppField = new CField*[pStage->m_field.nNum];

						if (pStage->m_field.ppField != nullptr)
						{ // �m�ۂɐ��������ꍇ

							// �������N���A
							memset(pStage->m_field.ppField, 0, sizeof(CField*) * pStage->m_field.nNum);
						}
						else { assert(false); return E_FAIL; }	// �m�ێ��s
					}
					else { assert(false); return E_FAIL; }	// �g�p��
				}
				else
				{ // �ǂݍ��ނ��̂��Ȃ��ꍇ

					// �����𔲂���
					break;
				}
			}
			else if (strcmp(&aString[0], "FIELDSET") == 0)
			{ // �ǂݍ��񂾕����� FIELDSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TEXTURE_ID") == 0)
					{ // �ǂݍ��񂾕����� TEXTURE_ID �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nTextureID);	// �e�N�X�`���C���f�b�N�X��ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
				} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v

				if (pStage->m_field.ppField[nCurrentID] == nullptr)
				{ // �g�p����Ă��Ȃ��ꍇ

					// �n�ʃI�u�W�F�N�g�̐���
					pStage->m_field.ppField[nCurrentID] = CField::Create((CField::ETexture)nTextureID, pos, D3DXToRadian(rot), size, col, part);
					if (pStage->m_field.ppField[nCurrentID] == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// �g�p��

				// �Ǎ������I�[�o�[
				assert(nCurrentID < pStage->m_field.nNum);

				// ���݂̓ǂݍ��ݐ������Z
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_FIELDSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_FIELDSET �ł͂Ȃ��ꍇ���[�v

		// �Ǎ������̕s��v
		assert(nCurrentID == pStage->m_field.nNum);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�Ǐ��̓Ǎ�����
//============================================================
HRESULT CStage::LoadWall(const char* pString, FILE *pFile, CStage *pStage)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXVECTOR2 size = VEC3_ZERO;	// �傫���̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p

	int nCurrentID = 0;	// ���݂̓ǂݍ��ݐ��̕ۑ��p
	int nTextureID = 0;	// �e�N�X�`���C���f�b�N�X�̑���p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr || pStage == nullptr)
	{ // ������E�t�@�C���E�X�e�[�W�����݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ǂ̐ݒ�
	if (strcmp(pString, "STAGE_WALLSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_WALLSET �̏ꍇ

		// ���݂̓ǂݍ��ݐ���������
		nCurrentID = 0;

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // �ǂݍ��񂾕����� NUM �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%d", &pStage->m_wall.nNum);	// �ǂݍ��ݐ���ǂݍ���

				if (pStage->m_wall.nNum > 0)
				{ // �ǂݍ��ނ��̂�����ꍇ

					if (pStage->m_wall.ppWall == nullptr)
					{ // �ǂ��g�p����Ă��Ȃ��ꍇ

						// �ǂ̓ǂݍ��ݐ����������m��
						pStage->m_wall.ppWall = new CWall*[pStage->m_wall.nNum];

						if (pStage->m_wall.ppWall != nullptr)
						{ // �m�ۂɐ��������ꍇ

							// �������N���A
							memset(pStage->m_wall.ppWall, 0, sizeof(CWall*) * pStage->m_wall.nNum);
						}
						else { assert(false); return E_FAIL; }	// �m�ێ��s
					}
					else { assert(false); return E_FAIL; }	// �g�p��
				}
				else
				{ // �ǂݍ��ނ��̂��Ȃ��ꍇ

					// �����𔲂���
					break;
				}
			}
			else if (strcmp(&aString[0], "WALLSET") == 0)
			{ // �ǂݍ��񂾕����� WALLSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_WALLSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TEXTURE_ID") == 0)
					{ // �ǂݍ��񂾕����� TEXTURE_ID �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nTextureID);	// �e�N�X�`���C���f�b�N�X��ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
				} while (strcmp(&aString[0], "END_WALLSET") != 0);	// �ǂݍ��񂾕����� END_WALLSET �ł͂Ȃ��ꍇ���[�v

				if (pStage->m_wall.ppWall[nCurrentID] == nullptr)
				{ // �g�p����Ă��Ȃ��ꍇ

					// �ǃI�u�W�F�N�g�̐���
					pStage->m_wall.ppWall[nCurrentID] = CWall::Create((CWall::ETexture)nTextureID, pos, D3DXToRadian(rot), size, col, part);
					if (pStage->m_wall.ppWall[nCurrentID] == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// �g�p��

				// �Ǎ������I�[�o�[
				assert(nCurrentID < pStage->m_wall.nNum);

				// ���݂̓ǂݍ��ݐ������Z
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_WALLSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_WALLSET �ł͂Ȃ��ꍇ���[�v

		// �Ǎ������̕s��v
		assert(nCurrentID == pStage->m_wall.nNum);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�i�F���̓Ǎ�����
//============================================================
HRESULT CStage::LoadScenery(const char* pString, FILE *pFile, CStage *pStage)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p

	float fRadius = 0.0f;	// ���a�̑���p
	float fHeight = 0.0f;	// �c���̑���p
	int nTextureID = 0;		// �e�N�X�`���C���f�b�N�X�̑���p
	int nCurrentID = 0;		// ���݂̓ǂݍ��ݐ��̕ۑ��p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr || pStage == nullptr)
	{ // ������E�t�@�C���E�X�e�[�W�����݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �i�F�̐ݒ�
	if (strcmp(pString, "STAGE_SCENERYSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_SCENERYSET �̏ꍇ

		// ���݂̓ǂݍ��ݐ���������
		nCurrentID = 0;

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // �ǂݍ��񂾕����� NUM �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%d", &pStage->m_scenery.nNum);	// �ǂݍ��ݐ���ǂݍ���

				if (pStage->m_scenery.nNum > 0)
				{ // �ǂݍ��ނ��̂�����ꍇ

					if (pStage->m_scenery.ppScenery == nullptr)
					{ // �i�F���g�p����Ă��Ȃ��ꍇ

						// �i�F�̓ǂݍ��ݐ����������m��
						pStage->m_scenery.ppScenery = new CScenery*[pStage->m_scenery.nNum];

						if (pStage->m_scenery.ppScenery != nullptr)
						{ // �m�ۂɐ��������ꍇ

							// �������N���A
							memset(pStage->m_scenery.ppScenery, 0, sizeof(CScenery*) * pStage->m_scenery.nNum);
						}
						else { assert(false); return E_FAIL; }	// �m�ێ��s
					}
					else { assert(false); return E_FAIL; }	// �g�p��
				}
				else
				{ // �ǂݍ��ނ��̂��Ȃ��ꍇ

					// �����𔲂���
					break;
				}
			}
			else if (strcmp(&aString[0], "SCENERYSET") == 0)
			{ // �ǂݍ��񂾕����� SCENERYSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_SCENERYSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TEXTURE_ID") == 0)
					{ // �ǂݍ��񂾕����� TEXTURE_ID �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nTextureID);	// �e�N�X�`���C���f�b�N�X��ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fHeight);		// �c����ǂݍ���
					}
				} while (strcmp(&aString[0], "END_SCENERYSET") != 0);	// �ǂݍ��񂾕����� END_SCENERYSET �ł͂Ȃ��ꍇ���[�v

				if (pStage->m_scenery.ppScenery[nCurrentID] == nullptr)
				{ // �g�p����Ă��Ȃ��ꍇ

					// �i�F�I�u�W�F�N�g�̐���
					pStage->m_scenery.ppScenery[nCurrentID] = CScenery::Create((CScenery::ETexture)nTextureID, pos, D3DXToRadian(rot), col, part, texPart, fRadius, fHeight);
					if (pStage->m_scenery.ppScenery[nCurrentID] == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// �g�p��

				// �Ǎ������I�[�o�[
				assert(nCurrentID < pStage->m_scenery.nNum);

				// ���݂̓ǂݍ��ݐ������Z
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_SCENERYSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v

		// �Ǎ������̕s��v
		assert(nCurrentID == pStage->m_scenery.nNum);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	����̓Ǎ�����
//============================================================
HRESULT CStage::LoadSky(const char* pString, FILE *pFile, CStage *pStage)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p

	float fRadius = 0.0f;	// ���a�̑���p
	int nTextureID = 0;		// �e�N�X�`���C���f�b�N�X�̑���p
	int nCurrentID = 0;		// ���݂̓ǂݍ��ݐ��̕ۑ��p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr || pStage == nullptr)
	{ // ������E�t�@�C���E�X�e�[�W�����݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ��̐ݒ�
	if (strcmp(pString, "STAGE_SKYSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_SKYSET �̏ꍇ

		// ���݂̓ǂݍ��ݐ���������
		nCurrentID = 0;

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SKYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // �ǂݍ��񂾕����� NUM �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%d", &pStage->m_sky.nNum);	// �ǂݍ��ݐ���ǂݍ���

				if (pStage->m_sky.nNum > 0)
				{ // �ǂݍ��ނ��̂�����ꍇ

					if (pStage->m_sky.ppSky == nullptr)
					{ // �󂪎g�p����Ă��Ȃ��ꍇ

						// ��̓ǂݍ��ݐ����������m��
						pStage->m_sky.ppSky = new CSky*[pStage->m_sky.nNum];

						if (pStage->m_sky.ppSky != nullptr)
						{ // �m�ۂɐ��������ꍇ

							// �������N���A
							memset(pStage->m_sky.ppSky, 0, sizeof(CSky*) * pStage->m_sky.nNum);
						}
						else { assert(false); return E_FAIL; }	// �m�ێ��s
					}
					else { assert(false); return E_FAIL; }	// �g�p��
				}
				else
				{ // �ǂݍ��ނ��̂��Ȃ��ꍇ

					// �����𔲂���
					break;
				}
			}
			else if (strcmp(&aString[0], "SKYSET") == 0)
			{ // �ǂݍ��񂾕����� SKYSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SKYSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TEXTURE_ID") == 0)
					{ // �ǂݍ��񂾕����� TEXTURE_ID �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nTextureID);	// �e�N�X�`���C���f�b�N�X��ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_SKYSET") != 0);	// �ǂݍ��񂾕����� END_SKYSET �ł͂Ȃ��ꍇ���[�v

				if (pStage->m_sky.ppSky[nCurrentID] == nullptr)
				{ // �g�p����Ă��Ȃ��ꍇ

					// ��I�u�W�F�N�g�̐���
					pStage->m_sky.ppSky[nCurrentID] = CSky::Create((CSky::ETexture)nTextureID, pos, D3DXToRadian(rot), col, part, texPart, fRadius);
					if (pStage->m_sky.ppSky[nCurrentID] == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// �g�p��

				// �Ǎ������I�[�o�[
				assert(nCurrentID < pStage->m_sky.nNum);

				// ���݂̓ǂݍ��ݐ������Z
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_SKYSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_SKYSET �ł͂Ȃ��ꍇ���[�v

		// �Ǎ������̕s��v
		assert(nCurrentID == pStage->m_sky.nNum);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�t�̏��̓Ǎ�����
//============================================================
HRESULT CStage::LoadLiquid(const char* pString, FILE *pFile, CStage *pStage)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXVECTOR2 size = VEC3_ZERO;	// �傫���̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	CLiquid::STexMove texMove;		// �e�N�X�`���ړ��ʂ̑���p

	float fMaxUp = 0.0f;		// �g�̍ō��㏸��
	float fAddSinRot = 0.0f;	// �g�ł��������Z��
	float fAddVtxRot = 0.0f;	// �הg�̌������Z��
	int nTypeID = 0;			// ��ރC���f�b�N�X�̑���p
	int nCurrentID = 0;			// ���݂̓ǂݍ��ݐ��̕ۑ��p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr || pStage == nullptr)
	{ // ������E�t�@�C���E�X�e�[�W�����݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �t�̂̐ݒ�
	if (strcmp(pString, "STAGE_LIQUIDSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_LIQUIDSET �̏ꍇ

		// ���݂̓ǂݍ��ݐ���������
		nCurrentID = 0;

		do
		{ // �ǂݍ��񂾕����� END_STAGE_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // �ǂݍ��񂾕����� NUM �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%d", &pStage->m_liquid.nNum);	// �ǂݍ��ݐ���ǂݍ���

				if (pStage->m_liquid.nNum > 0)
				{ // �ǂݍ��ނ��̂�����ꍇ

					if (pStage->m_liquid.ppLiquid == nullptr)
					{ // �t�̂��g�p����Ă��Ȃ��ꍇ

						// �t�̂̓ǂݍ��ݐ����������m��
						pStage->m_liquid.ppLiquid = new CLiquid*[pStage->m_liquid.nNum];

						if (pStage->m_liquid.ppLiquid != nullptr)
						{ // �m�ۂɐ��������ꍇ

							// �������N���A
							memset(pStage->m_liquid.ppLiquid, 0, sizeof(CLiquid*) * pStage->m_liquid.nNum);
						}
						else { assert(false); return E_FAIL; }	// �m�ێ��s
					}
					else { assert(false); return E_FAIL; }	// �g�p��
				}
				else
				{ // �ǂݍ��ނ��̂��Ȃ��ꍇ

					// �����𔲂���
					break;
				}
			}
			else if (strcmp(&aString[0], "LIQUIDSET") == 0)
			{ // �ǂݍ��񂾕����� LIQUIDSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nTypeID);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXMOVE_LOW") == 0)
					{ // �ǂݍ��񂾕����� TEXMOVE_LOW �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &texMove.texMoveLow.x);	// ���t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
						fscanf(pFile, "%f", &texMove.texMoveLow.y);	// ���t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXMOVE_HIGH") == 0)
					{ // �ǂݍ��񂾕����� TEXMOVE_HIGH �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &texMove.texMoveHigh.x);	// ��t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
						fscanf(pFile, "%f", &texMove.texMoveHigh.y);	// ��t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "MAX_UP") == 0)
					{ // �ǂݍ��񂾕����� MAX_UP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fMaxUp);		// �g�̍ō��㏸�ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "ADD_SINROT") == 0)
					{ // �ǂݍ��񂾕����� ADD_SINROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fAddSinRot);	// �g�ł��������Z�ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "ADD_VTXROT") == 0)
					{ // �ǂݍ��񂾕����� ADD_VTXROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fAddVtxRot);	// �הg�̌������Z�ʂ�ǂݍ���
					}
				} while (strcmp(&aString[0], "END_LIQUIDSET") != 0);	// �ǂݍ��񂾕����� END_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

				if (pStage->m_liquid.ppLiquid[nCurrentID] == nullptr)
				{ // �g�p����Ă��Ȃ��ꍇ

					// �t�̃I�u�W�F�N�g�̐���
					pStage->m_liquid.ppLiquid[nCurrentID] = CLiquid::Create
					( // ����
						(CLiquid::EType)nTypeID,	// ���
						pos,						// �ʒu
						D3DXToRadian(rot),			// ����
						size,						// �傫��
						col,						// �F
						part,						// ������
						texMove,					// �e�N�X�`���ړ���
						fMaxUp,						// �g�̍ō��㏸��
						D3DXToRadian(fAddSinRot),	// �g�ł��������Z��
						D3DXToRadian(fAddVtxRot)	// �הg�̌������Z��
					);
					if (pStage->m_liquid.ppLiquid[nCurrentID] == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// �g�p��

				// �Ǎ������I�[�o�[
				assert(nCurrentID < pStage->m_liquid.nNum);

				// ���݂̓ǂݍ��ݐ������Z
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_LIQUIDSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

		// �Ǎ������̕s��v
		assert(nCurrentID == pStage->m_liquid.nNum);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//
//	�^�C�}�[�}�l�[�W���[���� [timerManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timerManager.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "value.h"
#include "object2D.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\timer000.png",	// ��؂�\��
		"data\\TEXTURE\\timer001.png",	// ���S�\��
	};

	const int	PRIORITY	= 6;					// �^�C�}�[�̗D�揇��
	const DWORD	TIME_NUMMIN	= (DWORD)(0);			// �ŏ��^�C��
	const DWORD	TIME_NUMMAX	= (DWORD)(99 * 60000);	// �ő�^�C��
	const DWORD	TIME_NUMRED	= (DWORD)(1 * 60000);	// �Ԃ��Ȃ�^�C��

	const D3DXVECTOR3 ADDPOS_LOGO	= D3DXVECTOR3(-330.0f, 0.0f, 0.0f);					// ���S�̈ʒu���Z��
	const D3DXVECTOR3 SIZE_LOGO		= D3DXVECTOR3(1234.0f * 0.5f, 238.0f * 0.5f, 0.0f);	// ���S�̑傫��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CTimerManager::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CTimerManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTimerManager::CTimerManager() :
	m_pLogo				(nullptr),		// ���S�̏��
	m_pos				(VEC3_ZERO),	// �ʒu
	m_sizeValue			(VEC3_ZERO),	// �����̑傫��
	m_sizePart			(VEC3_ZERO),	// ��؂�̑傫��
	m_spaceValue		(VEC3_ZERO),	// �����̋�
	m_spacePart			(VEC3_ZERO),	// ��؂�̋�
	m_dwStartTime		(0),			// �J�n����
	m_dwTime			(0),			// �o�ߎ���
	m_dwStopStartTime	(0),			// ��~�J�n����
	m_dwStopTime		(0),			// ��~����
	m_dwTempTime		(0),			// �o�ߎ��Ԃ̌v�Z�p
	m_state				(STATE_NONE),	// �v�����
	m_bStop				(true),			// �v����~��
	m_nLimit			(0)				// ��������
{
	// �����o�ϐ����N���A
	memset(&m_apValue[0],	0, sizeof(m_apValue));	// ���l�̏��
	memset(&m_apPart[0],	0, sizeof(m_apPart));	// ��؂�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTimerManager::~CTimerManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTimerManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apValue[0],	0, sizeof(m_apValue));	// ���l�̏��
	memset(&m_apPart[0],	0, sizeof(m_apPart));	// ��؂�̏��
	m_pLogo				= nullptr;		// ���S�̏��
	m_pos				= VEC3_ZERO;	// �ʒu
	m_sizeValue			= VEC3_ZERO;	// �����̑傫��
	m_sizePart			= VEC3_ZERO;	// ��؂�̑傫��
	m_spaceValue		= VEC3_ZERO;	// �����̋�
	m_spacePart			= VEC3_ZERO;	// ��؂�̋�
	m_dwStartTime		= 0;			// �J�n����
	m_dwTime			= 0;			// �o�ߎ���
	m_dwStopStartTime	= 0;			// ��~�J�n����
	m_dwStopTime		= 0;			// ��~����
	m_dwTempTime		= 0;			// �o�ߎ��Ԃ̌v�Z�p
	m_state				= STATE_NONE;	// �v�����
	m_bStop				= true;			// �v����~��
	m_nLimit			= 0;			// ��������

	for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̐���
		m_apValue[nCntTimer] = CValue::Create(CValue::TEXTURE_NORMAL);
		if (m_apValue[nCntTimer] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apValue[nCntTimer]->SetPriority(PRIORITY);
	}

	for (int nCntTimer = 0; nCntTimer < timer::MAX_PART; nCntTimer++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̐���
		m_apPart[nCntTimer] = CObject2D::Create(VEC3_ZERO);
		if (m_apPart[nCntTimer] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apPart[nCntTimer]->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_PART]));

		// �D�揇�ʂ�ݒ�
		m_apPart[nCntTimer]->SetPriority(PRIORITY);
	}

	// ���S�̐���
	m_pLogo = CObject2D::Create(VEC3_ZERO, SIZE_LOGO);
	if (m_pLogo == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pLogo->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_LOGO]));

	// �D�揇�ʂ�ݒ�
	m_pLogo->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pLogo->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTimerManager::Uninit(void)
{
	// �����I�u�W�F�N�g��j��
	for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̏I��
		SAFE_UNINIT(m_apValue[nCntTimer]);
	}

	// ��؂�I�u�W�F�N�g��j��
	for (int nCntTimer = 0; nCntTimer < timer::MAX_PART; nCntTimer++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̏I��
		SAFE_UNINIT(m_apPart[nCntTimer]);
	}

	// ���S�̏I��
	SAFE_UNINIT(m_pLogo);
}

//============================================================
//	�X�V����
//============================================================
void CTimerManager::Update(void)
{
	switch (m_state)
	{ // �v����Ԃ��Ƃ̏���
	case STATE_NONE:

		// ����

		break;

	case STATE_MEASURE:

		if (m_bStop == false)
		{ // �v�����̏ꍇ

			if (m_nLimit <= 0)
			{ // �������Ԃ� 0�ȉ��̏ꍇ

				// ���݂̌v���~���b��ݒ�
				m_dwTime = timeGetTime() - m_dwTempTime;
			}
			else
			{ // �������Ԃ� 0���傫���ꍇ

				// �ϐ���錾
				long nTime = m_nLimit - (timeGetTime() - m_dwTempTime);	// ���݃^�C��

				if (nTime > 0)
				{ // �^�C���� 0���傫���ꍇ

					// ���݂̌v���~���b��ݒ�
					m_dwTime = nTime;

					if (m_dwTime <= TIME_NUMRED)
					{ // �^�C�}�[���c��ꕪ�̏ꍇ

						// �F��Ԃɐݒ�
						SetColor(XCOL_RED);
					}
				}
				else
				{  // �^�C���� 0�ȉ��̏ꍇ

					// ���݂̌v���~���b��ݒ�
					m_dwTime = 0;

					// �v�����I������
					End();
				}
			}
		}
		else
		{ // �v����~���̏ꍇ

			// ���݂̒�~�~���b��ݒ�
			m_dwStopTime = timeGetTime() - m_dwStopStartTime;
		}

		break;

	case STATE_END:

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();

	for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̍X�V
		m_apValue[nCntTimer]->Update();
	}

	for (int nCntTimer = 0; nCntTimer < timer::MAX_PART; nCntTimer++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̍X�V
		m_apPart[nCntTimer]->Update();
	}

	// ���S�̍X�V
	m_pLogo->Update();

	// �f�o�b�O�\��
	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�^�C�}�[�F[%d:%d:%d]\n", m_dwTime / 60000, (m_dwTime / 1000) % 60, m_dwTime % 1000);
	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "��~�^�C�}�[�F[%d:%d:%d]\n", m_dwStopTime / 60000, (m_dwStopTime / 1000) % 60, m_dwStopTime % 1000);
}

//============================================================
//	��������
//============================================================
CTimerManager *CTimerManager::Create
(
	const ETime time,				// �ݒ�^�C��
	const long nTime,				// ��������
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSizeValue,	// �����̑傫��
	const D3DXVECTOR3& rSizePart,	// ��؂�̑傫��
	const D3DXVECTOR3& rSpaceValue,	// �����̋�
	const D3DXVECTOR3& rSpacePart	// ��؂�̋�
)
{
	// �^�C�}�[�}�l�[�W���[�̐���
	CTimerManager *pTimerManager = new CTimerManager;
	if (pTimerManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C�}�[�}�l�[�W���[�̏�����
		if (FAILED(pTimerManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C�}�[�}�l�[�W���[�̔j��
			SAFE_DELETE(pTimerManager);
			return nullptr;
		}

		// �������Ԃ�ݒ�
		pTimerManager->SetLimit(time, nTime);

		// �ʒu��ݒ�
		pTimerManager->SetPosition(rPos);

		// �����̑傫����ݒ�
		pTimerManager->SetScalingValue(rSizeValue);

		// ��؂�̑傫����ݒ�
		pTimerManager->SetScalingPart(rSizePart);

		// �����̋󔒂�ݒ�
		pTimerManager->SetSpaceValue(rSpaceValue);

		// ��؂�̋󔒂�ݒ�
		pTimerManager->SetSpacePart(rSpacePart);

		// �m�ۂ����A�h���X��Ԃ�
		return pTimerManager;
	}
}

//============================================================
//	�j������
//============================================================
void CTimerManager::Release(CTimerManager *&prTimerManager)
{
	// �^�C�}�[�}�l�[�W���[�̏I��
	assert(prTimerManager != nullptr);
	prTimerManager->Uninit();

	// �������J��
	SAFE_DELETE(prTimerManager);
}

//============================================================
//	�ŏ��^�C���擾����
//============================================================
DWORD CTimerManager::GetMinTime(void)
{
	// �ŏ��^�C����Ԃ�
	return TIME_NUMMIN;
}

//============================================================
//	�ő�^�C���擾����
//============================================================
DWORD CTimerManager::GetMaxTime(void)
{
	// �ő�^�C����Ԃ�
	return TIME_NUMMAX;
}

//============================================================
//	�v���J�n����
//============================================================
void CTimerManager::Start(void)
{
	if (m_state != STATE_MEASURE)
	{ // �^�C���̌v�����ł͂Ȃ��ꍇ

		// �J�n�������擾
		m_dwStartTime = timeGetTime();
		m_dwTempTime  = m_dwStartTime;

		// ���~��Ԃɂ���
		EnableStop(false);

		// �v���J�n��Ԃɂ���
		m_state = STATE_MEASURE;
	}
}

//============================================================
//	�v���I������
//============================================================
void CTimerManager::End(void)
{
	// �v���I����Ԃɂ���
	m_state = STATE_END;
}

//============================================================
//	�v����~�̗L�������̐ݒ菈��
//============================================================
void CTimerManager::EnableStop(const bool bStop)
{
	// �����̒�~�󋵂���
	m_bStop = bStop;

	if (bStop)
	{ // ��~����ꍇ

		// ��~�J�n�������擾
		m_dwStopStartTime = timeGetTime();
	}
	else
	{ // �ĊJ����ꍇ

		// ��~���Ԃ����Z
		m_dwTempTime += m_dwStopTime;

		// ��~�֘A�̎��Ԃ�������
		m_dwStopStartTime = 0;	// ��~�J�n����
		m_dwStopTime = 0;		// ��~����
	}
}

//============================================================
//	�v����Ԏ擾����
//============================================================
CTimerManager::EState CTimerManager::GetState(void)
{
	// �v����Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	�~���b�̉��Z����
//============================================================
bool CTimerManager::AddMSec(long nMSec)
{
	// �ϐ���錾
	bool bAdd = true;	// ���Z��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}
	}

	if (bAdd)
	{ // ���Z�ł���ꍇ

		// ���Z�ʂ̕␳
		useful::LimitNum(nMSec, -(long)m_dwTime, (long)TIME_NUMMAX);

		// �����̃~���b�����Z
		m_dwTempTime -= (DWORD)nMSec;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// ���Z�̐������s��Ԃ�
	return bAdd;
}

//============================================================
//	�b�̉��Z����
//============================================================
bool CTimerManager::AddSec(long nSec)
{
	// �ϐ���錾
	bool bAdd = true;	// ���Z��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}
	}

	if (bAdd)
	{ // ���Z�ł���ꍇ

		// �������~���b�ɕϊ�
		nSec *= 1000;

		// ���Z�ʂ̕␳
		useful::LimitNum(nSec, -(long)m_dwTime, (long)TIME_NUMMAX);

		// �����̕b�����Z
		m_dwTempTime -= (DWORD)nSec;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// ���Z�̐������s��Ԃ�
	return bAdd;
}

//============================================================
//	���̉��Z����
//============================================================
bool CTimerManager::AddMin(long nMin)
{
	// �ϐ���錾
	bool bAdd = true;	// ���Z��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}
	}

	if (bAdd)
	{ // ���Z�ł���ꍇ

		// �������~���b�ɕϊ�
		nMin *= 60000;

		// ���Z�ʂ̕␳
		useful::LimitNum(nMin, -(long)m_dwTime, (long)TIME_NUMMAX);

		// �����̕������Z
		m_dwTempTime -= (DWORD)nMin;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// ���Z�̐������s��Ԃ�
	return bAdd;
}

//============================================================
//	�~���b�̐ݒ菈��
//============================================================
bool CTimerManager::SetMSec(long nMSec)
{
	// �ϐ���錾
	bool bSet = true;	// �ݒ��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}
	}

	if (bSet)
	{ // �ݒ�ł���ꍇ

		// ���Z�ʂ̕␳
		useful::LimitNum(nMSec, -(long)m_dwTime, (long)TIME_NUMMAX);

		// ������ݒ�
		m_dwStartTime = timeGetTime() - nMSec;
		m_dwTempTime = m_dwStartTime;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �l��������
		m_dwStopStartTime	= 0;	// ��~�J�n����
		m_dwStopTime		= 0;	// ��~����

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// �ݒ�̐������s��Ԃ�
	return bSet;
}

//============================================================
//	�b�̐ݒ菈��
//============================================================
bool CTimerManager::SetSec(long nSec)
{
	// �ϐ���錾
	bool bSet = true;	// �ݒ��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}
		
		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}
	}

	if (bSet)
	{ // �ݒ�ł���ꍇ

		// �������~���b�ɕϊ�
		nSec *= 1000;

		// ���Z�ʂ̕␳
		useful::LimitNum(nSec, -(long)m_dwTime, (long)TIME_NUMMAX);

		// ������ݒ�
		m_dwStartTime = timeGetTime() - nSec;
		m_dwTempTime = m_dwStartTime;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �l��������
		m_dwStopStartTime	= 0;	// ��~�J�n����
		m_dwStopTime		= 0;	// ��~����

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// �ݒ�̐������s��Ԃ�
	return bSet;
}

//============================================================
//	���̐ݒ菈��
//============================================================
bool CTimerManager::SetMin(long nMin)
{
	// �ϐ���錾
	bool bSet = true;	// �ݒ��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}
	}

	if (bSet)
	{ // �ݒ�ł���ꍇ

		// �������~���b�ɕϊ�
		nMin *= 60000;

		// ���Z�ʂ̕␳
		useful::LimitNum(nMin, -(long)m_dwTime, (long)TIME_NUMMAX);

		// ������ݒ�
		m_dwStartTime = timeGetTime() - nMin;
		m_dwTempTime = m_dwStartTime;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �l��������
		m_dwStopStartTime	= 0;	// ��~�J�n����
		m_dwStopTime		= 0;	// ��~����

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// �ݒ�̐������s��Ԃ�
	return bSet;
}

//============================================================
//	�^�C���̎擾����
//============================================================
int CTimerManager::Get(void)
{
	// �^�C����Ԃ�
	return m_dwTime;
}

//============================================================
//	�~���b�̎擾����
//============================================================
int CTimerManager::GetMSec(void)
{
	// �~���b��Ԃ�
	return m_dwTime % 1000;
}

//============================================================
//	�b�̎擾����
//============================================================
int CTimerManager::GetSec(void)
{
	// �b��Ԃ�
	return (m_dwTime / 1000) % 60;
}

//============================================================
//	���̎擾����
//============================================================
int CTimerManager::GetMin(void)
{
	// ����Ԃ�
	return m_dwTime / 60000;
}

//============================================================
//	�������Ԃ̎擾����
//============================================================
long CTimerManager::GetLimit(void)
{
	// �������Ԃ�Ԃ�
	return m_nLimit;
}

//============================================================
//	�������Ԃ̐ݒ菈��
//============================================================
void CTimerManager::SetLimit(const ETime time, const long nTime)
{
	// �ϐ���錾
	long nMSec = 0;	// �~���b�ϊ��p

	// �����̐������Ԃ�ݒ�
	switch (time)
	{ // �^�C�����Ƃ̏���
	case TIME_MSEC:	// �~���b

		m_nLimit = nTime;	// �������Ԃ�ݒ�

		break;

	case TIME_SEC:	// �b

		// �������~���b�ɕϊ�
		nMSec = nTime * 1000;
		m_nLimit = nMSec;	// �������Ԃ�ݒ�

		break;

	case TIME_MIN:	// ��

		// �������~���b�ɕϊ�
		nMSec = nTime * 60000;
		m_nLimit = nMSec;	// �������Ԃ�ݒ�

		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CTimerManager::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	�����̑傫���̐ݒ菈��
//============================================================
void CTimerManager::SetScalingValue(const D3DXVECTOR3& rSize)
{
	// �����̐����̑傫����ݒ�
	m_sizeValue = rSize;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	��؂�̑傫���̐ݒ菈��
//============================================================
void CTimerManager::SetScalingPart(const D3DXVECTOR3& rSize)
{
	// �����̋�؂�̑傫����ݒ�
	m_sizePart = rSize;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	�����̋󔒂̐ݒ菈��
//============================================================
void CTimerManager::SetSpaceValue(const D3DXVECTOR3& rSpace)
{
	// �����̐����̋󔒂�ݒ�
	m_spaceValue = rSpace;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	��؂�̋󔒂̐ݒ菈��
//============================================================
void CTimerManager::SetSpacePart(const D3DXVECTOR3& rSpace)
{
	// �����̋�؂�̋󔒂�ݒ�
	m_spacePart = rSpace;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CTimerManager::SetColor(const D3DXCOLOR& rCol)
{
	if (m_apValue[0] != nullptr)
	{ // �X�R�A�̐擪�̐��l���g�p����Ă���ꍇ

		for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT; nCntTimer++)
		{ // �^�C�}�[�̌������J��Ԃ�

			// �����̐F��ݒ�
			m_apValue[nCntTimer]->SetColor(rCol);
		}

		for (int nCntTimer = 0; nCntTimer < timer::MAX_PART; nCntTimer++)
		{ // ��؂�̐����J��Ԃ�

			// ��؂�̐F��ݒ�
			m_apPart[nCntTimer]->SetColor(rCol);
		}
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CTimerManager::SetPriority(const int nPriority)
{
	if (m_apValue[0] != nullptr)
	{ // �X�R�A�̐擪�̐��l���g�p����Ă���ꍇ

		// �����I�u�W�F�N�g�̗D�揇�ʂ�ݒ�
		for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT; nCntTimer++)
		{ // �^�C�}�[�̌������J��Ԃ�

			m_apValue[nCntTimer]->SetPriority(nPriority);
		}

		// ��؂�I�u�W�F�N�g�̗D�揇�ʂ�ݒ�
		for (int nCntTimer = 0; nCntTimer < timer::MAX_PART; nCntTimer++)
		{ // ��؂�̐����J��Ԃ�

			m_apPart[nCntTimer]->SetPriority(nPriority);
		}
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CTimerManager::SetEnableUpdate(const bool bUpdate)
{
	if (m_apValue[0] != nullptr)
	{ // �X�R�A�̐擪�̐��l���g�p����Ă���ꍇ

		// �����I�u�W�F�N�g�̍X�V�󋵂�ݒ�
		for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT; nCntTimer++)
		{ // �^�C�}�[�̌������J��Ԃ�

			m_apValue[nCntTimer]->SetEnableUpdate(bUpdate);
		}

		// ��؂�I�u�W�F�N�g�̍X�V�󋵂�ݒ�
		for (int nCntTimer = 0; nCntTimer < timer::MAX_PART; nCntTimer++)
		{ // ��؂�̐����J��Ԃ�

			m_apPart[nCntTimer]->SetEnableUpdate(bUpdate);
		}
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CTimerManager::SetEnableDraw(const bool bDraw)
{
	if (m_apValue[0] != nullptr)
	{ // �X�R�A�̐擪�̐��l���g�p����Ă���ꍇ

		// �����I�u�W�F�N�g�̕`��󋵂�ݒ�
		for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT; nCntTimer++)
		{ // �^�C�}�[�̌������J��Ԃ�

			m_apValue[nCntTimer]->SetEnableDraw(bDraw);
		}

		// ��؂�I�u�W�F�N�g�̕`��󋵂�ݒ�
		for (int nCntTimer = 0; nCntTimer < timer::MAX_PART; nCntTimer++)
		{ // ��؂�̐����J��Ԃ�

			m_apPart[nCntTimer]->SetEnableDraw(bDraw);
		}
	}
	else { assert(false); }	// ��g�p��

	// ���S�I�u�W�F�N�g�̕`��󋵂�ݒ�		
	if (m_pLogo != nullptr)
	{ // ���S���g�p����Ă���ꍇ

		m_pLogo->SetEnableDraw(bDraw);
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	���S�̕`��󋵂̐ݒ菈��
//============================================================
void CTimerManager::SetEnableLogoDraw(const bool bDraw)
{
	if (m_pLogo != nullptr)
	{ // ���S���g�p����Ă���ꍇ

		// ���������S�̕`��󋵂ɐݒ�		
		m_pLogo->SetEnableDraw(bDraw);
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̑傫���擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetScalingValue(void) const
{
	// �����̑傫����Ԃ�
	return m_sizeValue;
}

//============================================================
//	��؂�̑傫���擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetScalingPart(void) const
{
	// ��؂�̑傫����Ԃ�
	return m_sizePart;
}

//============================================================
//	�����̋󔒎擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetSpaceValue(void) const
{
	// �����̋󔒂�Ԃ�
	return m_spaceValue;
}

//============================================================
//	��؂�̋󔒎擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetSpacePart(void) const
{
	// ��؂�̋󔒂�Ԃ�
	return m_spacePart;
}

//============================================================
//	�����̕\���ݒ菈��
//============================================================
void CTimerManager::SetDrawValue(void)
{
	// �ϐ���錾
	D3DXVECTOR3 spaceValue = m_spaceValue * 0.5f;	// �����̋�
	D3DXVECTOR3 spacePart = m_spacePart * 0.5f;		// ��؂�̋�
	D3DXVECTOR3 posPoly = m_pos - spaceValue;		// �|���S�������ʒu
	int nNumValue = 0;	// �����̐�����
	int nNumPart = 0;	// ��؂�̐�����

	if (m_apValue[0] != nullptr && m_apPart[0] != nullptr)
	{ // �^�C�}�[�̐擪���l�A�^�C�}�[�̐擪��؂肪�g�p����Ă���ꍇ

		for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT + timer::MAX_PART; nCntTimer++)
		{ // �^�C�}�[�̌��� + �^�C�}�[�̋�؂萔���J��Ԃ�

			if (nCntTimer == timer::MAX_MIN || nCntTimer == timer::MAX_MIN + timer::MAX_SEC + 1)
			{ // ��؂�̐����^�C�~���O�������ꍇ

				// �|���S�������ʒu�����炷
				posPoly += spacePart;

				// ��؂�̈ʒu��ݒ�
				m_apPart[nNumValue]->SetVec3Position(posPoly);

				// ��؂�̑傫����ݒ�
				m_apPart[nNumValue]->SetVec3Sizing(m_sizePart);

				// ��؂萶���������Z
				nNumValue++;

				// �|���S�������ʒu�����炷
				posPoly += spacePart;
			}
			else
			{ // ��؂�̐����^�C�~���O�ł͂Ȃ��ꍇ

				// �|���S�������ʒu�����炷
				posPoly += spaceValue;

				// �����̈ʒu��ݒ�
				m_apValue[nNumPart]->SetVec3Position(posPoly);

				// �����̑傫����ݒ�
				m_apValue[nNumPart]->SetVec3Sizing(m_sizeValue);

				// ���������������Z
				nNumPart++;

				// �|���S�������ʒu�����炷
				posPoly += spaceValue;
			}
		}
	}
	else { assert(false); }	// ��g�p��

	if (m_pLogo != nullptr)
	{ // ���S���g�p����Ă���ꍇ

		// ���S�̈ʒu��ݒ�		
		m_pLogo->SetVec3Position(m_pos + ADDPOS_LOGO);
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CTimerManager::SetTexNum(void)
{
	// �ϐ���錾
	int aNumDivide[timer::MAX_DIGIT];	// ���̌������Ƃ̕���p

	// �����������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[0], GetMin(), timer::MAX_MIN);

	// �b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[timer::MAX_MIN], GetSec(), timer::MAX_SEC);

	// �~���b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[timer::MAX_MIN + timer::MAX_SEC], GetMSec(), timer::MAX_MSEC);

	for (int nCntTimer = 0; nCntTimer < timer::MAX_DIGIT; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̐ݒ�
		m_apValue[nCntTimer]->SetNumber(aNumDivide[nCntTimer]);
	}
}
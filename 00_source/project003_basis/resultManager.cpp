//============================================================
//
//	���U���g�}�l�[�W���[���� [resultManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "resultManager.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"
#include "timerManager.h"
#include "retentionManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\result000.png",		// �x������e�N�X�`��
		"data\\TEXTURE\\result001.png",		// �����e�N�X�`��
		"data\\TEXTURE\\result002.png",		// ���s�e�N�X�`��
		"data\\TEXTURE\\result003.png",		// �^�C���\���e�N�X�`��
		"data\\TEXTURE\\continue000.png",	// �R���e�j���[�\���e�N�X�`��
		"data\\TEXTURE\\continue001.png",	// YES�e�N�X�`��
		"data\\TEXTURE\\continue002.png",	// NO�e�N�X�`��
	};

	const int PRIORITY = 5;	// ���U���g�̗D�揇��

	const D3DXCOLOR SETCOL_FADE		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// ���l�̒�~�l
	const D3DXCOLOR INITCOL_FADE	= XCOL_AWHITE;			// ���l�̏����l
	const D3DXVECTOR3 SIZE_FADE		= SCREEN_SIZE * 0.95f;	// �t�F�[�h�̑傫��

	const float ADD_ALPHA = 0.008f;	// ���l�̉��Z��

	const D3DXVECTOR3 POS_RESULT_MISSION	= D3DXVECTOR3(380.0f, 150.0f, 0.0f);	// ���U���g�\���̒x������̈ʒu
	const D3DXVECTOR3 POS_RESULT_RESULT		= D3DXVECTOR3(960.0f, 150.0f, 0.0f);	// ���U���g�\���̐������s�̈ʒu
	const D3DXVECTOR3 SIZE_RESULT			= D3DXVECTOR3(632.7f, 203.5f, 0.0f);	// ���U���g�\���̑傫��

	const float SET_RESULT_SCALE	= 15.0f;	// ���U���g�\���̏����g�嗦
	const float SUB_RESULT_SCALE	= 0.65f;	// ���U���g�\���g�嗦�̌��Z��

	const D3DXVECTOR3 POS_TIME_LOGO		= D3DXVECTOR3(290.0f, 330.0f, 0.0f);	// �^�C�����S�ʒu
	const D3DXVECTOR3 SIZE_TIME_LOGO	= D3DXVECTOR3(487.5f, 154.7f, 0.0f);	// �^�C�����S�傫��
	const D3DXVECTOR3 POS_TIME			= D3DXVECTOR3(555.0f, 330.0f, 0.0f);	// �^�C���ʒu
	const D3DXVECTOR3 SIZE_TIME_VAL		= D3DXVECTOR3(98.0f, 117.0f, 0.0f);		// �^�C�������傫��
	const D3DXVECTOR3 SIZE_TIME_PART	= D3DXVECTOR3(50.0f, 117.0f, 0.0f);		// �^�C����؂�傫��
	const D3DXVECTOR3 SPACE_TIME_VAL	= D3DXVECTOR3(SIZE_TIME_VAL.x  * 0.85f, 0.0f, 0.0f);	// �^�C��������
	const D3DXVECTOR3 SPACE_TIME_PART	= D3DXVECTOR3(SIZE_TIME_PART.x * 0.85f, 0.0f, 0.0f);	// �^�C����؂��

	const float	SET_TIME_SCALE	= 8.0f;	// �^�C���\���̏����g�嗦
	const float	SUB_TIME_SCALE	= 0.3f;	// �^�C���\���g�嗦�̌��Z��
	const int	TIME_WAIT_CNT	= 3;	// �^�C���\����Ԃւ̕ύX�ҋ@�t���[����

	const D3DXVECTOR3 POS_CONT_LOGO		= D3DXVECTOR3(SCREEN_CENT.x, 490.0f, 0.0f);	// �R���e�j���[���S�ʒu
	const D3DXVECTOR3 SIZE_CONT_LOGO	= D3DXVECTOR3(576.0f, 172.0f, 0.0f);		// �R���e�j���[���S�傫��
	const D3DXVECTOR3 POS_CONT_YES		= D3DXVECTOR3(400.0f, 610.0f, 0.0f);		// �R���e�j���[�\����YES�̈ʒu
	const D3DXVECTOR3 POS_CONT_NO		= D3DXVECTOR3(880.0f, 610.0f, 0.0f);		// �R���e�j���[�\����NO�̈ʒu
	const D3DXVECTOR3 SIZE_CONT			= D3DXVECTOR3(222.0f, 94.0f, 0.0f);			// �R���e�j���[�\���̑傫��

	const float	SET_CONT_SCALE	= 8.0f;	// �R���e�j���[�\���̏����g�嗦
	const float	SUB_CONT_SCALE	= 0.3f;	// �R���e�j���[�\���g�嗦�̌��Z��
	const int	CONT_WAIT_CNT	= 3;	// �R���e�j���[�\����Ԃւ̕ύX�ҋ@�t���[����

	const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �I�𒆃J���[
	const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��I�𒆃J���[
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CResultManager::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�e�N���X [CResultManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CResultManager::CResultManager() :
	m_pContLogo		(nullptr),		// �R���e�j���[���S�̏��
	m_pTimeLogo		(nullptr),		// �^�C�����S�̏��
	m_pFade			(nullptr),		// �t�F�[�h�̏��
	m_pTime			(nullptr),		// �^�C���̏��
	m_state			(STATE_NONE),	// ���
	m_nSelect		(SELECT_YES),	// ���݂̑I��
	m_nOldSelect	(SELECT_YES),	// �O��̑I��
	m_fScale		(0.0f),			// �|���S���g�嗦
	m_nCounterState	(0)				// ��ԊǗ��J�E���^�[
{
	// �����o�ϐ����N���A
	memset(&m_apResult[0], 0, sizeof(m_apResult));		// ���U���g�\���̏��
	memset(&m_apContinue[0], 0, sizeof(m_apContinue));	// �R���e�j���[�\���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CResultManager::Init(void)
{
	// �ϐ��z���錾
	static D3DXVECTOR3 aPosResult[] =	// ���U���g�̈ʒu
	{
		POS_RESULT_MISSION,	// MISSION�ʒu
		POS_RESULT_RESULT,	// RESULT�ʒu
	};
	static D3DXVECTOR3 aPosContinue[] =	// �R���e�j���[�̈ʒu
	{
		POS_CONT_YES,	// YES�ʒu
		POS_CONT_NO	,	// NO�ʒu
	};

	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apResult[0], 0, sizeof(m_apResult));		// ���U���g�\���̏��
	memset(&m_apContinue[0], 0, sizeof(m_apContinue));	// �R���e�j���[�\���̏��
	m_pContLogo		= nullptr;		// �R���e�j���[���S�̏��
	m_pTimeLogo		= nullptr;		// �^�C�����S�̏��
	m_pFade			= nullptr;		// �t�F�[�h�̏��
	m_pTime			= nullptr;		// �^�C���̏��
	m_state			= STATE_FADEIN;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nSelect		= SELECT_YES;	// ���݂̑I��
	m_nOldSelect	= SELECT_YES;	// �O��̑I��
	m_fScale		= 0.0f;			// �|���S���g�嗦

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SIZE_FADE,		// �傫��
		VEC3_ZERO,		// ����
		INITCOL_FADE	// �F
	);
	if (m_pFade == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	���U���g�\���̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntResult = 0; nCntResult < result::NUM_POLYGON; nCntResult++)
	{ // ���U���g�\���̑������J��Ԃ�

		// ���U���g�\���̐���
		m_apResult[nCntResult] = CObject2D::Create
		( // ����
			aPosResult[nCntResult],			// �ʒu
			SIZE_RESULT * SET_RESULT_SCALE	// �傫��
		);
		if (m_apResult[nCntResult] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apResult[nCntResult]->SetPriority(PRIORITY);

		// �`������Ȃ��ݒ�ɂ���
		m_apResult[nCntResult]->SetEnableDraw(false);
	}

	// ���U���g�\���̃e�N�X�`����ݒ�
	SetTexResult();

	//--------------------------------------------------------
	//	�^�C�����S�\���̐����E�ݒ�
	//--------------------------------------------------------
	// �^�C�����S�\���̐���
	m_pTimeLogo = CObject2D::Create
	( // ����
		POS_TIME_LOGO,					// �ʒu
		SIZE_TIME_LOGO * SET_TIME_SCALE	// �傫��
	);
	if (m_pTimeLogo == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pTimeLogo->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_TIME]));

	// �D�揇�ʂ�ݒ�
	m_pTimeLogo->SetPriority(PRIORITY);

	// �`������Ȃ��ݒ�ɂ���
	m_pTimeLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�^�C���\���̐����E�ݒ�
	//--------------------------------------------------------
	// �^�C�}�[�}�l�[�W���[�̐���
	m_pTime = CTimerManager::Create
	( // ����
		CTimerManager::TIME_SEC,			// �ݒ�^�C��
		0,									// ��������
		POS_TIME,							// �ʒu
		SIZE_TIME_VAL * SET_TIME_SCALE,		// �����̑傫��
		SIZE_TIME_PART * SET_TIME_SCALE,	// ��؂�̑傫��
		SPACE_TIME_VAL,						// �����̋�
		SPACE_TIME_PART						// ��؂�̋�
	);
	if (m_pTime == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pTime->SetPriority(PRIORITY);

	// �`������Ȃ��ݒ�ɂ���
	m_pTime->SetEnableDraw(false);

	// �^�C����ݒ�
	if (!m_pTime->SetMSec(GET_RETENTION->GetTime()))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�R���e�j���[���S�\���̐����E�ݒ�
	//--------------------------------------------------------
	// �R���e�j���[���S�\���̐���
	m_pContLogo = CObject2D::Create
	( // ����
		POS_CONT_LOGO,					// �ʒu
		SIZE_CONT_LOGO * SET_CONT_SCALE	// �傫��
	);
	if (m_pContLogo == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pContLogo->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_CONTINUE]));

	// �D�揇�ʂ�ݒ�
	m_pContLogo->SetPriority(PRIORITY);

	// �`������Ȃ��ݒ�ɂ���
	m_pContLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�R���e�j���[�\���̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // �I�����̑������J��Ԃ�

		// �R���e�j���[�\���̐���
		m_apContinue[nCntResult] = CObject2D::Create
		( // ����
			aPosContinue[nCntResult],	// �ʒu
			SIZE_CONT * SET_CONT_SCALE	// �傫��
		);
		if (m_apContinue[nCntResult] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apContinue[nCntResult]->SetPriority(PRIORITY);

		// �`������Ȃ��ݒ�ɂ���
		m_apContinue[nCntResult]->SetEnableDraw(false);

		// �F��ݒ�
		m_apContinue[nCntResult]->SetColor(DEFAULT_COL);
	}

	// �e�N�X�`����o�^�E����
	m_apContinue[SELECT_YES]->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_YES]));
	m_apContinue[SELECT_NO]->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_NO]));

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CResultManager::Uninit(void)
{
	// �^�C���̔j��
	SAFE_REF_RELEASE(m_pTime);

	for (int nCntResult = 0; nCntResult < result::NUM_POLYGON; nCntResult++)
	{ // ���U���g�\���̑������J��Ԃ�

		// ���U���g�\���̏I��
		SAFE_UNINIT(m_apResult[nCntResult]);
	}

	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // �I�����̑������J��Ԃ�

		// �R���e�j���[�\���̏I��
		SAFE_UNINIT(m_apContinue[nCntResult]);
	}

	// �R���e�j���[���S�\���̏I��
	SAFE_UNINIT(m_pContLogo);

	// �^�C�����S�\���̏I��
	SAFE_UNINIT(m_pTimeLogo);

	// �t�F�[�h�̏I��
	SAFE_UNINIT(m_pFade);
}

//============================================================
//	�X�V����
//============================================================
void CResultManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_FADEIN:	// �t�F�[�h�C�����

		// �t�F�[�h�C���̍X�V
		UpdateFade();

		break;

	case STATE_RESULT:	// ���U���g�\�����

		// ���U���g�\���̍X�V
		UpdateResult();

		break;

	case STATE_TIME_WAIT:	// �^�C���\���ҋ@���

		// �\���ҋ@�̍X�V
		if (UpdateDrawWait(TIME_WAIT_CNT))
		{ // �ҋ@�����̏ꍇ

			// �^�C���\���̊g�嗦��ݒ�
			m_fScale = SET_TIME_SCALE;

			// �^�C���\���̕`��J�n
			m_pTimeLogo->SetEnableDraw(true);
			m_pTime->SetEnableDraw(true);

			// ��Ԃ�ύX
			m_state = STATE_TIME;	// �^�C���\�����
		}

		break;

	case STATE_TIME:	// �^�C���\�����

		// �^�C���\���̍X�V
		UpdateTime();

		break;

	case STATE_CONTINUE_WAIT:	// �R���e�j���[�\���ҋ@���

		// �\���ҋ@�̍X�V
		if (UpdateDrawWait(CONT_WAIT_CNT))
		{ // �ҋ@�����̏ꍇ

			// �R���e�j���[�\���̊g�嗦��ݒ�
			m_fScale = SET_CONT_SCALE;

			// �R���e�j���[���S�\���̕`��J�n
			m_pContLogo->SetEnableDraw(true);
			
			for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
			{ // �I�����̑������J��Ԃ�

				// �R���e�j���[�\���̕`��J�n
				m_apContinue[nCntResult]->SetEnableDraw(true);
			}

			// ��Ԃ�ύX
			m_state = STATE_CONTINUE;	// �R���e�j���[�\�����
		}

		break;

	case STATE_CONTINUE:	// �R���e�j���[�\���ҋ@��ԕ\�����

		// �R���e�j���[�\���̍X�V
		UpdateContinue();

		break;

	case STATE_WAIT:	// �J�ڑҋ@���

		// �I���̍X�V
		UpdateSelect();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �J�ڌ���̍X�V
	UpdateTransition();

	for (int nCntResult = 0; nCntResult < result::NUM_POLYGON; nCntResult++)
	{ // ���U���g�\���̑������J��Ԃ�

		// ���U���g�\���̍X�V
		m_apResult[nCntResult]->Update();
	}

	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // �I�����̑������J��Ԃ�

		// �R���e�j���[�\���̍X�V
		m_apContinue[nCntResult]->Update();
	}

	// �R���e�j���[���S�\���̍X�V
	m_pContLogo->Update();

	// �^�C�����S�\���̍X�V
	m_pTimeLogo->Update();

	// �^�C���̍X�V
	m_pTime->Update();

	// �t�F�[�h�̍X�V
	m_pFade->Update();
}

//============================================================
//	��������
//============================================================
CResultManager *CResultManager::Create(void)
{
	// ���U���g�}�l�[�W���[�̐���
	CResultManager *pResultManager = new CResultManager;
	if (pResultManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���U���g�}�l�[�W���[�̏�����
		if (FAILED(pResultManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���U���g�}�l�[�W���[�̔j��
			SAFE_DELETE(pResultManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pResultManager;
	}
}

//============================================================
//	�j������
//============================================================
void CResultManager::Release(CResultManager *&prResultManager)
{
	// ���U���g�}�l�[�W���[�̏I��
	assert(prResultManager != nullptr);
	prResultManager->Uninit();

	// �������J��
	SAFE_DELETE(prResultManager);
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CResultManager::UpdateFade(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	if (colFade.a < SETCOL_FADE.a)
	{ // �����ʂ��ݒ�l�����̏ꍇ

		// �����x�����Z
		colFade.a += ADD_ALPHA;
	}
	else
	{ // �����ʂ��ݒ�l�ȏ�̏ꍇ

		// �����x��␳
		colFade.a = SETCOL_FADE.a;

		for (int nCntResult = 0; nCntResult < result::NUM_POLYGON; nCntResult++)
		{ // ���U���g�\���̑������J��Ԃ�

			// ���U���g�\���̕`��J�n
			m_apResult[nCntResult]->SetEnableDraw(true);
		}

		// ���U���g�\���̊g�嗦��ݒ�
		m_fScale = SET_RESULT_SCALE;

		// ��Ԃ�ύX
		m_state = STATE_RESULT;	// ���U���g�\�����
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	���U���g�\���̍X�V����
//============================================================
void CResultManager::UpdateResult(void)
{
	if (m_fScale > 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale -= SUB_RESULT_SCALE;

		for (int nCntResult = 0; nCntResult < result::NUM_POLYGON; nCntResult++)
		{ // ���U���g�\���̑������J��Ԃ�

			// ���U���g�\���̑傫����ݒ�
			m_apResult[nCntResult]->SetVec3Sizing(SIZE_RESULT * m_fScale);
		}
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		for (int nCntResult = 0; nCntResult < result::NUM_POLYGON; nCntResult++)
		{ // ���U���g�\���̑������J��Ԃ�

			// ���U���g�\���̑傫����ݒ�
			m_apResult[nCntResult]->SetVec3Sizing(SIZE_RESULT);
		}

		switch (GET_RETENTION->GetResult())
		{ // ���U���g���Ƃ̏���
		case CRetentionManager::RESULT_FAILED:

			// ��Ԃ�ύX
			m_state = STATE_CONTINUE_WAIT;	// �R���e�j���[�\���ҋ@���

			break;

		case CRetentionManager::RESULT_CLEAR:

			// ��Ԃ�ύX
			m_state = STATE_TIME_WAIT;	// �^�C���\���ҋ@���

			break;

		default:

			// �G���[���b�Z�[�W�{�b�N�X
			MessageBox(nullptr, "���U���g�Ȃ����ݒ肳��Ă��܂�", "�x���I", MB_ICONWARNING);

			// ��Ԃ�ύX
			m_state = STATE_TIME_WAIT;	// �^�C���\���ҋ@���

			break;
		}

		// �T�E���h�̍Đ�
		GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
	}
}

//============================================================
//	�^�C���\������
//============================================================
void CResultManager::UpdateTime(void)
{
	if (m_fScale > 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale -= SUB_TIME_SCALE;

		// �^�C���\���̑傫����ݒ�
		m_pTimeLogo->SetVec3Sizing(SIZE_TIME_LOGO * m_fScale);
		m_pTime->SetScalingValue(SIZE_TIME_VAL * m_fScale);
		m_pTime->SetScalingPart(SIZE_TIME_PART * m_fScale);
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		// �g�嗦��␳
		m_fScale = 1.0f;

		// �^�C���\���̑傫����ݒ�
		m_pTimeLogo->SetVec3Sizing(SIZE_TIME_LOGO);
		m_pTime->SetScalingValue(SIZE_TIME_VAL);
		m_pTime->SetScalingPart(SIZE_TIME_PART);

		// ��Ԃ�ύX
		m_state = STATE_CONTINUE_WAIT;	// �R���e�j���[�\���ҋ@���

		// �T�E���h�̍Đ�
		GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
	}
}

//============================================================
//	�R���e�j���[�\������
//============================================================
void CResultManager::UpdateContinue(void)
{
	if (m_fScale > 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale -= SUB_CONT_SCALE;

		// �R���e�j���[���S�\���̑傫����ݒ�
		m_pContLogo->SetVec3Sizing(SIZE_CONT_LOGO * m_fScale);

		for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
		{ // �I�����̑������J��Ԃ�

			// �R���e�j���[�\���̑傫����ݒ�
			m_apContinue[nCntResult]->SetVec3Sizing(SIZE_CONT * m_fScale);
		}
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		// �g�嗦��␳
		m_fScale = 1.0f;

		// �R���e�j���[���S�\���̑傫����ݒ�
		m_pContLogo->SetVec3Sizing(SIZE_CONT_LOGO);

		for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
		{ // �I�����̑������J��Ԃ�

			// �R���e�j���[�\���̑傫����ݒ�
			m_apContinue[nCntResult]->SetVec3Sizing(SIZE_CONT);
		}

		// ��Ԃ�ύX
		m_state = STATE_WAIT;	// �J�ڑҋ@���

		// �T�E���h�̍Đ�
		GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
	}
}

//============================================================
//	�I���̍X�V����
//============================================================
void CResultManager::UpdateSelect(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// �L�[�{�[�h
	CInputPad		*pPad		= GET_INPUTPAD;		// �p�b�h

	if (pKeyboard->IsTrigger(DIK_A)
	||  pKeyboard->IsTrigger(DIK_LEFT)
	||  pPad->IsTrigger(CInputPad::KEY_LEFT))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// ���ɑI�������炷
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// �T�E���h�̍Đ�
		GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}
	if (pKeyboard->IsTrigger(DIK_D)
	||  pKeyboard->IsTrigger(DIK_RIGHT)
	||  pPad->IsTrigger(CInputPad::KEY_RIGHT))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �E�ɑI�������炷
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		// �T�E���h�̍Đ�
		GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}

	// �O��̑I��v�f�̐F�����ɐݒ�
	m_apContinue[m_nOldSelect]->SetColor(DEFAULT_COL);

	// ���݂̑I��v�f�̐F�𔒂ɐݒ�
	m_apContinue[m_nSelect]->SetColor(CHOICE_COL);

	// ���݂̑I��v�f����
	m_nOldSelect = m_nSelect;
}

//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CResultManager::UpdateTransition(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// �L�[�{�[�h
	CInputPad		*pPad		= GET_INPUTPAD;		// �p�b�h

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y)
	||  pPad->IsTrigger(CInputPad::KEY_START))
	{
		if (m_state != STATE_WAIT)
		{ // �J�ڑҋ@��Ԃł͂Ȃ��ꍇ

			// ���o�X�L�b�v
			SkipStaging();

			// �T�E���h�̍Đ�
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
		}
		else
		{ // �J�ڑҋ@��Ԃ̏ꍇ

			if (GET_MANAGER->GetFade()->GetState() == CFade::FADE_NONE)
			{ // �t�F�[�h���ł͂Ȃ��ꍇ

				switch (m_nSelect)
				{ // �I�����Ƃ̏���
				case SELECT_YES:

					// �V�[���̐ݒ�
					GET_MANAGER->SetScene(CScene::MODE_GAME);	// �Q�[�����

					break;

				case SELECT_NO:

					// �V�[���̐ݒ�
					GET_MANAGER->SetScene(CScene::MODE_RANKING);	// �����L���O���

					break;
				}

				// �T�E���h�̍Đ�
				GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00
			}
		}
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CResultManager::SkipStaging(void)
{
	// ���U���g�\���̕`���ON�ɂ��A�傫����ݒ�
	for (int nCntResult = 0; nCntResult < result::NUM_POLYGON; nCntResult++)
	{ // ���U���g�\���̑������J��Ԃ�

		// ���U���g�\���̕`��J�n
		m_apResult[nCntResult]->SetEnableDraw(true);

		// ���U���g�\���̑傫����ݒ�
		m_apResult[nCntResult]->SetVec3Sizing(SIZE_RESULT);
	}

	if (GET_RETENTION->GetResult() == CRetentionManager::RESULT_CLEAR)
	{ // �N���A���Ă���ꍇ

		// �^�C���\����ON�ɂ���
		m_pTimeLogo->SetEnableDraw(true);
		m_pTime->SetEnableDraw(true);

		// �^�C���\���̑傫����ݒ�
		m_pTimeLogo->SetVec3Sizing(SIZE_TIME_LOGO);
		m_pTime->SetScalingValue(SIZE_TIME_VAL);
		m_pTime->SetScalingPart(SIZE_TIME_PART);
	}

	// �R���e�j���[���S�\���̕`��J�n
	m_pContLogo->SetEnableDraw(true);

	// �R���e�j���[���S�\���̑傫����ݒ�
	m_pContLogo->SetVec3Sizing(SIZE_CONT_LOGO);

	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // �I�����̑������J��Ԃ�

		// �R���e�j���[�\���̕`��J�n
		m_apContinue[nCntResult]->SetEnableDraw(true);

		// �R���e�j���[�\���̑傫����ݒ�
		m_apContinue[nCntResult]->SetVec3Sizing(SIZE_CONT);
	}

	// �t�F�[�h�̓����x��ݒ�
	m_pFade->SetColor(SETCOL_FADE);

	// ��Ԃ�ύX
	m_state = STATE_WAIT;	// �J�ڑҋ@���
}

//============================================================
//	���U���g�\���̃e�N�X�`���̐ݒ菈��
//============================================================
void CResultManager::SetTexResult(void)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// MISSION�e�N�X�`����o�^�E����
	m_apResult[0]->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_MISSION]));

	// RESULT�e�N�X�`����o�^�E����
	switch (GET_RETENTION->GetResult())
	{ // ���U���g���Ƃ̏���
	case CRetentionManager::RESULT_FAILED:

		// FAILED�e�N�X�`��
		m_apResult[1]->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_FAILED]));

		break;

	case CRetentionManager::RESULT_CLEAR:

		// CLEAR�e�N�X�`��
		m_apResult[1]->BindTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_CLEAR]));

		break;

	default:

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "���U���g�Ȃ����ݒ肳��Ă��܂�", "�x���I", MB_ICONWARNING);

		break;
	}
}

//============================================================
//	�\���ҋ@����
//============================================================
bool CResultManager::UpdateDrawWait(const int nWait)
{
	if (m_nCounterState < nWait)
	{ // �J�E���^�[���ҋ@�J�E���g�܂ŒB���Ă��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;

		// �ҋ@��������Ԃ�
		return false;
	}
	else
	{ // �J�E���^�[���ҋ@���������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �ҋ@������Ԃ�
		return true;
	}
}

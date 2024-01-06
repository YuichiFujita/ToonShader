//============================================================
//
//	�^�C�g���}�l�[�W���[���� [titleManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "titleManager.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEX_LOGO_FILE[] =	// ���S�e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\title000.png",	// NEVER�e�N�X�`��
		"data\\TEXTURE\\title001.png",	// GIVE�e�N�X�`��
		"data\\TEXTURE\\title002.png",	// UP!�e�N�X�`��
	};

	const char *TEX_SELECT_FILE[] =	// �I�����ڃe�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\title003.png",	// START�e�N�X�`��
		"data\\TEXTURE\\title004.png",	// MANUAL�e�N�X�`��
	};

	const int PRIORITY = 5;	// �^�C�g���̗D�揇��

	namespace fade
	{
		const D3DXCOLOR COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);	// �^�C�g���t�F�[�h�̐F

		const float SUB_ALPHA = 0.008f;	// ���l�̌��Z��
	}

	namespace logo
	{
		const D3DXVECTOR3 POS_NEVER	= D3DXVECTOR3(340.0f,  140.0f, 0.0f);	// �^�C�g�����S�̈ʒu (NEVER)
		const D3DXVECTOR3 POS_GIVE	= D3DXVECTOR3(710.0f,  310.0f, 0.0f);	// �^�C�g�����S�̈ʒu (GIVE)
		const D3DXVECTOR3 POS_UP	= D3DXVECTOR3(1050.0f, 320.0f, 0.0f);	// �^�C�g�����S�̈ʒu (UP!)

		const D3DXVECTOR3 POS[] =	// �ʒu�z��
		{
			POS_NEVER,	// NEVER�̈ʒu
			POS_GIVE,	// GIVE�̈ʒu
			POS_UP,		// UP!�̈ʒu
		};

		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(666.0f, 290.0f, 0.0f) * 0.8f;	// �^�C�g�����S�̑傫��
		const float	INIT_SCALE	= 15.0f;	// �^�C�g�����S�̏����g�嗦
		const float	SUB_SCALE	= 0.65f;	// �^�C�g�����S�g�嗦�̌��Z��
	}

	namespace select
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(350.0f, 560.0f, 0.0f);	// �I���̈ʒu
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(560.0f, 0.0f, 0.0f);		// �I���̋��
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(420.0f, 140.0f, 0.0f);	// �I���̑傫��

		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �I�𒆃J���[
		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��I�𒆃J���[
	}

	namespace selectBG
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(640.0f, 560.0f, 0.0f);				// �I��w�i�̈ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3((float)SCREEN_WIDTH, 120.0f, 0.0f);	// �I��w�i�̑傫��

		const D3DXCOLOR COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);	// �I��w�i�̐F
	}
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEX_LOGO_FILE)   == CTitleManager::LOGO_MAX,   "ERROR : LogoTexture Count Mismatch");
static_assert(NUM_ARRAY(TEX_SELECT_FILE) == CTitleManager::SELECT_MAX, "ERROR : SelectTexture Count Mismatch");

//************************************************************
//	�e�N���X [CTitleManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleManager::CTitleManager() :
	m_pFade			(nullptr),		// �t�F�[�h�̏��
	m_pSelectBG		(nullptr),		// �I��w�i�̏��
	m_state			(STATE_NONE),	// ���
	m_fScale		(0.0f),			// �^�C�g���g�嗦
	m_nSelect		(0),			// ���݂̑I��
	m_nOldSelect	(0)				// �O��̑I��
{
	// �����o�ϐ����N���A
	memset(&m_apLogo[0],	0, sizeof(m_apLogo));	// �^�C�g���\���̏��
	memset(&m_apSelect[0],	0, sizeof(m_apSelect));	// �I��\���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTitleManager::~CTitleManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTitleManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apLogo[0],	0, sizeof(m_apLogo));	// �^�C�g���\���̏��
	memset(&m_apSelect[0],	0, sizeof(m_apSelect));	// �I��\���̏��
	m_pFade			= nullptr;			// �t�F�[�h�̏��
	m_pSelectBG		= nullptr;			// �I��w�i�̏��
	m_state			= STATE_FADEOUT;	// ���
	m_fScale		= logo::INIT_SCALE;	// �^�C�g���g�嗦
	m_nSelect		= 0;				// ���݂̑I��
	m_nOldSelect	= 0;				// �O��̑I��

	//--------------------------------------------------------
	//	�I��w�i�̐����E�ݒ�
	//--------------------------------------------------------
	// �I��w�i�̐���
	m_pSelectBG = CObject2D::Create
	( // ����
		selectBG::POS,	// �ʒu
		selectBG::SIZE,	// �傫��
		VEC3_ZERO,		// ����
		selectBG::COL	// �F
	);
	if (m_pSelectBG == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pSelectBG->SetPriority(PRIORITY);

	// �`������Ȃ��ݒ�ɂ���
	m_pSelectBG->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�I��\���̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
	{ // �I�����ڂ̑������J��Ԃ�

		// �I��\���̐���
		m_apSelect[nCntTitle] = CObject2D::Create
		( // ����
			select::POS + (select::SPACE * (float)nCntTitle),	// �ʒu
			select::SIZE,			// �傫��
			VEC3_ZERO,				// ����
			select::DEFAULT_COL		// �F
		);
		if (m_apSelect[nCntTitle] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apSelect[nCntTitle]->BindTexture(pTexture->Regist(TEX_SELECT_FILE[nCntTitle]));

		// �D�揇�ʂ�ݒ�
		m_apSelect[nCntTitle]->SetPriority(PRIORITY);

		// �`������Ȃ��ݒ�ɂ���
		m_apSelect[nCntTitle]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	�^�C�g�����S�̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
	{ // �^�C�g�����S�̑������J��Ԃ�

		// �^�C�g�����S�̐���
		m_apLogo[nCntTitle] = CObject2D::Create
		( // ����
			logo::POS[nCntTitle],	// �ʒu
			logo::SIZE				// �傫��
		);
		if (m_apLogo[nCntTitle] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apLogo[nCntTitle]->BindTexture(pTexture->Regist(TEX_LOGO_FILE[nCntTitle]));

		// �D�揇�ʂ�ݒ�
		m_apLogo[nCntTitle]->SetPriority(PRIORITY);

		// �`������Ȃ��ݒ�ɂ���
		m_apLogo[nCntTitle]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		fade::COL		// �F
	);
	if (m_pFade == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTitleManager::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
	{ // �^�C�g�����S�̑������J��Ԃ�

		// �^�C�g�����S�̏I��
		SAFE_UNINIT(m_apLogo[nCntTitle]);
	}

	for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
	{ // �I�����ڂ̑������J��Ԃ�

		// �I��\���̏I��
		SAFE_UNINIT(m_apSelect[nCntTitle]);
	}

	// �t�F�[�h�̏I��
	SAFE_UNINIT(m_pFade);

	// �I��w�i�̏I��
	SAFE_UNINIT(m_pSelectBG);
}

//============================================================
//	�X�V����
//============================================================
void CTitleManager::Update(void)
{
	// �J�ڌ���̍X�V
	UpdateStart();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_FADEOUT:	// �t�F�[�h�A�E�g���

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFade();

		break;

	case STATE_MOVE:	// �^�C�g���k�����

		// �^�C�g���ړ��̍X�V
		UpdateMove();

		break;

	case STATE_WAIT:	// �J�ڑҋ@���

		// �I�𑀍�
		ActSelect();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
	{ // �^�C�g�����S�̑������J��Ԃ�

		// �^�C�g�����S�̍X�V
		m_apLogo[nCntTitle]->Update();
	}

	for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
	{ // �I�����ڂ̑������J��Ԃ�

		// �I��\���̍X�V
		m_apSelect[nCntTitle]->Update();
	}

	// �t�F�[�h�̍X�V
	m_pFade->Update();

	// �I��w�i�̍X�V
	m_pSelectBG->Update();
}

//============================================================
//	��������
//============================================================
CTitleManager *CTitleManager::Create(void)
{
	// �^�C�g���}�l�[�W���[�̐���
	CTitleManager *pTitleManager = new CTitleManager;
	if (pTitleManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C�g���}�l�[�W���[�̏�����
		if (FAILED(pTitleManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C�g���}�l�[�W���[�̔j��
			SAFE_DELETE(pTitleManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTitleManager;
	}
}

//============================================================
//	�j������
//============================================================
void CTitleManager::Release(CTitleManager *&prTitleManager)
{
	// �^�C�g���}�l�[�W���[�̏I��
	assert(prTitleManager != nullptr);
	prTitleManager->Uninit();

	// �������J��
	SAFE_DELETE(prTitleManager);
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CTitleManager::UpdateFade(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	if (colFade.a > 0.0f)
	{ // �����ɂȂ��Ă��Ȃ��ꍇ

		// �����x�����Z
		colFade.a -= fade::SUB_ALPHA;
	}
	else
	{ // �����ɂȂ����ꍇ

		// �����x��␳
		colFade.a = 0.0f;

		for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
		{ // �^�C�g�����S�̑������J��Ԃ�

			// �`�������ݒ�ɂ���
			m_apLogo[nCntTitle]->SetEnableDraw(true);
		}

		for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
		{ // �I�����ڂ̑������J��Ԃ�

			// �I��\����`�悷��ݒ�ɂ���
			m_apSelect[nCntTitle]->SetEnableDraw(true);
		}

		// �I��w�i��`�悷��ݒ�ɂ���
		m_pSelectBG->SetEnableDraw(true);

		// ��Ԃ�ύX
		m_state = STATE_MOVE;	// �^�C�g���ړ����
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	�^�C�g���ړ��̍X�V����
//============================================================
void CTitleManager::UpdateMove(void)
{
	if (m_fScale > 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale -= logo::SUB_SCALE;

		for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
		{ // �^�C�g�����S�̑������J��Ԃ�

			// �^�C�g�����S�̑傫����ݒ�
			m_apLogo[nCntTitle]->SetVec3Sizing(logo::SIZE * m_fScale);
		}
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		// �g�嗦��␳
		m_fScale = 1.0f;

		for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
		{ // �^�C�g�����S�̑������J��Ԃ�

			// �^�C�g�����S�̑傫����ݒ�
			m_apLogo[nCntTitle]->SetVec3Sizing(logo::SIZE);
		}

		// ��Ԃ�ύX
		m_state = STATE_WAIT;	// �J�ڑҋ@���

		// �J�����̍X�V���ĊJ
		GET_MANAGER->GetCamera()->SetEnableUpdate(true);

		// �T�E���h�̍Đ�
		GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
	}
}

//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CTitleManager::UpdateStart(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// �L�[�{�[�h
	CInputPad		*pPad		= GET_INPUTPAD;	// �p�b�h

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
				case SELECT_GAME:

					// �V�[���̐ݒ�
					GET_MANAGER->SetScene(CScene::MODE_GAME);	// �Q�[�����

					break;

				case SELECT_TUTORIAL:

					// �V�[���̐ݒ�
					GET_MANAGER->SetScene(CScene::MODE_TUTORIAL);	// �`���[�g���A�����

					break;

				default:
					assert(false);
					break;
				}

				// �T�E���h�̍Đ�
				GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00
			}
		}
	}
}

//============================================================
//	�I�𑀍쏈��
//============================================================
void CTitleManager::ActSelect(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// �L�[�{�[�h
	CInputPad		*pPad		= GET_INPUTPAD;		// �p�b�h

	if (GET_MANAGER->GetFade()->GetState() == CFade::FADE_NONE)
	{ // �t�F�[�h���ł͂Ȃ��ꍇ

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
		m_apSelect[m_nOldSelect]->SetColor(select::DEFAULT_COL);

		// ���݂̑I��v�f�̐F�𔒂ɐݒ�
		m_apSelect[m_nSelect]->SetColor(select::CHOICE_COL);

		// ���݂̑I��v�f����
		m_nOldSelect = m_nSelect;
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CTitleManager::SkipStaging(void)
{
	// �^�C�g�����S��\����Ԃɐݒ�E�傫���𐳋K��
	for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
	{ // �^�C�g�����S�̑������J��Ԃ�

		// �^�C�g�����S�̑傫����ݒ�
		m_apLogo[nCntTitle]->SetVec3Sizing(logo::SIZE);

		// �`�������ݒ�ɂ���
		m_apLogo[nCntTitle]->SetEnableDraw(true);
	}

	// �I��\����`�悷��ݒ�ɂ���
	for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
	{ // �I�����ڂ̑������J��Ԃ�

		m_apSelect[nCntTitle]->SetEnableDraw(true);
	}

	// �t�F�[�h�𓧖��ɂ���
	m_pFade->SetColor(XCOL_ABLACK);

	// �I��w�i��`�悷��ݒ�ɂ���
	m_pSelectBG->SetEnableDraw(true);

	// �J�����̍X�V���ĊJ
	GET_MANAGER->GetCamera()->SetEnableUpdate(true);

	// ��Ԃ�ύX
	m_state = STATE_WAIT;	// �J�ڑҋ@���
}

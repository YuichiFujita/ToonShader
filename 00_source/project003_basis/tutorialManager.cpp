//============================================================
//
//	�`���[�g���A���}�l�[�W���[���� [tutorialManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "tutorialManager.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		nullptr,	// ��������e�N�X�`��
	};

	const char *TEX_LESSON_FILE[] =	// ���b�X���e�N�X�`���t�@�C��
	{
		nullptr,	// �W�����v�����e�N�X�`��
		nullptr,	// �X���C�f�B���O�����e�N�X�`��
		nullptr,	// �Ǒ�������e�N�X�`��
		nullptr,	// �ǃW�����v�����e�N�X�`��
	};

	const int PRIORITY = 5;	// �`���[�g���A���̗D�揇��

	namespace lesson
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(640.0f, 580.0f, 0.0f);	// ���b�X���\���̈ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(1000.0f, 270.0f, 0.0f);	// ���b�X���\���̈ʒu
	}

	namespace control
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1140.0f, 85.0f, 0.0f);	// ������@�̈ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(264.0f, 150.0f, 0.0f);	// ������@�̈ʒu
	}
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE)    == CTutorialManager::TEXTURE_MAX, "ERROR : Texture Count Mismatch");
static_assert(NUM_ARRAY(TEX_LESSON_FILE) == CTutorialManager::EXPLAIN_MAX, "ERROR : LessonTexture Count Mismatch");

//************************************************************
//	�e�N���X [CTutorialManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTutorialManager::CTutorialManager() :
	m_pExplain	(nullptr),	// �����\���̏��
	m_pControl	(nullptr)	// ��������̏��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTutorialManager::~CTutorialManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTutorialManager::Init(void)
{
	// �����o�ϐ���������
	m_pExplain = nullptr;	// �����\���̏��
	m_pControl = nullptr;	// ��������̏��

	// �����\���̐���
	m_pExplain = CObject2D::Create
	( // ����
		lesson::POS,	// �ʒu
		lesson::SIZE	// �傫��
	);
	if (m_pExplain == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pExplain->SetPriority(PRIORITY);

	// ��������̐���
	m_pControl = CObject2D::Create
	( // ����
		control::POS,	// �ʒu
		control::SIZE	// �傫��
	);
	if (m_pControl == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pControl->BindTexture(TEXTURE_FILE[TEXTURE_CONTROL]);

	// �D�揇�ʂ�ݒ�
	m_pControl->SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTutorialManager::Uninit(void)
{
	// �����\���̏I��
	SAFE_UNINIT(m_pExplain);

	// ��������̏I��
	SAFE_UNINIT(m_pControl);
}

//============================================================
//	�X�V����
//============================================================
void CTutorialManager::Update(void)
{
	if (GET_INPUTKEY->IsTrigger(DIK_RETURN) || GET_INPUTPAD->IsTrigger(CInputPad::KEY_START))
	{
		// �V�[���̐ݒ�
		GET_MANAGER->SetScene(CScene::MODE_TITLE);	// �^�C�g�����
	}

	// �����\���̍X�V
	m_pExplain->Update();

	// ��������̍X�V
	m_pControl->Update();
}

//============================================================
//	��������
//============================================================
CTutorialManager *CTutorialManager::Create(void)
{
	// �`���[�g���A���̐���
	CTutorialManager *pTutorialManager = new CTutorialManager;
	if (pTutorialManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �`���[�g���A���̏�����
		if (FAILED(pTutorialManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �`���[�g���A���̔j��
			SAFE_DELETE(pTutorialManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTutorialManager;
	}
}

//============================================================
//	�j������
//============================================================
void CTutorialManager::Release(CTutorialManager *&prTutorialManager)
{
	// �`���[�g���A���}�l�[�W���[�̏I��
	assert(prTutorialManager != nullptr);
	prTutorialManager->Uninit();

	// �������J��
	SAFE_DELETE(prTutorialManager);
}

//============================================================
//
//	�f�o�b�O���� [debug.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "debug.h"
#include "manager.h"
#include "object.h"
#include "renderer.h"
#include "scene.h"
#include "sceneGame.h"
#include "debugproc.h"
#include "pause.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int MEASURE_FPS	= 500;	// FPS�̌v���^�C�~���O
	const int FILL_MAX		= 3;	// �h��Ԃ����[�h�̑���
}

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_DEBUG_DISP		(DIK_F1)	// �f�o�b�O�\��ON/OFF�L�[
#define NAME_DEBUG_DISP		("F1")		// �f�o�b�O�\��ON/OFF�\��
#define KEY_CAMERA_CONTROL	(DIK_F2)	// �J��������ON/OFF�L�[
#define NAME_CAMERA_CONTROL	("F2")		// �J��������ON/OFF�\��
#define KEY_FILLMODE		(DIK_F3)	// �h��Ԃ����[�h�ύX�L�[
#define NAME_FILLMODE		("F3")		// �h��Ԃ����[�h�ύX�\��
#define KEY_2D_DISP			(DIK_F4)	// 2D�I�u�W�F�N�g�\��ON/OFF�L�[
#define NAME_2D_DISP		("F4")		// 2D�I�u�W�F�N�g�\��ON/OFF�\��
#define KEY_EDITMODE		(DIK_F5)	// �G�f�B�^�[���[�hON/OFF�L�[
#define NAME_EDITMODE		("F5")		// �G�f�B�^�[���[�hON/OFF�\��
#define KEY_PAUSE_DISP		(DIK_F6)	// �|�[�Y�\��ON/OFF�L�[
#define NAME_PAUSE_DISP		("F6")		// �|�[�Y�\��ON/OFF�\��

//************************************************************
//	�e�N���X [CDebug] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
#ifdef _DEBUG

CDebug::CDebug() :
	m_nFps				(0),					// FPS�J�E���^
	m_bDisp2D			(false),				// 2D�\����
	m_bDisp3D			(false),				// 3D�\����
	m_dwFrameCount		(0),					// �t���[���J�E���g
	m_dwFPSLastTime		(0),					// �Ō��FPS���v����������
	m_fillMode			(D3DFILL_SOLID),		// �h��Ԃ����[�h
	m_oldCameraState	(CCamera::STATE_NONE)	// �J�����̉ߋ����
{

}

#else	// NDEBUG

CDebug::CDebug() {}

#endif	// _DEBUG

//============================================================
//	�f�X�g���N�^
//============================================================
#ifdef _DEBUG

CDebug::~CDebug()
{

}

#else	// NDEBUG

CDebug::~CDebug() {}

#endif	// _DEBUG

//============================================================
//	����������
//============================================================
#ifdef _DEBUG

HRESULT CDebug::Init(void)
{
	// �����o�ϐ���������
	m_nFps				= 0;					// FPS�J�E���^
	m_bDisp2D			= true;					// 2D�\����
	m_bDisp3D			= true;					// 3D�\����
	m_dwFrameCount		= 0;					// �t���[���J�E���g
	m_dwFPSLastTime		= timeGetTime();		// ���ݎ������擾
	m_fillMode			= D3DFILL_SOLID;		// �h��Ԃ����[�h
	m_oldCameraState	= CCamera::STATE_NONE;	// �J�����̉ߋ����

	// �I�u�W�F�N�g�̕\���󋵂�ݒ�
	CObject::SetEnableDebugDispAll(m_bDisp2D, m_bDisp3D);

	// ������Ԃ�
	return S_OK;
}

#else	// NDEBUG

HRESULT CDebug::Init(void) { return S_OK; }

#endif	// _DEBUG

//============================================================
//	�I������
//============================================================
#ifdef _DEBUG

void CDebug::Uninit(void)
{

}

#else	// NDEBUG

void CDebug::Uninit(void) {}

#endif	// _DEBUG

//============================================================
//	FPS���菈��
//============================================================
#ifdef _DEBUG

void CDebug::MeasureFps(const DWORD dwCurrentTime)
{
	if ((dwCurrentTime - m_dwFPSLastTime) >= MEASURE_FPS)
	{ // 0.5�b�o��

		// FPS���v��
		m_nFps = (m_dwFrameCount * 1000) / (dwCurrentTime - m_dwFPSLastTime);

		// FPS�𑪒肵��������ۑ�
		m_dwFPSLastTime = dwCurrentTime;

		// �t���[���J�E���g���N���A
		m_dwFrameCount = 0;
	}
}

#else	// NDEBUG

void CDebug::MeasureFps(const DWORD /*dwCurrentTime*/) {}

#endif	// _DEBUG

//============================================================
//	�t���[���J�E���g���Z����
//============================================================
#ifdef _DEBUG

void CDebug::AddFrameCount(void)
{
	// �t���[���J�E���g�����Z
	m_dwFrameCount++;
}

#else	// NDEBUG

void CDebug::AddFrameCount(void) {}

#endif	// _DEBUG

//============================================================
//	FPS�擾����
//============================================================
#ifdef _DEBUG

int CDebug::GetFps(void) const
{
	// FPS��Ԃ�
	return m_nFps;
}

#else	// NDEBUG

int CDebug::GetFps(void) const { return 0; }

#endif	// _DEBUG

//============================================================
//	�h��Ԃ����[�h�ݒ菈��
//============================================================
#ifdef _DEBUG

void CDebug::SetFillMode(void)
{
	// �h��Ԃ����[�h�����݂̃��[�h�ɐݒ�
	GET_DEVICE->SetRenderState(D3DRS_FILLMODE, m_fillMode);
}

#else	// NDEBUG

void CDebug::SetFillMode(void) {}

#endif	// _DEBUG

//============================================================
//	�f�o�b�O����X�V����
//============================================================
#ifdef _DEBUG

void CDebug::UpdateDebugControl(void)
{
	// �f�o�b�O�\���ύX
	ChangeDispDebug();

	// �J��������ύX
	ChangeControlCamera();

	// �h��Ԃ����[�h�ύX
	ChangeFillMode();

	// 2D�I�u�W�F�N�g�\���ύX
	ChangeDisp2D();

	switch (GET_MANAGER->GetMode())
	{ // ���[�h���Ƃ̏���
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_TUTORIAL:
		break;

	case CScene::MODE_GAME:

		// �G�f�B�^�[���[�h�ύX
		ChangeEditMode();

		// �|�[�Y�\���ύX
		ChangeDispPause();

		break;

	case CScene::MODE_RESULT:
		break;

	case CScene::MODE_RANKING:
		break;

	default:
		assert(false);
		break;
	}
}

#else	// NDEBUG

void CDebug::UpdateDebugControl(void) {}

#endif	// _DEBUG

//============================================================
//	�f�o�b�O����\������
//============================================================
#ifdef _DEBUG

void CDebug::DrawDebugControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebugProc = GET_MANAGER->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebugProc->Print(CDebugProc::POINT_LEFT, "======================================\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O����]\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "======================================\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]�F�f�o�b�O�\����ON/OFF\n", NAME_DEBUG_DISP);
	pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]�F�J���������ON/OFF\n", NAME_CAMERA_CONTROL);
	pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]�F�h��Ԃ����[�h�̕ύX\n", NAME_FILLMODE);
	pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]�F2D�I�u�W�F�N�g�\����ON/OFF\n", NAME_2D_DISP);

	switch (GET_MANAGER->GetMode())
	{ // ���[�h���Ƃ̏���
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_TUTORIAL:
		break;

	case CScene::MODE_GAME:

		pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]�F�G�f�B�b�g���[�h��ON/OFF\n", NAME_EDITMODE);
		pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]�F�|�[�Y�`���ON/OFF\n", NAME_PAUSE_DISP);

		break;

	case CScene::MODE_RESULT:
		break;

	case CScene::MODE_RANKING:
		break;

	default:
		assert(false);
		break;
	}
}

#else	// NDEBUG

void CDebug::DrawDebugControl(void) {}

#endif	// _DEBUG

//============================================================
//	�f�o�b�O���\������
//============================================================
#ifdef _DEBUG

void CDebug::DrawDebugData(void)
{
	// �|�C���^��錾
	CDebugProc *pDebugProc = GET_MANAGER->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebugProc->Print(CDebugProc::POINT_LEFT, "======================================\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O���]\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "======================================\n");

	pDebugProc->Print(CDebugProc::POINT_CENTER, "[FPS]�F%d\n", m_nFps);
}

#else	// NDEBUG

void CDebug::DrawDebugData(void) {}

#endif	// _DEBUG

//============================================================
//	��������
//============================================================
CDebug *CDebug::Create(void)
{
	// �f�o�b�O�̐���
	CDebug *pDebug = new CDebug;
	if (pDebug == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �f�o�b�O�̏�����
		if (FAILED(pDebug->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �f�o�b�O�̔j��
			SAFE_DELETE(pDebug);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pDebug;
	}
}

//============================================================
//	�j������
//============================================================
void CDebug::Release(CDebug *&prDebug)
{
	// �f�o�b�O�̏I��
	assert(prDebug != nullptr);
	prDebug->Uninit();

	// �������J��
	SAFE_DELETE(prDebug);
}

//************************************************************
//	private �����o�֐� (�f�o�b�O���ȊO���݂��Ȃ�)
//************************************************************
#ifdef _DEBUG

//============================================================
//	�f�o�b�O�\���ύX����
//============================================================
void CDebug::ChangeDispDebug(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_DEBUG_DISP))
	{
		// �f�o�b�O�̕\���t���O�𔽓]
		GET_MANAGER->GetDebugProc()->SetDisp(!GET_MANAGER->GetDebugProc()->IsDisp());
	}
}

//============================================================
//	�J��������ύX����
//============================================================
void CDebug::ChangeControlCamera(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_CAMERA_CONTROL))
	{
		if (GET_MANAGER->GetCamera()->GetState() != CCamera::STATE_CONTROL)
		{ // �J�������삪OFF�̏ꍇ

			// ���݂̏�Ԃ�ۑ�
			m_oldCameraState = GET_MANAGER->GetCamera()->GetState();

			// ����J������ݒ�
			GET_MANAGER->GetCamera()->SetState(CCamera::STATE_CONTROL);
		}
		else
		{ // �J�������삪ON�̏ꍇ

			// �ߋ��̏�Ԃ��Đݒ�
			GET_MANAGER->GetCamera()->SetState(m_oldCameraState);

			// �ߋ��̏�Ԃ�������
			m_oldCameraState = CCamera::STATE_NONE;
		}
	}
}

//============================================================
//	�h��Ԃ����[�h�ύX����
//============================================================
void CDebug::ChangeFillMode(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_FILLMODE))
	{
		// �h��Ԃ����[�h��ύX����
		m_fillMode = (D3DFILLMODE)((m_fillMode + 1) % FILL_MAX);
	}
}

//============================================================
//	2D�I�u�W�F�N�g�\���ύX����
//============================================================
void CDebug::ChangeDisp2D(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_2D_DISP))
	{
		// 2D�̕\���t���O�𔽓]
		m_bDisp2D = !m_bDisp2D;

		// �I�u�W�F�N�g�̕\���󋵂�ݒ�
		CObject::SetEnableDebugDispAll(m_bDisp2D, m_bDisp3D);
	}
}

//============================================================
//	�G�f�B�^�[���[�h�ύX����
//============================================================
void CDebug::ChangeEditMode(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_EDITMODE))
	{
		// �G�f�B�b�g�󋵂̃t���O�𔽓]
		//CGameManager::GetEditStage()->SetEnableEdit(!CGameManager::GetEditStage()->IsEdit());
	}
}

//============================================================
//	�|�[�Y�\���ύX����
//============================================================
void CDebug::ChangeDispPause(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_PAUSE_DISP))
	{
		// �|�[�Y�̕\���󋵂�ݒ�
		CSceneGame::GetPause()->SetEnableDebugDisp(!CSceneGame::GetPause()->IsDebugDisp());
	}
}

#endif	// _DEBUG

//============================================================
//
//	�}�l�[�W���[�w�b�_�[ [manager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CTime;				// �^�C���N���X
class CRenderer;			// �����_���[�N���X
class CInputKeyboard;		// �L�[�{�[�h�N���X
class CInputMouse;			// �}�E�X�N���X
class CInputPad;			// �p�b�h�N���X
class CSound;				// �T�E���h�N���X
class CCamera;				// �J�����N���X
class CLight;				// ���C�g�N���X
class CTexture;				// �e�N�X�`���N���X
class CModel;				// ���f���N���X
class CFade;				// �t�F�[�h�N���X
class CRetentionManager;	// �f�[�^�ۑ��}�l�[�W���[�N���X
class CDebugProc;			// �f�o�b�O�\���N���X
class CDebug;				// �f�o�b�O�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �}�l�[�W���[�N���X
class CManager
{
public:
	// �R���X�g���N�^
	CManager();

	// �f�X�g���N�^
	~CManager();

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CManager *Create(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// ����
	static CManager *GetInstance(void);			// �擾
	static void Release(CManager *&prManager);	// �j��

	// �����o�֐�
	void SetScene(const CScene::EMode mode, const int nWait = 0);	// �V�[���ݒ�
	HRESULT SetMode(const CScene::EMode mode);	// ���[�h�ݒ�
	CScene::EMode GetMode(void);				// ���[�h�擾

	CTime				*GetTime(void);			// �^�C���擾
	CRenderer			*GetRenderer(void);		// �����_���[�擾
	CInputKeyboard		*GetKeyboard(void);		// �L�[�{�[�h�擾
	CInputMouse			*GetMouse(void);		// �}�E�X�擾
	CInputPad			*GetPad(void);			// �p�b�h�擾
	CSound				*GetSound(void);		// �T�E���h�擾
	CCamera				*GetCamera(void);		// �J�����擾
	CLight				*GetLight(void);		// ���C�g�擾
	CTexture			*GetTexture(void);		// �e�N�X�`���擾
	CModel				*GetModel(void);		// ���f���擾
	CFade				*GetFade(void);			// �t�F�[�h�擾
	CScene				*GetScene(void);		// �V�[���擾
	CRetentionManager	*GetRetention(void);	// �f�[�^�ۑ��}�l�[�W���[�擾
	CDebugProc			*GetDebugProc(void);	// �f�o�b�O�\���擾
	CDebug				*GetDebug(void);		// �f�o�b�O�擾

private:
	// �ÓI�����o�ϐ�
	static CManager *m_pManager;	// �}�l�[�W���[

	// �����o�ϐ�
	CTime				*m_pTime;		// �^�C���C���X�^���X
	CRenderer			*m_pRenderer;	// �����_���[�C���X�^���X
	CInputKeyboard		*m_pKeyboard;	// �L�[�{�[�h�C���X�^���X
	CInputMouse			*m_pMouse;		// �}�E�X�C���X�^���X
	CInputPad			*m_pPad;		// �p�b�h�C���X�^���X
	CSound				*m_pSound;		// �T�E���h�C���X�^���X
	CCamera				*m_pCamera;		// �J�����C���X�^���X
	CLight				*m_pLight;		// ���C�g�C���X�^���X
	CTexture			*m_pTexture;	// �e�N�X�`���C���X�^���X
	CModel				*m_pModel;		// ���f���C���X�^���X
	CFade				*m_pFade;		// �t�F�[�h�C���X�^���X
	CScene				*m_pScene;		// �V�[���C���X�^���X
	CRetentionManager	*m_pRetention;	// �f�[�^�ۑ��}�l�[�W���[
	CDebugProc			*m_pDebugProc;	// �f�o�b�O�\��
	CDebug				*m_pDebug;		// �f�o�b�O
};

#endif	// _MANAGER_H_

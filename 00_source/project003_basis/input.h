//============================================================
//
//	���̓w�b�_�[ [input.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _INPUT_H_
#define _INPUT_H_

//************************************************************
//	�萔�錾
//************************************************************
namespace keyboard
{
	const int MAX_KEY = 256;	// �L�[�̍ő吔
}

namespace pad
{
	const int MAX_NUM = 4;	// �p�b�h�̍ő吔
}

//************************************************************
//	�N���X��`
//************************************************************
// ���̓N���X
class CInput
{
public:
	// �R���X�g���N�^
	CInput();

	// �f�X�g���N�^
	virtual ~CInput();

	// ���z�֐�
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ������
	virtual void Uninit(void);	// �I��

	// �������z�֐�
	virtual void Update(void) = 0;	// �X�V

protected:
	// �����o�ϐ�
	static int m_nNumAll;				// ���̓f�o�C�X�̑���
	static LPDIRECTINPUT8 m_pInput;		// DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8  m_pDevice;	// DirectInput�f�o�C�X
};

// �L�[�{�[�h�N���X
class CInputKeyboard : public CInput
{
public:
	// �R���X�g���N�^
	CInputKeyboard();

	// �f�X�g���N�^
	~CInputKeyboard();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// ������
	void Uninit(void) override;	// �I��
	void Update(void) override;	// �X�V

	// �ÓI�����o�֐�
	static CInputKeyboard *Create(HINSTANCE hInstance, HWND hWnd);	// ����
	static void Release(CInputKeyboard *&pKeyboard);				// �j��

	// �����o�֐�
	bool IsPress(int nKey);		// �v���X�擾
	bool IsTrigger(int nKey);	// �g���K�[�擾
	bool IsRelease(int nKey);	// �����[�X�擾

private:
	// �����o�ϐ�
	BYTE m_aKeyStatePress[keyboard::MAX_KEY];	// �v���X���
	BYTE m_aKeyStateTrigger[keyboard::MAX_KEY];	// �g���K�[���
	BYTE m_aKeyStateRelease[keyboard::MAX_KEY];	// �����[�X���
};

// �}�E�X�N���X
class CInputMouse : public CInput
{
public:
	// �R���X�g���N�^
	CInputMouse();

	// �f�X�g���N�^
	~CInputMouse();

	// �}�E�X�L�[��
	enum EKey
	{
		KEY_LEFT = 0,	// �}�E�X�E�N���b�N
		KEY_RIGHT,		// �}�E�X���N���b�N
		KEY_WHEEL,		// �}�E�X�z�C�[����������
		KEY_MAX			// ���̗񋓌^�̑���
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// ������
	void Uninit(void) override;	// �I��
	void Update(void) override;	// �X�V

	// �ÓI�����o�֐�
	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);	// ����
	static void Release(CInputMouse *&pMouse);					// �j��

	// �����o�֐�
	bool IsPress(EKey mouKey);		// �v���X�擾
	bool IsTrigger(EKey mouKey);	// �g���K�[�擾
	bool IsRelease(EKey mouKey);	// �����[�X�擾
	D3DXVECTOR3 GetMove(void);		// �ړ��ʎ擾

private:
	// �����o�ϐ�
	DIMOUSESTATE m_keyStatePress;	// �v���X���
	DIMOUSESTATE m_keyStateTrigger;	// �g���K�[���
	DIMOUSESTATE m_keyStateRelease;	// �����[�X���
};

// �p�b�h�N���X
class CInputPad
{
public:
	// �R���X�g���N�^
	CInputPad();

	// �f�X�g���N�^
	~CInputPad();

	// �p�b�h�L�[��
	enum EKey
	{
		KEY_UP = 0,		// �����L�[ ��
		KEY_DOWN,		// �����L�[ ��
		KEY_LEFT,		// �����L�[ ��
		KEY_RIGHT,		// �����L�[ �E
		KEY_START,		// START�{�^��
		KEY_BACK,		// BACK�{�^��
		KEY_LSTICKPUSH,	// ���X�e�B�b�N��������
		KEY_RSTICKPUSH,	// �E�X�e�B�b�N��������
		KEY_L1,			// LB�{�^��
		KEY_R1,			// RB�{�^��
		KEY_L2,			// LT�{�^��
		KEY_R2,			// RT�{�^��
		KEY_A,			// A�{�^��
		KEY_B,			// B�{�^��
		KEY_X,			// X�{�^��
		KEY_Y,			// Y�{�^��
		KEY_LSTICK_X,	// ���X�e�B�b�N ���E
		KEY_LSTICK_Y,	// ���X�e�B�b�N �㉺
		KEY_RSTICK_X,	// �E�X�e�B�b�N ���E
		KEY_RSTICK_Y,	// �E�X�e�B�b�N �㉺
		KEY_MAX			// ���̗񋓌^�̑���
	};
	
	// �U����ޗ�
	enum EType
	{
		TYPE_NONE = 0,		// �������Ă��Ȃ�
		TYPE_DAMAGE,		// �_���[�W
		TYPE_BIG_DAMAGE,	// ��_���[�W
		TYPE_DEATH,			// ���S
		TYPE_WALLDASH,		// �Ǒ���
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �U���\����
	struct SVibration
	{
		_XINPUT_VIBRATION vibration;	// �o�C�u���
		EType type;	// �o�C�u���
		int nTime;	// �o�C�u����
	};

	// �ÓI�����o�֐�
	static CInputPad *Create(void);			// ����
	static void Release(CInputPad *&pPad);	// �j��

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetVibration(EType type, int nPlayer = 0);	// �o�C�u�ݒ�
	bool IsPress(EKey joyKey, int nPlayer = 0);		// �v���X�擾	(�{�^��)
	bool IsTrigger(EKey joyKey, int nPlayer = 0);	// �g���K�[�擾	(�{�^��)
	bool IsRelease(EKey joyKey, int nPlayer = 0);	// �����[�X�擾	(�{�^��)

	bool IsPressAll(EKey joyKey);	// �S�v���X�擾		(�{�^��)
	bool IsTriggerAll(EKey joyKey);	// �S�g���K�[�擾	(�{�^��)
	bool IsReleaseAll(EKey joyKey);	// �S�����[�X�擾	(�{�^��)

	SHORT GetPressLStickX(int nPlayer = 0);		// �v���X�擾 (L�X�e�B�b�NX)
	SHORT GetPressLStickY(int nPlayer = 0);		// �v���X�擾 (L�X�e�B�b�NY)
	SHORT GetPressRStickX(int nPlayer = 0);		// �v���X�擾 (R�X�e�B�b�NX)
	SHORT GetPressRStickY(int nPlayer = 0);		// �v���X�擾 (R�X�e�B�b�NY)
	float GetPressLStickRot(int nPlayer = 0);	// �v���X�擾 (L�X�e�B�b�N����)
	float GetPressRStickRot(int nPlayer = 0);	// �v���X�擾 (R�X�e�B�b�N����)

private:
	// �����o�֐�
	void UpdateVibration(SVibration *pVibration, int nPlayer);	// �o�C�u���[�V�����X�V

	// �����o�ϐ�
	SVibration   m_aVibration[pad::MAX_NUM];		// �o�C�u���
	XINPUT_STATE m_aKeyStatePress[pad::MAX_NUM];	// �v���X���
	XINPUT_STATE m_aKeyStateTrigger[pad::MAX_NUM];	// �g���K�[���
	XINPUT_STATE m_aKeyStateRelease[pad::MAX_NUM];	// �����[�X���
};

#endif	// _INPUT_H_

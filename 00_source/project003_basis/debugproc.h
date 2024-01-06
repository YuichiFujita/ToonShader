//==========================================================
//
//�f�o�b�O�\������ [debugproc.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _DEBUGPROC_H_		//���̃}�N������`����Ă��Ȃ��ꍇ
#define _DEBUGPROC_H_

//**********************************************************
//	�萔�錾
//**********************************************************
namespace debugproc
{
	const int MAX_DEBUGSTRING = 2048;	// �f�o�b�O�\���̍ő啶����
}

//**********************************************************
//�N���X�̒�`
//**********************************************************
class CDebugProc
{
public:
	// �f�o�b�O�ʒu��
	enum EPoint
	{
		POINT_CENTER = 0,	// ����
		POINT_LEFT,			// ��
		POINT_RIGHT,		// �E
		POINT_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CDebugProc();

	// �f�X�g���N�^
	~CDebugProc();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Print(const EPoint point, const char *fmt, ...);
	
	void SetDisp(const bool bDisp)	{ m_bDisp = bDisp; }	// �\���ݒ�
	bool IsDisp(void) const			{ return m_bDisp; }		// �\���擾

	// �ÓI�����o�֐�
	static CDebugProc *Create(void);				// ����
	static void Release(CDebugProc *&pDebugProc);	// �j��

private:
	static LPD3DXFONT m_pFont;	// �t�H���g�ւ̃|�C���^
	bool m_bDisp;				// �f�o�b�O�\����ON/OFF
	char m_aStr[POINT_MAX][debugproc::MAX_DEBUGSTRING];	// �f�o�b�O�\���p�̕�����
};

#endif	// _DEBUGPROC_H_

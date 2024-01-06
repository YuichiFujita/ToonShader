//============================================================
//
//	�����_�[�X�e�[�g�w�b�_�[ [renderState.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RENDER_STATE_H_
#define _RENDER_STATE_H_

//************************************************************
//	�N���X��`
//************************************************************
// �����_�[�X�e�[�g�N���X
class CRenderState
{
public:
	// ���u�����h��
	enum EBlend
	{
		BLEND_NORMAL = 0,	// �ʏ퍇��
		BLEND_ADD,			// ���Z����
		BLEND_SUB,			// ���Z����
		BLEND_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CRenderState();

	// �f�X�g���N�^
	~CRenderState();

	// ���u�����h���
	struct SBlendAlpha
	{
	public:
		// �R���X�g���N�^
		SBlendAlpha() {}
		SBlendAlpha(const D3DBLENDOP Op, const D3DBLEND Scr, const D3DBLEND Dest)
		{ op = Op; scr = Scr; dest = Dest; bBlend = true; }

		// �f�X�g���N�^
		~SBlendAlpha() {}

		// �����o�ϐ�
		D3DBLENDOP	op;		// ���u�����h���
		D3DBLEND	scr;	// SCR�u�����h
		D3DBLEND	dest;	// DEST�u�����h
		bool		bBlend;	// ���u�����h��
	};

	// ���e�X�g���
	struct STestAlpha
	{
		D3DCMPFUNC	func;	// ���e�X�g���
		int			nRef;	// ���e�X�g�Q�ƒl
		bool		bTest;	// ���e�X�g��
	};

	// Z�e�X�g���
	struct STestZ
	{
		D3DCMPFUNC	func;		// Z�e�X�g���
		bool		bUpdate;	// Z�o�b�t�@�X�V��
	};

	// �����_�[�X�e�[�g���
	struct SInfo
	{
		SBlendAlpha	blendAlpha;	// ���u�����h���
		STestAlpha	testAlpha;	// ���e�X�g���
		STestZ		testZ;		// Z�e�X�g���
		D3DCULL		cull;		// �J�����O���
		bool		bLight;		// ���C�e�B���O��
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Set(void);		// �ݒ�
	void Reset(void);	// �Đݒ�
	void SetInfoRenderState(const SInfo& rInfo);	// �����_�[�X�e�[�g���ݒ�
	SInfo GetInfoRenderState(void);					// �����_�[�X�e�[�g���擾
	void SetAlphaBlend(const EBlend blend);			// ���u�����h���ݒ�

	void SetAlphaFunc(const D3DCMPFUNC func)	{ m_info.testAlpha.func		= func; }		// ���e�X�g�̏��ݒ�
	void SetAlphaNumRef(const int nRef)			{ m_info.testAlpha.nRef		= nRef; }		// ���e�X�g�̎Q�ƒl�ݒ�
	void SetAlphaTest(const bool bTest)			{ m_info.testAlpha.bTest	= bTest; }		// ���e�X�g�̏󋵐ݒ�
	void SetZFunc(const D3DCMPFUNC func)		{ m_info.testZ.func			= func; }		// Z�e�X�g�̏��ݒ�
	void SetZUpdate(const bool bUpdate)			{ m_info.testZ.bUpdate		= bUpdate; }	// Z�e�X�g�̃o�b�t�@�X�V�ݒ�

	void SetCulling(const D3DCULL cull)			{ m_info.cull	= cull; }	// �J�����O�ݒ�
	void SetLighting(const bool bLight)			{ m_info.bLight	= bLight; }	// ���C�e�B���O�ݒ�

	// �ÓI�����o�֐�
	static CRenderState *Create(void);	// ����
	static void Release(CRenderState *&prRenderState);		// �j��
	static void BindDevice(LPDIRECT3DDEVICE9 *ppDevice);	// �f�o�C�X����
	static void InitRenderState(void);		// �����_�[�X�e�[�g��񏉊���
	static SInfo GetSaveRenderState(void);	// �ۑ������_�[�X�e�[�g���擾

private:
	// �ÓI�����o�֐�
	static void SetRenderState(const SInfo& rInfo);	// �����_�[�X�e�[�g���f

	// �ÓI�����o�ϐ�
	static LPDIRECT3DDEVICE9 *m_ppDevice;	// Direct3D�f�o�C�X�ւ̃|�C���^
	static SInfo m_save;	// �ۑ������_�[�X�e�[�g���

	// �����o�ϐ�
	SInfo m_info;	// �����_�[�X�e�[�g���
};

#endif	// _RENDER_STATE_H_

//============================================================
//
//	�I�u�W�F�N�g�w�b�_�[ [object.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//************************************************************
//	�萔�錾
//************************************************************
namespace object
{
	const DWORD FVF_VERTEX_2D		= (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);				// ���_�t�H�[�}�b�g [2D]
	const DWORD FVF_VERTEX_3D		= (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);	// ���_�t�H�[�}�b�g [3D]
	const DWORD FVF_VERTEX_MULTEX3D	= (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2);	// ���_�t�H�[�}�b�g [MULTEX3D]

	const int MAX_PRIO		= 8;	// �D�揇�ʂ̑���
	const int DEFAULT_PRIO	= 3;	// �f�t�H���g�̗D�揇��
}

//************************************************************
//	�\���̒�`
//************************************************************
// ���_��� [2D]
struct VERTEX_2D
{
	D3DXVECTOR3	pos;	// ���_���W
	float		rhw;	// ���W�ϊ��p�W�� (1.0f�ŌŒ�)
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex;	// �e�N�X�`�����W
};

// ���_��� [3D]
struct VERTEX_3D
{
	D3DXVECTOR3	pos;	// ���_���W
	D3DXVECTOR3	nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex;	// �e�N�X�`�����W
};

// ���_��� [MULTEX3D]
struct VERTEX_MULTEX3D
{
	D3DXVECTOR3	pos;	// ���_���W
	D3DXVECTOR3	nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex0;	// �e�N�X�`�����W_0����
	D3DXVECTOR2	tex1;	// �e�N�X�`�����W_1����
};

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�N���X
class CObject
{
public:
	// ������
	enum EDim
	{
		DIM_3D = 0,	// 3D�`��
		DIM_2D,		// 2D�`��
		DIM_MAX		// ���̗񋓌^�̑���
	};

	// ���x����
	enum ELabel
	{
		LABEL_NONE = 0,		// �Ȃ�
		LABEL_SCREEN,		// �X�N���[��
		LABEL_UI,			// UI
		LABEL_WAVE,			// �g��
		LABEL_EFFECT,		// �G�t�F�N�g
		LABEL_PARTICLE,		// �p�[�e�B�N��
		LABEL_PLAYER,		// �v���C���[
		LABEL_FIELD,		// �n��
		LABEL_WALL,			// ��
		LABEL_SCENERY,		// �i�F
		LABEL_SKY,			// ��
		LABEL_LIQUID,		// �t��
		LABEL_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CObject(const ELabel label = LABEL_NONE, const EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	virtual ~CObject();

	// �������z�֐�
	virtual HRESULT Init(void)	= 0;	// ������
	virtual void Uninit(void)	= 0;	// �I��
	virtual void Update(void)	= 0;	// �X�V
	virtual void Draw(void)		= 0;	// �`��
	virtual void Release(void)	= 0;	// �j��

	// ���z�֐�
	virtual void Hit(void);	// �q�b�g
	virtual void Hit(const int nDmg);	// �_���[�W�q�b�g
	virtual void HitKnockBack(const int nDmg, const D3DXVECTOR3& vecKnock);	// �m�b�N�o�b�N�q�b�g

	virtual void BindTexture(const int nTextureID);		// �e�N�X�`������ (�C���f�b�N�X)
	virtual void BindTexture(const char *pTexturePass);	// �e�N�X�`������ (�p�X)
	virtual void BindModel(const int nModelID);			// ���f������ (�C���f�b�N�X)
	virtual void BindModel(const char *pModelPass);		// ���f������ (�p�X)
	virtual void SetIndex(const int nIndex);			// �C���f�b�N�X�ݒ�
	virtual int GetIndex(void) const;					// �C���f�b�N�X�擾
	virtual void SetState(const int nState);			// ��Ԑݒ�
	virtual int GetState(void) const;					// ��Ԏ擾
	virtual void SetType(const int nType);				// ��ސݒ�
	virtual int GetType(void) const;					// ��ގ擾
	virtual void SetAngle(const float fAngle);			// �p�x�ݒ�
	virtual float GetAngle(void) const;					// �p�x�擾
	virtual void SetLength(const float fLength);		// �����ݒ�
	virtual float GetLength(void) const;				// �����擾
	virtual void SetRadius(const float fRadius);		// ���a�ݒ�
	virtual float GetRadius(void) const;				// ���a�擾
	virtual void SetHeight(const float fHeight);		// �c���ݒ�
	virtual float GetHeight(void) const;				// �c���擾
	virtual void SetScale(const float fScale);			// �g�嗦�ݒ�
	virtual float GetScale(void) const;					// �g�嗦�擾

	virtual void SetVec2Position(const D3DXVECTOR2& rPos);	// �񎲂̈ʒu�ݒ�
	virtual D3DXVECTOR2 GetVec2Position(void) const;		// �񎲂̈ʒu�擾
	virtual void SetVec3Position(const D3DXVECTOR3& rPos);	// �O���̈ʒu�ݒ�
	virtual D3DXVECTOR3 GetVec3Position(void) const;		// �O���̈ʒu�擾
	virtual void SetVec3Rotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	virtual D3DXVECTOR3 GetVec3Rotation(void) const;		// �����擾
	virtual void SetVec2Sizing(const D3DXVECTOR2& rSize);	// �񎲂̑傫���ݒ�
	virtual D3DXVECTOR2 GetVec2Sizing(void) const;			// �񎲂̑傫���擾
	virtual void SetVec3Sizing(const D3DXVECTOR3& rSize);	// �O���̑傫���ݒ�
	virtual D3DXVECTOR3 GetVec3Sizing(void) const;			// �O���̑傫���擾
	virtual void SetVec3Scaling(const D3DXVECTOR3& rSacle);	// �g�嗦�ݒ�
	virtual D3DXVECTOR3 GetVec3Scaling(void) const;			// �g�嗦�擾
	virtual void SetColor(const D3DXCOLOR& rCol);			// �F�ݒ�
	virtual D3DXCOLOR GetColor(void) const;					// �F�擾
	virtual void SetAllMaterial(const D3DXMATERIAL& rMat);	// �}�e���A���S�ݒ�
	virtual void ResetMaterial(void);						// �}�e���A���Đݒ�

	virtual D3DXMATRIX *GetPtrMtxWorld(void);			// �}�g���b�N�X�|�C���^�擾
	virtual D3DXMATRIX GetMtxWorld(void) const;			// �}�g���b�N�X�擾
	virtual D3DXMATRIX CalcMtxWorld(void) const;		// �}�g���b�N�X�v�Z���ʎ擾
	virtual void SetPriority(const int nPriority);		// �D�揇�ʐݒ�
	virtual void SetEnableUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	virtual void SetEnableDraw(const bool bDraw);		// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static void	ReleaseAll(void);	// �S�j��
	static void	UpdateAll(void);	// �S�X�V
	static void	DrawAll(void);		// �S�`��
	static int	GetNumAll(void);	// �����擾
	static CObject *GetTop(const EDim dimension, const int nPriority);	// �擪�I�u�W�F�N�g�擾
	static CObject *GetCur(const EDim dimension, const int nPriority);	// �Ō���I�u�W�F�N�g�擾

	// �����o�֐�
	void	SetLabel(const ELabel label);		// ���x���ݒ�
	ELabel	GetLabel(void) const;				// ���x���擾
	void	SetDimension(const EDim dimension);	// �����ݒ�
	EDim	GetDimension(void) const;			// �����擾
	int		GetPriority(void) const;			// �D�揇�ʎ擾

	DWORD	GetUniqueID(void) const;	// ���j�[�NID�擾
	bool	IsUpdate(void) const;		// �X�V�󋵎擾
	bool	IsDraw(void) const;			// �`��󋵎擾
	bool	IsDeath(void) const;		// ���S�t���O�擾
	CObject	*GetObject(void);			// �I�u�W�F�N�g�擾
	CObject	*GetPrev(void) const;		// �O�I�u�W�F�N�g�擾
	CObject	*GetNext(void) const;		// ���I�u�W�F�N�g�擾

#ifdef _DEBUG

	// �ÓI�����o�֐�
	static void	SetEnableDebugDispAll(const bool bDisp2D, const bool bDisp3D);	// �S�\���󋵐ݒ�

	// �����o�֐�
	void SetEnableDebugDisp(const bool bDisp)	{ m_bDebugDisp = bDisp; }	// �\���󋵐ݒ�
	bool IsDebugDisp(void) const				{ return m_bDebugDisp; }	// �\���󋵎擾

#endif	// _DEBUG

private:
	// �ÓI�����o�֐�
	static void DeathAll(void);	// �S���S

	// �ÓI�����o�ϐ�
	static CObject	*m_apTop[DIM_MAX][object::MAX_PRIO];	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject	*m_apCur[DIM_MAX][object::MAX_PRIO];	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	static DWORD	m_dwNextID;	// ���̃��j�[�NID
	static int		m_nNumAll;	// �I�u�W�F�N�g�̑���

	// �����o�ϐ�
	ELabel	m_label;		// ���g�̃I�u�W�F�N�g���x��
	EDim	m_dimension;	// ���g�̎���
	int		m_nPriority;	// ���g�̗D�揇��
	DWORD	m_dwID;			// ���g�̃��j�[�NID
	bool	m_bUpdate;		// ���g�̍X�V��
	bool	m_bDraw;		// ���g�̕`���
	bool	m_bDeath;		// ���g�̎��S�t���O
	CObject	*m_pPrev;		// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject	*m_pNext;		// ���̃I�u�W�F�N�g�ւ̃|�C���^

#ifdef _DEBUG

	bool m_bDebugDisp;	// ���g�̕\����

#endif	// _DEBUG

};

#endif	// _OBJECT_H_

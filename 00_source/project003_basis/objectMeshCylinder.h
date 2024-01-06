//============================================================
//
//	�I�u�W�F�N�g���b�V���V�����_�[�w�b�_�[ [objectMeshCylinder.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_MESHCYLINDER_H_
#define _OBJECT_MESHCYLINDER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g���b�V���V�����_�[�N���X
class CObjectMeshCylinder : public CObject
{
public:
	// �e�N�X�`��������
	enum ETexDir
	{
		TEXDIR_OUTSIDE = 0,	// �e�N�X�`���O������
		TEXDIR_INSIDE,		// �e�N�X�`����������
		TEXDIR_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CObjectMeshCylinder(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectMeshCylinder() override;

	// ���b�V���V�����_�[�\����
	struct SMeshCylinder
	{
		D3DXVECTOR3	pos;		// �ʒu
		D3DXVECTOR3	rot;		// ����
		D3DXCOLOR	col;		// �F
		D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
		ETexDir		texDir;		// �e�N�X�`������
		float		fRadius;	// ���a
		float		fHeight;	// �c��
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	void BindTexture(const int nTextureID) override;		// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const char *pTexturePass) override;	// �e�N�X�`������ (�p�X)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾

	void SetColor(const D3DXCOLOR& rCol) override;	// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;		// �F�擾
	void SetRadius(const float fRadius) override;	// ���a�ݒ�
	float GetRadius(void) const override;			// ���a�擾
	void SetHeight(const float fHeight) override;	// �c���ݒ�
	float GetHeight(void) const override;			// �c���擾

	// �ÓI�����o�֐�
	static CObjectMeshCylinder *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart,	// �e�N�X�`��������
		const float fRadius,		// ���a
		const float fHeight			// �c��
	);

	// �����o�֐�
	void SetTexDir(const ETexDir texDir);			// �e�N�X�`�������ݒ�
	ETexDir GetTexDir(void) const;					// �e�N�X�`�������擾
	HRESULT SetPattern(const POSGRID2& rPart);		// �������ݒ�
	POSGRID2 GetPattern(void) const;				// �������擾
	void SetTexPattern(const POSGRID2& rTexPart);	// �e�N�X�`���������ݒ�
	POSGRID2 GetTexPattern(void) const;				// �e�N�X�`���������擾
	CRenderState *GetRenderState(void);				// �����_�[�X�e�[�g���擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�
	void SetIdx(void);	// �C���f�b�N�X���̐ݒ�
	void SetScrollTex	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	( // ����
		const float fTexU,	// �e�N�X�`���̉����W�̊J�n�ʒu
		const float fTexV	// �e�N�X�`���̏c���W�̊J�n�ʒu
	);

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	CRenderState *m_pRenderState;		// �����_�[�X�e�[�g�̏��

	SMeshCylinder m_meshCylinder;	// ���b�V���V�����_�[�̏��
	POSGRID2 m_part;	// ������
	POSGRID2 m_texPart;	// �e�N�X�`��������
	int m_nNumVtx;		// �K�v���_��
	int m_nNumIdx;		// �K�v�C���f�b�N�X��
	int m_nTextureID;	// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_MESHCYLINDER_H_

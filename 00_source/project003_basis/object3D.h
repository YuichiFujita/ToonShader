//============================================================
//
//	�I�u�W�F�N�g3D�w�b�_�[ [object3D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g3D�N���X
class CObject3D : public CObject
{
public:
	// ���_��
	enum EOrigin
	{
		ORIGIN_CENTER = 0,	// ���S���_
		ORIGIN_DOWN,		// �����_
		ORIGIN_MAX,			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CObject3D(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObject3D() override;

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
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	D3DXVECTOR3 GetVec3Sizing(void) const override;			// �傫���擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾

	// �ÓI�����o�֐�
	static CObject3D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rSize,				// �傫��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE,		// �F
		const EOrigin origin = ORIGIN_CENTER	// ���_
	);

	// �����o�֐�
	void SetOrigin(const EOrigin origin);	// ���_�ݒ�
	EOrigin GetOrigin(void) const;			// ���_�擾
	CRenderState *GetRenderState(void);		// �����_�[�X�e�[�g���擾

	void SetVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// ���_�ʒu�ݒ�
	D3DXVECTOR3 GetVertexPosition(const int nID);					// ���_�ʒu�擾
	void SetGapPosition(const int nID, const D3DXVECTOR3& rPos);	// ���W�̂���ݒ�
	D3DXVECTOR3 GetGapPosition(const int nID);						// ���W�̂���擾
	float GetPositionHeight(const D3DXVECTOR3&rPos);				// �|���S���̒��n�擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void NormalizeNormal(void);	// �@���̐��K��

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	CRenderState *m_pRenderState;		// �����_�[�X�e�[�g�̏��
	D3DXVECTOR3 *m_pPosGapBuff;			// ���W�̂���o�b�t�@�ւ̃|�C���^
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;			// �ʒu
	D3DXVECTOR3	m_rot;			// ����
	D3DXVECTOR3	m_size;			// �傫��
	D3DXCOLOR	m_col;			// �F
	EOrigin		m_origin;		// ���_
	int			m_nTextureID;	// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT3D_H_

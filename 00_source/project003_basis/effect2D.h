//============================================================
//
//	�G�t�F�N�g2D�w�b�_�[ [effect2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"
#include "renderState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�t�F�N�g2D�N���X
class CEffect2D : public CObject2D
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEffect2D(const EType type, const CObject::ELabel label = LABEL_NONE);

	// �f�X�g���N�^
	~CEffect2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �����o�֐�
	void SetAlphaBlend(const CRenderState::EBlend blend);	// ���u�����h�ݒ�

	// �ÓI�����o�֐�
	static CEffect2D *Create	// ����
	( // ����
		const D3DXVECTOR3&	rPos,		// �ʒu
		const float			fRadius,	// ���a
		const EType			type		= TYPE_NORMAL,	// �e�N�X�`��
		const int			nLife		= 10,			// ����
		const D3DXVECTOR3&	rMove		= VEC3_ZERO,	// �ړ���
		const D3DXVECTOR3&	rRot		= VEC3_ZERO,	// ����
		const D3DXCOLOR&	rCol		= XCOL_WHITE,	// �F
		const float			fSubSize	= 0.0f,			// ���a�̌��Z��
		const CRenderState::EBlend	blend	= CRenderState::BLEND_ADD,	// ���u�����h��
		const CObject::ELabel		label	= LABEL_EFFECT				// �I�u�W�F�N�g���x��
	);

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	const EType m_type;		// ��ޒ萔
	int		m_nLife;		// ����
	float	m_fSubSize;		// �傫���̌��Z��
	float	m_fSubAlpha;	// �����x�̌��Z��
	CRenderState::EBlend	m_blend;	// ���u�����h��
};

#endif	// _EFFECT2D_H_

//============================================================
//
//	�I�u�W�F�N�g�����w�b�_�[ [objectTornado.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_TORNADO_H_
#define _OBJECT_TORNADO_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace tornado
{
	const int	NUM_AROUND	= 3;		// �Q�̎���
	const int	NUM_PTRN	= 16;		// �Q�̕�����
	const float	MOVE_ROT	= 0.3f;		// �����̕ύX��
	const float	THICKNESS	= 25.0f;	// �|���S���̑���
	const float	OUTER_PLUSY	= 35.0f;	// �|���S���O����Y���W���Z��
	const float	SET_WIDTH	= 80.0f;	// �������̉������
	const float	SET_ALPHA	= 0.85f;	// �������̓����x
	const float	ADD_WIDTH	= 7.0f;		// ������̉��Z��
	const float	ADD_HEIGHT	= 7.0f;		// �c����̉��Z��
	const float	SUB_ALPHA	= 0.015f;	// �����x�̌��Z��
	const float	GROW_WIDTH	= 0.0f;		// ������̐�����
	const float	GROW_HEIGHT	= 0.0f;		// �c����̐�����
	const float	GROW_ALPHA	= 0.0f;		// �����x�̐�����
}

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�����N���X
class CObjectTornado : public CObject
{
public:
	// �R���X�g���N�^
	CObjectTornado();

	// �f�X�g���N�^
	~CObjectTornado() override;

	// �����\����
	struct STornado
	{
		D3DXVECTOR3	pos;		// �ʒu
		D3DXVECTOR3	rot;		// ����
		D3DXVECTOR3	direRot;	// ��������
		D3DXCOLOR	col;		// �F
		D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
		D3DXMATRIX *pMtxParent;	// �e�̃}�g���b�N�X
		float fMoveRot;			// �����̕ύX��
		float fThickness;		// �|���S���̑���
		float fOuterPlusY;		// �|���S���O����Y���W���Z��
		float fSetWidth;		// �������̉������
		float fSetAlpha;		// �������̓����x
		float fAddWidth;		// ������̉��Z��
		float fAddHeight;		// �c����̉��Z��
		float fSubAlpha;		// �����x�̌��Z��
		float fGrowWidth;		// ������̐�����
		float fGrowHeight;		// �c����̐�����
		float fGrowAlpha;		// �����x�̐�����
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾

	// �ÓI�����o�֐�
	static CObjectTornado *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rDireRot,	// ��������
		const D3DXCOLOR& rCol,			// �F
		D3DXMATRIX *pMtxParent	= nullptr,				// �e�̃}�g���b�N�X
		const int	nNumAround	= tornado::NUM_AROUND,	// �Q�̎���
		const int	nPattern	= tornado::NUM_PTRN,	// �Q�̕�����
		const float	fMoveRot	= tornado::MOVE_ROT,	// �����̕ύX��
		const float	fThickness	= tornado::THICKNESS,	// �|���S���̑���
		const float	fOuterPlusY	= tornado::OUTER_PLUSY,	// �|���S���O����Y���W���Z��
		const float	fSetWidth	= tornado::SET_WIDTH,	// �������̉������
		const float	fSetAlpha	= tornado::SET_ALPHA,	// �������̓����x
		const float	fAddWidth	= tornado::ADD_WIDTH,	// ������̉��Z��
		const float	fAddHeight	= tornado::ADD_HEIGHT,	// �c����̉��Z��
		const float	fSubAlpha	= tornado::SUB_ALPHA,	// �����x�̌��Z��
		const float	fGrowWidth	= tornado::GROW_WIDTH,	// ������̐�����
		const float	fGrowHeight	= tornado::GROW_HEIGHT,	// �c����̐�����
		const float	fGrowAlpha	= tornado::GROW_ALPHA	// �����x�̐�����
	);

	// �����o�֐�
	void SetGrow	// �����ݒ�
	( // ����
		float fMoveRot,		// �����̕ύX��
		float fGrowWidth,	// ������̐�����
		float fGrowHeight,	// �c����̐�����
		float fGrowAlpha	// �����x�̐�����
	);
	HRESULT SetVortex	// �Q�ݒ�
	( // ����
		const int nNumAround,	// �Q�̎���
		const int nPattern		// �Q�̕�����
	);

	void DeleteMatrixParent(void);						// �e�}�g���b�N�X�폜
	void SetMatrixParent(D3DXMATRIX *pMtxParent);		// �e�}�g���b�N�X�ݒ�
	void SetDirectionRotation(const D3DXVECTOR3& rRot);	// ���������ݒ�
	D3DXVECTOR3 GetDirectionRotation(void) const;		// ���������擾

	void SetThickness(const float fThickness);		// �|���S���̑����ݒ�
	float GetThickness(void) const;					// �|���S���̑����擾
	void SetOuterPlusY(const float fOuterPlusY);	// �|���S���O����Y���W���Z�ʐݒ�
	float GetOuterPlusY(void) const;				// �|���S���O����Y���W���Z�ʎ擾
	void SetWidth(const float fSetWidth);			// �������̉�����ʐݒ�
	float GetWidth(void) const;						// �������̉�����ʎ擾
	void SetAlpha(const float fSetAlpha);			// �������̓����x�ݒ�
	float GetAlpha(void) const;						// �������̓����x�擾
	void SetAddWidth(const float fAddWidth);		// ������̉��Z�ʐݒ�
	float GetAddWidth(void) const;					// ������̉��Z�ʎ擾
	void SetAddHeight(const float fAddHeight);		// �c����̉��Z�ʐݒ�
	float GetAddHeight(void) const;					// �c����̉��Z�ʎ擾
	void SetSubAlpha(const float fSubAlpha);		// �����x�̌��Z�ʐݒ�
	float GetSubAlpha(void) const;					// �����x�̌��Z�ʎ擾
	CRenderState *GetRenderState(void);				// �����_�[�X�e�[�g���擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	CRenderState *m_pRenderState;		// �����_�[�X�e�[�g�̏��
	STornado m_tornado;	// �����̏��
	int m_nNumVtx;		// �K�v���_��
	int m_nNumAround;	// �Q�̎���
	int m_nPattern;		// �Q�̕�����
	int m_nTextureID;	// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_TORNADO_H_

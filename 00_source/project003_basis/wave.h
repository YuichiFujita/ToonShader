//============================================================
//
//	�g���w�b�_�[ [wave.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WAVE_H_
#define _WAVE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scrollMeshRing.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace ring
{
	const POSGRID2 PART		= POSGRID2(16, 1);	// ������
	const POSGRID2 TEX_PART	= POSGRID2(16, 1);	// �e�N�X�`��������
	const float HOLE_RADIUS	= 200.0f;	// ���̔��a
	const float THICKNESS	= 80.0f;	// ����
	const float OUTER_PLUSY	= 0.0f;		// �O����Y���W���Z��
}

//************************************************************
//	�N���X��`
//************************************************************
// �g���N���X
class CWave : public CScrollMeshRing
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_NONE = 0,	// �e�N�X�`���Ȃ�
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CWave();

	// �f�X�g���N�^
	~CWave() override;

	// �����\����
	struct SGrow
	{
		// �R���X�g���N�^
		SGrow() {}
		SGrow(const float fRadius, const float fThick, const float fAlpha)
		{ fAddHoleRadius = fRadius; fAddThickness = fThick; fSubAlpha = fAlpha; }

		// �f�X�g���N�^
		~SGrow() {}

		// �����o�ϐ�
		float fAddHoleRadius;	// ���̔��a�̐�����
		float fAddThickness;	// �����̐�����
		float fSubAlpha;		// �����x�̐�����
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CWave *Create	// ����
	( // ����
		const ETexture texture,		// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol,		// �F
		const SGrow& rGrow,			// ������
		const POSGRID2& rPart		= ring::PART,			// ������
		const POSGRID2& rTexPart	= ring::TEX_PART,		// �e�N�X�`��������
		const float fHoleRadius		= ring::HOLE_RADIUS,	// ���̔��a
		const float fThickness		= ring::THICKNESS,		// ����
		const float fOuterPlusY		= ring::OUTER_PLUSY		// �O����Y���W���Z��
	);

	// �����o�֐�
	void SetGrow(const SGrow& rGrow);	// �������ݒ�
	SGrow GetGrow(void) const;			// �������擾

private:
	// �����o�ϐ�
	SGrow m_grow;	// �����̏��
};

#endif	// _WAVE_H_

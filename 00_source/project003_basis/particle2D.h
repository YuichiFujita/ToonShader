//============================================================
//
//	�p�[�e�B�N��2D�w�b�_�[ [particle2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PARTICLE2D_H_
#define _PARTICLE2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �p�[�e�B�N��2D�N���X
class CParticle2D : public CObject
{
public:
	// �R���X�g���N�^
	CParticle2D();

	// �f�X�g���N�^
	~CParticle2D() override;

	// ��ޗ�
	enum EType
	{
		TYPE_NONE = 0,	// �Ȃ�
		TYPE_DAMAGE,	// �_���[�W
		TYPE_ITEM,		// �A�C�e��
		TYPE_GETITEM,	// �A�C�e���擾
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetType(const int nType) override;					// ��ސݒ�
	int GetType(void) const override;						// ��ގ擾
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾

	// �ÓI�����o�֐�
	static CParticle2D *Create	// ����
	( // ����
		const EType type,					// ���
		const D3DXVECTOR3& rPos,			// �ʒu
		const D3DXCOLOR& rCol = XCOL_WHITE	// �F
	);

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// �_���[�W
	void Item(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);		// �A�C�e��
	void GetItem(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// �A�C�e���擾

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXCOLOR m_col;	// �F
	EType m_type;		// ���
	int  m_nLife;		// ����
};

#endif	// _PARTICLE2D_H_

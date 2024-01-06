//============================================================
//
//	�p�[�e�B�N��2D���� [particle2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "particle2D.h"
#include "effect2D.h"
#include "renderState.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int SET_LIFE[] =	// ��ނ��Ƃ̎���
	{
		0,	// �Ȃ�
		18,	// �_���[�W
		1,	// �A�C�e��
		2,	// �A�C�e���擾
	};

	// �_���[�W
	namespace damage
	{
		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �_���[�W�̃��u�����h

		const float	MOVE		= 3.0f;		// �_���[�W�̈ړ���
		const int	SPAWN		= 35;		// �_���[�W�̐�����
		const int	EFF_LIFE	= 120;		// �_���[�W�̎���
		const float	SIZE		= 50.0f;	// �_���[�W�̑傫��
		const float	SUB_SIZE	= 1.8f;		// �_���[�W�̔��a�̌��Z��
	}

	// �A�C�e��
	namespace item
	{

		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �A�C�e���̃��u�����h

		const float	MOVE		= 2.0f;		// �A�C�e���̈ړ���
		const int	SPAWN		= 6;		// �A�C�e���̐�����
		const int	EFF_LIFE	= 80;		// �A�C�e���̎���
		const float	SIZE		= 30.0f;	// �A�C�e���̑傫��
		const float	SUB_SIZE	= 1.2f;		// �A�C�e���̔��a�̌��Z��
	}

	// �A�C�e���擾
	namespace itemGet
	{
		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �A�C�e���擾�̃��u�����h

		const float	MOVE		= 3.0f;		// �A�C�e���擾�̈ړ���
		const int	SPAWN		= 35;		// �A�C�e���擾�̐�����
		const int	EFF_LIFE	= 120;		// �A�C�e���擾�̎���
		const float	SIZE		= 50.0f;	// �A�C�e���擾�̑傫��
		const float	SUB_SIZE	= 1.8f;		// �A�C�e���擾�̔��a�̌��Z��
	}
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(SET_LIFE) == CParticle2D::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�q�N���X [CParticle2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CParticle2D::CParticle2D() : CObject(CObject::LABEL_PARTICLE, CObject::DIM_2D),
	m_pos	(VEC3_ZERO),	// �ʒu
	m_col	(XCOL_WHITE),	// �F
	m_type	(TYPE_NONE),	// ���
	m_nLife	(0)				// ����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CParticle2D::~CParticle2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CParticle2D::Init(void)
{
	// �����o�ϐ���������
	m_pos	= VEC3_ZERO;	// �ʒu
	m_col	= XCOL_WHITE;	// �F
	m_type	= TYPE_NONE;	// ���
	m_nLife	= 0;			// ����

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CParticle2D::Uninit(void)
{
	// �p�[�e�B�N��2D�I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CParticle2D::Update(void)
{
	if (m_nLife > 0)
	{ // ����������ꍇ

		// ���������Z
		m_nLife--;
	}
	else
	{ // �������Ȃ��ꍇ

		// �p�[�e�B�N��2D�I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	switch (m_type)
	{ // ��ނ��Ƃ̏���
	case TYPE_DAMAGE:

		// �_���[�W�p�[�e�B�N��2D
		Damage(m_pos, m_col);

		break;

	case TYPE_ITEM:

		// �A�C�e���p�[�e�B�N��2D
		Item(m_pos, m_col);

		break;

	case TYPE_GETITEM:

		// �A�C�e���擾�p�[�e�B�N��2D
		GetItem(m_pos, m_col);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CParticle2D::Draw(void)
{

}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CParticle2D::SetType(const int nType)
{
	if (nType > NONE_IDX && nType < TYPE_MAX)
	{ // ��ނ����K�̏ꍇ

		// ��ނ�ݒ�
		m_type = (EType)nType;

		// ������ݒ�
		m_nLife = SET_LIFE[nType];
	}
	else { assert(false); }
}

//============================================================
//	��ގ擾����
//============================================================
int CParticle2D::GetType(void) const
{
	// ��ނ�Ԃ�
	return (int)m_type;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CParticle2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CParticle2D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CParticle2D::SetColor(const D3DXCOLOR& rCol)
{
	// �F��ݒ�
	m_col = rCol;
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CParticle2D::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}

//============================================================
//	�j������
//============================================================
void CParticle2D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�_���[�W�p�[�e�B�N��2D����
//============================================================
void CParticle2D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	if ((m_nLife + 1) % 9 == 0)
	{ // ������9�̔{���̏ꍇ

		for (int nCntPart = 0; nCntPart < damage::SPAWN; nCntPart++)
		{ // ���������G�t�F�N�g�����J��Ԃ�

			// �x�N�g���������_���ɐݒ�
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = 0.0f;

			// �x�N�g���𐳋K��
			D3DXVec3Normalize(&move, &move);

			// �ړ��ʂ�ݒ�
			move.x *= damage::MOVE;
			move.y *= damage::MOVE;
			move.z = 0.0f;

			// ������ݒ�
			rot = VEC3_ZERO;

			// �G�t�F�N�g2D�I�u�W�F�N�g�̐���
			CEffect2D::Create
			( // ����
				rPos,					// �ʒu
				damage::SIZE,			// ���a
				CEffect2D::TYPE_NORMAL,	// �e�N�X�`��
				damage::EFF_LIFE,		// ����
				move,					// �ړ���
				rot,					// ����
				rCol,					// �F
				damage::SUB_SIZE,		// ���a�̌��Z��
				damage::BLEND,			// ���Z������
				LABEL_PARTICLE			// �I�u�W�F�N�g���x��
			);
		}
	}
}

//============================================================
//	�A�C�e���p�[�e�B�N��2D����
//============================================================
void CParticle2D::Item(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	for (int nCntPart = 0; nCntPart < item::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = 0.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= item::MOVE;
		move.y *= item::MOVE;
		move.z = 0.0f;

		// ������ݒ�
		rot = VEC3_ZERO;

		// �G�t�F�N�g2D�I�u�W�F�N�g�̐���
		CEffect2D::Create
		( // ����
			rPos,					// �ʒu
			item::SIZE,				// ���a
			CEffect2D::TYPE_NORMAL,	// �e�N�X�`��
			item::EFF_LIFE,			// ����
			move,					// �ړ���
			rot,					// ����
			rCol,					// �F
			item::SUB_SIZE,			// ���a�̌��Z��
			item::BLEND,			// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�A�C�e���擾�p�[�e�B�N��2D����
//============================================================
void CParticle2D::GetItem(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	for (int nCntPart = 0; nCntPart < itemGet::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = 0.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= itemGet::MOVE;
		move.y *= itemGet::MOVE;
		move.z = 0.0f;

		// ������ݒ�
		rot = VEC3_ZERO;

		// �G�t�F�N�g2D�I�u�W�F�N�g�̐���
		CEffect2D::Create
		( // ����
			rPos,					// �ʒu
			itemGet::SIZE,			// ���a
			CEffect2D::TYPE_NORMAL,	// �e�N�X�`��
			itemGet::EFF_LIFE,		// ����
			move,					// �ړ���
			rot,					// ����
			rCol,					// �F
			itemGet::SUB_SIZE,		// ���a�̌��Z��
			itemGet::BLEND,			// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	��������
//============================================================
CParticle2D *CParticle2D::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �p�[�e�B�N��2D�̐���
	CParticle2D *pParticle2D = new CParticle2D;
	if (pParticle2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �p�[�e�B�N��2D�̏�����
		if (FAILED(pParticle2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �p�[�e�B�N��2D�̔j��
			SAFE_DELETE(pParticle2D);
			return nullptr;
		}

		// ��ނ�ݒ�
		pParticle2D->SetType(type);

		// �ʒu��ݒ�
		pParticle2D->SetVec3Position(rPos);

		// �F��ݒ�
		pParticle2D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pParticle2D;
	}
}

//============================================================
//
//	�p�[�e�B�N��3D���� [particle3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "particle3D.h"
#include "effect3D.h"
#include "renderState.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int SET_LIFE[] =	// ��ނ��Ƃ̎���
	{
		0,	// �Ȃ�
		24,	// �_���[�W
		1,	// ��
		1,	// �A�����݂Ԃ�
		1,	// ������
		1,	// �唚��
		1,	// �v���C���[�_���[�W
		1,	// �}�Y���t���b�V��
		1,	// �^���N�t�@�C�A
		24,	// �o�u������
	};

	// �_���[�W�E�o�u������
	namespace damage
	{
		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �_���[�W�E�o�u�������̃��u�����h

		const float	MOVE		= 5.0f;		// �_���[�W�E�o�u�������̈ړ���
		const int	SPAWN		= 35;		// �_���[�W�E�o�u�������̐�����
		const int	EFF_LIFE	= 120;		// �_���[�W�E�o�u�������̎���
		const float	SIZE		= 80.0f;	// �_���[�W�E�o�u�������̑傫��
		const float	SUB_SIZE	= 2.4f;		// �_���[�W�E�o�u�������̔��a�̌��Z��
	}

	// ��
	namespace heal
	{
		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �񕜂̃��u�����h

		const float	POSGAP		= 24.0f;	// �񕜂̈ʒu�����
		const float	MOVE		= 1.2f;		// �񕜂̈ړ���
		const int	SPAWN		= 6;		// �񕜂̐�����
		const int	EFF_LIFE	= 50;		// �񕜂̎���
		const float	SIZE		= 55.0f;	// �񕜂̑傫��
		const float	SUB_SIZE	= 0.5f;		// �񕜂̔��a�̌��Z��
	}

	// �A�����݂Ԃ�
	namespace stomp
	{
		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �A�����݂Ԃ��̃��u�����h

		const float	MOVE		= 2.5;		// �A�����݂Ԃ��̈ړ���
		const int	SPAWN		= 8;		// �A�����݂Ԃ��̐�����
		const int	EFF_LIFE	= 28;		// �A�����݂Ԃ��̎���
		const float	SIZE		= 36.0f;	// �A�����݂Ԃ��̑傫��
		const float	SUB_SIZE	= 0.05f;	// �A�����݂Ԃ��̔��a�̌��Z��
	}

	// ������
	namespace smallExplosion
	{
		namespace fire
		{
			const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �����̉��̃��u�����h
			const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f);	// �����̉��̐F

			const float	POSGAP		= 5.0f;		// �����̉��̈ʒu�����
			const float	MOVE		= 2.0f;		// �����̉��̈ړ���
			const int	SPAWN		= 48;		// �����̉��̐�����
			const int	EFF_LIFE	= 18;		// �����̉��̎���
			const int	RAND_LIFE	= 8;		// �����̉��̃����_���������Z�ʂ̍ő�l
			const float	SIZE		= 3.27f;	// �����̉��̑傫��
			const float	SUB_SIZE	= -8.5f;	// �����̉��̔��a�̌��Z��
		}

		namespace smoke
		{
			const CRenderState::EBlend BLEND = CRenderState::BLEND_NORMAL;	// �����̉��̃��u�����h
			const D3DXCOLOR COL = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);		// �����̉��̐F

			const float	POSGAP		= 3.0f;		// �����̉��̈ʒu�����
			const float	MOVE		= 1.2f;		// �����̉��̈ړ���
			const int	SPAWN		= 66;		// �����̉��̐�����
			const int	EFF_LIFE	= 44;		// �����̉��̎���
			const int	RAND_LIFE	= 12;		// �����̉��̃����_���������Z�ʂ̍ő�l
			const float	SIZE		= 70.0f;	// �����̉��̑傫��
			const float	SUB_SIZE	= -1.0f;	// �����̉��̔��a�̌��Z��
		}
	}

	// �唚��
	namespace bigExplosion
	{
		namespace fire
		{
			const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �����̉��̃��u�����h
			const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f);	// �����̉��̐F

			const float	POSGAP		= 10.0f;	// �����̉��̈ʒu�����
			const float	MOVE		= 2.54f;	// �����̉��̈ړ���
			const int	SPAWN		= 52;		// �����̉��̐�����
			const int	EFF_LIFE	= 36;		// �����̉��̎���
			const int	RAND_LIFE	= 12;		// �����̉��̃����_���������Z�ʂ̍ő�l
			const float	SIZE		= 3.57f;	// �����̉��̑傫��
			const float	SUB_SIZE	= -7.0f;	// �����̉��̔��a�̌��Z��
		}

		namespace smoke
		{
			const CRenderState::EBlend BLEND = CRenderState::BLEND_NORMAL;	// �����̉��̃��u�����h
			const D3DXCOLOR COL = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// �����̉��̐F

			const float	POSGAP		= 8.0f;		// �����̉��̈ʒu�����
			const float	MOVE		= 0.9f;		// �����̉��̈ړ���
			const int	SPAWN		= 72;		// �����̉��̐�����
			const int	EFF_LIFE	= 96;		// �����̉��̎���
			const int	RAND_LIFE	= 18;		// �����̉��̃����_���������Z�ʂ̍ő�l
			const float	SIZE		= 70.0f;	// �����̉��̑傫��
			const float	SUB_SIZE	= -2.5f;	// �����̉��̔��a�̌��Z��
		}
	}

	// �v���C���[�_���[�W
	namespace playerDamage
	{
		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �v���C���[�_���[�W�̃��u�����h

		const float	POSGAP		= 12.0f;	// �v���C���[�_���[�W�̈ʒu�����
		const float	MOVE_S		= 6.6f;		// �v���C���[�_���[�W�̈ړ��� (��)
		const float	MOVE_M		= 5.4f;		// �v���C���[�_���[�W�̈ړ��� (��)
		const float	MOVE_L		= 4.2f;		// �v���C���[�_���[�W�̈ړ��� (��)
		const int	EFF_SPAWN	= 4;		// �v���C���[�_���[�W�̐�����
		const int	RAND_SPAWN	= 6;		// �v���C���[�_���[�W�̃����_�����������Z�ʂ̍ő�l
		const int	EFF_LIFE	= 28;		// �v���C���[�_���[�W�̎���
		const int	RAND_LIFE	= 12;		// �v���C���[�_���[�W�̃����_���������Z�ʂ̍ő�l
		const float	SIZE_S		= 32.0f;	// �v���C���[�_���[�W�̑傫�� (��)
		const float	SIZE_M		= 42.0f;	// �v���C���[�_���[�W�̑傫�� (��)
		const float	SIZE_L		= 52.0f;	// �v���C���[�_���[�W�̑傫�� (��)
		const float	SUB_SIZE	= 0.25f;	// �v���C���[�_���[�W�̔��a�̌��Z��
	}

	// �}�Y���t���b�V��
	namespace muzzleflash
	{
		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �}�Y���t���b�V���̃��u�����h
		const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f);	// �}�Y���t���b�V���̐F

		const float	MOVE		= 0.64f;	// �}�Y���t���b�V���̈ړ���
		const int	SPAWN		= 48;		// �}�Y���t���b�V���̐�����
		const int	EFF_LIFE	= 8;		// �}�Y���t���b�V���̎���
		const float	SIZE		= 0.57f;	// �}�Y���t���b�V���̑傫��
		const float	SUB_SIZE	= -1.42f;	// �}�Y���t���b�V���̔��a�̌��Z��
	}

	// �^���N�t�@�C�A
	namespace tankfire
	{
		const CRenderState::EBlend BLEND = CRenderState::BLEND_ADD;	// �^���N�t�@�C�A�̃��u�����h
		const D3DXCOLOR	COL = D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f);	// �^���N�t�@�C�A�̐F

		const float	MOVE		= 2.64f;	// �^���N�t�@�C�A�̈ړ���
		const int	SPAWN		= 58;		// �^���N�t�@�C�A�̐�����
		const int	EFF_LIFE	= 22;		// �^���N�t�@�C�A�̎���
		const float	SIZE		= 2.57f;	// �^���N�t�@�C�A�̑傫��
		const float	SUB_SIZE	= -3.42f;	// �^���N�t�@�C�A�̔��a�̌��Z��
	}
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(SET_LIFE) == CParticle3D::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�q�N���X [CParticle3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CParticle3D::CParticle3D() : CObject(CObject::LABEL_PARTICLE, CObject::DIM_3D),
	m_pos	(VEC3_ZERO),	// �ʒu
	m_col	(XCOL_WHITE),	// �F
	m_type	(TYPE_NONE),	// ���
	m_nLife	(0)				// ����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CParticle3D::~CParticle3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CParticle3D::Init(void)
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
void CParticle3D::Uninit(void)
{
	// �p�[�e�B�N��3D�I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CParticle3D::Update(void)
{
	if (m_nLife > 0)
	{ // ����������ꍇ

		// ���������Z
		m_nLife--;
	}
	else
	{ // �������Ȃ��ꍇ

		// �p�[�e�B�N��3D�I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	switch (m_type)
	{ // ��ނ��Ƃ̏���
	case TYPE_DAMAGE:

		// �_���[�W
		Damage(m_pos, m_col);

		break;

	case TYPE_HEAL:

		// ��
		Heal(m_pos, m_col);

		break;

	case TYPE_STOMP_PLANT:

		// �A�����݂Ԃ�
		StompPlant(m_pos, m_col);

		break;

	case TYPE_SMALL_EXPLOSION:

		// ������
		SmallExplosion(m_pos);

		break;

	case TYPE_BIG_EXPLOSION:

		// �唚��
		BigExplosion(m_pos);

		break;

	case TYPE_PLAYER_DAMAGE:

		// �v���C���[�_���[�W
		PlayerDamage(m_pos);

		break;

	case TYPE_MUZZLE_FLASH:

		// �}�Y���t���b�V��
		MuzzleFlash(m_pos);

		break;

	case TYPE_TANK_FIRE:

		// �^���N�t�@�C�A
		TankFire(m_pos);

		break;

	case TYPE_BUBBLE_EXPLOSION:

		// �o�u������
		BubbleExplosion(m_pos);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CParticle3D::Draw(void)
{

}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CParticle3D::SetType(const int nType)
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
int CParticle3D::GetType(void) const
{
	// ��ނ�Ԃ�
	return (int)m_type;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CParticle3D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CParticle3D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CParticle3D::SetColor(const D3DXCOLOR& rCol)
{
	// �F��ݒ�
	m_col = rCol;
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CParticle3D::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}

//============================================================
//	�j������
//============================================================
void CParticle3D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�_���[�W
//============================================================
void CParticle3D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	if ((m_nLife + 1) % 12 == 0)
	{ // ������12�̔{���̏ꍇ

		for (int nCntPart = 0; nCntPart < damage::SPAWN; nCntPart++)
		{ // ���������G�t�F�N�g�����J��Ԃ�

			// �x�N�g���������_���ɐݒ�
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

			// �x�N�g���𐳋K��
			D3DXVec3Normalize(&move, &move);

			// �ړ��ʂ�ݒ�
			move.x *= damage::MOVE;
			move.y *= damage::MOVE;
			move.z *= damage::MOVE;

			// ������ݒ�
			rot = VEC3_ZERO;

			// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
			CEffect3D::Create
			( // ����
				rPos,					// �ʒu
				damage::SIZE,			// ���a
				CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
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
//	��
//============================================================
void CParticle3D::Heal(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	for (int nCntPart = 0; nCntPart < heal::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * heal::POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * heal::MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,					// �ʒu
			heal::SIZE,				// ���a
			CEffect3D::TYPE_HEAL,	// �e�N�X�`��
			heal::EFF_LIFE,			// ����
			move,					// �ړ���
			rot,					// ����
			rCol,					// �F
			heal::SUB_SIZE,			// ���a�̌��Z��
			heal::BLEND,			// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�A�����݂Ԃ�
//============================================================
void CParticle3D::StompPlant(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	for (int nCntPart = 0; nCntPart < stomp::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= stomp::MOVE;
		move.y *= stomp::MOVE;
		move.z *= stomp::MOVE;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			rPos,					// �ʒu
			stomp::SIZE,			// ���a
			CEffect3D::TYPE_LEAF,	// �e�N�X�`��
			stomp::EFF_LIFE,		// ����
			move,					// �ړ���
			rot,					// ����
			rCol,					// �F
			stomp::SUB_SIZE,		// ���a�̌��Z��
			stomp::BLEND,			// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	������
//============================================================
void CParticle3D::SmallExplosion(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < smallExplosion::smoke::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * smallExplosion::smoke::POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * smallExplosion::smoke::MOVE;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// ������ݒ�
		nLife = (rand() % smallExplosion::smoke::RAND_LIFE) + smallExplosion::smoke::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,								// �ʒu
			smallExplosion::smoke::SIZE,		// ���a
			CEffect3D::TYPE_SMOKE,				// �e�N�X�`��
			nLife,								// ����
			move,								// �ړ���
			rot,								// ����
			smallExplosion::smoke::COL,			// �F
			smallExplosion::smoke::SUB_SIZE,	// ���a�̌��Z��
			smallExplosion::smoke::BLEND,		// ���Z������
			LABEL_PARTICLE						// �I�u�W�F�N�g���x��
		);
	}

	for (int nCntPart = 0; nCntPart < smallExplosion::fire::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * smallExplosion::fire::POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * smallExplosion::fire::MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % smallExplosion::fire::RAND_LIFE) + smallExplosion::fire::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,							// �ʒu
			smallExplosion::fire::SIZE,		// ���a
			CEffect3D::TYPE_NORMAL,			// �e�N�X�`��
			nLife,							// ����
			move,							// �ړ���
			rot,							// ����
			smallExplosion::fire::COL,		// �F
			smallExplosion::fire::SUB_SIZE,	// ���a�̌��Z��
			smallExplosion::fire::BLEND,	// ���Z������
			LABEL_PARTICLE					// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�唚��
//============================================================
void CParticle3D::BigExplosion(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < bigExplosion::smoke::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * bigExplosion::smoke::POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * bigExplosion::smoke::MOVE;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// ������ݒ�
		nLife = (rand() % bigExplosion::smoke::RAND_LIFE) + bigExplosion::smoke::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,							// �ʒu
			bigExplosion::smoke::SIZE,		// ���a
			CEffect3D::TYPE_SMOKE,			// �e�N�X�`��
			nLife,							// ����
			move,							// �ړ���
			rot,							// ����
			bigExplosion::smoke::COL,		// �F
			bigExplosion::smoke::SUB_SIZE,	// ���a�̌��Z��
			bigExplosion::smoke::BLEND,		// ���Z������
			LABEL_PARTICLE					// �I�u�W�F�N�g���x��
		);
	}

	for (int nCntPart = 0; nCntPart < bigExplosion::fire::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * bigExplosion::fire::POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * bigExplosion::fire::MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % bigExplosion::fire::RAND_LIFE) + bigExplosion::fire::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,							// �ʒu
			bigExplosion::fire::SIZE,		// ���a
			CEffect3D::TYPE_NORMAL,			// �e�N�X�`��
			nLife,							// ����
			move,							// �ړ���
			rot,							// ����
			bigExplosion::fire::COL,		// �F
			bigExplosion::fire::SUB_SIZE,	// ���a�̌��Z��
			bigExplosion::fire::BLEND,		// ���Z������
			LABEL_PARTICLE					// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�v���C���[�_���[�W
//============================================================
void CParticle3D::PlayerDamage(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	D3DXCOLOR   col  = XCOL_WHITE;	// �F�̑���p
	int nSpawn = 0;	// �������̑���p
	int nLife = 0;	// �����̑���p

	// ��������ݒ�
	nSpawn = (rand() % playerDamage::EFF_SPAWN) + playerDamage::RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * playerDamage::POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * playerDamage::MOVE_S;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// �F��ݒ�
		col.r = (float)(rand() % 80 + 20) / 100.0f;
		col.g = (float)(rand() % 20 + 80) / 100.0f;
		col.b = (float)(rand() % 80 + 20) / 100.0f;
		col.a = 1.0f;

		// ������ݒ�
		nLife = (rand() % playerDamage::RAND_LIFE) + playerDamage::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,						// �ʒu
			playerDamage::SIZE_S,		// ���a
			CEffect3D::TYPE_PIECE_S,	// �e�N�X�`��
			nLife,						// ����
			move,						// �ړ���
			rot,						// ����
			col,						// �F
			playerDamage::SUB_SIZE,		// ���a�̌��Z��
			playerDamage::BLEND,		// ���Z������
			LABEL_PARTICLE				// �I�u�W�F�N�g���x��
		);
	}

	// ��������ݒ�
	nSpawn = (rand() % playerDamage::EFF_SPAWN) + playerDamage::RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * playerDamage::POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * playerDamage::MOVE_M;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// �F��ݒ�
		col.r = (float)(rand() % 20 + 80) / 100.0f;
		col.g = (float)(rand() % 80 + 20) / 100.0f;
		col.b = (float)(rand() % 80 + 20) / 100.0f;
		col.a = 1.0f;

		// ������ݒ�
		nLife = (rand() % playerDamage::RAND_LIFE) + playerDamage::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,						// �ʒu
			playerDamage::SIZE_M,		// ���a
			CEffect3D::TYPE_PIECE_M,	// �e�N�X�`��
			nLife,						// ����
			move,						// �ړ���
			rot,						// ����
			col,						// �F
			playerDamage::SUB_SIZE,		// ���a�̌��Z��
			playerDamage::BLEND,		// ���Z������
			LABEL_PARTICLE				// �I�u�W�F�N�g���x��
		);
	}

	// ��������ݒ�
	nSpawn = (rand() % playerDamage::EFF_SPAWN) + playerDamage::RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * playerDamage::POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * playerDamage::MOVE_L;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// �F��ݒ�
		col.r = (float)(rand() % 80 + 20) / 100.0f;
		col.g = (float)(rand() % 80 + 20) / 100.0f;
		col.b = (float)(rand() % 20 + 80) / 100.0f;
		col.a = 1.0f;

		// ������ݒ�
		nLife = (rand() % playerDamage::RAND_LIFE) + playerDamage::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,						// �ʒu
			playerDamage::SIZE_L,		// ���a
			CEffect3D::TYPE_PIECE_L,	// �e�N�X�`��
			nLife,						// ����
			move,						// �ړ���
			rot,						// ����
			col,						// �F
			playerDamage::SUB_SIZE,		// ���a�̌��Z��
			playerDamage::BLEND,		// ���Z������
			LABEL_PARTICLE				// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�}�Y���t���b�V��
//============================================================
void CParticle3D::MuzzleFlash(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < muzzleflash::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= muzzleflash::MOVE;
		move.y *= muzzleflash::MOVE;
		move.z *= muzzleflash::MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % 6) + muzzleflash::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			rPos,					// �ʒu
			muzzleflash::SIZE,		// ���a
			CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
			nLife,					// ����
			move,					// �ړ���
			rot,					// ����
			muzzleflash::COL,		// �F
			muzzleflash::SUB_SIZE,	// ���a�̌��Z��
			muzzleflash::BLEND,		// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�^���N�t�@�C�A
//============================================================
void CParticle3D::TankFire(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < tankfire::SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= tankfire::MOVE;
		move.y *= tankfire::MOVE;
		move.z *= tankfire::MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % 6) + tankfire::EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			rPos,					// �ʒu
			tankfire::SIZE,			// ���a
			CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
			nLife,					// ����
			move,					// �ړ���
			rot,					// ����
			tankfire::COL,			// �F
			tankfire::SUB_SIZE,		// ���a�̌��Z��
			tankfire::BLEND,		// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�o�u������
//============================================================
void CParticle3D::BubbleExplosion(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	if ((m_nLife + 1) % 12 == 0)
	{ // ������12�̔{���̏ꍇ

		for (int nCntPart = 0; nCntPart < damage::SPAWN; nCntPart++)
		{ // ���������G�t�F�N�g�����J��Ԃ�

			// �x�N�g���������_���ɐݒ�
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

			// �x�N�g���𐳋K��
			D3DXVec3Normalize(&move, &move);

			// �ړ��ʂ�ݒ�
			move.x *= damage::MOVE;
			move.y *= damage::MOVE;
			move.z *= damage::MOVE;

			// ������ݒ�
			rot = VEC3_ZERO;

			// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
			CEffect3D::Create
			( // ����
				rPos,					// �ʒu
				damage::SIZE,			// ���a
				CEffect3D::TYPE_BUBBLE,	// �e�N�X�`��
				damage::EFF_LIFE,		// ����
				move,					// �ړ���
				rot,					// ����
				XCOL_WHITE,				// �F
				damage::SUB_SIZE,		// ���a�̌��Z��
				damage::BLEND,			// ���Z������
				LABEL_PARTICLE			// �I�u�W�F�N�g���x��
			);
		}
	}
}

//============================================================
//	��������
//============================================================
CParticle3D *CParticle3D::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �p�[�e�B�N��3D�̐���
	CParticle3D *pParticle3D = new CParticle3D;
	if (pParticle3D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �p�[�e�B�N��3D�̏�����
		if (FAILED(pParticle3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �p�[�e�B�N��3D�̔j��
			SAFE_DELETE(pParticle3D);
			return nullptr;
		}

		// ��ނ�ݒ�
		pParticle3D->SetType(type);

		// �ʒu��ݒ�
		pParticle3D->SetVec3Position(rPos);

		// �F��ݒ�
		pParticle3D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pParticle3D;
	}
}

//============================================================
//
//	�G�t�F�N�g2D���� [effect2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\EFFECT\\effect000.jpg",	// �ʏ�e�N�X�`��
	};

	const int EFF_PRIO	= 3;	// �G�t�F�N�g2D�̗D�揇��
	const int PART_PRIO	= 4;	// �p�[�e�B�N��2D�̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CEffect2D::TYPE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CEffect2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEffect2D::CEffect2D(const EType type, const CObject::ELabel label) : CObject2D(CObject::LABEL_EFFECT, CObject::DIM_2D, (label == LABEL_EFFECT) ? EFF_PRIO : PART_PRIO),
	m_type		(type),			// ���
	m_move		(VEC3_ZERO),	// �ړ���
	m_nLife		(0),			// ����
	m_fSubSize	(0.0f),			// �傫���̌��Z��
	m_fSubAlpha	(0.0f),			// �����x�̌��Z��
	m_blend		(CRenderState::BLEND_ADD)	// ���u�����h��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEffect2D::~CEffect2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEffect2D::Init(void)
{
	// �����o�ϐ���������
	m_move		= VEC3_ZERO;	// �ړ���
	m_nLife		= 0;			// ����
	m_fSubSize	= 0.0f;			// �傫���̌��Z��
	m_fSubAlpha	= 0.0f;			// �����x�̌��Z��
	m_blend		= CRenderState::BLEND_ADD;	// ���u�����h��

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEffect2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEffect2D::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos  = GetVec3Position();	// �ʒu
	D3DXVECTOR3 rot  = GetVec3Rotation();	// ����
	D3DXVECTOR3 size = GetVec3Sizing();		// �傫��
	D3DXCOLOR   col  = GetColor();			// �F
	float fRadius = size.x;					// ���a

	if (m_nLife <= 0		// �������}����
	||  fRadius <= 0.0f)	// ���a��0.0f�ȉ�
	{ // ��L�̂ǂꂩ�ɂȂ����ꍇ

		// �I�u�W�F�N�g��j��
		Uninit();

		// �֐��𔲂���
		return;
	}

	// �ړ��ʂ����Z
	pos += m_move;

	// ���������Z
	m_nLife--;

	// ���a�����Z
	fRadius -= m_fSubSize;
	if (fRadius < 0.0f)
	{ // ���a��0.0f��菬�����ꍇ

		// ���a��␳
		fRadius = 0.0f;
	}

	// ���l�����Z
	col.a -= m_fSubAlpha;
	useful::LimitNum(col.a, 0.0f, 1.0f);	// ���l����

	// �ʒu��ݒ�
	CObject2D::SetVec3Position(pos);

	// ������ݒ�
	CObject2D::SetVec3Rotation(rot);

	// �傫����ݒ�
	CObject2D::SetVec3Sizing(D3DXVECTOR3(fRadius, fRadius, 0.0f));

	// �F��ݒ�
	CObject2D::SetColor(col);

	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEffect2D::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw();
}

//============================================================
//	���u�����h�̐ݒ菈��
//============================================================
void CEffect2D::SetAlphaBlend(const CRenderState::EBlend blend)
{
	// �����̃��u�����h��ݒ�
	m_blend = blend;

	// ���u�����h��ύX
	GetRenderState()->SetAlphaBlend(blend);
}

//============================================================
//	��������
//============================================================
CEffect2D *CEffect2D::Create
(
	const D3DXVECTOR3& rPos,			// �ʒu
	const float fRadius,				// ���a
	const EType type,					// �e�N�X�`��
	const int nLife,					// ����
	const D3DXVECTOR3& rMove,			// �ړ���
	const D3DXVECTOR3& rRot,			// ����
	const D3DXCOLOR& rCol,				// �F
	const float fSubSize,				// ���a�̌��Z��
	const CRenderState::EBlend blend,	// ���u�����h��
	const CObject::ELabel label			// �I�u�W�F�N�g���x��
)
{
	// �G�t�F�N�g2D�̐���
	CEffect2D *pEffect2D = new CEffect2D(type, label);
	if (pEffect2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�t�F�N�g2D�̏�����
		if (FAILED(pEffect2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�t�F�N�g2D�̔j��
			SAFE_DELETE(pEffect2D);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pEffect2D->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[type]));

		// �ʒu��ݒ�
		pEffect2D->SetVec3Position(rPos);

		// ������ݒ�
		pEffect2D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pEffect2D->SetVec3Sizing(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// �F��ݒ�
		pEffect2D->SetColor(rCol);

		// ���u�����h��ݒ�
		pEffect2D->SetAlphaBlend(blend);

		// �����̏���ݒ�
		pEffect2D->m_move		= rMove;		// �ړ���
		pEffect2D->m_nLife		= nLife;		// ����
		pEffect2D->m_fSubSize	= fSubSize;		// �傫���̌��Z��
		pEffect2D->m_fSubAlpha	= 1.0f / nLife;	// �����x�̌��Z��

		// �m�ۂ����A�h���X��Ԃ�
		return pEffect2D;
	}
}

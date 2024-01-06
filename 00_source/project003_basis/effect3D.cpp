//============================================================
//
//	�G�t�F�N�g3D���� [effect3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "effect3D.h"
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
		"data\\TEXTURE\\EFFECT\\effect001.png",	// �o�u���e�N�X�`��
		"data\\TEXTURE\\EFFECT\\effect002.tga",	// ���e�N�X�`��
		"data\\TEXTURE\\EFFECT\\effect003.jpg",	// �񕜃e�N�X�`��
		"data\\TEXTURE\\EFFECT\\effect004.png",	// �t���σe�N�X�`��
		"data\\TEXTURE\\EFFECT\\effect005.jpg",	// ������e�N�X�`�� (��)
		"data\\TEXTURE\\EFFECT\\effect006.jpg",	// ������e�N�X�`�� (��)
		"data\\TEXTURE\\EFFECT\\effect007.jpg",	// ������e�N�X�`�� (��)
	};

	const int EFF_PRIO	= 3;	// �G�t�F�N�g3D�̗D�揇��
	const int PART_PRIO	= 4;	// �p�[�e�B�N��3D�̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CEffect3D::TYPE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CEffect3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEffect3D::CEffect3D(const EType type, const CObject::ELabel label) : CObjectBillboard(CObject::LABEL_EFFECT, CObject::DIM_3D, (label == LABEL_EFFECT) ? EFF_PRIO : PART_PRIO),
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
CEffect3D::~CEffect3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEffect3D::Init(void)
{
	// �����o�ϐ���������
	m_move		= VEC3_ZERO;	// �ړ���
	m_nLife		= 0;			// ����
	m_fSubSize	= 0.0f;			// �傫���̌��Z��
	m_fSubAlpha	= 0.0f;			// �����x�̌��Z��
	m_blend		= CRenderState::BLEND_ADD;	// ���u�����h��

	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// Z�e�X�g�̐ݒ�
	pRenderState->SetZFunc(D3DCMP_ALWAYS);

	// Z�o�b�t�@�̎g�p�󋵂̐ݒ�
	pRenderState->SetZUpdate(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEffect3D::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEffect3D::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos  = GetVec3Position();	// �ʒu
	D3DXVECTOR3 rot  = GetVec3Rotation();	// ����
	D3DXVECTOR3 size = GetVec3Sizing();		// �傫��
	D3DXCOLOR   col  = GetColor();			// �F
	float fRadius    = size.x;				// ���a

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
	CObjectBillboard::SetVec3Position(pos);

	// ������ݒ�
	CObjectBillboard::SetVec3Rotation(rot);

	// �傫����ݒ�
	CObjectBillboard::SetVec3Sizing(D3DXVECTOR3(fRadius, fRadius, 0.0f));

	// �F��ݒ�
	CObjectBillboard::SetColor(col);

	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEffect3D::Draw(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw();
}

//============================================================
//	���u�����h�̐ݒ菈��
//============================================================
void CEffect3D::SetAlphaBlend(const CRenderState::EBlend blend)
{
	// �����̃��u�����h��ݒ�
	m_blend = blend;

	// ���u�����h��ύX
	GetRenderState()->SetAlphaBlend(blend);
}

//============================================================
//	��������
//============================================================
CEffect3D *CEffect3D::Create
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
	// �G�t�F�N�g3D�̐���
	CEffect3D *pEffect3D = new CEffect3D(type, label);
	if (pEffect3D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�t�F�N�g3D�̏�����
		if (FAILED(pEffect3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�t�F�N�g3D�̔j��
			SAFE_DELETE(pEffect3D);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pEffect3D->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[type]));

		// �ʒu��ݒ�
		pEffect3D->SetVec3Position(rPos);

		// ������ݒ�
		pEffect3D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pEffect3D->SetVec3Sizing(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// �F��ݒ�
		pEffect3D->SetColor(rCol);

		// ���u�����h��ݒ�
		pEffect3D->SetAlphaBlend(blend);

		// �����̏���ݒ�
		pEffect3D->m_move		= rMove;		// �ړ���
		pEffect3D->m_nLife		= nLife;		// ����
		pEffect3D->m_fSubSize	= fSubSize;		// �傫���̌��Z��
		pEffect3D->m_fSubAlpha	= 1.0f / nLife;	// �����x�̌��Z��

		// �m�ۂ����A�h���X��Ԃ�
		return pEffect3D;
	}
}

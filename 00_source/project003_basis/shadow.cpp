//============================================================
//
//	�e���� [shadow.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shadow.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "field.h"
#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\shadow000.jpg",	// �ۉe�e�N�X�`��
	};

	const int	PRIORITY		= 2;		// �e�̗D�揇��
	const float	PLUS_POSY		= 0.01f;	// ������h�~�p�̏c���W���Z��
	const float	MAX_DIS_HEIGHT	= 200.0f;	// �e�Ɛe�̏c�̋����̍ő�l
	const float	MAX_PLUS_SIZE	= 120.0f;	// �e�̑傫�����Z�ʂ̍ő�l
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CShadow::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CShadow] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CShadow::CShadow(const float fMinAlpha, const float fMaxAlpha) : CObject3D(CObject::LABEL_NONE, CObject::DIM_3D, PRIORITY),
	m_pParentObject	(nullptr),		// �e�I�u�W�F�N�g
	m_sizeOrigin	(VEC3_ZERO),	// ���̑傫��
	m_fMinAlpha		(fMinAlpha),	// �����x�̍ŏ��l�萔
	m_fMaxAlpha		(fMaxAlpha)		// �����x�̍ő�l�萔
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CShadow::~CShadow()
{

}

//============================================================
//	����������
//============================================================
HRESULT CShadow::Init(void)
{
	// �����o�ϐ���������
	m_pParentObject	= nullptr;		// �e�I�u�W�F�N�g
	m_sizeOrigin	= VEC3_ZERO;	// ���̑傫��

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// Z�e�X�g�̐ݒ�
	pRenderState->SetZFunc(D3DCMP_LESSEQUAL);

	// Z�o�b�t�@�̎g�p�󋵂̐ݒ�
	pRenderState->SetZUpdate(false);

	// ���u�����h�̐ݒ�
	pRenderState->SetAlphaBlend(CRenderState::BLEND_SUB);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CShadow::Uninit(void)
{
	// �e�I�u�W�F�N�g�̍폜
	DeleteObjectParent();

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CShadow::Update(void)
{
	// �`�����ݒ�
	SetDrawInfo();

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CShadow::Draw(void)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();
}

//============================================================
//	��������
//============================================================
CShadow *CShadow::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rSize,	// �傫��
	CObject *pObject,			// �e�I�u�W�F�N�g
	const float fMinAlpha,		// �����x�̍ŏ��l
	const float fMaxAlpha		// �����x�̍ő�l
)
{
	// �e�̐���
	CShadow *pShadow = new CShadow(fMinAlpha, fMaxAlpha);
	if (pShadow == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �e�̏�����
		if (FAILED(pShadow->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �e�̔j��
			SAFE_DELETE(pShadow);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pShadow->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[texture]));

		// ���̑傫����ݒ�
		pShadow->SetSizingOrigin(rSize);

		// �e�I�u�W�F�N�g��ݒ�
		pShadow->SetParentObject(pObject);

		// �`�����ݒ�
		pShadow->SetDrawInfo();

		// �m�ۂ����A�h���X��Ԃ�
		return pShadow;
	}
}

//============================================================
//	���̑傫���̐ݒ菈��
//============================================================
void CShadow::SetSizingOrigin(const D3DXVECTOR3& rSize)
{
	// �����̑傫������
	m_sizeOrigin = rSize;

	// �傫���̐ݒ�
	CObject3D::SetVec3Sizing(rSize);
}

//============================================================
//	�`����̐ݒ菈��
//============================================================
void CShadow::SetDrawInfo(void)
{
	if (m_pParentObject != nullptr)
	{ // �e�I�u�W�F�N�g���g�p����Ă����ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posParent = m_pParentObject->GetVec3Position();	// �e�I�u�W�F�N�g�ʒu
		D3DXVECTOR3 posShadow  = VEC3_ZERO;	// �e�ʒu
		D3DXVECTOR3 sizeShadow = VEC3_ZERO;	// �e�傫��
		float fDis = 0.0f;		// �e�Ɛe�̋���
		float fAlpha = 0.0f;	// �e�̓����x

		// �`��ʒu�̐ݒ�
		posShadow = SetDrawPosition();

		// �e�Ɛe�̏c���W�̋��������߂�
		fDis = fabsf(posParent.y - posShadow.y);		// �c�̋��������߂�
		useful::LimitNum(fDis, 0.0f, MAX_DIS_HEIGHT);	// �c�̋����𐧌�
		fDis *= 1.0f / MAX_DIS_HEIGHT;					// ������������

		// �e�̑傫�������߂�
		sizeShadow = D3DXVECTOR3(m_sizeOrigin.x + (MAX_PLUS_SIZE * fDis), 0.0f, m_sizeOrigin.z + (MAX_PLUS_SIZE * fDis));

		// ���l�����߂�
		fAlpha = fabsf(fDis - 1.0f);	// ���l��ݒ�
		useful::LimitNum(fAlpha, m_fMinAlpha, m_fMaxAlpha);	// ���l�𐧌�

		// �e�̕`�����ݒ�
		SetVec3Position(posShadow);	// �ʒu
		SetVec3Sizing(sizeShadow);	// �傫��
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));	// �F
	}
}

//============================================================
//	�`��ʒu�̐ݒ菈��
//============================================================
D3DXVECTOR3 CShadow::SetDrawPosition(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posParent = m_pParentObject->GetVec3Position();	// �e�I�u�W�F�N�g�ʒu
	D3DXVECTOR3 posShadow = VEC3_ZERO;	// �e�ʒu

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W�̏��
	if (pStage == nullptr)
	{ // �X�e�[�W�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		return VEC3_ZERO;
	}

	// �e�̈ʒu�����߂�
	posShadow = posParent;	// �e�I�u�W�F�N�g�̍��W���

	if (pStage->IsFieldPositionRange(posParent))
	{ // �n�ʂ͈͓̔��̏ꍇ

		// ������n�ʂɐݒ�
		posShadow.y = pStage->GetFieldPositionHeight(posParent) + PLUS_POSY;
	}
	else
	{ // �S�Ă͈̔͊O�̏ꍇ

		// �����𐧌��ʒu�ɐݒ�
		posShadow.y = pStage->GetStageLimit().fField + PLUS_POSY;
	}

	// �e�ʒu��Ԃ�
	return posShadow;
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CShadow::SetParentObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g��ݒ�
	m_pParentObject = pObject;
}

//============================================================
//	�e�I�u�W�F�N�g�̍폜����
//============================================================
void CShadow::DeleteObjectParent(void)
{
	// �e�I�u�W�F�N�g��nullptr�ɂ���
	m_pParentObject = nullptr;
}

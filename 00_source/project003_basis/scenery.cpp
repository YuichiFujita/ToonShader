//============================================================
//
//	�i�F���� [scenery.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scenery.h"
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
		"data\\TEXTURE\\scenery000.png",	// �R�e�N�X�`��
		"data\\TEXTURE\\scenery001.png",	// �R�e�N�X�`��
		"data\\TEXTURE\\scenery002.png",	// �R�e�N�X�`��
	};

	const int PRIORITY		= 0;	// �i�F�̗D�揇��
	const int ALPHA_NUMREF	= 180;	// ���e�X�g�̎Q�ƒl
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CScenery::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CScenery] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScenery::CScenery() : CObjectMeshCylinder(CObject::LABEL_SCENERY, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScenery::~CScenery()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScenery::Init(void)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̏�����
	if (FAILED(CObjectMeshCylinder::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������ݒ�
	SetTexDir(CObjectMeshCylinder::TEXDIR_INSIDE);

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// ���e�X�g�̎Q�ƒl�ݒ�

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pRenderState->SetCulling(D3DCULL_CW);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScenery::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̏I��
	CObjectMeshCylinder::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScenery::Update(void)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̍X�V
	CObjectMeshCylinder::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CScenery::Draw(void)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̕`��
	CObjectMeshCylinder::Draw();
}

//============================================================
//	��������
//============================================================
CScenery *CScenery::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart,	// �e�N�X�`��������
	const float fRadius,		// ���a
	const float fHeight			// �c��
)
{
	// �i�F�̐���
	CScenery *pScenery = new CScenery;
	if (pScenery == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �i�F�̏�����
		if (FAILED(pScenery->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �i�F�̔j��
			SAFE_DELETE(pScenery);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pScenery->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[texture]));

		// �ʒu��ݒ�
		pScenery->SetVec3Position(rPos);

		// ������ݒ�
		pScenery->SetVec3Rotation(rRot);

		// �F��ݒ�
		pScenery->SetColor(rCol);

		// ���a��ݒ�
		pScenery->SetRadius(fRadius);

		// �c����ݒ�
		pScenery->SetHeight(fHeight);

		// ��������ݒ�
		if (FAILED(pScenery->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �i�F�̔j��
			SAFE_DELETE(pScenery);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pScenery->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pScenery;
	}
}

//============================================================
//
//	�󏈗� [sky.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sky.h"
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
		"data\\TEXTURE\\sky000.png",	// �ʏ�e�N�X�`��
	};

	const int	PRIORITY	= 0;		// ��̗D�揇��
	const float	SKY_ROTMOVE	= 0.001f;	// �����ύX��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CSky::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CSky] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSky::CSky() : CObjectMeshDome(CObject::LABEL_SKY, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSky::~CSky()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSky::Init(void)
{
	// �I�u�W�F�N�g���b�V���h�[���̏�����
	if (FAILED(CObjectMeshDome::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������ݒ�
	SetTexDir(CObjectMeshDome::TEXDIR_INSIDE);

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

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
void CSky::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���h�[���̏I��
	CObjectMeshDome::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSky::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rot = GetVec3Rotation();	// ����

	// ������ύX
	rot.y -= SKY_ROTMOVE;

	// ������ݒ�
	SetVec3Rotation(rot);

	// �I�u�W�F�N�g���b�V���h�[���̍X�V
	CObjectMeshDome::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSky::Draw(void)
{
	// �I�u�W�F�N�g���b�V���h�[���̕`��
	CObjectMeshDome::Draw();
}

//============================================================
//	��������
//============================================================
CSky *CSky::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart,	// �e�N�X�`��������
	const float fRadius			// ���a
)
{
	// ��̐���
	CSky *pSky = new CSky;
	if (pSky == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ��̏�����
		if (FAILED(pSky->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��̔j��
			SAFE_DELETE(pSky);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pSky->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[texture]));

		// �ʒu��ݒ�
		pSky->SetVec3Position(rPos);

		// ������ݒ�
		pSky->SetVec3Rotation(rRot);

		// �F��ݒ�
		pSky->SetColor(rCol);

		// ���a��ݒ�
		pSky->SetRadius(fRadius);

		// ��������ݒ�
		if (FAILED(pSky->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// ��̔j��
			SAFE_DELETE(pSky);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pSky->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pSky;
	}
}

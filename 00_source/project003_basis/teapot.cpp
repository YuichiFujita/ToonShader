//============================================================
//
//	�e�B�[�|�b�g���� [teapot.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "teapot.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shaderToon.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// ���f���萔
	{
		"data\\MODEL\\TEAPOT\\teapot000.x",			// �e�B�[�|�b�g
		//"data\\MODEL\\OBSTACLE\\obstacle025.x",	// �e�B�[�|�b�g
	};

	const int PRIORITY = 0;	// �e�B�[�|�b�g�̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CTeapot::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	�q�N���X [CTeapot] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTeapot::CTeapot() : CObjectModel(CObject::LABEL_TEAPOT, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTeapot::~CTeapot()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTeapot::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f����o�^�E����
	BindModel(MODEL_FILE[MODEL_TEAPOT]);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTeapot::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTeapot::Update(void)
{
	SetVec3Rotation(GetVec3Rotation() + D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	//SetAllMaterial(material::White());

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CTeapot::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice	= CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���
	CTexture	*pTexture		= CManager::GetInstance()->GetTexture();	// �e�N�X�`�����
	CToonShader	*pToonShader	= CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���

	if (pDevice == nullptr || pTexture == nullptr || pToonShader == nullptr)
	{ // ��񂪖������̂��������ꍇ

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw();

		// �����𔲂���
		assert(false);
		return;
	}

	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �`��J�n
		pToonShader->Begin();

		// �}�g���b�N�X����ݒ�
		pToonShader->SetMatrix(GetPtrMtxWorld());

		// ���C�g������ݒ�
		pToonShader->SetLightDirect(GetPtrMtxWorld(), 0);

		// �e�N�X�`���g�p��ݒ�
		pToonShader->SetUseTexture(GetModelData().pTextureID[0]);

		// �}�e���A����ݒ�
		pToonShader->SetMaterial(GetMaterial(0));

		// ��ԕύX�̓`�B
		pToonShader->CommitChanges();

		// �p�X��ݒ�
		pToonShader->BeginPass(0);

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw();

		// �`��I��
		pToonShader->EndPass();
		pToonShader->End();
	}
}

//============================================================
//	��������
//============================================================
CTeapot *CTeapot::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �|�C���^��錾
	CTeapot *pTeapot = nullptr;	// �e�B�[�|�b�g�����p

	if (pTeapot == nullptr)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pTeapot = new CTeapot;	// �e�B�[�|�b�g
	}
	else { assert(false); return nullptr; }	// �g�p��

	if (pTeapot != nullptr)
	{ // �m�ۂɐ������Ă���ꍇ

		// �e�B�[�|�b�g�̏�����
		if (FAILED(pTeapot->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pTeapot;
			pTeapot = nullptr;

			// ���s��Ԃ�
			return nullptr;
		}

		// �ʒu��ݒ�
		pTeapot->SetVec3Position(rPos);

		// ������ݒ�
		pTeapot->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pTeapot;
	}
	else { assert(false); return nullptr; }	// �m�ێ��s
}

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
	const char *TEXTURE_FILE[] =	// �e�N�X�`���萔
	{
		"dataSHADER\\\\TEXTURE\\normal5.png",	// �@���e�N�X�`��
	};
	const char *MODEL_FILE[] =		// ���f���萔
	{
		"data\\MODEL\\TEAPOT\\teapot005.x",		// �e�B�[�|�b�g
	};

	const int PRIORITY = 0;	// �e�B�[�|�b�g�̗D�揇��
}

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
	// �ϐ��z���錾
	D3DVERTEXELEMENT9 aDecl[] =	// ���_�f�[�^��`
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f����o�^�E����
	BindModel(MODEL_FILE[MODEL_TEAPOT]);

	//// ���f���̒��_�f�[�^��ϊ�
	//if (FAILED(SetVertexDecl(&aDecl[0])))
	//{ // �ϊ��Ɏ��s�����ꍇ

	//	// ���s��Ԃ�
	//	return E_FAIL;
	//}

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
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CTeapot::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`�����
	CToonShader *pToonShader = CToonShader::GetInstance();		// �g�D�[���V�F�[�_�[���

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

		// �X�e�[�W1�̃e�N�X�`����ݒ�
		pDevice->SetTexture(1, pTexture->GetTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_NORMAL])));	// �@���e�N�X�`��

		// �`��J�n
		pToonShader->Begin();

		// �}�g���b�N�X����ݒ�
		pToonShader->SetMatrix(GetPtrMtxWorld());

		// ��ԕύX�̓`�B
		pToonShader->CommitChanges();

		// �p�X��ݒ�
		pToonShader->BeginPass(0);

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw();

		// �`��I��
		pToonShader->EndPass();
		pToonShader->End();

		// �X�e�[�W1�̃e�N�X�`����������
		pDevice->SetTexture(1, nullptr);
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

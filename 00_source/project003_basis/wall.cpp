//============================================================
//
//	�Ǐ��� [wall.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shaderToon.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		nullptr,	// �ʏ�e�N�X�`��
	};

	const int PRIORITY = 0;	// �ǂ̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CWall::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CWall] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWall::CWall() : CObjectMeshWall(CObject::LABEL_WALL, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CWall::~CWall()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWall::Init(void)
{
	// �I�u�W�F�N�g���b�V���E�H�[���̏�����
	if (FAILED(CObjectMeshWall::Init()))
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
void CWall::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���E�H�[���̏I��
	CObjectMeshWall::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWall::Update(void)
{
	// �I�u�W�F�N�g���b�V���E�H�[���̍X�V
	CObjectMeshWall::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CWall::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice	= CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���
	CTexture	*pTexture		= CManager::GetInstance()->GetTexture();	// �e�N�X�`�����
	CToonShader	*pToonShader	= CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���

	if (pDevice == nullptr || pTexture == nullptr || pToonShader == nullptr)
	{ // ��񂪖������̂��������ꍇ

		// �I�u�W�F�N�g���b�V���E�H�[���̕`��
		CObjectMeshWall::Draw();

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
		pToonShader->SetUseTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[0]));

		// �g�U����ݒ�
		pToonShader->SetDiffuse(GetColor());

		// ��ԕύX�̓`�B
		pToonShader->CommitChanges();

		// �p�X��ݒ�
		pToonShader->BeginPass(0);

		// �I�u�W�F�N�g���b�V���E�H�[���̕`��
		CObjectMeshWall::Draw();

		// �`��I��
		pToonShader->EndPass();
		pToonShader->End();
	}
}

//============================================================
//	��������
//============================================================
CWall *CWall::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart		// ������
)
{
	// �ǂ̐���
	CWall *pWall = new CWall;
	if (pWall == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �ǂ̏�����
		if (FAILED(pWall->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �ǂ̔j��
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pWall->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[texture]));

		// �ʒu��ݒ�
		pWall->SetVec3Position(rPos);

		// ������ݒ�
		pWall->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pWall->SetVec2Sizing(rSize);

		// �F��ݒ�
		pWall->SetColor(rCol);

		// ��������ݒ�
		if (FAILED(pWall->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �ǂ̔j��
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pWall;
	}
}

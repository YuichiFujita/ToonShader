//============================================================
//
//	�g�D�[���V�F�[�_�[�w�b�_�[ [shaderToon.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shaderToon.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���萔
	{
		"data\\TEXTURE\\SHADER\\toon000.png",	// �ʏ�e�N�X�`��
	};

	const char* EFFECT_FX = "shaderToon.fx";	// �g�D�[���V�F�[�_�[�̃G�t�F�N�g�t�@�C��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CToonShader::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CToonShader *CToonShader::m_pShader = nullptr;	// �V�F�[�_�[���

//************************************************************
//	�e�N���X [CToonShader] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CToonShader::CToonShader() :
	m_pTextureToon	(nullptr),	// �g�D�[���}�b�v�e�N�X�`��
	m_pDirectLight	(nullptr),	// ���s�����̕����x�N�g��
	m_pDiffuse		(nullptr),	// �g�U��
	m_pUseTexture	(nullptr)	// �e�N�X�`���̎g�p��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CToonShader::~CToonShader()
{

}

//============================================================
//	����������
//============================================================
HRESULT CToonShader::Init(void)
{
	// �ϐ���錾
	D3DCAPS9 caps;	// �n�[�h�E�F�A�@�\
	HRESULT hr;		// �ُ�I���̊m�F�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���
	LPD3DXBUFFER pError  = nullptr;	// �R���p�C���G���[���
	LPD3DXEFFECT pEffect = nullptr;	// �G�t�F�N�g�ݒ�p

	// �����o�ϐ���������
	m_pTextureToon	= nullptr;	// �g�D�[���}�b�v�e�N�X�`��
	m_pDirectLight	= nullptr;	// ���s�����̕����x�N�g��
	m_pDiffuse		= nullptr;	// �g�U��
	m_pUseTexture	= nullptr;	// �e�N�X�`���̎g�p��

	// �V�F�[�_�[�̏�����
	if (FAILED(CShader::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�[�h�E�F�A�@�\���擾
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1)
	&&  caps.PixelShaderVersion  >= D3DPS_VERSION(2, 0))
	{ // ���_�E�s�N�Z���V�F�[�_�̃o�[�W�������g�p�\�ȏꍇ

		// �G�t�F�N�g�t�@�C���̓Ǎ�
		hr = D3DXCreateEffectFromFile
		( // ����
			pDevice,	// �f�o�C�X�ւ̃|�C���^
			EFFECT_FX,	// �G�t�F�N�g�t�@�C��
			nullptr,	// �v���v���Z�b�T��`
			nullptr,	// �C���N���[�h����
			0,			// �Ǎ��I�v�V�����t���O
			nullptr,	// �O���[�o���ϐ��C���^�[�t�F�[�X
			&pEffect,	// �G�t�F�N�g�C���^�[�t�F�[�X
			&pError		// �R���p�C���G���[���
		);
		if (SUCCEEDED(hr))
		{ // �Ǎ��ɐ��������ꍇ

			// �G�t�F�N�g����ݒ�
			SetEffect(pEffect);

			// �e�N�j�b�N�֐����擾�E�ݒ�
			SetTechnique(pEffect->GetTechniqueByName("TShader"));

			// �e�}�g���b�N�X�̃O���[�o���ϐ����擾�E�ݒ�
			SetMatrixWorld(pEffect->GetParameterByName(nullptr, "g_mtxWorld"));		// ���[���h�}�g���b�N�X
			SetMatrixView(pEffect->GetParameterByName(nullptr, "g_mtxView"));		// �r���[�}�g���b�N�X
			SetMatrixProjection(pEffect->GetParameterByName(nullptr, "g_mtxProj"));	// �v���W�F�N�V�����}�g���b�N�X

			// �O���[�o���ϐ����擾
			m_pTextureToon	= pEffect->GetParameterByName(nullptr, "g_textureToon");	// �g�D�[���}�b�v�e�N�X�`��
			m_pDirectLight	= pEffect->GetParameterByName(nullptr, "g_dirLight");		// ���s�����̕����x�N�g��
			m_pDiffuse		= pEffect->GetParameterByName(nullptr, "g_diffuse");		// �g�U��
			m_pUseTexture	= pEffect->GetParameterByName(nullptr, "g_bUseTex");		// �e�N�X�`���̎g�p��

			// �g�D�[���}�b�v�e�N�X�`����ݒ�
			SetToonMapTexture(TEXTURE_NORMAL);
		}
		else
		{ // �Ǎ��Ɏ��s�����ꍇ

			// �ǂݍ��ݎ��s��Ԃ�
			return E_FAIL;
		}
	}
	else
	{ // �o�[�W�������g�p�s�ȏꍇ

		// �Â��o�[�W�����ɂ�鎸�s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CToonShader::Uninit(void)
{
	// �V�F�[�_�[�̏I��
	CShader::Uninit();
}

//============================================================
//	���C�g�����x�N�g���̐ݒ菈��
//============================================================
void CToonShader::SetLightDirect(D3DXMATRIX *pMtxWorld, const int nLightID)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR3 dir = GET_MANAGER->GetLight()->GetLight(nLightID).Direction;	// ���C�g���� (�O��)
	D3DXVECTOR4 dirLight = D3DXVECTOR4(dir.x, dir.y, dir.z, 0.0f);				// ���C�g���� (�l��)
	D3DXMATRIX  mtxInvWorld;	// ���[���h�}�g���b�N�X�t�s��

	// ���s�����̕����x�N�g�����v�Z
	D3DXMatrixInverse(&mtxInvWorld, nullptr, pMtxWorld);					// ���[���h�}�g���b�N�X�̋t�s����v�Z
	D3DXVec4Transform(&dirLight, &dirLight, &mtxInvWorld);					// �}�g���b�N�X���x�N�g���ɕϊ�
	D3DXVec3Normalize((D3DXVECTOR3*)&dirLight, (D3DXVECTOR3*)&dirLight);	// �x�N�g�����O�����ϊ������K��

	// �G�t�F�N�g�ɕ��s�����̕����x�N�g����ݒ�
	GetEffect()->SetVector(m_pDirectLight, &dirLight);
}

//============================================================
//	�e�N�X�`���g�p�̐ݒ菈�� (�|�C���^)
//============================================================
void CToonShader::SetUseTexture(const LPDIRECT3DTEXTURE9 *pTexture)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�X�`���̎g�p�󋵂�ݒ�
	if (pTexture == nullptr)
	{ // �e�N�X�`�����g�p����Ă��Ȃ��ꍇ

		GetEffect()->SetBool(m_pUseTexture, false);
	}
	else
	{ // �e�N�X�`�����g�p����Ă���ꍇ

		GetEffect()->SetBool(m_pUseTexture, true);
	}
}

//============================================================
//	�e�N�X�`���g�p�̐ݒ菈�� (�C���f�b�N�X)
//============================================================
void CToonShader::SetUseTexture(const int nTextureID)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�X�`���̎g�p�󋵂�ݒ�
	if (nTextureID == NONE_IDX)
	{ // �e�N�X�`�����g�p����Ă��Ȃ��ꍇ

		GetEffect()->SetBool(m_pUseTexture, false);
	}
	else if (nTextureID > NONE_IDX)
	{ // �e�N�X�`�����g�p����Ă���ꍇ

		GetEffect()->SetBool(m_pUseTexture, true);
	}
	else { assert(false); }	// �g�p�s�C���f�b�N�X
}

//============================================================
//	�}�e���A���̐ݒ菈��
//============================================================
void CToonShader::SetMaterial(const D3DXMATERIAL& rMaterial)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR4 diffuse = D3DXVECTOR4(rMaterial.MatD3D.Diffuse.r, rMaterial.MatD3D.Diffuse.g, rMaterial.MatD3D.Diffuse.b, rMaterial.MatD3D.Diffuse.a);	// �g�U��

	// �G�t�F�N�g�Ƀ}�e���A���̊g�U����ݒ�
	GetEffect()->SetVector(m_pDiffuse, &diffuse);
}

//============================================================
//	�g�U���̐ݒ菈��
//============================================================
void CToonShader::SetDiffuse(const D3DXCOLOR& rDiffuse)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR4 diffuse = D3DXVECTOR4(rDiffuse.r, rDiffuse.g, rDiffuse.b, rDiffuse.a);	// �g�U��

	// �G�t�F�N�g�Ɋg�U����ݒ�
	GetEffect()->SetVector(m_pDiffuse, &diffuse);
}

//============================================================
//	��������
//============================================================
CToonShader *CToonShader::Create(void)
{
	// �C���X�^���X�g�p��
	assert(m_pShader == nullptr);

	// �g�D�[���V�F�[�_�[�̐���
	m_pShader = new CToonShader;
	if (m_pShader == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �g�D�[���V�F�[�_�[�̏�����
		if (FAILED(m_pShader->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �g�D�[���V�F�[�_�[�̔j��
			SAFE_DELETE(m_pShader);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return m_pShader;
	}
}

//============================================================
//	�擾����
//============================================================
CToonShader *CToonShader::GetInstance(void)
{
	// �C���X�^���X���g�p
	assert(m_pShader != nullptr);

	// �g�D�[���V�F�[�_�[�̃|�C���^��Ԃ�
	return m_pShader;
}

//============================================================
//	�j������
//============================================================
void CToonShader::Release(void)
{
	// �g�D�[���V�F�[�_�[�̏I��
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// �������J��
	SAFE_DELETE(m_pShader);
}

//============================================================
//	�g�D�[���}�b�v�e�N�X�`���̐ݒ菈��
//============================================================
void CToonShader::SetToonMapTexture(const ETexture texture)
{
	if (texture > NONE_IDX && texture < TEXTURE_MAX)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p
		CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`�����

		// �G�t�F�N�g�Ƀg�D�[���}�b�v�p�e�N�X�`����ݒ�
		GetEffect()->SetTexture(m_pTextureToon, pTexture->GetTexture(pTexture->Regist(TEXTURE_FILE[texture])));
	}
	else { assert(false); }	// �͈͊O
}

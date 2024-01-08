//============================================================
//
//	�G�b�W�V�F�[�_�[�w�b�_�[ [shaderEdge.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shaderEdge.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* EFFECT_FX = "shaderEdgeUE5.fx";	// �G�b�W�V�F�[�_�[�̃G�t�F�N�g�t�@�C��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CEdgeShader *CEdgeShader::m_pShader = nullptr;	// �V�F�[�_�[���

//************************************************************
//	�e�N���X [CEdgeShader] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEdgeShader::CEdgeShader() :
	m_pDrawEdge	(nullptr),	// �����`��p��2D�|���S��
	m_pTexel	(nullptr),	// �e�N�Z���T�C�Y
	m_pLimit	(nullptr)	// �G�b�W��l
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEdgeShader::~CEdgeShader()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEdgeShader::Init(void)
{
	// �ϐ���錾
	D3DCAPS9 caps;	// �n�[�h�E�F�A�@�\
	HRESULT hr;		// �ُ�I���̊m�F�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���
	LPD3DXBUFFER pError  = nullptr;	// �R���p�C���G���[���
	LPD3DXEFFECT pEffect = nullptr;	// �G�t�F�N�g�ݒ�p

	// �����o�ϐ���������
	m_pDrawEdge = nullptr;	// �����`��p��2D�|���S��
	m_pTexel	= nullptr;	// �e�N�Z���T�C�Y
	m_pLimit	= nullptr;	// �G�b�W��l

	// �V�F�[�_�[�̏�����
	if (FAILED(CShader::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����`��p��2D�|���S���̐���
	m_pDrawEdge = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
	if (m_pDrawEdge == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���x�����X�N���[���ɐݒ�
	m_pDrawEdge->SetLabel(CObject::LABEL_SCREEN);

	// �����`���OFF�ɂ���
	m_pDrawEdge->SetEnableDraw(false);

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

			// �O���[�o���ϐ����擾
			m_pTexel = pEffect->GetParameterByName(nullptr, "m_Texel");	// �e�N�Z���T�C�Y
			m_pLimit = pEffect->GetParameterByName(nullptr, "m_Limit");	// �G�b�W��l

			// �e�N�Z���T�C�Y��ݒ�
			SetTexelSize(TEXEL_SIZE);
		}
		else
		{ // �Ǎ��Ɏ��s�����ꍇ

			// �ǂݍ��ݎ��s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else
	{ // �o�[�W�������g�p�s�ȏꍇ

		// �Â��o�[�W�����ɂ�鎸�s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEdgeShader::Uninit(void)
{
	// �����`��p��2D�|���S���̏I��
	SAFE_UNINIT(m_pDrawEdge);

	// �V�F�[�_�[�̏I��
	CShader::Uninit();
}

//============================================================
//	����背���_�����O����
//============================================================
void CEdgeShader::Render(LPDIRECT3DTEXTURE9 pBackBuffer, LPDIRECT3DTEXTURE9 pZBuffer)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���

	pDevice->SetTexture(0, pBackBuffer);
	pDevice->SetTexture(1, pZBuffer);

	Begin();
	BeginPass(0);

	m_pDrawEdge->Draw();

	EndPass();
	End();

	//pDevice->SetTexture(0, nullptr);
	pDevice->SetTexture(1, nullptr);
}

//============================================================
//	�G�b�W��l�̐ݒ菈��
//============================================================
void CEdgeShader::SetRefValue(const float fLimit)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�ɃG�b�W��l��ݒ�
	GetEffect()->SetFloat(m_pLimit, fLimit);
}

//============================================================
//	��������
//============================================================
CEdgeShader *CEdgeShader::Create(void)
{
	// �C���X�^���X�g�p��
	assert(m_pShader == nullptr);

	// �G�b�W�V�F�[�_�[�̐���
	m_pShader = new CEdgeShader;
	if (m_pShader == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�b�W�V�F�[�_�[�̏�����
		if (FAILED(m_pShader->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�b�W�V�F�[�_�[�̔j��
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
CEdgeShader *CEdgeShader::GetInstance(void)
{
	// �C���X�^���X���g�p
	assert(m_pShader != nullptr);

	// �G�b�W�V�F�[�_�[�̃|�C���^��Ԃ�
	return m_pShader;
}

//============================================================
//	�j������
//============================================================
void CEdgeShader::Release(void)
{
	// �G�b�W�V�F�[�_�[�̏I��
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// �������J��
	SAFE_DELETE(m_pShader);
}

//============================================================
//	�e�N�Z���T�C�Y�̐ݒ菈��
//============================================================
void CEdgeShader::SetTexelSize(const D3DXVECTOR2& rTexel)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�Z���T�C�Y��ݒ�
	GetEffect()->SetValue(m_pTexel, (LPCVOID)&rTexel, sizeof(float) * 2);
}

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
#include "camera.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* EFFECT_FX = "Normal.fx";	// �o���v�}�b�v�̃G�t�F�N�g�t�@�C��
}

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
	m_pLightLocalDirect	(nullptr),	// ���s�����̕����x�N�g��
	m_pCameraVec		(nullptr)	// �J�����x�N�g��
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
	LPD3DXBUFFER pError = nullptr;	// �R���p�C���G���[���
	LPD3DXEFFECT pEffect = nullptr;	// �G�t�F�N�g�ݒ�p

	// �����o�ϐ���������
	m_pLightLocalDirect	= nullptr;	// ���s�����̕����x�N�g��
	m_pCameraVec		= nullptr;	// �J�����x�N�g��

	// �V�F�[�_�[�̏�����
	if (FAILED(CShader::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�[�h�E�F�A�@�\���擾
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(2, 0)
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
			SetTechnique(pEffect->GetTechniqueByName("NormalMap"));

			// �e�}�g���b�N�X�̃O���[�o���ϐ����擾�E�ݒ�
			SetMatrixWorld(pEffect->GetParameterByName(nullptr, "g_worldMat"));		// ���[���h�}�g���b�N�X
			SetMatrixView(pEffect->GetParameterByName(nullptr, "g_viewMat"));		// �r���[�}�g���b�N�X
			SetMatrixProjection(pEffect->GetParameterByName(nullptr, "g_projMat"));	// �v���W�F�N�V�����}�g���b�N�X

			// �O���[�o���ϐ����擾
			m_pLightLocalDirect	= pEffect->GetParameterByName(nullptr, "g_LightLocalDirect");	// ���s�����̕����x�N�g��
			m_pCameraVec		= pEffect->GetParameterByName(nullptr, "cameraVec");			// �J�����x�N�g��
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

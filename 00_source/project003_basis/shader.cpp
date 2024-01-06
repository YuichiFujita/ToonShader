//============================================================
//
//	�V�F�[�_�[���� [shader.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shader.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�e�N���X [CShader] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CShader::CShader() :
	m_pEffect			(nullptr),	// �G�t�F�N�g�|�C���^
	m_pTechnique		(nullptr),	// �e�N�j�b�N�֐�
	m_pMtxWorld			(nullptr),	// ���[���h�}�g���b�N�X
	m_pMtxView			(nullptr),	// �r���[�}�g���b�N�X
	m_pMtxProjection	(nullptr)	// �v���W�F�N�V�����}�g���b�N�X
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CShader::~CShader()
{

}

//============================================================
//	����������
//============================================================
HRESULT CShader::Init(void)
{
	// �����o�ϐ���������
	m_pEffect			= nullptr;	// �G�t�F�N�g�|�C���^
	m_pTechnique		= nullptr;	// �e�N�j�b�N�֐�
	m_pMtxWorld			= nullptr;	// ���[���h�}�g���b�N�X
	m_pMtxView			= nullptr;	// �r���[�}�g���b�N�X
	m_pMtxProjection	= nullptr;	// �v���W�F�N�V�����}�g���b�N�X

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CShader::Uninit(void)
{
	// �G�t�F�N�g�̔j��
	SAFE_RELEASE(m_pEffect);
}

//============================================================
//	�J�n����
//============================================================
void CShader::Begin(void)
{
	if (m_pEffect == nullptr) { return; }	// �G�t�F�N�g���g�p

	// �J�n
	m_pEffect->Begin(nullptr, 0);
}

//============================================================
//	�p�X�J�n����
//============================================================
void CShader::BeginPass(const BYTE pass)
{
	if (m_pEffect == nullptr) { return; }	// �G�t�F�N�g���g�p

	// �p�X�̊J�n
	m_pEffect->BeginPass(pass);
}

//============================================================
//	�p�X�I������
//============================================================
void CShader::EndPass(void)
{
	if (m_pEffect == nullptr) { return; }	// �G�t�F�N�g���g�p

	// �p�X�̏I��
	m_pEffect->EndPass();
}

//============================================================
//	�I������
//============================================================
void CShader::End(void)
{
	if (m_pEffect == nullptr) { return; }	// �G�t�F�N�g���g�p

	// �I��
	m_pEffect->End();
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CShader::SetMatrix(D3DXMATRIX* pMtxWorld)
{
	if (m_pEffect == nullptr) { return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���

	// �r���[�}�g���b�N�X�E�v���W�F�N�V�����}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);

	// �e�}�g���b�N�X��ݒ�
	m_pEffect->SetMatrix(m_pMtxWorld, pMtxWorld);
	m_pEffect->SetMatrix(m_pMtxView, &mtxView);
	m_pEffect->SetMatrix(m_pMtxProjection, &mtxProjection);
}

//============================================================
//	��ԕύX�̓`�B����
//============================================================
void CShader::CommitChanges(void)
{
	if (m_pEffect == nullptr) { return; }	// �G�t�F�N�g���g�p

	// ��ԕύX�̓`�B
	m_pEffect->CommitChanges();
}

//============================================================
//	�G�t�F�N�g�g�p�\�󋵂̎擾����
//============================================================
bool CShader::IsEffectOK(void) const
{
	// �G�t�F�N�g�|�C���^�̎g�p�󋵂�Ԃ�
	return (m_pEffect != nullptr) ? true : false;
}

//============================================================
//	�G�t�F�N�g�|�C���^�擾����
//============================================================
LPD3DXEFFECT CShader::GetEffect(void) const
{
	// �G�t�F�N�g�|�C���^��Ԃ�
	return m_pEffect;
}

//============================================================
//	��������
//============================================================
void CShader::Create(void)
{

}

//============================================================
//	�j������
//============================================================
void CShader::Release(void)
{

}

//============================================================
//	�G�t�F�N�g�|�C���^�̐ݒ菈��
//============================================================
void CShader::SetEffect(const LPD3DXEFFECT pEffect)
{
	// �G�t�F�N�g�|�C���^��ݒ�
	m_pEffect = pEffect;
}

//============================================================
//	�e�N�j�b�N�֐��̐ݒ菈��
//============================================================
void CShader::SetTechnique(const D3DXHANDLE pTechnique)
{
	// �e�N�j�b�N�֐���ݒ�
	m_pTechnique = pTechnique;

	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// �G�t�F�N�g�Ƀe�N�j�b�N�֐���ݒ�
		m_pEffect->SetTechnique(m_pTechnique);
	}
}

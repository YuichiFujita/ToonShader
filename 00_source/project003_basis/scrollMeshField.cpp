//============================================================
//
//	�X�N���[�����b�V���t�B�[���h���� [scrollMeshField.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scrollMeshField.h"

//************************************************************
//	�q�N���X [CScrollMeshField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScrollMeshField::CScrollMeshField(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObjectMeshField(label, dimension, nPriority),
	m_fTexU		(0.0f),	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV		(0.0f),	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU	(0.0f),	// �e�N�X�`�������W�̈ړ���
	m_fMoveV	(0.0f)	// �e�N�X�`���c���W�̈ړ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScrollMeshField::~CScrollMeshField()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScrollMeshField::Init(void)
{
	// �����o�ϐ���������
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = 0.0f;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = 0.0f;	// �e�N�X�`���c���W�̈ړ���

	// ���b�V���t�B�[���h�̏�����
	if (FAILED(CObjectMeshField::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScrollMeshField::Uninit(void)
{
	// ���b�V���t�B�[���h�̏I��
	CObjectMeshField::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScrollMeshField::Update(void)
{
	// �X�N���[�������Z
	m_fTexU += m_fMoveU;
	m_fTexV += m_fMoveV;

	if (m_fTexU > 1.0f)
	{ // 1.0f���傫���Ȃ����ꍇ

		// �J�n�n�_��␳
		m_fTexU -= 1.0f;
	}

	if (m_fTexV > 1.0f)
	{ // 1.0f���傫���Ȃ����ꍇ

		// �J�n�n�_��␳
		m_fTexV -= 1.0f;
	}

	// ���b�V���t�B�[���h�̍X�V
	CObjectMeshField::Update();

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�`�揈��
//============================================================
void CScrollMeshField::Draw(void)
{
	// ���b�V���t�B�[���h�̕`��
	CObjectMeshField::Draw();
}

//============================================================
//	��������
//============================================================
CScrollMeshField *CScrollMeshField::Create
(
	const float fMoveU,			// �����W�̈ړ���
	const float fMoveV,			// �c���W�̈ړ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart		// ������
)
{
	// �X�N���[�����b�V���t�B�[���h�̐���
	CScrollMeshField *pScrollMeshField = new CScrollMeshField;
	if (pScrollMeshField == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�N���[�����b�V���t�B�[���h�̏�����
		if (FAILED(pScrollMeshField->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�N���[�����b�V���t�B�[���h�̔j��
			SAFE_DELETE(pScrollMeshField);
			return nullptr;
		}

		// �ʒu��ݒ�
		pScrollMeshField->SetVec3Position(rPos);

		// ������ݒ�
		pScrollMeshField->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pScrollMeshField->SetVec2Sizing(rSize);

		// �F��ݒ�
		pScrollMeshField->SetColor(rCol);

		// �����W�̈ړ��ʂ�ݒ�
		pScrollMeshField->SetMoveU(fMoveU);

		// �c���W�̈ړ��ʂ�ݒ�
		pScrollMeshField->SetMoveV(fMoveV);

		// ��������ݒ�
		if (FAILED(pScrollMeshField->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �X�N���[�����b�V���t�B�[���h�̔j��
			SAFE_DELETE(pScrollMeshField);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pScrollMeshField;
	}
}

//============================================================
//	�����W�̊J�n�ʒu�ݒ菈��
//============================================================
void CScrollMeshField::SetTexU(const float fTexU)
{
	// �����̉����W�̊J�n�ʒu����
	m_fTexU = fTexU;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�c���W�̊J�n�ʒu�ݒ菈��
//============================================================
void CScrollMeshField::SetTexV(const float fTexV)
{
	// �����̏c���W�̊J�n�ʒu����
	m_fTexV = fTexV;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�����W�̈ړ��ʐݒ菈��
//============================================================
void CScrollMeshField::SetMoveU(const float fMoveU)
{
	// �����̉����W�̈ړ��ʂ���
	m_fMoveU = fMoveU;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�����W�̈ړ��ʎ擾����
//============================================================
float CScrollMeshField::GetMoveU(void) const
{
	// �e�N�X�`�������W�̈ړ��ʂ�Ԃ�
	return m_fMoveU;
}

//============================================================
//	�c���W�̈ړ��ʐݒ菈��
//============================================================
void CScrollMeshField::SetMoveV(const float fMoveV)
{
	// �����̏c���W�̈ړ��ʂ���
	m_fMoveV = fMoveV;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�c���W�̈ړ��ʎ擾����
//============================================================
float CScrollMeshField::GetMoveV(void) const
{
	// �e�N�X�`���c���W�̈ړ��ʂ�Ԃ�
	return m_fMoveV;
}

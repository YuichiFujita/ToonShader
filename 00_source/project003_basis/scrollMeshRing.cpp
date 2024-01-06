//============================================================
//
//	�X�N���[�����b�V�������O���� [scrollMeshRing.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scrollMeshRing.h"

//************************************************************
//	�q�N���X [CScrollMeshRing] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScrollMeshRing::CScrollMeshRing(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObjectMeshRing(label, dimension, nPriority),
	m_fTexU		(0.0f),	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV		(0.0f),	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU	(0.0f),	// �e�N�X�`�������W�̈ړ���
	m_fMoveV	(0.0f)	// �e�N�X�`���c���W�̈ړ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScrollMeshRing::~CScrollMeshRing()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScrollMeshRing::Init(void)
{
	// �����o�ϐ���������
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = 0.0f;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = 0.0f;	// �e�N�X�`���c���W�̈ړ���

	// ���b�V�������O�̏�����
	if (FAILED(CObjectMeshRing::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScrollMeshRing::Uninit(void)
{
	// ���b�V�������O�̏I��
	CObjectMeshRing::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScrollMeshRing::Update(void)
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

	// ���b�V�������O�̍X�V
	CObjectMeshRing::Update();

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�`�揈��
//============================================================
void CScrollMeshRing::Draw(void)
{
	// ���b�V�������O�̕`��
	CObjectMeshRing::Draw();
}

//============================================================
//	��������
//============================================================
CScrollMeshRing *CScrollMeshRing::Create
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
	// �X�N���[�����b�V�������O�̐���
	CScrollMeshRing *pScrollMeshRing = new CScrollMeshRing;
	if (pScrollMeshRing == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�N���[�����b�V�������O�̏�����
		if (FAILED(pScrollMeshRing->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�N���[�����b�V�������O�̔j��
			SAFE_DELETE(pScrollMeshRing);
			return nullptr;
		}

		// �ʒu��ݒ�
		pScrollMeshRing->SetVec3Position(rPos);

		// ������ݒ�
		pScrollMeshRing->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pScrollMeshRing->SetVec2Sizing(rSize);

		// �F��ݒ�
		pScrollMeshRing->SetColor(rCol);

		// �����W�̈ړ��ʂ�ݒ�
		pScrollMeshRing->SetMoveU(fMoveU);

		// �c���W�̈ړ��ʂ�ݒ�
		pScrollMeshRing->SetMoveV(fMoveV);

		// ��������ݒ�
		if (FAILED(pScrollMeshRing->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �X�N���[�����b�V�������O�̔j��
			SAFE_DELETE(pScrollMeshRing);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pScrollMeshRing;
	}
}

//============================================================
//	�����W�̊J�n�ʒu�ݒ菈��
//============================================================
void CScrollMeshRing::SetTexU(const float fTexU)
{
	// �����̉����W�̊J�n�ʒu����
	m_fTexU = fTexU;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�c���W�̊J�n�ʒu�ݒ菈��
//============================================================
void CScrollMeshRing::SetTexV(const float fTexV)
{
	// �����̏c���W�̊J�n�ʒu����
	m_fTexV = fTexV;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�����W�̈ړ��ʐݒ菈��
//============================================================
void CScrollMeshRing::SetMoveU(const float fMoveU)
{
	// �����̉����W�̈ړ��ʂ���
	m_fMoveU = fMoveU;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�����W�̈ړ��ʎ擾����
//============================================================
float CScrollMeshRing::GetMoveU(void) const
{
	// �e�N�X�`�������W�̈ړ��ʂ�Ԃ�
	return m_fMoveU;
}

//============================================================
//	�c���W�̈ړ��ʐݒ菈��
//============================================================
void CScrollMeshRing::SetMoveV(const float fMoveV)
{
	// �����̏c���W�̈ړ��ʂ���
	m_fMoveV = fMoveV;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�c���W�̈ړ��ʎ擾����
//============================================================
float CScrollMeshRing::GetMoveV(void) const
{
	// �e�N�X�`���c���W�̈ړ��ʂ�Ԃ�
	return m_fMoveV;
}

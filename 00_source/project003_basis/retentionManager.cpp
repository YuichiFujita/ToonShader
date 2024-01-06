//============================================================
//
//	�f�[�^�ۑ��}�l�[�W���[���� [retentionManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "retentionManager.h"
#include "manager.h"

//************************************************************
//	�e�N���X [CRetentionManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRetentionManager::CRetentionManager() :
	m_result	(RESULT_NONE),	// �N���A��
	m_nTime		(0)				// �o�߃^�C��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CRetentionManager::~CRetentionManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRetentionManager::Init(void)
{
	// �����o�ϐ���������
	m_result = RESULT_NONE;	// �N���A��
	m_nTime  = 0;			// �o�߃^�C��

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRetentionManager::Uninit(void)
{

}

//============================================================
//	��������
//============================================================
CRetentionManager *CRetentionManager::Create(void)
{
	// �f�[�^�ۑ��}�l�[�W���[�̐���
	CRetentionManager *pRetentionManager = new CRetentionManager;
	if (pRetentionManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �f�[�^�ۑ��}�l�[�W���[�̏�����
		if (FAILED(pRetentionManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �f�[�^�ۑ��}�l�[�W���[�̔j��
			SAFE_DELETE(pRetentionManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRetentionManager;
	}
}

//============================================================
//	�j������
//============================================================
void CRetentionManager::Release(CRetentionManager *&prRetentionManager)
{
	// �f�[�^�ۑ��}�l�[�W���[�̏I��
	assert(prRetentionManager != nullptr);
	prRetentionManager->Uninit();

	// �������J��
	SAFE_DELETE(prRetentionManager);
}

//============================================================
//	�N���A�󋵂̐ݒ菈��
//============================================================
void CRetentionManager::SetResult(const EResult result)
{
	if (result > RESULT_NONE && result < RESULT_MAX)
	{ // ���U���g���͈͓��̏ꍇ

		// �����̃N���A�󋵂�ݒ�
		m_result = result;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�N���A�󋵎擾����
//============================================================
CRetentionManager::EResult CRetentionManager::GetResult(void) const
{
	// �N���A�󋵂�Ԃ�
	return m_result;
}

//============================================================
//	�o�߃^�C���̐ݒ菈��
//============================================================
void CRetentionManager::SetTime(const long nTime)
{
	// �����̌o�߃^�C����ݒ�
	m_nTime = nTime;
}

//============================================================
//	�o�߃^�C���擾����
//============================================================
long CRetentionManager::GetTime(void) const
{
	// �o�߃^�C����Ԃ�
	return m_nTime;
}

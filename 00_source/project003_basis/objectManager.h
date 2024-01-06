//============================================================
//
//	�I�u�W�F�N�g�}�l�[�W���[�w�b�_�[ [objectManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include <list>

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�}�l�[�W���[�N���X
template<class T> class CObjectManager
{
public:
	// �R���X�g���N�^
	CObjectManager();

	// �f�X�g���N�^
	~CObjectManager();

	// �C�e���[�^�[�^�G�C���A�X�錾
	using AIterator = std::_List_iterator<std::_List_val<std::_List_simple_types<T*>>>;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	AIterator AddList(T *pObject);				// ���X�g�ǉ�
	void DeleteList(const AIterator iterator);	// ���X�g�폜
	int GetNumAll(void);			// ���X�g���̗v�f���擾
	std::list<T*> GetList(void);	// ���X�g�擾

	// �ÓI�����o�֐�
	static CObjectManager *Create(void);	// ����
	static void Release(CObjectManager *&prObjectManager);	// �j��

private:
	// �����o�ϐ�
	std::list<T*> m_list;	// �I�u�W�F�N�g���X�g
};

//************************************************************
//	�e�N���X [CObjectManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
template<class T> CObjectManager<T>::CObjectManager()
{
	// �I�u�W�F�N�g���X�g�̑S�v�f���폜
	m_list.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
template<class T> CObjectManager<T>::~CObjectManager()
{

}

//============================================================
//	����������
//============================================================
template<class T> HRESULT CObjectManager<T>::Init(void)
{
	// �I�u�W�F�N�g���X�g�̑S�v�f���폜
	m_list.clear();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
template<class T> void CObjectManager<T>::Uninit(void)
{
	// �I�u�W�F�N�g���X�g�̑S�v�f���폜
	m_list.clear();
}

//============================================================
//	���X�g�ǉ�����
//============================================================
template<class T>
std::_List_iterator<std::_List_val<std::_List_simple_types<T*>>> CObjectManager<T>::AddList(T *pObject)
{
	// ���X�g�̍Ō���ɃI�u�W�F�N�g��ǉ����A�C�e���[�^�[��Ԃ�
	return m_list.insert(m_list.end(), pObject);
}

//============================================================
//	���X�g�폜����
//============================================================
template<class T> void CObjectManager<T>::DeleteList(const AIterator iterator)
{
	// �����C�e���[�^�[�̗v�f�����X�g����폜
	m_list.erase(iterator);
}

//============================================================
//	���X�g���̗v�f���̎擾����
//============================================================
template<class T> int CObjectManager<T>::GetNumAll(void)
{
	// ���X�g���̗v�f����Ԃ�
	return m_list.size();
}

//============================================================
//	���X�g�擾����
//============================================================
template<class T> std::list<T*> CObjectManager<T>::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_list;
}

//============================================================
//	��������
//============================================================
template<class T> CObjectManager<T> *CObjectManager<T>::Create(void)
{
	// �I�u�W�F�N�g�}�l�[�W���[�̐���
	CObjectManager *pObjectManager = new CObjectManager;
	if (pObjectManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g�}�l�[�W���[�̏�����
		if (FAILED(pObjectManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�}�l�[�W���[�̔j��
			SAFE_DELETE(pObjectManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectManager;
	}
}

//============================================================
//	�j������
//============================================================
template<class T> void CObjectManager<T>::Release(CObjectManager *&prObjectManager)
{
	// �I�u�W�F�N�g�}�l�[�W���[�̏I��
	assert(prObjectManager != nullptr);
	prObjectManager->Uninit();

	// �������J��
	SAFE_DELETE(prObjectManager);
}

#endif	// _OBJECT_MANAGER_H_

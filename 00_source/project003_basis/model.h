//============================================================
//
//	���f���w�b�_�[ [model.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MODEL_H_
#define _MODEL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include <vector>
#include <string>

//************************************************************
//	�萔�錾
//************************************************************
namespace model
{
	const int MAX_NUM = 256;	// ���f���̍ő吔
}

//************************************************************
//	�N���X��`
//************************************************************
// ���f���N���X
class CModel
{
public:
	// �R���X�g���N�^
	CModel();

	// �f�X�g���N�^
	~CModel();

	// ���f���\����
	struct SModel
	{
		LPD3DXMESH		pMesh;			// ���b�V�� (���_���) �ւ̃|�C���^
		LPD3DXBUFFER	pBuffMat;		// �}�e���A���ւ̃|�C���^
		DWORD			dwNumMat;		// �}�e���A���̐�
		D3DXVECTOR3		vtxMin;			// �ŏ��̒��_���W
		D3DXVECTOR3		vtxMax;			// �ő�̒��_���W
		D3DXVECTOR3		size;			// �傫��
		float			fRadius;		// ���a
		int				*pTextureID;	// �e�N�X�`���C���f�b�N�X
	};

	// �����o�֐�
	int Regist(const char *pFileName);	// ���f���o�^
	SModel *GetModel(const int nID);	// ���f�����擾

	// �ÓI�����o�֐�
	static CModel *Create(void);			// ����
	static void Release(CModel *&pModel);	// �j��

private:
	// �����o�֐�
	HRESULT Load(void);		// ���f������
	void Unload(void);		// ���f���j��
	void LoadSetup(void);	// �Z�b�g�A�b�v
	HRESULT LoadXFileModel(const int nID, const char *pFileName);	// x�t�@�C���̓ǂݍ���
	HRESULT LoadTextureModel(const int nID);	// �e�N�X�`���̓ǂݍ���
	void SetCollisionModel(const int nID);		// �����蔻��̍쐬

	// �����o�ϐ�
	SModel m_aModel[model::MAX_NUM];		// ���f���ւ̃|�C���^
	std::vector<std::string> m_sFileName;	// �ǂݍ��񂾃e�N�X�`���t�@�C����

	// �ÓI�����o�ϐ�
	static int m_nNumAll;	// ���f���̑���
};

#endif	// _MODEL_H_

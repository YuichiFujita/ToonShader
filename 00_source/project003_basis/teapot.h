//============================================================
//
//	�e�B�[�|�b�g�w�b�_�[ [teapot.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TEAPOT_H_
#define _TEAPOT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �e�B�[�|�b�g�N���X
class CTeapot : public CObjectModel
{
public:
	// �e�N�X�`����
	enum ETeture
	{
		TEXTURE_NORMAL = 0,	// �@���e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// ���f����
	enum EModel
	{
		MODEL_TEAPOT = 0,	// �e�B�[�|�b�g
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTeapot();

	// �f�X�g���N�^
	~CTeapot();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CTeapot *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
};

#endif	// _TEAPOT_H_

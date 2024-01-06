//============================================================
//
//	�i�F�w�b�_�[ [scenery.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENERY_H_
#define _SCENERY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshCylinder.h"

//************************************************************
//	�N���X��`
//************************************************************
// �i�F�N���X
class CScenery : public CObjectMeshCylinder
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_MOUNTAIN_SMALL_00 = 0,	// �R�e�N�X�`��
		TEXTURE_MOUNTAIN_SMALL_01,		// �R�e�N�X�`��
		TEXTURE_MOUNTAIN_BIG,			// �R�e�N�X�`��
		TEXTURE_MAX						// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CScenery();

	// �f�X�g���N�^
	~CScenery() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CScenery *Create	// ����
	( // ����
		const ETexture texture,		// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart,	// �e�N�X�`��������
		const float fRadius,		// ���a
		const float fHeight			// �c��
	);
};

#endif	// _SCENERY_H_

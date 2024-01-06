//============================================================
//
//	�ǃw�b�_�[ [wall.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WALL_H_
#define _WALL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshWall.h"

//************************************************************
//	�N���X��`
//************************************************************
// �ǃN���X
class CWall : public CObjectMeshWall
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CWall();

	// �f�X�g���N�^
	~CWall() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CWall *Create	// ����
	( // ����
		const ETexture texture,		// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart		// ������
	);
};

#endif	// _WALL_H_

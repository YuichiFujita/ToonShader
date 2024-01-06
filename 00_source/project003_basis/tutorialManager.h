//============================================================
//
//	�`���[�g���A���}�l�[�W���[�w�b�_�[ [tutorialManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TUTORIAL_MANAGER_H_
#define _TUTORIAL_MANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �`���[�g���A���}�l�[�W���[�N���X
class CTutorialManager
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_CONTROL = 0,	// ������@�e�N�X�`��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	};

	// ������
	enum EExplain
	{
		EXPLAIN_JUMP = 0,	// �W�����v�����e�N�X�`��
		EXPLAIN_SLIDE,		// �X���C�f�B���O�����e�N�X�`��
		EXPLAIN_WALLDASH,	// �Ǒ�������e�N�X�`��
		EXPLAIN_WALLJUMP,	// �ǃW�����v�����e�N�X�`��
		EXPLAIN_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTutorialManager();

	// �f�X�g���N�^
	~CTutorialManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CTutorialManager *Create(void);	// ����
	static void Release(CTutorialManager *&prTutorialManager);	// �j��

private:
	// �����o�ϐ�
	CObject2D *m_pExplain;	// �����\���̏��
	CObject2D *m_pControl;	// ��������̏��
};

#endif	// _TUTORIAL_MANAGER_H_
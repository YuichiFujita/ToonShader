//============================================================
//
//	�Q�[����ʃw�b�_�[ [sceneGame.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CGameManager;		// �Q�[���}�l�[�W���[�N���X
class CTimerManager;	// �^�C�}�[�}�l�[�W���[�N���X
class CPause;			// �|�[�Y�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[����ʃN���X
class CSceneGame : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneGame(const EMode mode);

	// �f�X�g���N�^
	~CSceneGame() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CGameManager		*GetGameManager(void);		// �Q�[���}�l�[�W���[�擾
	static CTimerManager	*GetTimerManager(void);		// �^�C�}�[�}�l�[�W���[�擾
	static CPause			*GetPause(void);			// �|�[�Y�擾

private:
	// �ÓI�����o�ϐ�
	static CGameManager		*m_pGameManager;	// �Q�[���}�l�[�W���[
	static CTimerManager	*m_pTimerManager;	// �^�C�}�[�}�l�[�W���[
	static CPause			*m_pPause;			// �|�[�Y
};

#endif	// _SCENE_GAME_H_

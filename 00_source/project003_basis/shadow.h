//============================================================
//
//	�e�w�b�_�[ [shadow.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace shadow
{
	const float	MIN_ALPHA = 0.25f;	// ���l�̍ŏ��l
	const float	MAX_ALPHA = 0.65f;	// ���l�̍ő�l
}

//************************************************************
//	�N���X��`
//************************************************************
// �e�N���X
class CShadow : public CObject3D
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CShadow(const float fMinAlpha, const float fMaxAlpha);

	// �f�X�g���N�^
	~CShadow() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CShadow *Create	// ����
	( // ����
		const ETexture texture,		// ���
		const D3DXVECTOR3& rSize,	// �傫��
		CObject *pObject,			// �e�I�u�W�F�N�g
		const float fMinAlpha = shadow::MIN_ALPHA,	// �����x�̍ŏ��l
		const float fMaxAlpha = shadow::MAX_ALPHA	// �����x�̍ő�l
	);

	// �����o�֐�
	void SetSizingOrigin(const D3DXVECTOR3& rSize);	// ���̑傫���ݒ�
	void SetDrawInfo(void);				// �`����ݒ�
	D3DXVECTOR3 SetDrawPosition(void);	// �`��ʒu�ݒ�
	void DeleteObjectParent(void);		// �e�I�u�W�F�N�g�폜

private:
	// �����o�֐�
	void SetParentObject(CObject *pObject);	// �e�I�u�W�F�N�g�ݒ�

	// �����o�ϐ�
	CObject *m_pParentObject;	// �e�I�u�W�F�N�g
	D3DXVECTOR3 m_sizeOrigin;	// ���̑傫��
	const float m_fMinAlpha;	// �����x�̍ŏ��l�萔
	const float m_fMaxAlpha;	// �����x�̍ő�l�萔
};

#endif	// _SHADOW_H_

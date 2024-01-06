//============================================================
//
//	�g�D�[���V�F�[�_�[�w�b�_�[ [shaderToon.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SHADER_TOON_H_
#define _SHADER_TOON_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shader.h"

//************************************************************
//	�N���X��`
//************************************************************
// �g�D�[���V�F�[�_�[�N���X
class CToonShader : public CShader
{
public:
	// �R���X�g���N�^
	CToonShader();

	// �f�X�g���N�^
	~CToonShader() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��

	// �ÓI�����o�֐�
	static CToonShader *Create(void);		// ����
	static CToonShader *GetInstance(void);	// �擾
	static void Release(void);				// �j��

private:
	// �ÓI�����o�ϐ�
	static CToonShader *m_pShader;	// �V�F�[�_�[���

	// �����o�ϐ�
	D3DXHANDLE m_pLightLocalDirect;	// ���s�����̕����x�N�g��
	D3DXHANDLE m_pCameraVec;		// �J�����x�N�g��
};

#endif	// _SHADER_H_

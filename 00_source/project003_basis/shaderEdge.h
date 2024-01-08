//============================================================
//
//	�G�b�W�V�F�[�_�[�w�b�_�[ [shaderEdge.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SHADER_EDGE_H_
#define _SHADER_EDGE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shader.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�b�W�V�F�[�_�[�N���X
class CEdgeShader : public CShader
{
public:
	// �R���X�g���N�^
	CEdgeShader();

	// �f�X�g���N�^
	~CEdgeShader() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��

	// �����o�֐�
	void Render(LPDIRECT3DTEXTURE9 pBackBuffer, LPDIRECT3DTEXTURE9 pZBuffer);	// ����背���_�����O
	void SetRefValue(const float fLimit);	// �G�b�W��l�ݒ�

	// �ÓI�����o�֐�
	static CEdgeShader *Create(void);		// ����
	static CEdgeShader *GetInstance(void);	// �擾
	static void Release(void);				// �j��

private:
	// �����o�֐�
	void SetTexelSize(const D3DXVECTOR2& rTexel);	// �e�N�Z���T�C�Y�ݒ�

	// �ÓI�����o�ϐ�
	static CEdgeShader *m_pShader;	// �V�F�[�_�[���

	// �����o�ϐ�
	CObject2D *m_pDrawEdge;	// �����`��p��2D�|���S��
	D3DXHANDLE m_pTexel;	// �e�N�Z���T�C�Y
	D3DXHANDLE m_pLimit;	// �G�b�W��l
};

#endif	// _SHADER_H_

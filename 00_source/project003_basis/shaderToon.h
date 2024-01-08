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
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CToonShader();

	// �f�X�g���N�^
	~CToonShader() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��

	// �����o�֐�
	void SetLightDirect(D3DXMATRIX *pMtxWorld, const int nLightID);	// ���C�g�����x�N�g���ݒ�
	void SetTexture(const LPDIRECT3DTEXTURE9 *pTexture);	// �e�N�X�`���ݒ� (�|�C���^)
	void SetTexture(const int nTextureID);					// �e�N�X�`���ݒ� (�C���f�b�N�X)
	void SetMaterial(const D3DMATERIAL9& rMaterial);		// �}�e���A���ݒ�
	void SetDiffuse(const D3DXCOLOR& rDiffuse);				// �g�U���ݒ�

	// �ÓI�����o�֐�
	static CToonShader *Create(void);		// ����
	static CToonShader *GetInstance(void);	// �擾
	static void Release(void);				// �j��

private:
	// �����o�֐�
	void SetToonMapTexture(const ETexture texture);	// �g�D�[���}�b�v�e�N�X�`���ݒ�

	// �ÓI�����o�ϐ�
	static CToonShader *m_pShader;	// �V�F�[�_�[���

	// �����o�ϐ�
	D3DXHANDLE m_pTextureToon;	// �g�D�[���}�b�v�e�N�X�`��
	D3DXHANDLE m_pDirectLight;	// ���s�����̕����x�N�g��
	D3DXHANDLE m_pDiffuse;		// �g�U��
	D3DXHANDLE m_pUseTexture;	// �e�N�X�`���̎g�p��
};

#endif	// _SHADER_H_

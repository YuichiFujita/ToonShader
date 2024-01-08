//============================================================
//
//	�V�F�[�_�[�w�b�_�[ [shader.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SHADER_H_
#define _SHADER_H_

//************************************************************
//	�N���X��`
//************************************************************
// �V�F�[�_�[�N���X
class CShader
{
public:
	// �R���X�g���N�^
	CShader();

	// �f�X�g���N�^
	virtual ~CShader();

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��

	// �����o�֐�
	void Begin(void);						// �J�n
	void BeginPass(const BYTE pass);		// �p�X�J�n
	void EndPass(void);						// �p�X�I��
	void End(void);							// �I��
	void SetMatrix(D3DXMATRIX *pMtxWorld);	// �}�g���b�N�X�ݒ�
	void CommitChanges(void);				// ��ԕύX�̓`�B
	bool IsEffectOK(void) const;			// �G�t�F�N�g�g�p�\�󋵂̎擾
	LPD3DXEFFECT GetEffect(void) const;		// �G�t�F�N�g�|�C���^�擾

	// �ÓI�����o�֐�
	static HRESULT Create(void);	// ����
	static void Release(void);		// �j��

protected:
	// �����o�֐�
	void SetEffect(const LPD3DXEFFECT pEffect);			// �G�t�F�N�g�|�C���^�ݒ�
	void SetTechnique(const D3DXHANDLE pTechnique);		// �e�N�j�b�N�֐��ݒ�
	void SetMatrixWorld(const D3DXHANDLE pMtxWorld)		{ m_pMtxWorld = pMtxWorld; }		// ���[���h�}�g���b�N�X�ݒ�
	void SetMatrixView(const D3DXHANDLE pMtxView)		{ m_pMtxView = pMtxView; }			// �r���[�}�g���b�N�X�ݒ�
	void SetMatrixProjection(const D3DXHANDLE pMtxProj)	{ m_pMtxProjection = pMtxProj; }	// �v���W�F�N�V�����}�g���b�N�X�ݒ�

private:
	// �����o�ϐ�
	LPD3DXEFFECT m_pEffect;			// �G�t�F�N�g�|�C���^
	D3DXHANDLE m_pTechnique;		// �e�N�j�b�N�֐�
	D3DXHANDLE m_pMtxWorld;			// ���[���h�}�g���b�N�X
	D3DXHANDLE m_pMtxView;			// �r���[�}�g���b�N�X
	D3DXHANDLE m_pMtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
};

#endif	// _SHADER_H_

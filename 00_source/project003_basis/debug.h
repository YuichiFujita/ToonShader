//============================================================
//
//	�f�o�b�O�w�b�_�[ [debug.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _DEBUG_H_
#define _DEBUG_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "camera.h"

//************************************************************
//	�N���X��`
//************************************************************
// �f�o�b�O�N���X
class CDebug
{
public:
	// �R���X�g���N�^
	CDebug();

	// �f�X�g���N�^
	~CDebug();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void MeasureFps(const DWORD dwCurrentTime);	// FPS����
	void AddFrameCount(void);		// �t���[���J�E���g���Z
	int  GetFps(void) const;		// FPS�擾
	void SetFillMode(void);			// �h��Ԃ����[�h�ݒ�
	void UpdateDebugControl(void);	// �f�o�b�O����X�V
	void DrawDebugControl(void);	// �f�o�b�O����\��
	void DrawDebugData(void);		// �f�o�b�O���\��

	// �ÓI�����o�֐�
	static CDebug *Create(void);			// ����
	static void Release(CDebug *&pDebug);	// �j��

private:

#ifdef _DEBUG

	// �����o�֐�
	void ChangeDispDebug(void);		// �f�o�b�O�\���ύX
	void ChangeControlCamera(void);	// �J��������ύX
	void ChangeFillMode(void);		// �h��Ԃ����[�h�ύX
	void ChangeDisp2D(void);		// 2D�I�u�W�F�N�g�\���ύX
	void ChangeEditMode(void);		// �G�f�B�^�[���[�h�ύX
	void ChangeDispPause(void);		// �|�[�Y�\���ύX

	// �����o�ϐ�
	int  m_nFps;	// FPS�J�E���^
	bool m_bDisp2D;	// 2D�\����
	bool m_bDisp3D;	// 3D�\����
	DWORD m_dwFrameCount;	// �t���[���J�E���g
	DWORD m_dwFPSLastTime;	// �Ō��FPS���v����������
	D3DFILLMODE m_fillMode;	// �h��Ԃ����[�h
	CCamera::EState m_oldCameraState;	// �J�����̉ߋ����

#endif	// _DEBUG

};

#endif	// _DEBUG_H_

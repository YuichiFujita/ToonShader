//==========================================================
//
// �f�o�b�O�\������ [debugproc.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "debugproc.h"
#include <string.h>
#include <stdio.h>
#include "manager.h"
#include "renderer.h"

//**********************************************************
//	�萔�錾
//**********************************************************
namespace
{
	const int MAX_FLOATNUM = 2;	// �����_�ȉ��̕\������
}

//**********************************************************
//	�ÓI�����o�ϐ��錾
//**********************************************************
LPD3DXFONT CDebugProc::m_pFont = nullptr;	// �f�o�b�O�t�H���g�ւ̃|�C���^

//==========================================================
// �R���X�g���N�^
//==========================================================
CDebugProc::CDebugProc()
{
	//�f�o�b�O�\�����̃N���A
	m_bDisp = false;
	m_pFont = nullptr;
	memset(&m_aStr[0][0], 0, sizeof(m_aStr));
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CDebugProc::~CDebugProc()
{
	
}

//==========================================================
//�f�o�b�O�\���̏���������
//==========================================================
HRESULT CDebugProc::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�b�O�\�����̏�����
	m_bDisp = false;
	m_pFont = nullptr;
	memset(&m_aStr[0][0], 0, sizeof(m_aStr));

	// �f�o�b�O�\���p�t�H���g�̐���
	if (FAILED(D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont)))
	{ // �t�H���g�̐����Ɏ��s�����ꍇ

		return E_FAIL;
	}

#ifdef _DEBUG

	// �����\���ݒ�
	m_bDisp = true;

#else

	// �����\���ݒ�
	m_bDisp = false;

#endif

	// ������Ԃ�
	return S_OK;
}

//==========================================================
//�f�o�b�O�\���̏I������
//==========================================================
void CDebugProc::Uninit(void)
{
	// �f�o�b�O�\���p�t�H���g�̔j��
	SAFE_RELEASE(m_pFont);
}

//==========================================================
//�f�o�b�O�\���̍X�V����
//==========================================================
void CDebugProc::Update(void)
{

}

//==========================================================
//�f�o�b�O�\���̕`�揈��
//==========================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_bDisp == true)
	{//�f�o�b�N���[�h���I���̎�

		//�e�L�X�g�̕`��
		m_pFont->DrawText(nullptr, &m_aStr[POINT_CENTER][0], NONE_IDX, &rect, DT_CENTER, D3DCOLOR_RGBA(0, 0, 0, 255));
		m_pFont->DrawText(nullptr, &m_aStr[POINT_LEFT][0],   NONE_IDX, &rect, DT_LEFT,   D3DCOLOR_RGBA(0, 0, 0, 255));
		m_pFont->DrawText(nullptr, &m_aStr[POINT_RIGHT][0],  NONE_IDX, &rect, DT_RIGHT,  D3DCOLOR_RGBA(0, 0, 0, 255));
	}

	//�f�o�b�O�\�����̃N���A
	memset(&m_aStr[0][0], 0, sizeof(m_aStr));
}

//==========================================================
//�f�o�b�O�\���̐ݒ菈��
//==========================================================
void CDebugProc::Print(const EPoint point, const char *fmt, ...)
{
	va_list args;
	char aString[debugproc::MAX_DEBUGSTRING];		// �w�蕶���i�[�p
	char aSaveString[debugproc::MAX_DEBUGSTRING];	// �ψ������g�i�[�p
	int nLength = 0;	// �ψ������̕����̒���
	int nStopLength;	// �ψ����}���ꏊ���

	//������̑��
	strcpy(&aString[0], fmt);

	va_start(args, fmt);

	for (char *p = &aString[0]; *p != '\0'; p++)
	{
		if (*p == '%')
		{//�m�F����������ꍇ

			p++;	//�m�F�����܂Ői�߂�

			nLength = 0;	// �����̒������Z�b�g

			// �w�肳�ꂽ��ނ��Ƃɕϐ��̒l�𔲂��o��(va_arg)
			switch (*p)
			{
			case 'd':	//����

				sprintf(&aSaveString[0], "%d", va_arg(args, int));

				break;
			case 'f':	//����

				sprintf(&aSaveString[0], "%f", va_arg(args, double));

				//������̒������擾
				nLength = (int)strlen(&aSaveString[0]);

				//�����_�ȉ���1�����ڂ܂Ŋm�F
				for (int nCntlength = 0; aSaveString[nCntlength] != '\0'; nCntlength++)
				{
					if (aSaveString[nCntlength] == '.')
					{//�����_���������ꍇ

						//�����_�ȉ��̌��������߂�
						int nMin = (int)strlen(&aSaveString[nCntlength + 1]);

						//�����_����̕������Ǝw�肵���l�܂ł̍��������߂�
						nMin -= MAX_FLOATNUM;

						//������̒����������_�ȉ����ʂ܂łɕ␳
						nLength -= nMin;
					}
				}

				break;
			case 'c':	//����

				sprintf(&aSaveString[0], "%c", va_arg(args, char));

				break;
			case 's':	//������

				sprintf(&aSaveString[0], "%s", va_arg(args, const char*));

				break;
			}

			if (nLength == 0)
			{// �����̒������擾���Ă��Ȃ��ꍇ

				//������̒������擾
				nLength = (int)strlen(&aSaveString[0]);
			}

			nStopLength = (int)strlen(p) + 1;

			//���������̊m�F����(%�`)�����̕���������炷
			memmove(p + nLength - 2, p, nStopLength);

			p--;	//�|�C���^��%�܂Ŗ߂�

			//�ψ������w�肳�ꂽ�ꏊ�ɑ}��
			memcpy(p, &aSaveString[0], nLength);
		}
	}

	va_end(args);

	//�������A������
	strcat(&m_aStr[point][0], &aString[0]);
}

//==========================================================
//	��������
//==========================================================
CDebugProc *CDebugProc::Create(void)
{
	// �f�o�b�O�\���̐���
	CDebugProc *pDebugProc = new CDebugProc;
	if (pDebugProc == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �f�o�b�O�\���̏�����
		if (FAILED(pDebugProc->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �f�o�b�O�\���̔j��
			SAFE_DELETE(pDebugProc);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pDebugProc;
	}
}

//==========================================================
//	�j������
//==========================================================
void CDebugProc::Release(CDebugProc *&prDebugProc)
{
	// �f�o�b�O�v���b�N�̏I��
	assert(prDebugProc != nullptr);
	prDebugProc->Uninit();

	// �������J��
	SAFE_DELETE(prDebugProc);
}

//============================================================
//
//	�T�E���h���� [sound.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sound.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const CSound::SSoundInfo SOUND_FILE[] =	// �T�E���h�t�@�C��
	{
		{ "data\\BGM\\bgm_general.wav", -1 },	// BGM (�ėp)
		{ "data\\BGM\\bgm_tutorial.wav", -1 },	// BGM (�`���[�g���A��)
		{ "data\\BGM\\bgm_game.wav", -1 },		// BGM (�Q�[��)

		{ "data\\SE\\select000.wav",  0 },		// �I�𑀍쉹00
		{ "data\\SE\\select001.wav",  0 },		// �I�𑀍쉹01
		{ "data\\SE\\decision000.wav",  0 },	// ���艹00
		{ "data\\SE\\decision001.wav",  0 },	// ���艹01
		{ "data\\SE\\decision002.wav",  0 },	// ���艹02
		{ "data\\SE\\vibration000.wav",  0 },	// �X�}�z�o�C�u��
		{ "data\\SE\\message000.wav",  0 },		// ���b�Z�[�W��
		{ "data\\SE\\spawn000.wav",  0 },		// ������
		{ "data\\SE\\save000.wav",  0 },		// �Z�[�u��
		{ "data\\SE\\walk000.wav",  0 },		// ���s���i�r���j
		{ "data\\SE\\walk001.wav",  0 },		// ���s���i��Q���j
		{ "data\\SE\\jump000.wav",  0 },		// �W�����v
		{ "data\\SE\\slideing000.wav",  0 },	// �X���C�f�B���O
		{ "data\\SE\\jumppad000.wav",  0 },		// �W�����v�p�b�h
		{ "data\\SE\\stick000.wav",  0 },		// ��������
		{ "data\\SE\\hit000.wav",  0 },			// �Ԃ��艹
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(SOUND_FILE) == CSound::LABEL_MAX, "ERROR : Sound Count Mismatch");

//************************************************************
//	�e�N���X [CSound] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSound::CSound() :
	m_pXAudio2			(nullptr),	// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	m_pMasteringVoice	(nullptr)	// �}�X�^�[�{�C�X
{
	// �����o�ϐ����N���A
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// �\�[�X�{�C�X
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// �I�[�f�B�I�f�[�^
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// �I�[�f�B�I�f�[�^�T�C�Y
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSound::~CSound()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSound::Init(HWND hWnd)
{
	// �ϐ���錾
	HRESULT hr;		// �I���m�F�p

	// �����o�ϐ���������
	m_pXAudio2			= nullptr;	// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	m_pMasteringVoice	= nullptr;	// �}�X�^�[�{�C�X
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// �\�[�X�{�C�X
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// �I�[�f�B�I�f�[�^
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// �I�[�f�B�I�f�[�^�T�C�Y

	// COM���C�u�����̏�����
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COM���C�u�����̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(m_pXAudio2 != nullptr)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = nullptr;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(SOUND_FILE[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{ // �t�@�C���|�C���^��擪�Ɉړ�

			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes	= m_aSizeAudio[nCntSound];
		buffer.pAudioData	= m_apDataAudio[nCntSound];
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.LoopCount	= SOUND_FILE[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSound::Uninit(void)
{
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != nullptr)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
		}

		// �I�[�f�B�I�f�[�^�̊J��
		SAFE_FREE(m_apDataAudio[nCntSound]);
	}
	
	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = nullptr;

	// XAudio2�I�u�W�F�N�g�̊J��
	SAFE_RELEASE(m_pXAudio2);

	// COM���C�u�����̏I������
	CoUninitialize();
}

//============================================================
//	�Z�O�����g�Đ� (�Đ����Ȃ��~)
//============================================================
HRESULT CSound::Play(ELabel label)
{
	// �ϐ���錾
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes	= m_aSizeAudio[label];
	buffer.pAudioData	= m_apDataAudio[label];
	buffer.Flags		= XAUDIO2_END_OF_STREAM;
	buffer.LoopCount	= SOUND_FILE[label].nCntLoop;

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // �Đ���

		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//============================================================
//	�Z�O�����g��~ (���x���w��)
//============================================================
void CSound::Stop(ELabel label)
{
	// �ϐ���錾
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // �Đ���

		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//============================================================
//	�Z�O�����g��~ (�S��)
//============================================================
void CSound::Stop(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != nullptr)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//============================================================
//	��������
//============================================================
CSound *CSound::Create(HWND hWnd)
{
	// �T�E���h�̐���
	CSound *pSound = new CSound;
	if (pSound == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �T�E���h�̏�����
		if (FAILED(pSound->Init(hWnd)))
		{ // �������Ɏ��s�����ꍇ

			// �T�E���h�̔j��
			SAFE_DELETE(pSound);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pSound;
	}
}

//============================================================
//	�j������
//============================================================
void CSound::Release(CSound *&prSound)
{
	// �T�E���h�̏I��
	assert(prSound != nullptr);
	prSound->Uninit();

	// �������J��
	SAFE_DELETE(prSound);
}

//============================================================
//	�`�����N�̃`�F�b�N
//============================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	// �ϐ���錾
	HRESULT hr = S_OK;	// �I���m�F�p
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // �t�@�C���|�C���^��擪�Ɉړ�

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{ // �`�����N�̓ǂݍ���

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{ // �`�����N�f�[�^�̓ǂݍ���

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':

			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{ // �t�@�C���^�C�v�̓ǂݍ���

				hr = HRESULT_FROM_WIN32(GetLastError());
			}

			break;

		default:

			if (SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{ // �t�@�C���|�C���^���`�����N�f�[�^���ړ�

				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//============================================================
//	�`�����N�f�[�^�̓ǂݍ���
//============================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	// �ϐ���錾
	DWORD dwRead;
	
	if (SetFilePointer(hFile, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // �t�@�C���|�C���^���w��ʒu�܂ňړ�

		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{ // �f�[�^�̓ǂݍ���

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

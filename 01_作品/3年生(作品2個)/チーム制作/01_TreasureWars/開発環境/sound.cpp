//=============================================================================
//
// sound.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "sound.h"

// �Ȃ̏ꏊ��ݒ�
const CSound::SOUNDPARAM CSound::m_aParam[SOUND_LABEL_MAX] =
{
	{ "data/sounds/BGM/Title.wav", -1 },				// �^�C�g�����BGM
	{ "data/sounds/BGM/Entry.wav", -1 },				// �G���g���[���BGM
	{ "data/sounds/BGM/Game1.wav", -1 },				// �Q�[�����BGM1
	{ "data/sounds/BGM/Game2.wav", -1 },				// �Q�[�����BGM2
	{ "data/sounds/BGM/Game3.wav", -1 },				// �Q�[�����BGM3
	{ "data/sounds/BGM/Result.wav", -1 },				// ���U���g���BGM

	{ "data/sounds/SE/Select/Select.wav", 0 },			// �I��
	{ "data/sounds/SE/Decision/Decision.wav", 0 },		// ����
	{ "data/sounds/SE/Whistle/WhistleStart.wav", 0 },	// �J_�J�n
	{ "data/sounds/SE/Whistle/WhistleStop.wav", 0 },	// �J_��~
	{ "data/sounds/SE/Whistle/WhistleFinish.wav", 0 },	// �J_�I��
	{ "data/sounds/SE/Countdown/Countdown.wav", 0 },	// �J�E���g�_�E��
	{ "data/sounds/SE/Countdown/GameStart.wav", 0 },	// �Q�[���J�n
	{ "data/sounds/SE/Mine/Swing.wav", 0 },				// �̌@���[�V����_�U�艺�낷
	{ "data/sounds/SE/Mine/Hit.wav", 0 },				// �̌@���[�V����_�@��
	{ "data/sounds/SE/Mine/Mine.wav", 0 },				// �̌@���[�V����_�̌@
	{ "data/sounds/SE/Get/GemGet.wav", 0 },				// ���_�l��
	{ "data/sounds/SE/Sale/Sale.wav", 0 },				// ���p
	{ "data/sounds/SE/Damage/Damage.wav", 0 },			// �_���[�W
	{ "data/sounds/SE/Item/Item.wav", 0 },				// �A�C�e���擾
	{ "data/sounds/SE/TV/TVStart.wav", 0 },				// �e���r_�J�n
	{ "data/sounds/SE/TV/TVFinish.wav", 0 },			// �e���r_�I��
	{ "data/sounds/SE/TV/TVDisplay.wav", 0 },			// �e���r_�\��
	{ "data/sounds/SE/Warning/Warning.wav", 0 },		// �x��
};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSound::CSound()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

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

		if (m_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		//memset(&m_aWfx[nCntSound], 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
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
		hr = ReadChunkData(hFile, &m_aWfx[nCntSound], dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
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

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSound::Uninit()
{
	// ���X�g�̑S�Ă̗v�f�̏I������
	while (!m_listSourceVoice.empty())
	{
		// �ꎞ��~
		m_listSourceVoice.back().pSourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_listSourceVoice.back().pSourceVoice->FlushSourceBuffers();

		// �\�[�X�{�C�X�̔j��
		m_listSourceVoice.back().pSourceVoice->DestroyVoice();
		m_listSourceVoice.back().pSourceVoice = NULL;

		// ��Ԍ��̔z������O
		m_listSourceVoice.pop_back();
	}

	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		// �I�[�f�B�I�f�[�^�̊J��
		free(m_apDataAudio[nCntSound]);
		m_apDataAudio[nCntSound] = NULL;
	}

	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	if (m_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSound::Update()
{
	// �����I�����\�[�X�{�C�X���I�������鏈��
	Destroy();
}

//=============================================================================
// �����I�����\�[�X�{�C�X���I�������鏈��
//=============================================================================
void CSound::Destroy()
{
	// �����I�����\�[�X�{�C�X���I�������鏈��
	for (auto itr = m_listSourceVoice.begin(); itr != m_listSourceVoice.end();)
	{
		SSourceVoice sourceVoice = *itr;

		XAUDIO2_VOICE_STATE xa2state;

		// ��Ԏ擾
		sourceVoice.pSourceVoice->GetState(&xa2state);

		// �o�b�t�@���g�p����Ă��Ȃ��ꍇ
		if (xa2state.BuffersQueued == 0)
		{
			// �ꎞ��~
			sourceVoice.pSourceVoice->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			sourceVoice.pSourceVoice->FlushSourceBuffers();

			// �\�[�X�{�C�X�̔j��
			sourceVoice.pSourceVoice->DestroyVoice();
			sourceVoice.pSourceVoice = NULL;

			// ���g�����X�g���珜�O
			itr = m_listSourceVoice.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT CSound::Play(SOUND_LABEL label)
{
	// ��̃\�[�X�{�C�X�̍\����
	SSourceVoice sourceVoice = {};

	// �\�[�X�{�C�X�̐���
	m_pXAudio2->CreateSourceVoice(&sourceVoice.pSourceVoice, &(m_aWfx[label].Format));

	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aParam[label].nCntLoop;

	// �I�[�f�B�I�o�b�t�@�̓o�^
	sourceVoice.pSourceVoice->SubmitSourceBuffer(&buffer);

	// �Đ�
	sourceVoice.pSourceVoice->Start(0);

	// ���x���̎�ނ�ݒ肷��
	sourceVoice.label = label;

	// ���X�g�̈�Ԍ��ɓ����
	m_listSourceVoice.push_back(sourceVoice);

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~(�I�������Ȃ�S�ď���)
//=============================================================================
void CSound::StopLabel(SOUND_LABEL label)
{
	// �����I�����\�[�X�{�C�X���I�������鏈��
	for (auto itr = m_listSourceVoice.begin(); itr != m_listSourceVoice.end();)
	{
		SSourceVoice sourceVoice = *itr;

		// ���x���̎�ނ���v�����ꍇ
		if (sourceVoice.label == label)
		{
			// �ꎞ��~
			sourceVoice.pSourceVoice->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			sourceVoice.pSourceVoice->FlushSourceBuffers();

			// �\�[�X�{�C�X�̔j��
			sourceVoice.pSourceVoice->DestroyVoice();
			sourceVoice.pSourceVoice = NULL;

			// ���g�����X�g���珜�O
			itr = m_listSourceVoice.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void CSound::StopAll()
{
	// ���X�g�̑S�Ă̗v�f�̏I������
	while (!m_listSourceVoice.empty())
	{
		// �ꎞ��~
		m_listSourceVoice.back().pSourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_listSourceVoice.back().pSourceVoice->FlushSourceBuffers();

		// �\�[�X�{�C�X�̔j��
		m_listSourceVoice.back().pSourceVoice->DestroyVoice();
		m_listSourceVoice.back().pSourceVoice = NULL;

		// ��Ԍ��̔z������O
		m_listSourceVoice.pop_back();
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
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

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//=============================================================================
// ��������
//=============================================================================
CSound* CSound::Create(HWND hWnd)
{
	CSound* pSound = new CSound;

	if (FAILED(pSound->Init(hWnd)))
	{
		return nullptr;
	}

	return pSound;
}
#include "SoundEngine.h"
#include "GameEngine.h"
#include "sound.cpp"

SoundEngine::SoundEngine(GameEngine* gameEngine)
{
	pGameEngine = gameEngine;
	HWND hWnd = pGameEngine->GetWindowHandle();
	HRESULT hr;
	hr= CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(pGameEngine->GetWindowHandle(), "COM���C�u�����̏������Ɏ��s", "�x���I", MB_ICONWARNING);

		return;

	}
	hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
	{
		MessageBox(pGameEngine->GetWindowHandle(), "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s", "�x���I", MB_ICONWARNING);
		// COM���C�u�����̏I������
		CoUninitialize();

		return;

	}

	// �}�X�^�[�{�C�X�̐���
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return;
	}



}

SoundEngine::~SoundEngine()
{


	for (AudioData audioData : this->audioDataArray)
	{
		IXAudio2SourceVoice* sourceVoice = audioData.sorceVoice;
		if (sourceVoice)
		{
			sourceVoice->Stop(0);
			sourceVoice->DestroyVoice();
		}
	}
	if (pMasteringVoice)
	{ 
		pMasteringVoice->DestroyVoice();
	}

	if (pXAudio2)
	{
		pXAudio2->Release(); 
	}
	// COM���C�u�����̏I������
	CoUninitialize();

}

int SoundEngine::LoadSoundData(string filePath, SoundType type)
{

	HWND hWnd = pGameEngine->GetWindowHandle();
    HRESULT hr;
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	AudioData audioData;

	// �o�b�t�@�̃N���A
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// �T�E���h�f�[�^�t�@�C���̐���
	hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return false;
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return false;
	}

	// WAVE�t�@�C���̃`�F�b�N
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return false;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return false;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �t�H�[�}�b�g�`�F�b�N
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return false;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �I�[�f�B�I�f�[�^�ǂݍ���
	hr = CheckChunk(hFile, 'atad', &audioData.sizeAudio, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return false;
	}
	audioData.pDataAudio = (BYTE*)malloc(audioData.sizeAudio);
	hr = ReadChunkData(hFile, audioData.pDataAudio, audioData.sizeAudio, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �\�[�X�{�C�X�̐���
	hr = this->pXAudio2->CreateSourceVoice(&audioData.sorceVoice, &(wfx.Format));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = audioData.sizeAudio;
	buffer.pAudioData = audioData.pDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	switch (type)
	{
	case SoundType::BGM:
		buffer.LoopCount = -1;

		break;
	case SoundType::SE:
		buffer.LoopCount = 0;

		break;
	case SoundType::VOICE:
		buffer.LoopCount = 0;

		break;
	default:
		break;
	}

	audioData.type = type;

	// �I�[�f�B�I�o�b�t�@�̓o�^
	audioData.sorceVoice->SubmitSourceBuffer(&buffer);

	audioDataArray.push_back(audioData);

    // �t�@�C�������
    CloseHandle(hFile);

	return audioDataArray.size() - 1;
}

HRESULT SoundEngine::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
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

HRESULT SoundEngine::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
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
void SoundEngine::StartSound(int index)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = audioDataArray[index].sizeAudio;
	buffer.pAudioData = audioDataArray[index].pDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;
	switch (audioDataArray[index].type)
	{
	case SoundType::BGM:
		buffer.LoopCount = -1;

		break;
	case SoundType::SE:
		buffer.LoopCount = 0;

		break;
	case SoundType::VOICE:
		buffer.LoopCount = 0;

		break;
	default:
		break;
	}


	// ��Ԏ擾
	audioDataArray[index].sorceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		audioDataArray[index].sorceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		audioDataArray[index].sorceVoice->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	audioDataArray[index].sorceVoice->SubmitSourceBuffer(&buffer);

	// �Đ�
	audioDataArray[index].sorceVoice->Start(0);

}

void SoundEngine::StopSound(int index)
{

	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	audioDataArray[index].sorceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		audioDataArray[index].sorceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		audioDataArray[index].sorceVoice->FlushSourceBuffers();
	}

}

void SoundEngine::StopAllSound(void)
{

	// �ꎞ��~
	for (int i = 0; i < audioDataArray.size(); i++)
	{


		if (audioDataArray[i].sorceVoice)
		{
			audioDataArray[i].sorceVoice->Stop(0);

		}
		
	}
}




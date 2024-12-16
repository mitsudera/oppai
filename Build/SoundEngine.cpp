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
		MessageBox(pGameEngine->GetWindowHandle(), "COMライブラリの初期化に失敗", "警告！", MB_ICONWARNING);

		return;

	}
	hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
	{
		MessageBox(pGameEngine->GetWindowHandle(), "XAudio2オブジェクトの作成に失敗", "警告！", MB_ICONWARNING);
		// COMライブラリの終了処理
		CoUninitialize();

		return;

	}

	// マスターボイスの生成
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (pXAudio2)
		{
			// XAudio2オブジェクトの開放
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
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
	// COMライブラリの終了処理
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

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
		return false;
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
		return false;
	}

	// WAVEファイルのチェック
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return false;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return false;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
		return false;
	}

	// フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return false;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return false;
	}

	// オーディオデータ読み込み
	hr = CheckChunk(hFile, 'atad', &audioData.sizeAudio, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
		return false;
	}
	audioData.pDataAudio = (BYTE*)malloc(audioData.sizeAudio);
	hr = ReadChunkData(hFile, audioData.pDataAudio, audioData.sizeAudio, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
		return false;
	}

	// ソースボイスの生成
	hr = this->pXAudio2->CreateSourceVoice(&audioData.sorceVoice, &(wfx.Format));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
		return false;
	}

	// バッファの値設定
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

	// オーディオバッファの登録
	audioData.sorceVoice->SubmitSourceBuffer(&buffer);

	audioDataArray.push_back(audioData);

    // ファイルを閉じる
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
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
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
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
void SoundEngine::StartSound(int index)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
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


	// 状態取得
	audioDataArray[index].sorceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		audioDataArray[index].sorceVoice->Stop(0);

		// オーディオバッファの削除
		audioDataArray[index].sorceVoice->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	audioDataArray[index].sorceVoice->SubmitSourceBuffer(&buffer);

	// 再生
	audioDataArray[index].sorceVoice->Start(0);

}

void SoundEngine::StopSound(int index)
{

	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	audioDataArray[index].sorceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		audioDataArray[index].sorceVoice->Stop(0);

		// オーディオバッファの削除
		audioDataArray[index].sorceVoice->FlushSourceBuffers();
	}

}

void SoundEngine::StopAllSound(void)
{

	// 一時停止
	for (int i = 0; i < audioDataArray.size(); i++)
	{


		if (audioDataArray[i].sorceVoice)
		{
			audioDataArray[i].sorceVoice->Stop(0);

		}
		
	}
}




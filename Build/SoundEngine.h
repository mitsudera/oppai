#pragma once
#include "Coreminimal.h"
#include "xaudio2.h"
class GameEngine;

enum class SoundType
{
	BGM,
	SE,
	VOICE,
};


class SoundEngine
{
public:


	struct AudioData
	{
		IXAudio2SourceVoice* sorceVoice;
		BYTE* pDataAudio;					// �I�[�f�B�I�f�[�^
		DWORD sizeAudio;					// �I�[�f�B�I�f�[�^�T�C�Y
		SoundType type;
	};


	SoundEngine(GameEngine* gameEngine);
	~SoundEngine();

	int LoadSoundData(string filePath, SoundType type);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);

	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	
	void StartSound(int index);
	void StopSound(int index);
	void StopAllSound(void);

private:
	GameEngine* pGameEngine;
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasteringVoice;
	vector<AudioData> audioDataArray;

};


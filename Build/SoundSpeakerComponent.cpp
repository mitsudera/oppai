#include "SoundSpeakerComponent.h"
#include "gameobject.h"
#include "SoundEngine.h"
#include "GameEngine.h"


const string soundPath = "Data/Sound/";

SoundSpeakerComponent::SoundSpeakerComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

SoundSpeakerComponent::~SoundSpeakerComponent()
{
}

void SoundSpeakerComponent::Init(void)
{
	Component::Init();
	pSoundEngine = pGameEngine->GetSoundEngine();
}

void SoundSpeakerComponent::Uninit(void)
{
	Component::Uninit();
}

void SoundSpeakerComponent::Update(void)
{
	Component::Update();
}

int SoundSpeakerComponent::LoadSound(string fileName, string name, SoundType type)
{
	SoundData* newData = new SoundData;
	newData->fileName = fileName;
	newData->name = name;
	newData->type = type;

	string filePath;

	switch (type)
	{
	case SoundType::BGM:
		filePath = soundPath + "BGM/" + fileName;

		break;
	case SoundType::SE:
		filePath = soundPath + "SE/" + fileName;

		break;
	case SoundType::VOICE:
		filePath = soundPath + "VOICE/" + fileName;

		break;
	default:
		break;
	}

	newData->index = pSoundEngine->LoadSoundData(filePath,type);
	
	soundDataArray.push_back(newData);

	return soundDataArray.size() - 1;
}

void SoundSpeakerComponent::StartSound(int index)
{
	pSoundEngine->StartSound(this->soundDataArray[index]->index);
}

void SoundSpeakerComponent::StartSound(string name)
{
	for (SoundData* soundData:soundDataArray)
	{
		if (soundData->name == name)
		{
			pSoundEngine->StartSound(soundData->index);
			return;
		}
	}
}

void SoundSpeakerComponent::StopSound(int index)
{
	pSoundEngine->StopSound(this->soundDataArray[index]->index);

}

void SoundSpeakerComponent::StopSound(string name)
{
	for (SoundData* soundData : soundDataArray)
	{
		if (soundData->name == name)
		{
			pSoundEngine->StopSound(soundData->index);
			return;
		}
	}

}

void SoundSpeakerComponent::StopAllSound(void)
{
	for (SoundData* soundData : soundDataArray)
	{
		pSoundEngine->StopSound(soundData->index);
	}

}

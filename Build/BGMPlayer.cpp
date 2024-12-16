#include "BGMPlayer.h"
#include "SoundSpeakerComponent.h"

BGMPlayer::BGMPlayer(Scene* scene)
{
	pScene = scene;
}

BGMPlayer::~BGMPlayer()
{
}

void BGMPlayer::Init(void)
{
	GameObject::Init();

	SoundSpeakerComponent* speaker = AddComponent<SoundSpeakerComponent>();

	int index = speaker->LoadSound("bgm_maoudamashii_neorock73.wav", "bgm1", SoundType::BGM);
	speaker->StartSound(index);

}

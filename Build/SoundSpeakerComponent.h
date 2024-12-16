#pragma once
#include "component.h"
#include "SoundEngine.h"


class SoundSpeakerComponent : public Component
{
public:



	struct SoundData
	{
		string fileName;
		string name;
		SoundType type;
		int index;
	};

	SoundSpeakerComponent(GameObject* gameObject);
	~SoundSpeakerComponent();

	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	int LoadSound(string fileName, string name, SoundType type);

	void StartSound(int index);
	void StartSound(string name);
	void StopSound(int index);
	void StopSound(string name);
	void StopAllSound(void);


private:
	
	SoundEngine* pSoundEngine;

	vector<SoundData*> soundDataArray;
	


};


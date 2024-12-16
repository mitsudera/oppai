#pragma once
#include "gameobject.h"
class BGMPlayer :public GameObject
{
public:
	BGMPlayer(Scene* scene);
	~BGMPlayer();
	virtual void Init(void) override;

};


#pragma once
#include "gameobject.h"
class TitleManager:public GameObject
{
public:
	TitleManager(Scene* scene);
	~TitleManager();
	virtual void Init(void) override;
};


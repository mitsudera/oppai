#pragma once
#include "gameobject.h"
class GameManager :public GameObject
{
public:
	GameManager(Scene* scene);
	~GameManager();

	virtual void Init(void);
};


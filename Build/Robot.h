#pragma once
#include "gameobject.h"
class Robot :public GameObject
{
public:
	Robot(Scene* scene);
	~Robot();

	virtual void Init(void) override;

};


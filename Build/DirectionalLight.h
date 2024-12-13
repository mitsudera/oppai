#pragma once
#include "gameobject.h"
class DirectionalLight : public GameObject
{
public:
	DirectionalLight(Scene* scene);
	~DirectionalLight();

	virtual void Init(void) override;

};


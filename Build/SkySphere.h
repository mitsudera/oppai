#pragma once
#include "gameobject.h"
class SkySphere : public GameObject
{
public:
	SkySphere(Scene* scene);
	~SkySphere();

	virtual void Init(void) override;

};


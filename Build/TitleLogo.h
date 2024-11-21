#pragma once
#include "gameobject.h"
class TitleLogo :public GameObject
{
public:
	TitleLogo(Scene* scene);
	~TitleLogo();
	virtual void Init(void) override;


};


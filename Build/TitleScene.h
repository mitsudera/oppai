#pragma once
#include "Scene.h"
class TitleScene :public Scene
{
public:
	
	TitleScene(GameEngine* pGameEngine);
	~TitleScene();

	virtual void Init() override;


};


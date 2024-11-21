#pragma once
#include "Scene.h"
class TitleScene :public Scene
{
public:
	
	TitleScene(GameEngine* gameEngine);
	~TitleScene();

	virtual void Init() override;


};


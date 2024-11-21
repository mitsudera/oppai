#pragma once
#include "Scene.h"
class GameScene : public Scene
{
public:
	GameScene(GameEngine* gameEngine);
	~GameScene();

	virtual void Init() override;

};


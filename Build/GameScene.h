#pragma once
#include "Scene.h"
class GameScene : public Scene
{
public:
	GameScene(GameEngine* pGameEngine);
	~GameScene();


	virtual void Init() override;

};


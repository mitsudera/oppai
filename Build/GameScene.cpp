#include "GameScene.h"

GameScene::GameScene(GameEngine* gameEngine)
{
	this->gameEngine = gameEngine;
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Scene::Init();
}

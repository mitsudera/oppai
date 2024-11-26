#include "TitleScene.h"
#include "TitleLogo.h"
#include "TitleManager.h"
TitleScene::TitleScene(GameEngine* gameEngine)
{
	this->gameEngine = gameEngine;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{

	TitleLogo* titleLogo = new TitleLogo(this);
	this->gameObjectList.push_back(titleLogo);
	TitleManager* titleManager = new TitleManager(this);
	this->gameObjectList.push_back(titleManager);
	Scene::Init();
}

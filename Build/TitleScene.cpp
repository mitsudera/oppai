#include "TitleScene.h"
#include "TitleLogo.h"

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
	Scene::Init();
}

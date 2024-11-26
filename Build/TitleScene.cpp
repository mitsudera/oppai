#include "TitleScene.h"
#include "TitleLogo.h"
#include "TitleManager.h"
#include "Camera.h"
TitleScene::TitleScene(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{


	TitleManager* titleManager = new TitleManager(this);
	this->gameObjectList.push_back(titleManager);

	TitleLogo* titleLogo = new TitleLogo(this);
	this->gameObjectList.push_back(titleLogo);



	Scene::Init();
}

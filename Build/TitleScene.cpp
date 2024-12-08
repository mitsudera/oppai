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

	Scene::Init();


	TitleLogo* titleLogo = new TitleLogo(this);
	this->gameObjectList.push_back(titleLogo);

	Camera* titleCamera = new Camera(this);
	this->gameObjectList.push_back(titleCamera);



	//他のオブジェクトを取得する可能性があるオブジェクトは後から
	TitleManager* titleManager = new TitleManager(this);
	this->gameObjectList.push_back(titleManager);

	Scene::InitAllObject();

}

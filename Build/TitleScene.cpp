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



	//���̃I�u�W�F�N�g���擾����\��������I�u�W�F�N�g�͌ォ��
	TitleManager* titleManager = new TitleManager(this);
	this->gameObjectList.push_back(titleManager);

	Scene::InitAllObject();

}

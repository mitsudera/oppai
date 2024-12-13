#include "GameScene.h"
#include "Robot.h"
#include "GameManager.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "SkySphere.h"
#include "MeshComponent.h"
#include "Tree.h"
#include "DirectionalLightComponent.h"
#include "LightManager.h"
#include "transformcomponent.h"
#include "DirectionalLight.h"
#include "GameEngine.h"
#include "AssetsManager.h"

GameScene::GameScene(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Scene::Init();


	DirectionalLight* light = new DirectionalLight(this);
	this->gameObjectList.push_back(light);


	SkySphere* sky = new SkySphere(this);
	this->gameObjectList.push_back(sky);

	Robot* robot = new Robot(this);
	this->gameObjectList.push_back(robot);

	GameObject* field = new GameObject(this);
	this->gameObjectList.push_back(field);
	field->LoadFbxFileMesh("blueplane.fbx");

	DebugCamera* debugCamera=new DebugCamera(this);
	this->gameObjectList.push_back(debugCamera);



	//他のオブジェクトを取得する可能性があるオブジェクトは後から
	GameManager* gameManager = new GameManager(this);
	this->gameObjectList.push_back(gameManager);



	//最後に行う
	Scene::InitAllObject();
}

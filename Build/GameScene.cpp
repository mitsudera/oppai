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

	SkySphere* sky = new SkySphere(this);
	this->gameObjectList.push_back(sky);

	Robot* robot = new Robot(this);
	this->gameObjectList.push_back(robot);

	//Tree* tree = new Tree(this);
	//this->gameObjectList.push_back(tree);

	DebugCamera* debugCamera=new DebugCamera(this);
	this->gameObjectList.push_back(debugCamera);

	GameObject* light = new GameObject(this);
	this->gameObjectList.push_back(light);

	DirectionalLightComponent* lcom= light->AddComponent<DirectionalLightComponent>();
	DIREC_LIGHT_PARAM lParam;
	lParam.m_Direction = { 0.0f, - 1.0f, 0.0f, 0.0f };
	lParam.m_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	lParam.m_Enable = 1;
	lParam.m_Ambient= { 0.2f, 0.2f, 0.2f, 1.0f };
	lcom->SetLight(lParam);



	//他のオブジェクトを取得する可能性があるオブジェクトは後から
	GameManager* gameManager = new GameManager(this);
	this->gameObjectList.push_back(gameManager);



	//最後に行う
	Scene::InitAllObject();
}

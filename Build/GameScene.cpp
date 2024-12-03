#include "GameScene.h"
#include "Robot.h"
#include "GameManager.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "SkySphere.h"

GameScene::GameScene(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	SkySphere* sky = new SkySphere(this);
	this->gameObjectList.push_back(sky);
	Robot* robot = new Robot(this);
	this->gameObjectList.push_back(robot);
	//Camera* gameCamera=new Camera(this);
	//this->gameObjectList.push_back(gameCamera);
	DebugCamera* debugCamera=new DebugCamera(this);
	this->gameObjectList.push_back(debugCamera);




	//他のオブジェクトを取得する可能性があるオブジェクトは後から
	GameManager* gameManager = new GameManager(this);
	this->gameObjectList.push_back(gameManager);



	//最後に行う
	Scene::Init();
}

#include "GameScene.h"
#include "Robot.h"
#include "GameManager.h"
#include "Camera.h"

GameScene::GameScene(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Robot* robot = new Robot(this);
	this->gameObjectList.push_back(robot);
	GameManager* gameManager = new GameManager(this);
	this->gameObjectList.push_back(gameManager);
	Camera* gameCamera=new Camera(this);
	this->gameObjectList.push_back(gameCamera);

	this->mainCamera = gameCamera->GetCameraComponent();


	//ÅŒã‚És‚¤
	Scene::Init();
}

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
#include "BGMPlayer.h"
#include "MeshField.h"

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
	sky->SetHasShadowAll(FALSE);





	Robot* robot = new Robot(this);
	this->gameObjectList.push_back(robot);

	MeshField* field = new MeshField(this);
	this->gameObjectList.push_back(field);

	//GameObject* field = new GameObject(this);
	//this->gameObjectList.push_back(field);
	//field->LoadFbxFileMesh("blueplane.fbx");
	//field->SetHasShadowAll(FALSE);


	DebugCamera* debugCamera=new DebugCamera(this);
	this->gameObjectList.push_back(debugCamera);


	BGMPlayer* bgmPlayer = new BGMPlayer(this);
	this->gameObjectList.push_back(bgmPlayer);


	//他のオブジェクトを取得する可能性があるオブジェクトは後から
	GameManager* gameManager = new GameManager(this);
	this->gameObjectList.push_back(gameManager);



	//最後に行う
	Scene::InitAllObject();
}

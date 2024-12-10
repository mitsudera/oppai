#include "GameManagerComponent.h"
#include "GameScene.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "input.h"
#include "CameraComponent.h"
#include "RobotComponent.h"

GameManagerComponent::GameManagerComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

GameManagerComponent::~GameManagerComponent()
{
}

void GameManagerComponent::Init(void)
{
	Component::Init();
	attribute = Attribute::Manager;
	this->gameScene = pGameObject->GetScene();
	this->gameCamera = gameScene->GetGameObjectName("Robot")->GetChild("Camera")->GetComponentAttrbute(Component::Attribute::Camera, 0);
	this->debugCamera = gameScene->GetGameObjectName("DebugCamera")->GetComponentAttrbute(Component::Attribute::Camera, 0);

	SetCameraModeGame();
}

void GameManagerComponent::Uninit(void)
{
	Component::Uninit();
}

void GameManagerComponent::Update(void)
{
	Component::Update();
	if(input->GetKeyboardTrigger(DIK_1))
	{
		SetCameraModeGame();
	}
	if(input->GetKeyboardTrigger(DIK_2))
	{
		SetCameraModeDebug();
	}

	if(input->GetKeyboardTrigger(DIK_3))
	{
		pGameEngine->SetFullScreen(TRUE);
	}
	if(input->GetKeyboardTrigger(DIK_4))
	{
		pGameEngine->SetFullScreen(FALSE);
	}
	if(input->GetKeyboardTrigger(DIK_5))
	{
		pGameEngine->ChengeWindowSize(960,560);
	}



}

void GameManagerComponent::SetCameraModeGame(void)
{
	gameCamera->SetActive(TRUE);
	debugCamera->SetActive(FALSE);
	pGameObject->GetScene()->GetGameObjectName("Robot")->GetComponent<RobotComponent>()->SetActive(TRUE);

}

void GameManagerComponent::SetCameraModeDebug(void)
{
	gameCamera->SetActive(FALSE);
	debugCamera->SetActive(TRUE);
	pGameObject->GetScene()->GetGameObjectName("Robot")->GetComponent<RobotComponent>()->SetActive(FALSE);

}


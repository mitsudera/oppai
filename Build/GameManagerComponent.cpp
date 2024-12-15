#include "GameManagerComponent.h"
#include "GameScene.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "input.h"
#include "CameraComponent.h"
#include "RobotComponent.h"
#include "CameraComponent.h"
#include "ShadowMap.h"

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
	this->gameCamera = gameScene->GetGameObjectName("Robot")->GetChild("Camera")->GetComponent<CameraComponent>();
	this->debugCamera = gameScene->GetGameObjectName("DebugCamera")->GetComponent<CameraComponent>();

	SetCameraModeDebug();

	pGameEngine->GetShadowMap()->SetEnable(TRUE);
	pGameEngine->GetShadowMap()->SetVariance(TRUE);

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



}

void GameManagerComponent::SetCameraModeGame(void)
{
	gameCamera->SetActive(TRUE);
	gameCamera->SetMainCamera();
	debugCamera->SetActive(FALSE);
	//pGameObject->GetScene()->GetGameObjectName("Robot")->GetComponent<RobotComponent>()->SetActive(TRUE);


}

void GameManagerComponent::SetCameraModeDebug(void)
{
	gameCamera->SetActive(FALSE);
	debugCamera->SetActive(TRUE);
	debugCamera->SetMainCamera();
	//pGameObject->GetScene()->GetGameObjectName("Robot")->GetComponent<RobotComponent>()->SetActive(FALSE);

}


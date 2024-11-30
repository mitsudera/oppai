#include "TitleManagerComponent.h"
#include "GameEngine.h"
#include "Scene.h"
#include "input.h"
#include "SceneManager.h"
TitleManagerComponent::TitleManagerComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	attribute = Attribute::Component;
}

TitleManagerComponent::~TitleManagerComponent()
{
}

void TitleManagerComponent::Init(void)
{
}

void TitleManagerComponent::Uninit(void)
{
}

void TitleManagerComponent::Update(void)
{
	Component::Update();

	Input* input= pGameObject->GetScene()->GetGameEngine()->GetInput();
	SceneManager* sceneManager= pGameObject->GetScene()->GetGameEngine()->GetSceneManager();
	if (input->GetKeyboardTrigger(DIK_SPACE))
	{
		sceneManager->SetScene(SCENE::GAME);
	}
}

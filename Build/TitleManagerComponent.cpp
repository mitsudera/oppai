#include "TitleManagerComponent.h"
#include "GameEngine.h"
#include "Scene.h"
#include "input.h"
#include "SceneManager.h"
TitleManagerComponent::TitleManagerComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

TitleManagerComponent::~TitleManagerComponent()
{
}

void TitleManagerComponent::Init(void)
{

	Component::Init();
	attribute = Attribute::Manager;

}

void TitleManagerComponent::Uninit(void)
{
	Component::Uninit();
}

void TitleManagerComponent::Update(void)
{
	Component::Update();

	SceneManager* sceneManager= pGameObject->GetScene()->GetGameEngine()->GetSceneManager();
	if (input->GetKeyboardTrigger(DIK_SPACE))
	{
		sceneManager->SetScene(SCENE::GAME);
	}
}

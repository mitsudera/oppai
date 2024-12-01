#include "GameManagerComponent.h"

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

}

void GameManagerComponent::Uninit(void)
{
	Component::Uninit();
}

void GameManagerComponent::Update(void)
{
	Component::Update();
}

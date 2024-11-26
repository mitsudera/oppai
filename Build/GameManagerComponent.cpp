#include "GameManagerComponent.h"

GameManagerComponent::GameManagerComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	attribute = Attribute::Manager;
}

GameManagerComponent::~GameManagerComponent()
{
}

void GameManagerComponent::Init(void)
{
}

void GameManagerComponent::Uninit(void)
{
}

void GameManagerComponent::Update(void)
{
}

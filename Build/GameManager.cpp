#include "GameManager.h"
#include "GameManagerComponent.h"
GameManager::GameManager(Scene* scene)
{
	this->pScene = scene;

}

GameManager::~GameManager()
{
}

void GameManager::Init(void)
{
	GameObject::Init();
	this->name = "GameManager";
	GameManagerComponent* gameManagerComponent = new GameManagerComponent(this);
	gameManagerComponent->Init();
	componentList.push_back(gameManagerComponent);
}

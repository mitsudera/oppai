#include "TitleManager.h"
#include "TitleManagerComponent.h"

TitleManager::TitleManager(Scene* scene)
{
	this->pScene = scene;
}

TitleManager::~TitleManager()
{
}

void TitleManager::Init(void)
{
	GameObject::Init();
	this->name = "TitleManager";
	TitleManagerComponent* titleManager = new TitleManagerComponent(this);
	titleManager->Init();
	this->componentList.push_back(titleManager);


}

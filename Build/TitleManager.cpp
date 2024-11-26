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
	TitleManagerComponent* titleManager = new TitleManagerComponent(this);
	this->componentList.push_back(titleManager);


}

#include "LightManager.h"
#include "GameEngine.h"
#include "renderer.h"
#include "ligh"

LightManager::LightManager(GameEngine* gameEngine)
{
	this->gameEngine = gameEngine;
}

LightManager::~LightManager()
{
	
}

void LightManager::Init()
{
	activeLight = new LightComponent[MAX_LIGHT];

}

void LightManager::Update()
{
}

void LightManager::Draw()
{
	Renderer* renderer = gameEngine->GetRenderer();

}

void LightManager::Uninit()
{
}

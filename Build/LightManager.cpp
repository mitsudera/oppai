#include "LightManager.h"
#include "GameEngine.h"
#include "renderer.h"

LightManager::LightManager(GameEngine* gameEngine)
{
	this->gameEngine = gameEngine;
}

LightManager::~LightManager()
{
	
}

void LightManager::Init()
{
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

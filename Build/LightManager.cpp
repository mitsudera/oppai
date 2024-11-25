#include "LightManager.h"
#include "GameEngine.h"
#include "renderer.h"
#include "light.h"
#include "LightComponent.h"

LightManager::LightManager(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
}

LightManager::~LightManager()
{
	
}

void LightManager::Init()
{

}

void LightManager::Update()
{
	XMFLOAT3 mainCamPos;


	for (int i = 0; i < MAX_LIGHT; i++)
	{
		activeLight[i] = lightList[i];
	}

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		pGameEngine->GetRenderer()->SetLight(i,activeLight[i]->GetLightParam());
	}

}

void LightManager::Draw()
{
	Renderer* renderer = pGameEngine->GetRenderer();

}

void LightManager::Uninit()
{
}

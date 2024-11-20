#include "Scene.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "CollisionManger.h"

Scene::Scene(GameEngine* gameEngine)
{
	this->gameEngine = gameEngine;
	this->coliisionManager = new CollisionManger(this);
}

Scene::~Scene()
{
}

void Scene::Init()
{


	for (int i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->Init();
	}
}

void Scene::Update()
{
	for (int i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->Update();
	}

}

void Scene::Draw()
{
	for (int i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->Draw();
	}

}

void Scene::Uninit()
{
	for (int i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->Uninit();
	}

}

GameEngine* Scene::GetGameEngine(void)
{
	return this->gameEngine;
}

CollisionManger* Scene::GetCollisionManager(void)
{
	return this->coliisionManager;
}

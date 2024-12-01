#include "Scene.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "CollisionManger.h"
#include "CameraComponent.h"

Scene::Scene()
{
}

Scene::Scene(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
	this->coliisionManager = new CollisionManger(this);

}

Scene::~Scene()
{
	delete this->coliisionManager;

}

void Scene::Init()
{
	this->coliisionManager->Init();

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
	//•`‰æˆ—
	for (GameObject* gameObject :GetGameObject())
	{

		for (Component* component : gameObject->GetComponentList())
		{
			if (component->GetAttribute() != Component::Attribute::Camera)
				continue;

			CameraComponent* cameraComponent = static_cast<CameraComponent*>(component);


			cameraComponent->Render();

		}

	}


}

void Scene::Uninit()
{
	for (int i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->Uninit();
	}

	this->coliisionManager->Uninit();

}

GameEngine* Scene::GetGameEngine(void)
{
	return this->pGameEngine;
}

CollisionManger* Scene::GetCollisionManager(void)
{
	return this->coliisionManager;
}

vector<GameObject*>& Scene::GetGameObject(void)
{
	return this->gameObjectList;
}

#include "Scene.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "CollisionManger.h"
#include "CameraComponent.h"
#include "component.h"
#include "transformcomponent.h"

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



}

void Scene::Update()
{

	for (GameObject* gameObject : GetGameObject())
	{
		if (!gameObject->GetActive())
			continue;
		gameObject->Update();

	}


	for (GameObject* gameObject : GetGameObject())
	{
		if (!gameObject->GetActive())
			continue;

		gameObject->UpdateMatrix();

	}



}

void Scene::Draw()
{

	for (CameraComponent* camera:cameraArray)
	{
		if (!camera->GetActive())
			continue;

		camera->Render();
	}
}

void Scene::Uninit()
{
	for (GameObject* gameObject : GetGameObject())
	{
		gameObject->Uninit();

	}
	this->coliisionManager->Uninit();

}

void Scene::InitAllObject(void)
{

	for (GameObject* gameObject : GetGameObject())
	{
		gameObject->Init();

	}

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

GameObject* Scene::GetGameObjectName(string name)
{
	for (GameObject* object:gameObjectList)
	{
		if (object->GetName() == name)
		{
			return object;

		}

	}
	return nullptr;
}

void Scene::AddCamera(CameraComponent* camera)
{

	this->cameraArray.push_back(camera);
}

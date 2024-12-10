#include "Scene.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "CameraComponent.h"
#include "component.h"
#include "transformcomponent.h"

Scene::Scene()
{
}

Scene::Scene(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;

}

Scene::~Scene()
{

}

void Scene::Init()
{



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

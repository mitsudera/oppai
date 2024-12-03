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


	for (GameObject* gameObject : GetGameObject())
	{
		gameObject->Init();

	}

}

void Scene::Update()
{

	for (GameObject* gameObject : GetGameObject())
	{
		if (!gameObject->GetActive())
			continue;
		gameObject->Update();

	}

}

void Scene::Draw()
{
	//•`‰æˆ—
	for (GameObject* gameObject :GetGameObject())
	{
		if (!gameObject->GetActive())
			continue;

		for (Component* component : gameObject->GetComponentList())
		{
			if (component->GetAttribute() != Component::Attribute::Camera||!component->GetActive())
				continue;

			CameraComponent* cameraComponent = static_cast<CameraComponent*>(component);


			cameraComponent->Render();

		}

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

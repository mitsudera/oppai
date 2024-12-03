#include "gameobject.h"
#include "GameEngine.h"
#include "transformcomponent.h"
#include "Scene.h"
#include "ColliderComponent.h"
#include "component.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"
#include "ColliderComponent.h"
#include "GameManagerComponent.h"
#include "LightComponent.h"
#include "MeshComponent.h"
#include "primitivecomponent.h"
#include "SpriteComponent.h"
#include "TitleManagerComponent.h"
#include "transformcomponent.h"
#include "UIManagerComponent.h"

GameObject::GameObject()
{
	pScene =nullptr;
	this->transformComponent = new TransformComponent(this);

	tag = ObjectTag::TagNone;
}


GameObject::GameObject(Scene* scene)
{
	this->parent = nullptr;
	this->pScene = scene;
}

GameObject::GameObject(GameObject* parent)
{
	this->parent = parent;
	this->pScene = parent->GetScene();
}

GameObject::~GameObject()
{
	delete this->transformComponent;
}

void GameObject::Init(void)
{
	this->name = "notname";
	this->transformComponent = new TransformComponent(this);
	this->transformComponent->Init();
	componentList.push_back(transformComponent);
	this->collider = nullptr;
	this->isActive = TRUE;
	this->layer = Layer::Default;

}

void GameObject::Uninit(void)
{
	for (int i = 0; i < this->componentList.size(); i++)
	{
		componentList[i]->Uninit();
	}
	for (int i = 0; i < child.size(); i++)
	{
		child[i]->Uninit();
	}

}

void GameObject::Update(void)
{
	for (Component* com:componentList)
	{
		if (!com->GetActive())
			continue;

		com->Update();
	}
	for (GameObject* gameObject:child)
	{
		if (!gameObject->GetActive())
			continue;

		gameObject->Update();
	}

}

void GameObject::Draw(void)
{
	for (int i = 0; i < this->componentList.size(); i++)
	{
		componentList[i]->Draw();
	}
	for (int i = 0; i < child.size(); i++)
	{
		child[i]->Draw();
	}

}

Scene* GameObject::GetScene(void)
{

	return this->pScene;
}

TransformComponent* GameObject::GetTransFormComponent(void)
{
	return this->transformComponent;
}

ColliderComponent* GameObject::GetCollider(void)
{
	return this->collider;
}

ObjectTag GameObject::GetTag(void)
{
	return tag;
}

Layer GameObject::GetLayer(void)
{
	return this->layer;
}

BOOL GameObject::GetActive(void)
{
	return isActive;
}

void GameObject::SetActive(BOOL isActive)
{
	this->isActive = isActive;
}

GameObject* GameObject::GetParent(void)
{
	return this->parent;
}

GameObject* GameObject::GetChild(int index)
{
	return this->child[index];
}

vector<Component*>& GameObject::GetComponentList(void)
{
	return this->componentList;
}

Component* GameObject::GetComponentAttrbute(Component::Attribute attr, int n)
{
	int cnt = 0;

	for (int i = 0; i < componentList.size(); i++)
	{
		if (componentList[i]->GetAttribute()==attr)
		{
			if (cnt==n)
			{
				return componentList[i];

			}
			else
			{
				cnt++;
			}
		}
	}
	return nullptr;
}

string GameObject::GetName(void)
{
	return this->name;
}

void GameObject::SetName(string name)
{
	this->name = name;
}




template<class T>
T* GameObject::GetComponent(void)
{
	for (Component* com : componentList) {
		T* buff = dynamic_cast<T*>(com);
		if (buff != nullptr)
			return buff;
	}
	return nullptr;

}
// 具体的な型に対する明示的なインスタンス化
template Component* GameObject::GetComponent<Component>();
template CameraComponent* GameObject::GetComponent<CameraComponent>();
template CameraComponent* GameObject::GetComponent<CameraComponent>();
template CameraControllerComponent* GameObject::GetComponent<CameraControllerComponent>();
template GameManagerComponent* GameObject::GetComponent<GameManagerComponent>();
template LightComponent* GameObject::GetComponent<LightComponent>();
template MeshComponent* GameObject::GetComponent<MeshComponent>();
template PrimitiveComponent* GameObject::GetComponent<PrimitiveComponent>();
template SpriteComponent* GameObject::GetComponent<SpriteComponent>();
template TitleManagerComponent* GameObject::GetComponent<TitleManagerComponent>();
template UIManagerComponent* GameObject::GetComponent<UIManagerComponent>();


#include "gameobject.h"
#include "GameEngine.h"
#include "transformcomponent.h"
#include "Scene.h"
#include "ColliderComponent.h"
#include "component.h"

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
	this->transformComponent = new TransformComponent(this);
	componentList.push_back(transformComponent);
	this->collider = nullptr;




	//child




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
	for (int i = 0; i < this->componentList.size(); i++)
	{
		componentList[i]->Update();
	}
	for (int i = 0; i < child.size(); i++)
	{
		child[i]->Update();
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

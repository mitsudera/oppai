#include "gameobject.h"
#include "GameEngine.h"
#include "transformcomponent.h"
#include "Scene.h"
#include "ColliderComponent.h"

GameObject::GameObject()
{
	pScene =nullptr;
	this->transformComponent = new TransformComponent(this);

	tag = ObjectTag::TagNone;
}


GameObject::GameObject(Scene* scene)
{
	this->pScene = scene;
	this->transformComponent = new TransformComponent(this);
	this->collider = new ColliderComponent(this);
}

GameObject::~GameObject()
{
	delete this->transformComponent;
}

void GameObject::Init(void)
{
}

void GameObject::Uninit(void)
{
}

void GameObject::Update(void)
{
}

void GameObject::Draw(void)
{
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

BOOL GameObject::GetUse(void)
{
	return use;
}

void GameObject::SetUse(BOOL use)
{
	this->use = use;
}

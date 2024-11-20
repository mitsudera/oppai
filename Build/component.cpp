#include "component.h"
#include "transformcomponent.h"
#include "gameobject.h"
#include "GameEngine.h"

Component::Component()
{
}
Component::Component(GameObject* gameObject)
{
	this->pGameObject = gameObject;

}

Component::~Component()
{
}

void Component::Init(void)
{
}

void Component::Uninit(void)
{
}

void Component::Update(void)
{
}

GameObject* Component::GetGameObject(void)
{
	return this->pGameObject;
}


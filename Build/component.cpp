#include "component.h"
#include "transformcomponent.h"
#include "gameobject.h"
#include "GameEngine.h"

Component::Component()
{
	attribute = Attribute::Component;
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
	this->attribute = Attribute::Component;
}

void Component::Uninit(void)
{
}

void Component::Update(void)
{
	if (!isActive) return;
}

void Component::Draw(void)
{
	if (!isActive) return;

}

GameObject* Component::GetGameObject(void)
{
	return this->pGameObject;
}

Component::Attribute Component::GetAttribute(void)
{
	return this->attribute;
}

void Component::SetActive(BOOL b)
{
	isActive = b;
}

BOOL Component::GetActive(void)
{
	if (!GetGameObject()->GetActive())
	{
		return FALSE;
	}

	return this->isActive;
}


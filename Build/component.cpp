#include "component.h"
#include "transformcomponent.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "input.h"
#include "Scene.h"

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
	this->pGameEngine = pGameObject->GetScene()->GetGameEngine();
	this->input = pGameEngine->GetInput();
	this->isActive = TRUE;
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

XMMATRIX Component::GetWorldMtx(XMMATRIX world)
{
	return this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
}

XMMATRIX Component::GetWorldMtx(void)
{
	return this->pGameObject->GetTransFormComponent()->GetWorldMtx();
}

XMFLOAT3 Component::GetWorldPos()
{
	return this->pGameObject->GetTransFormComponent()->GetWorldPos();
}

TransformComponent* Component::GetTransFormComponent(void)
{
	return this->pGameObject->GetTransFormComponent();
}


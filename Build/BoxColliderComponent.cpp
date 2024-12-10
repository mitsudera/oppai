#include "BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

BoxColliderComponent::~BoxColliderComponent()
{
}

void BoxColliderComponent::Init(void)
{
	ColliderComponent::Init();
	this->shape = Shape::Box;
}

void BoxColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();
}

void BoxColliderComponent::Update(void)
{
	ColliderComponent::Update();

}

void BoxColliderComponent::SetBox(XMFLOAT3 size)
{
	this->size = size;
	XMVECTOR len = XMVector3Length(XMLoadFloat3(&size));
	XMStoreFloat(&checkRadius, len / 2);


}

XMFLOAT3 BoxColliderComponent::GetSize(void)
{
	return this->size;
}

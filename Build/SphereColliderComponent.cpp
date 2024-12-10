#include "SphereColliderComponent.h"

SphereColliderComponent::SphereColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

SphereColliderComponent::~SphereColliderComponent()
{
}

void SphereColliderComponent::Init(void)
{
	ColliderComponent::Init();
	this->shape = Shape::Sphere;
}

void SphereColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();
}

void SphereColliderComponent::Update(void)
{
	ColliderComponent::Update();

}

void SphereColliderComponent::SetRadius(float r)
{
	this->checkRadius = r;
}

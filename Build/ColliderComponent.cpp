#include "ColliderComponent.h"
#include "gameobject.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "GameEngine.h"
ColliderComponent::ColliderComponent()
{
	this->attribute = Component::Attribute::Collider;

}
ColliderComponent::ColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	this->attribute = Component::Attribute::Collider;
	
}

ColliderComponent::~ColliderComponent()
{

}

void ColliderComponent::Init(void)
{
	Component::Init();
	
	this->attribute = Attribute::Collider;
	result.hitObject.clear();
	
	for (int i = 0; i < ObjectTag::ObjectTagMax; i++)
	{
		result.isHit[i] = FALSE;

	}

}

void ColliderComponent::Uninit(void)
{
	Component::Uninit();
	this->GetGameObject()->GetScene()->GetGameEngine()->GetCollisionManager()->DeleteCllider(this);

}

void ColliderComponent::Update(void)
{
	Component::Update();

	this->center = GetWorldPos();


}

BOOL ColliderComponent::GetHitTag(ObjectTag tag)
{
	return result.isHit[tag];
}

void ColliderComponent::SetHitTag(ObjectTag tag, BOOL isHit)
{
	this->result.isHit[tag] = isHit;
}

ColliderComponent::Shape ColliderComponent::GetShape(void)
{
	return this->shape;

}

void ColliderComponent::SetShape(Shape shape)
{
	this->shape = shape;
}

ObjectTag ColliderComponent::GetTag(void)
{
	return pGameObject->GetTag();
}


void ColliderComponent::SetHitObject(GameObject* gameObject)
{
	this->result.hitObject.push_back(gameObject);

}

BOOL ColliderComponent::GetHitObject(GameObject* gameObject)
{
	BOOL ans = FALSE;

	if (gameObject==nullptr)
	{
		return FALSE;
	}

	for (int i = 0; i < result.hitObject.size(); i++)
	{
		if (result.hitObject[i]==gameObject)
		{
			ans = TRUE;
		}
	}


	return ans;
}

void ColliderComponent::Clear(void)
{
	this->result.hitObject.clear();

	for (int i = 0; i < ObjectTag::ObjectTagMax; i++)
	{
		this->result.isHit[i] = FALSE;
	}


}

XMFLOAT3 ColliderComponent::GetCenter(void)
{
	return this->center;
}

float ColliderComponent::GetCheckRadius(void)
{
	return this->checkRadius;
}




void ColliderComponent::onCollider(void)
{
	this->Init();
	this->GetGameObject()->GetScene()->GetGameEngine()->GetCollisionManager()->AddCollider(this);
}

void ColliderComponent::offCollider(void)
{

	this->GetGameObject()->GetScene()->GetGameEngine()->GetCollisionManager()->DeleteCllider(this);

}


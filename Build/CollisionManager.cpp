#include "GameEngine.h"
#include "CollisionManager.h"
#include "collision.h"
#include "ColliderComponent.h"
#include "PointColliderComponent.h"
#include "SphereColliderComponent.h"
#include "LineColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "BoxColliderComponent.h"

CollisionManager::CollisionManager(GameEngine* gameEngine)
{
	this->pGameEngine = gameEngine;

}

CollisionManager::~CollisionManager()
{
}


void CollisionManager::Update(void)
{

	for (int i = 0; i < colliderList.size(); i++)
	{
		colliderList[i]->Clear();
	}


	for (int i = 0; i < pairList.size(); i++)
	{
		
		BOOL ans = CheckCillision(pairList[i]);



		pairList[i].collider1->SetHitTag(pairList[i].collider2->GetTag(), ans);
		pairList[i].collider2->SetHitTag(pairList[i].collider1->GetTag(), ans);

		if (ans==TRUE)
		{
			pairList[i].collider1->SetHitObject(pairList[i].collider2->GetGameObject());
			pairList[i].collider2->SetHitObject(pairList[i].collider1->GetGameObject());

		}



	}

	
}

void CollisionManager::AddCollider(ColliderComponent* collider)
{

	for (int i = 0; i < colliderList.size(); i++)
	{
		ColliderPair pair;

		pair.collider1 = colliderList[i];
		pair.collider2 = collider;

		pairList.push_back(pair);
	}

	colliderList.push_back(collider);

}

void CollisionManager::DeleteCllider(ColliderComponent* collider)
{
	for (int i = 0; i < pairList.size(); i++)
	{
		if (pairList[i].collider1 == collider || pairList[i].collider2 == collider)
		{
			pairList.erase(pairList.begin()+i);
		}

	}

	for (int i = 0; i < colliderList.size(); i++)
	{
		if (colliderList[i] == collider)
		{
			colliderList.erase(colliderList.begin()+i);
		}

	}


}

BOOL CollisionManager::CheckCillision(ColliderPair pair)
{
	XMFLOAT3 pos1 = pair.collider1->GetCenter();
	XMFLOAT3 pos2 = pair.collider2->GetCenter();

	float check1 = pair.collider1->GetCheckRadius();
	float check2 = pair.collider2->GetCheckRadius();

	//‚±‚±‚Å‘å‚Ü‚©‚É“–‚½‚Á‚Ä‚¢‚é‚©”»’è
	if (CollisionSphereSphere(pos1, check1, pos2, check2) == FALSE)
	{
		return FALSE;
	}






	if (pair.collider1->GetShape() == ColliderComponent::ColliderComponent::ColliderComponent::Shape::Point)
	{
		PointColliderComponent* point= static_cast<PointColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return CollisionPointSphere(pos1,pos2,sphere->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionPointCapsule(pos1,capsule->GetStart(),capsule->GetEnd(),capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionPointBox(pos1, box->GetCenter(),box->GetSize());

		}

	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Line)
	{
		LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{

			return FALSE;
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);

			return CollisionLineSphere(line->GetStart(), line->GetEnd(),pos2,sphere->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionLineCapsule(line->GetStart(), line->GetEnd(), capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionLineBox(line->GetStart(), line->GetEnd(), box->GetCenter(), box->GetSize());

		}

	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Sphere)
	{
		SphereColliderComponent* sphere1 = static_cast<SphereColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			PointColliderComponent* point = static_cast<PointColliderComponent*>(pair.collider2);


			return CollisionPointSphere(pos2,pos1,sphere1->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);

			return CollisionLineSphere(line->GetStart(), line->GetEnd(), pos1, sphere1->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere2 = static_cast<SphereColliderComponent*>(pair.collider2);

			return CollisionSphereSphere(pos1, sphere1->GetCheckRadius(), pos2, sphere2->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionSphereCapsule(pos1, sphere1->GetCheckRadius(), capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionSphereBox(pos1, sphere1->GetCheckRadius(), box->GetCenter(), box->GetSize());

		}



	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Capsule)
	{
	CapsuleColliderComponent* capsule1 = static_cast<CapsuleColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return CollisionPointCapsule(pos2, capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);


			return CollisionLineCapsule(line->GetStart(), line->GetEnd(), capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{

			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return CollisionSphereCapsule(pos2, sphere->GetCheckRadius(), capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule2 = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionCapsuleCapsule(capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius(),capsule2->GetStart(), capsule2->GetEnd(), capsule2->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionCapsuleBox(capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius(), box->GetCenter(), box->GetSize());

		}

	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Box)
	{
	BoxColliderComponent* box1 = static_cast<BoxColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return CollisionPointBox(pos2, box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);


			return CollisionLineBox(line->GetStart(), line->GetEnd(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{

			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return CollisionSphereBox(pos2, sphere->GetCheckRadius(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionCapsuleBox(capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box2 = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionBoxBox(box1->GetCenter(), box1->GetSize(), box2->GetCenter(), box2->GetSize());

		}

	}

	return FALSE;
}


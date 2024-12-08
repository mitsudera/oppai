#include "AnimationControlerComponent.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "Scene.h"
#include "MeshComponent.h"

AnimationControlerComponent::AnimationControlerComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
	pAssetsManager = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager();
	//pMeshTree= pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->GetMeshTree(pGameObject->GetComponent<MeshComponent>()->)

}

AnimationControlerComponent::~AnimationControlerComponent()
{

}

void AnimationControlerComponent::Init(void)
{
	Component::Init();
}

void AnimationControlerComponent::Update(void)
{
	Component::Update();
}

void AnimationControlerComponent::Uninit(void)
{
	Component::Uninit();
}

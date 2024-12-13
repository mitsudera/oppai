#include "AnimationControlerComponent.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "Scene.h"
#include "MeshComponent.h"
#include "AnimationData.h"
#include "transformcomponent.h"
#include "input.h"

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
	defaultAnimIndex = 0;
	animindex = 0;
	framecnt = 0.0f;
}

void AnimationControlerComponent::Update(void)
{
	Component::Update();


	AnimationData* animData = pAssetsManager->GetAnimationData(animIndexArray[animindex]);

	MtxNode* root = animData->GetMtxTreeRoot();

	for (int i = 0; i < root->GetChildCnt(); i++)
	{
		MtxNode* child = root->GetChild(i);
		GameObject* childObj = pGameObject->GetChild(i);

		UpdateAnimation(child, childObj);
	}
	
	framecnt += pGameEngine->GetDeltaTime()*60.0f;
}

void AnimationControlerComponent::Uninit(void)
{
	Component::Uninit();
}

int AnimationControlerComponent::LoadAnimationData(string fileName,string name)
{
	int index = pAssetsManager->LoadAnimationData(fileName);
	animIndexArray.push_back(index);
	animNameArray.push_back(name);
	return animIndexArray.size() - 1;
}

void AnimationControlerComponent::UpdateAnimation(MtxNode* node, GameObject* gameObject)
{

	XMMATRIX frameMtx = node->GetFrameMtx(framecnt);

	gameObject->GetTransFormComponent()->SetMtxUpdate(FALSE);
	gameObject->GetTransFormComponent()->SetLocalMtx(frameMtx);

	for (int i = 0; i < node->GetChildCnt(); i++)
	{
		MtxNode* child = node->GetChild(i);
		GameObject* childObj = gameObject->GetChild(i);

		UpdateAnimation(child, childObj);

	}

}

void AnimationControlerComponent::SetAnimation(int index)
{
	this->animindex = index;
}

void AnimationControlerComponent::SetAnimation(string name)
{
	for (int i = 0; i < animNameArray.size(); i++)
	{
		if (animNameArray[i] == name)
			animindex = i;

	}
}



AnimationNode::~AnimationNode()
{
}

int AnimationNode::GetAnimIndex(void)
{
	return animDataIndex;
}

string AnimationNode::GetName(void)
{
	return name;
}


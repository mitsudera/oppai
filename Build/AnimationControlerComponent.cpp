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


	AnimationData* animData = pAssetsManager->GetAnimationData(animindex);

	MtxNode* root = animData->GetMtxTreeRoot();

	for (int i = 0; i < root->GetChildCnt(); i++)
	{
		MtxNode* child = root->GetChild(i);
		GameObject* childObj = pGameObject->GetChild(i);

		UpdateAnimation(child, childObj);
	}
	
	if (pGameObject->GetScene()->GetGameEngine()->GetInput()->GetKeyboardPress(DIK_K)==TRUE)
	{
		framecnt+=1;
	}

}

void AnimationControlerComponent::Uninit(void)
{
	Component::Uninit();
}

int AnimationControlerComponent::LoadAnimationData(string fileName,string name)
{
	int index = pAssetsManager->LoadAnimationData(fileName);

	AnimationNode* newNode = new AnimationNode(index, name);

	this->animNodeArray.push_back(newNode);

	return (int)this->animNodeArray.size()-1;
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



AnimationNode::AnimationNode(int animIndex, string name)
{
	animDataIndex = animIndex;
	this->name = name;
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


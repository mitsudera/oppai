#include "RobotComponent.h"
#include "input.h"
#include "gameobject.h"
#include "transformcomponent.h"
#include "AnimationControlerComponent.h"
RobotComponent::RobotComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

RobotComponent::~RobotComponent()
{
}

void RobotComponent::Init(void)
{
	Component::Init();
}

void RobotComponent::Uninit(void)
{
	Component::Uninit();
}

void RobotComponent::Update(void)
{
	Component::Update();
	//if (input->GetKeyboardPress(DIK_W))
	//{
	//	this->GetGameObject()->GetTransFormComponent()->MoveZAxis(1.0f);

	//}
	//if (input->GetKeyboardPress(DIK_S))
	//{
	//	this->GetGameObject()->GetTransFormComponent()->MoveZAxis(-1.0f);

	//}
	//if (input->GetKeyboardPress(DIK_D))
	//{
	//	this->GetGameObject()->GetTransFormComponent()->MoveXAxis(1.0f);

	//}
	//if (input->GetKeyboardPress(DIK_A))
	//{
	//	this->GetGameObject()->GetTransFormComponent()->MoveXAxis(-1.0f);

	//}

	if (input->GetKeyboardPress(DIK_K))
	{
		this->GetComponent<AnimationControlerComponent>()->SetAnimation("walk");

	}

	if (input->GetKeyboardPress(DIK_L))
	{
		this->GetComponent<AnimationControlerComponent>()->SetAnimation("run");

	}



}

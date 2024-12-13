#include "CameraControllerComponent.h"
#include "gameobject.h"
#include "CameraComponent.h"
#include "GameEngine.h"
#include "Scene.h"
#include "input.h"
#include "transformcomponent.h"
CameraControllerComponent::CameraControllerComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

CameraControllerComponent::~CameraControllerComponent()
{
}

void CameraControllerComponent::Init(void)
{
	Component::Init();
	this->camara = pGameObject;
}

void CameraControllerComponent::Uninit(void)
{
	Component::Uninit();
}

void CameraControllerComponent::Update(void)
{
	Component::Update();
	if (input->GetKeyboardPress(DIK_W))
	{
		this->camara->GetTransFormComponent()->MoveZAxis(1.0f);

	}
	if (input->GetKeyboardPress(DIK_S))
	{
		this->camara->GetTransFormComponent()->MoveZAxis(-1.0f);

	}
	if (input->GetKeyboardPress(DIK_D))
	{
		this->camara->GetTransFormComponent()->MoveXAxis(1.0f);

	}
	if (input->GetKeyboardPress(DIK_A))
	{
		this->camara->GetTransFormComponent()->MoveXAxis(-1.0f);

	}
	if (input->GetKeyboardPress(DIK_E))
	{
		this->camara->GetTransFormComponent()->MoveYAxis(1.0f);

	}
	if (input->GetKeyboardPress(DIK_Q))
	{
		this->camara->GetTransFormComponent()->MoveYAxis(-1.0f);

	}
	if (input->IsMouseRightPressed())
	{
		float x, y;

		x = (float)input->GetMouseX();
		y = (float)input->GetMouseY();


		x *= 0.001f;
		y *= 0.001f;

		this->camara->GetTransFormComponent()->RotWorldYaw(x);
		this->camara->GetTransFormComponent()->RotPitch(y);

	}

}

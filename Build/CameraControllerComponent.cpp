#include "CameraControllerComponent.h"
#include "gameobject.h"
#include "CameraComponent.h"
#include "GameEngine.h"
#include "Scene.h"
#include "input.h"

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
	this->camara = pGameObject->GetComponent<CameraComponent>();
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
		this->camara->MoveZAxis(1.0f);

	}
	if (input->GetKeyboardPress(DIK_S))
	{
		this->camara->MoveZAxis(-1.0f);

	}
	if (input->GetKeyboardPress(DIK_D))
	{
		this->camara->MoveXAxis(1.0f);

	}
	if (input->GetKeyboardPress(DIK_A))
	{
		this->camara->MoveXAxis(-1.0f);

	}
	if (input->GetKeyboardPress(DIK_E))
	{
		this->camara->MoveYAxis(1.0f);

	}
	if (input->GetKeyboardPress(DIK_Q))
	{
		this->camara->MoveYAxis(-1.0f);

	}
	if (input->IsMouseRightPressed())
	{
		float x, y;

		x = input->GetMouseX();
		y = input->GetMouseY();


		x *= 0.001f;
		y *= 0.001f;

		this->camara->RotYaw(x);
		this->camara->RotPitch(y);

	}

}

#include "Camera.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "GameEngine.h"
#include "renderer.h"

Camera::Camera(Scene* scene)
{
	this->pScene = scene;
	this->parent = nullptr;
}

Camera::Camera(GameObject* gameObject)
{
	this->parent = gameObject;
	this->pScene = gameObject->GetScene();
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	GameObject::Init();
	cameraComponent = new CameraComponent(this);
	cameraComponent->Init();

	cameraComponent->SetRenderTarget(pScene->GetGameEngine()->GetRenderer()->GetBackBuffer());
	cameraComponent->SetDepthStencilView(pScene->GetGameEngine()->GetRenderer()->GetBackBufferDSV());

	this->componentList.push_back(cameraComponent);
}

CameraComponent* Camera::GetCameraComponent(void)
{
	return this->cameraComponent;
}

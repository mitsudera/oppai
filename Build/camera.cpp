#include "Camera.h"
#include "CameraComponent.h"


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
	cameraComponent = new CameraComponent(this);
}

CameraComponent* Camera::GetCameraComponent(void)
{
	return this->cameraComponent;
}

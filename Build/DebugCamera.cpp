#include "DebugCamera.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "GameEngine.h"
#include "renderer.h"
#include "CameraControllerComponent.h"

DebugCamera::DebugCamera(Scene* scene)
{
	this->pScene = scene;
	this->parent = nullptr;


}

DebugCamera::DebugCamera(GameObject* gameObject)
{
	this->parent = gameObject;
	this->pScene = gameObject->GetScene();
}


DebugCamera::~DebugCamera()
{
}

void DebugCamera::Init(void)
{
	GameObject::Init();
	this->name = "DebugCamera";
	cameraComponent = new CameraComponent(this);
	cameraComponent->Init();

	cameraComponent->SetRenderTarget(pScene->GetGameEngine()->GetRenderer()->GetBackBuffer());
	cameraComponent->SetDepthStencilView(pScene->GetGameEngine()->GetRenderer()->GetBackBufferDSV());
	cameraComponent->SetSky(pScene->GetGameObjectName("SkySphere"));


	this->componentList.push_back(cameraComponent);

	this->AddComponent<CameraControllerComponent>(); 

}


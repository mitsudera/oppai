#include "Robot.h"
#include "MeshComponent.h"
#include "transformcomponent.h"
#include "RobotComponent.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "GameEngine.h"
#include "renderer.h"

Robot::Robot(Scene* scene)
{
	this->pScene = scene;
}

Robot::~Robot()
{
}

void Robot::Init(void)
{

	GameObject::Init();

	this->name = "Robot";
	this->transformComponent->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));
	this->transformComponent->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

	this->LoadFbxFileMesh("robot.fbx");


	
	RobotComponent* robotComponent = new RobotComponent(this);
	robotComponent->Init();
	this->componentList.push_back(robotComponent);

	CameraComponent* cameraComponent = new CameraComponent(this);
	cameraComponent->Init();

	cameraComponent->SetRenderTarget(pScene->GetGameEngine()->GetRenderer()->GetBackBuffer());
	cameraComponent->SetDepthStencilView(pScene->GetGameEngine()->GetRenderer()->GetBackBufferDSV());

	cameraComponent->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));

	cameraComponent->SetSky(pScene->GetGameObjectName("SkySphere"));

	this->componentList.push_back(cameraComponent);

}

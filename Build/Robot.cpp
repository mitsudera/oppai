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


	//�q�I�u�W�F�N�g�Ƃ��ăJ�������쐬
	{
		//�Q�[���I�u�W�F�N�g����
		GameObject* child = AddChild("Camera");
		child->Init();
		child->GetTransFormComponent()->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));

		//�J�����R���|�[�l���g�̐���
		CameraComponent* cameraComponent = child->AddComponent<CameraComponent>();
		cameraComponent->Init();
		cameraComponent->SetRenderTarget(pScene->GetGameEngine()->GetRenderer()->GetBackBuffer());
		cameraComponent->SetDepthStencilView(pScene->GetGameEngine()->GetRenderer()->GetBackBufferDSV());
		cameraComponent->SetSky(pScene->GetGameObjectName("SkySphere"));


	}

}

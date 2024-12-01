#include "Robot.h"
#include "MeshComponent.h"
#include "transformcomponent.h"

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
	this->transformComponent->SetPosition(XMFLOAT3(0.0f, 0.0f, -10.0f));
	this->transformComponent->SetScale(XMFLOAT3(10.0f, 10.0f, 10.0f));

	MeshComponent* mesh = new MeshComponent(this);
	mesh->Init();
	mesh->SetMeshComponent("robot.fbx");
	mesh->SetMeshDataList();
	this->componentList.push_back(mesh);


}

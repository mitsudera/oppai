#include "Tree.h"
#include "MeshComponent.h"
#include "transformcomponent.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "GameEngine.h"
#include "renderer.h"

Tree::Tree(Scene* scene)
{
	this->pScene = scene;
}

Tree::~Tree()
{
}

void Tree::Init(void)
{

	GameObject::Init();

	this->name = "Tree";
	this->transformComponent->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));
	this->transformComponent->SetScale(XMFLOAT3(10.0f, 10.0f, 10.0f));

	

}

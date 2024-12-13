#include "SkySphere.h"
#include "transformcomponent.h"
#include "MeshComponent.h"
#include "renderer.h"
#include "SkyMaterial.h"
#include "Scene.h"
#include "GameEngine.h"


SkySphere::SkySphere(Scene* scene)
{
	pScene = scene;

}

SkySphere::~SkySphere()
{
}

void SkySphere::Init(void)
{
	GameObject::Init();

	this->layer = Layer::Sky;
	this->name = "SkySphere";
	this->transformComponent->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	this->transformComponent->SetScale(XMFLOAT3(1000.0f, 1000.0f, 1000.0f));


	this->LoadFbxFileMesh("SkySphere.fbx");
	


}

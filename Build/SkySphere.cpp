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
	this->transformComponent->SetScale(XMFLOAT3(100.0f, 100.0f, 100.0f));

	MeshComponent* mesh = new MeshComponent(this);
	mesh->Init();
	mesh->SetMeshComponent("skysphire.fbx");
	mesh->SetCullingMode(CULL_MODE_BACK);
	//SkyMaterial* material = new SkyMaterial(pScene->GetGameEngine()->GetAssetsManager());
	//material->LoadDiffuseTex("Sky_07.png");
	//material->SetDiffuse(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	//mesh->SetMaterial(material);
	
	this->componentList.push_back(mesh);



}

#include "MeshField.h"
#include "MeshFieldComponent.h"
#include "Material.h"
#include "LambartMaterial.h"
#include "GameEngine.h"
#include "AssetsManager.h"
#include "Scene.h"
MeshField::MeshField(Scene* scene)
{
	pScene = scene;
}

MeshField::~MeshField()
{
}

void MeshField::Init(void)
{
	GameObject::Init();
	this->layer =Layer::Field;
	MeshFieldComponent* meshField = AddComponent<MeshFieldComponent>();
	meshField->CreateVetexBuffer(XMFLOAT2(100.0f, 100.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	LambartMaterial* lam = new LambartMaterial(pScene->GetGameEngine()->GetAssetsManager());
	lam->LoadDiffuseTex("data/texture/gravel_diff.png");
	meshField->LoadMaterial(lam);
	meshField->SetHasShadow(FALSE);

}

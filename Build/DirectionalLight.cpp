#include "DirectionalLight.h"
#include "DirectionalLightComponent.h"
#include "transformcomponent.h"

DirectionalLight::DirectionalLight(Scene* scene)
{
	pScene = scene;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Init(void)
{
	GameObject::Init();

	DirectionalLightComponent* lcom = this->AddComponent<DirectionalLightComponent>();

	lcom->SetDiffuse(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	lcom->SetAmbient(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f));
	lcom->SetEnable(TRUE);

	this->transformComponent->RotWorldPitch(XM_PIDIV2);
	this->transformComponent->RotWorldRoll(XM_PIDIV4);

}

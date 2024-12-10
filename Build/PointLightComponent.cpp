#include "PointLightComponent.h"
#include "gameobject.h"
#include "LightManager.h"
#include "GameEngine.h"
PointLightComponent::PointLightComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	
}

PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::Init(void)
{
	Component::Init();
	index= pGameEngine->GetLightmanager()->AddLight(this);

}

void PointLightComponent::Uninit(void)
{
	Component::Uninit();
}

void PointLightComponent::Update(void)
{
	Component::Update();
}

void PointLightComponent::SetLight(POINT_LIGHT_PARAM pointLight)
{
	this->param = pointLight;
}

POINT_LIGHT_PARAM& PointLightComponent::GetLightParam(void)
{
	return this->param;
}



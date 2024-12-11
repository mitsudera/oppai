#include "DirectionalLightComponent.h"
#include "gameobject.h"
#include "LightManager.h"
#include "GameEngine.h"
DirectionalLightComponent::DirectionalLightComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}

void DirectionalLightComponent::Init(void)
{
	Component::Init();
	index = pGameEngine->GetLightmanager()->AddLight(this);
}

void DirectionalLightComponent::Uninit(void)
{
	Component::Uninit();
}

void DirectionalLightComponent::Update(void)
{
	Component::Update();
}

void DirectionalLightComponent::SetLight(DIREC_LIGHT_PARAM direcLight)
{
	this->param = direcLight;
	pGameEngine->GetLightmanager()->SetDirecLight(this, index);
}

DIREC_LIGHT_PARAM& DirectionalLightComponent::GetLightParam(void)
{
	return this->param;
}

XMFLOAT3 DirectionalLightComponent::GetDirection(void)
{
	return XMFLOAT3(param.m_Direction.x, param.m_Direction.y, param.m_Direction.z);
}



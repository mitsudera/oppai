#include "DirectionalLightComponent.h"
#include "gameobject.h"
#include "LightManager.h"
#include "GameEngine.h"
#include "transformcomponent.h"
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


	this->param.m_Direction = XMFLOAT4(this->GetTransFormComponent()->GetForward().x,this->GetTransFormComponent()->GetForward().y,this->GetTransFormComponent()->GetForward().z, 1.0f);

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

void DirectionalLightComponent::SetDiffuse(XMFLOAT4 dif)
{
	this->param.m_Diffuse = dif;
}

void DirectionalLightComponent::SetAmbient(XMFLOAT4 amb)
{
	this->param.m_Ambient = amb;

}

void DirectionalLightComponent::SetEnable(BOOL enable)
{
	this->param.m_Enable = enable;

}



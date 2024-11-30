#include "LightComponent.h"

LightComponent::LightComponent()
{
    attribute = Attribute::Light;
}

LightComponent::LightComponent(GameObject* gameObject)
{
    this->pGameObject = gameObject;
    attribute = Attribute::Light;

}

LightComponent::~LightComponent()
{
}

void LightComponent::Init(void)
{
}

void LightComponent::Uninit(void)
{
}

void LightComponent::Update(void)
{
    TransformComponent::Update();
}

LIGHT_PARAM LightComponent::GetLightParam(void)
{
    return this->lightParam;
}

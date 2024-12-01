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
    TransformComponent::Init();
}

void LightComponent::Uninit(void)
{
    TransformComponent::Uninit();
}

void LightComponent::Update(void)
{
    TransformComponent::Update();
}

LIGHT_PARAM LightComponent::GetLightParam(void)
{
    return this->lightParam;
}

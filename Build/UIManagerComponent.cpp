#include "UIManagerComponent.h"

UIManagerComponent::UIManagerComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

UIManagerComponent::~UIManagerComponent()
{
}

void UIManagerComponent::Init(void)
{
	Component::Init();
	attribute = Attribute::Manager;

}

void UIManagerComponent::Uninit(void)
{
	Component::Uninit();
}

void UIManagerComponent::Update(void)
{
	Component::Update();
}

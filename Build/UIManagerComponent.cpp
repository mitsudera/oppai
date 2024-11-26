#include "UIManagerComponent.h"

UIManagerComponent::UIManagerComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	attribute = Attribute::Manager;
}

UIManagerComponent::~UIManagerComponent()
{
}

void UIManagerComponent::Init(void)
{
}

void UIManagerComponent::Uninit(void)
{
}

void UIManagerComponent::Update(void)
{

}

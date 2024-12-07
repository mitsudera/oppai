#pragma once
#include "PrimitiveComponent.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "UIMaterial.h"
#include "SkyMaterial.h"

PrimitiveComponent::PrimitiveComponent()
{

}

PrimitiveComponent::PrimitiveComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

PrimitiveComponent::~PrimitiveComponent()
{
}

void PrimitiveComponent::Init(void)
{
	TransformComponent::Init();
	attribute = Attribute::Primitive;

	hasShadow = FALSE;
	drawShadow = FALSE;
}

void PrimitiveComponent::Update(void)
{
	TransformComponent::Update();

}

void PrimitiveComponent::Uninit(void)
{
	TransformComponent::Uninit();

}



void PrimitiveComponent::Draw(void)
{
	TransformComponent::Draw();
	if (drawShadow)
	{

	}
}

int PrimitiveComponent::GetMaterialIndex(void)
{
	return this->materialIndex;
}



BOOL PrimitiveComponent::GetHasShadow(void)
{
	return hasShadow;
}

void PrimitiveComponent::SetHasShadow(BOOL b)
{
	hasShadow = b;
}

BOOL PrimitiveComponent::GetDrawShadow(void)
{
	return drawShadow;
}

void PrimitiveComponent::SetDrawShadow(BOOL b)
{
	drawShadow = b;
}

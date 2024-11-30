#pragma once
#include "PrimitiveComponent.h"

PrimitiveComponent::PrimitiveComponent()
{
	attribute = Attribute::Primitive;

}

PrimitiveComponent::~PrimitiveComponent()
{
}

void PrimitiveComponent::Init(void)
{
	TransformComponent::Init();
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
	if (drawShadow)
	{

	}
}

Material* PrimitiveComponent::GetMaterial(void)
{
	return this->material;
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

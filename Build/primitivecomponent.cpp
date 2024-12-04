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

Material* PrimitiveComponent::GetMaterial(void)
{
	return this->material;
}

void PrimitiveComponent::SetMaterial(Material* m)
{
	this->material = m;
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
template<class T>
T* PrimitiveComponent::GetMaterial(void)
{
	T* m = dynamic_cast<T*>(this->material);
	return m;

}
// 具体的な型に対する明示的なインスタンス化
template LambartMaterial* PrimitiveComponent::GetMaterial<LambartMaterial>();
template PhongMaterial* PrimitiveComponent::GetMaterial<PhongMaterial>();
template UIMaterial* PrimitiveComponent::GetMaterial<UIMaterial>();
template SkyMaterial* PrimitiveComponent::GetMaterial<SkyMaterial>();

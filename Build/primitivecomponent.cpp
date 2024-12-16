#pragma once
#include "PrimitiveComponent.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "UIMaterial.h"
#include "SkyMaterial.h"
#include "GameEngine.h"
#include "Material.h"
#include "AssetsManager.h"

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
	Component::Init();
	this->pRenderer = pGameEngine->GetRenderer();
	this->pCBufferManager = pGameEngine->GetCBufferManager();
	this->pAssetsManager = pGameEngine->GetAssetsManager();
	attribute = Attribute::Primitive;

	hasShadow = FALSE;
	drawShadow = FALSE;
}

void PrimitiveComponent::Update(void)
{
	Component::Update();
}

void PrimitiveComponent::Uninit(void)
{
	Component::Uninit();
}



void PrimitiveComponent::Draw(void)
{
	Component::Draw();
	if (drawShadow)
	{

	}
}

void PrimitiveComponent::ShadowMapping(void)
{
	if (!hasShadow)
		return;
}

int PrimitiveComponent::GetMaterialIndex(void)
{
	return this->materialIndex;
}

void PrimitiveComponent::SetMaterialIndex(int index)
{
	this->materialIndex = index;
}

int PrimitiveComponent::LoadMaterial(Material* material)
{

	materialIndex = pGameEngine->GetAssetsManager()->LoadMaterial(material);
	return materialIndex;
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

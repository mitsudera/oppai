#pragma once
#include "MeshComponent.h"
#include "GameEngine.h"
#include "Renderer.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "MeshAnimData.h"
#include "DX11Texture.h"
#include "gameobject.h"
#include "GameScene.h"
#include "CBufferManager.h"
#include "Material.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "transformcomponent.h"

#define TEXTURE_PATH	"data/TEXTURE/"


MeshComponent::MeshComponent()
{
	MeshDataIndex = 0;
	cullMode = CULL_MODE::CULL_MODE_FRONT;
	this->alphaTest = FALSE;

}

MeshComponent::MeshComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
	attribute = Attribute::Primitive;

	MeshDataIndex = 0;
	cullMode = CULL_MODE::CULL_MODE_FRONT;
	this->alphaTest = FALSE;


}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::Init(void)
{
	PrimitiveComponent::Init();

	hasShadow = TRUE;


}

void MeshComponent::Update(void)
{
	PrimitiveComponent::Update();
}

void MeshComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();



}


void MeshComponent::Draw(void)
{
	PrimitiveComponent::Draw();





	if (this->alphaTest==TRUE)
	{
		this->pRenderer->SetAlphaTestEnable(TRUE);
	}
	
	MeshData* meshData = this->pGameEngine->GetAssetsManager()->GetMeshData(this->MeshDataIndex);


	this->pRenderer->SetCullingMode((CULL_MODE)cullMode);
	meshData->BufferSetVertex();
	meshData->BufferSetIndex();



	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);


	this->pGameEngine->GetAssetsManager()->GetMaterial(this->materialIndex)->SetBufferMaterial();

	this->pRenderer->GetDeviceContext()->DrawIndexed(meshData->GetIndexNum(), 0, 0);

	this->pRenderer->SetAlphaTestEnable(FALSE);

}

void MeshComponent::ShadowMapping(void)
{
	PrimitiveComponent::ShadowMapping();





	if (this->alphaTest == TRUE)
	{
		this->pRenderer->SetAlphaTestEnable(TRUE);
	}

	MeshData* meshData = this->pGameEngine->GetAssetsManager()->GetMeshData(this->MeshDataIndex);


	this->pRenderer->SetCullingMode((CULL_MODE)cullMode);
	meshData->BufferSetVertex();
	meshData->BufferSetIndex();



	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);


	this->pGameEngine->GetAssetsManager()->GetMaterial(this->shadowMaterialIndex)->SetBufferMaterial();

	this->pRenderer->GetDeviceContext()->DrawIndexed(meshData->GetIndexNum(), 0, 0);

	this->pRenderer->SetAlphaTestEnable(FALSE);

}


void MeshComponent::SetCullingMode(int cullMode)
{
	this->cullMode = cullMode;
}

int MeshComponent::GetCullingMode(void)
{
	return this->cullMode;
}

void MeshComponent::SetAlphaTest(BOOL enable)
{
	this->alphaTest = enable;
}

BOOL MeshComponent::GetAlphaTest(void)
{
	return this->alphaTest;
}


void MeshComponent::SetMeshDataIndex(int index)
{
	this->MeshDataIndex = index;



	MeshData* meshData = this->pGameEngine->GetAssetsManager()->GetMeshData(index);

	this->materialIndex = meshData->GetMaterialIndex();
	this->shadowMaterialIndex = meshData->GetShadowMaterialIndex();

	this->GetTransFormComponent()->SetPosition(meshData->GetPosOffset());
	this->GetTransFormComponent()->SetRotation(meshData->GetRotOffset());
	this->GetTransFormComponent()->SetScale(meshData->GetSclOffset());

	this->GetTransFormComponent()->UpdateMatrix();


}

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

	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();

	Renderer* renderer = pGameEngine->GetRenderer();



	if (this->alphaTest==TRUE)
	{
		renderer->SetAlphaTestEnable(TRUE);
	}
	
	MeshData* meshData = pGameEngine->GetAssetsManager()->GetMeshData(this->MeshDataIndex);


	renderer->SetCullingMode((CULL_MODE)cullMode);
	meshData->BufferSetVertex();
	meshData->BufferSetIndex();



	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	pGameEngine->GetCBufferManager()->SetWorldMtx(&world);


	pGameEngine->GetAssetsManager()->GetMaterial(this->materialIndex)->SetBufferMaterial();

	renderer->GetDeviceContext()->DrawIndexed(meshData->GetIndexNum(), 0, 0);

	renderer->SetAlphaTestEnable(FALSE);

}

void MeshComponent::ShadowMapping(void)
{
	PrimitiveComponent::ShadowMapping();


	Renderer* renderer = pGameEngine->GetRenderer();



	if (this->alphaTest == TRUE)
	{
		renderer->SetAlphaTestEnable(TRUE);
	}

	MeshData* meshData = pGameEngine->GetAssetsManager()->GetMeshData(this->MeshDataIndex);


	renderer->SetCullingMode((CULL_MODE)cullMode);
	meshData->BufferSetVertex();
	meshData->BufferSetIndex();



	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	pGameEngine->GetCBufferManager()->SetWorldMtx(&world);


	pGameEngine->GetAssetsManager()->GetMaterial(this->shadowMaterialIndex)->SetBufferMaterial();

	renderer->GetDeviceContext()->DrawIndexed(meshData->GetIndexNum(), 0, 0);

	renderer->SetAlphaTestEnable(FALSE);

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

	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();

	Renderer* renderer = pGameEngine->GetRenderer();

	MeshData* meshData = pGameEngine->GetAssetsManager()->GetMeshData(index);

	this->materialIndex = meshData->GetMaterialIndex();
	this->shadowMaterialIndex = meshData->GetShadowMaterialIndex();

	this->GetTransFormComponent()->SetPosition(meshData->GetPosOffset());
	this->GetTransFormComponent()->SetRotation(meshData->GetRotOffset());
	this->GetTransFormComponent()->SetScale(meshData->GetSclOffset());

	this->GetTransFormComponent()->UpdateMatrix();


}


//void MeshComponent::SetMeshDataList(void)
//{
//
//	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();
//
//	Renderer* renderer = pGameEngine->GetRenderer();
//
//	this->MeshDataIndex = pGameEngine->GetAssetsManager()->LoadMeshTree(this->meshFilePath);
//	MeshDataList* l = pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex);
//
//	MeshData* md=nullptr;
//
//
//	DX11_SUBSET* sub = nullptr;
//	int rootNum;
//
//	for (int i = 0; i < l->GetMeshDataNum(); i++)
//	{
//		md = &l->GetMeshData()[i];
//		sub = &md->GetSubset()[0];
//
//		if (sub != nullptr)
//		{
//			rootNum = i;
//			break;
//		}
//	}
//
//	if (md->GetSubset()->GetMaterial()->GetShaderSet()->GetShaderIndex() == ShaderSet::ShaderIndex::Lambart)
//	{
//		this->material = new LambartMaterial(dynamic_cast<LambartMaterial*>( pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()->GetSubset()->GetMaterial()));
//	}
//	else if (md->GetSubset()->GetMaterial()->GetShaderSet()->GetShaderIndex() == ShaderSet::ShaderIndex::Phong)
//	{
//		this->material = new PhongMaterial(dynamic_cast<PhongMaterial*>(pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()->GetSubset()->GetMaterial()));
//
//	}
//
//	this->CreateMeshArray(pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshDataNum());
//
//	for (int i = 0; i < this->meshNum; i++)
//	{
//		if (pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()[i].GetSubset() == nullptr)
//			continue;
//
//		mesh[i].SetMtx(pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()[i].GetWorldOffset());
//		mesh[i].SetMaterial(pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()[i].GetSubset()->GetMaterial());
//	}
//	this->SetBlendMtx();
//	isOriginalDiffuse = FALSE;
//}


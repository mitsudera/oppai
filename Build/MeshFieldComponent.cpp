#include "MeshFieldComponent.h"
#include "renderer.h"
#include "gameobject.h"
#include "transformcomponent.h"
#include "CBufferManager.h"
#include "AssetsManager.h"
#include "Material.h"
MeshFieldComponent::MeshFieldComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

MeshFieldComponent::~MeshFieldComponent()
{
}

void MeshFieldComponent::Init(void)
{
	PrimitiveComponent::Init();

}

void MeshFieldComponent::Update(void)
{
	PrimitiveComponent::Update();

}

void MeshFieldComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();

}

void MeshFieldComponent::Draw(void)
{
	PrimitiveComponent::Draw();
	if (this->alphaTest == TRUE)
	{
		this->pRenderer->SetAlphaTestEnable(TRUE);
	}



	this->pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);

	UINT stride = sizeof(VERTEX_3D);
	UINT worldOffset = 0;

	pRenderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &worldOffset);
	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	pCBufferManager->SetWorldMtx(&world);


	pAssetsManager->GetMaterial(this->materialIndex)->SetBufferMaterial();

	this->pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->pRenderer->GetDeviceContext()->Draw(4, 0);

	this->pRenderer->SetAlphaTestEnable(FALSE);


}

void MeshFieldComponent::CreateVetexBuffer(XMFLOAT2 scale,XMFLOAT2 uvScale,XMFLOAT4 color)
{

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);



	vertexArray[0].Position = { -scale.x,0.0f,scale.y };
	vertexArray[0].Normal = { 0.0f,1.0f,0.0 };
	vertexArray[0].Diffuse = color;
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { scale.x,0.0f,scale.y };
	vertexArray[1].Normal = { 0.0f,1.0f,0.0 };
	vertexArray[1].Diffuse = color;
	vertexArray[1].TexCoord = { uvScale.x,0.0f };

	vertexArray[2].Position = { -scale.x,0.0f ,-scale.y };
	vertexArray[2].Normal = { 0.0f,1.0f,0.0 };
	vertexArray[2].Diffuse = color;
	vertexArray[2].TexCoord = { 0.0f,uvScale.y };

	vertexArray[3].Position = { scale.x ,0.0f,-scale.y };
	vertexArray[3].Normal = { 0.0f,1.0f,0.0 };
	vertexArray[3].Diffuse = color;
	vertexArray[3].TexCoord = { uvScale.x,uvScale.y };

	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	this->pRenderer->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

	this->pRenderer->GetDeviceContext()->Unmap(this->vertexBuffer, 0);

}

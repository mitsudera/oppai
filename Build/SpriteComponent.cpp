#include "SpriteComponent.h"
#include "renderer.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "Scene.h"
#include "AssetsManager.h"
#include "DX11Texture.h"
#include "UIMaterial.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "AssetsManager.h"
#include "Scene.h"
#include "CBufferManager.h"

SpriteComponent::SpriteComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Init(void)
{
	PrimitiveComponent::Init();
	CreateVertexBuffer();
	Material* uiMaterial = new UIMaterial(pGameObject->GetScene()->GetGameEngine()->GetAssetsManager());
	this->materialIndex = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->LoadMaterial(uiMaterial);

}

void SpriteComponent::Update(void)
{
	PrimitiveComponent::Update();
}

void SpriteComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();
}

void SpriteComponent::Draw(void)
{

	PrimitiveComponent::Draw();

	Renderer* renderer = this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer();




	renderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->GetTexture(texIndex)->SetShaderResourcePS(0);


	renderer->GetDeviceContext()->Draw(4, 0);

}

void SpriteComponent::CreateVertexBuffer(void)
{

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);

}

void SpriteComponent::SetSpriteCenter(string texPath, XMFLOAT3 pos, float width, float height)
{
	this->texIndex = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->LoadTexture(texPath);

	XMFLOAT2 screenHW = pGameEngine->GetWindowSize();
	float z = pos.z;

	float w = (width / screenHW.x) * 2.0f;
	float h = (height / screenHW.y) * 2.0f;
	float l = (((pos.x / screenHW.x) - 0.5f) * 2.0f) - w * 0.5f;
	float t = (((pos.y / screenHW.y) - 0.5f) * 2.0f) - h * 0.5f;


	this->texIndex = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->LoadTexture(texPath);

	VERTEX_3D vertexArray[4];


	vertexArray[0].Position = { l,t + h,z };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { l + w,t + h,z };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { l,t ,z };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { l + w ,t,z };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };

	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);




}

void SpriteComponent::SetSpriteLeftTop(string texPath, XMFLOAT3 pos, float width, float height)
{


	XMFLOAT2 screenHW = pGameEngine->GetWindowSize();
	float z = pos.z;

	float w = (width / screenHW.x) * 2.0f;
	float h = (height / screenHW.y) * 2.0f;
	float l = ((pos.x / screenHW.x) - 0.5f) * 2.0f;
	float t = ((pos.y / screenHW.y) - 0.5f) * 2.0f;


	this->texIndex = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->LoadTexture(texPath);

	VERTEX_3D vertexArray[4];


	vertexArray[0].Position = { l,t+h,z };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { l + w,t+h,z };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { l,t ,z };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { l+w ,t,z };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };

	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);

}

#include "SpriteComponent.h"
#include "renderer.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "Scene.h"
#include "AssetsManager.h"
#include "DX11Texture.h"
SpriteComponent::SpriteComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	attribute = Attribute::Primitive;
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Init(void)
{
	CreateVertexBuffer();
}

void SpriteComponent::Update(void)
{
	PrimitiveComponent::Update();
}

void SpriteComponent::Uninit(void)
{
	
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
	renderer->SetWorldViewProjection2D();
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	renderer->SetMaterial(material);
	
	pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->GetTexture(texIndex)->SetShaderResource(0);

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

	VERTEX_3D vertexArray[4];

	vertexArray[0].Position = { (pos.x - (width * 0.5f)),(pos.y - (height * 0.5f)),0.0f };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { (pos.x + (width * 0.5f)),(pos.y - (height * 0.5f)),0.0f };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { (pos.x - (width * 0.5f)),(pos.y + (height * 0.5f)),0.0f };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { (pos.x + (width * 0.5f)),(pos.y + (height * 0.5f)),0.0f };
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


	this->texIndex = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->LoadTexture(texPath);

	VERTEX_3D vertexArray[4];

	vertexArray[0].Position = { 0.0f,0.0f,0.0f };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { (pos.x + width),0.0f,0.0f };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { 0.0f,(pos.y + height),0.0f };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { (pos.x + width),(pos.y + height),0.0f };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };

	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);

}

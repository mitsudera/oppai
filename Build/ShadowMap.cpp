#include "ShadowMap.h"
#include "GameEngine.h"
#include "AssetsManager.h"
#include "RenderTexture.h"
#include "renderer.h"
#include "CBufferManager.h"
#include "primitivecomponent.h"
#include "DirectionalLightComponent.h"
#include "LightManager.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "gameobject.h"
#include "ShadowShader.h"

ShadowMap::ShadowMap(GameEngine* gameEngine)
{
	pGameEngine = gameEngine;
	pAssetsManager = pGameEngine->GetAssetsManager();
	pRenderer = gameEngine->GetRenderer();
	pCBufferManager = gameEngine->GetCBufferManager();
	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(ShadowCbuffer);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	pRenderer->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->shadowBuffer);
	pCBufferManager->SetShadowBuffer(this->shadowBuffer);
	quality = ShadowQuality::Low;
	vhwn = 128.0f;
	vhwn = 1024.0f;
	variance = TRUE;

	vNear = 10.0f;
	vFar = 1000.0f;


}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::CreateShadowMap(ShadowQuality quality)
{

	switch (quality)
	{
	case ShadowQuality::Low:
		this->shadowNearTextureIndex = pAssetsManager->CreateRenderTexture(1024, 1024, "ShadowMapNear");
		this->shadowFarTextureIndex = pAssetsManager->CreateRenderTexture(1024, 1024, "ShadowMapFar");

		// ビューポートの設定
		ViewPortShadowMap.TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
		ViewPortShadowMap.TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
		ViewPortShadowMap.Width = 1024.0f;	// ビューポート領域の幅
		ViewPortShadowMap.Height = 1024.0f;	// ビューポート領域の高さ
		ViewPortShadowMap.MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
		ViewPortShadowMap.MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値

		break;

	case ShadowQuality::Middle:
		this->shadowNearTextureIndex = pAssetsManager->CreateRenderTexture(2048, 2048, "ShadowMapNear");
		this->shadowFarTextureIndex = pAssetsManager->CreateRenderTexture(2048, 2048, "ShadowMapFar");
		// ビューポートの設定
		ViewPortShadowMap.TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
		ViewPortShadowMap.TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
		ViewPortShadowMap.Width = 2048.0f;	// ビューポート領域の幅
		ViewPortShadowMap.Height = 2048.0f;	// ビューポート領域の高さ
		ViewPortShadowMap.MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
		ViewPortShadowMap.MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値


		break;

	case ShadowQuality::High:
		this->shadowNearTextureIndex = pAssetsManager->CreateRenderTexture(4096, 4096, "ShadowMapNear");
		this->shadowFarTextureIndex = pAssetsManager->CreateRenderTexture(4096, 4096, "ShadowMapFar");
		// ビューポートの設定
		ViewPortShadowMap.TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
		ViewPortShadowMap.TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
		ViewPortShadowMap.Width = 4096.0f;	// ビューポート領域の幅
		ViewPortShadowMap.Height = 4096.0f;	// ビューポート領域の高さ
		ViewPortShadowMap.MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
		ViewPortShadowMap.MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値


		break;

	}

}

void ShadowMap::ShadowMapping(void)
{
	this->shadowBufferStruct.mode = variance;

	XMMATRIX view;
	XMMATRIX proj;

	XMVECTOR mapPos;
	XMVECTOR at;
	XMVECTOR lDir;


	at = XMLoadFloat3(&pGameEngine->GetMainCamera()->GetWorldPos());

	lDir = XMLoadFloat3(&pGameEngine->GetLightmanager()->GetMainLight()->GetDirection());



	mapPos = at - lDir;

	mapPos = XMVector3Normalize(mapPos);

	mapPos = mapPos * (vFar / 2);

	view = XMMatrixLookAtLH(mapPos, at, yonevec());

	proj = XMMatrixOrthographicLH(vhwn, vhwn, vNear, vFar);
	pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->ClearRTV(XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f));
	pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->ClearDSV(1.0f);

	
	pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->SetRTV(RenderTexture::BindMode::BOTH, 0);

	pAssetsManager->GetShadowShader()->SetShaderRenderer();

	pRenderer->GetDeviceContext()->RSSetViewports(1,&ViewPortShadowMap);

	
	pCBufferManager->SetViewMtx(&view);
	pCBufferManager->SetProjectionMtx(&proj);

	this->shadowBufferStruct.wvp = XMMatrixTranspose(XMMatrixIdentity() * view * proj);

	for (GameObject* gameObj : pGameEngine->GetActiveScene()->GetGameObject())
	{

		gameObj->ShadowMapping();

	}
	
	pRenderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);


	pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->SetPSSRV(RenderTexture::BindMode::BOTH, 3);

	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(shadowBuffer, 0, NULL, &shadowBufferStruct, 0, 0);
}

int ShadowMap::GetNearShadowTexIndex(void)
{
	return this->shadowNearTextureIndex;
}

int ShadowMap::GetFarShadowTexIndex(void)
{
	return this->shadowFarTextureIndex;
}

void ShadowMap::SetFar(float vFar)
{
	this->vFar = vFar;
}

void ShadowMap::SetNear(float vNear)
{
	this->vNear = vNear;
}

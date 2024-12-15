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
#include "transformcomponent.h"
#include "GausianBlurShader.h"

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
	quality = ShadowQuality::High;
	vhwn = 64.0f;
	vhwf = 1024.0f;
	variance = TRUE;

	vNear = 0.0f;
	vFar = 50.0f;

}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::CreateShadowMap(ShadowQuality quality)
{
	int w = 64;
	int h = 64;

	switch (quality)
	{
	case ShadowQuality::Low:
		w = 1024;
		h = 1024;
		break;

	case ShadowQuality::Middle:
		w = 2048;
		h = 2048;
		break;

	case ShadowQuality::High:
		w = 4096;
		h = 4096;
		break;

	}

	this->shadowNearTextureIndex = pAssetsManager->CreateRenderTexture(w, h, "ShadowMapNear");
	this->shadowNearResultTextureIndex = pAssetsManager->CreateRenderTexture(w, h, "ShadowMapNearResult");
	this->shadowFarTextureIndex = pAssetsManager->CreateRenderTexture(w, h, "ShadowMapFar");

	// ビューポートの設定
	ViewPortShadowMap.TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	ViewPortShadowMap.TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	ViewPortShadowMap.Width = (float)w ;	// ビューポート領域の幅
	ViewPortShadowMap.Height = (float)h;	// ビューポート領域の高さ
	ViewPortShadowMap.MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	ViewPortShadowMap.MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値

	blurShader = new GausianBlurShader(pRenderer);
	blurShader->SetTexSize((float)w, (float)h);
	blurShader->Init();


}

void ShadowMap::ShadowMapping(void)
{
	if (!shadowBufferStruct.enable)
		return;


	this->shadowBufferStruct.mode = variance;

	XMMATRIX view;
	XMMATRIX proj;

	XMVECTOR mapPos;
	XMVECTOR camPos;
	XMVECTOR lDir;


	camPos = XMLoadFloat3(&pGameEngine->GetMainCamera()->GetWorldPos());

	lDir = pGameEngine->GetLightmanager()->GetMainLight()->GetTransFormComponent()->GetAxisZ();

	lDir = XMVector3Normalize(lDir);

	mapPos = camPos - (lDir * vFar * 0.5f);




	view = XMMatrixLookToLH(mapPos, lDir, zonevec());

	proj = XMMatrixOrthographicLH(vhwn, vhwn, vNear, vFar);
	//proj = XMMatrixPerspectiveFovLH(90.0f, 1.0f, vNear, vFar);
	pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->ClearRTV(XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f));
	pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->ClearDSV(1.0f);


	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	pRenderer->GetDeviceContext()->PSSetShaderResources(3, 1, nullSRV);
	pRenderer->GetDeviceContext()->PSSetShaderResources(4, 1, nullSRV);

	pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->SetRTV(RenderTexture::BindMode::BOTH, 0);


	pRenderer->GetDeviceContext()->RSSetViewports(1,&ViewPortShadowMap);

	
	pCBufferManager->SetViewMtx(&view);
	pCBufferManager->SetProjectionMtx(&proj);

	this->shadowBufferStruct.wvp = XMMatrixTranspose(XMMatrixIdentity() * view * proj);
	pAssetsManager->GetShadowShader()->SetShaderRenderer();


	for (GameObject* gameObj : pGameEngine->GetActiveScene()->GetGameObject())
	{

		gameObj->ShadowMapping();

	}
	
	pRenderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);



	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(shadowBuffer, 0, NULL, &shadowBufferStruct, 0, 0);

	if (variance)
	{
		//分散シャドウマップならブラーをかける


		blurShader->PostEffectDraw(pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->GetSRV(), pAssetsManager->GetRenderTexture(this->shadowNearResultTextureIndex)->GetRenderTargetView());
		pAssetsManager->GetRenderTexture(this->shadowNearResultTextureIndex)->SetPSSRV(RenderTexture::BindMode::Texture, 3);

	}
	else
	{
		pAssetsManager->GetRenderTexture(this->shadowNearTextureIndex)->SetPSSRV(RenderTexture::BindMode::DepthTexture, 3);

	}

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

void ShadowMap::SetEnable(BOOL enable)
{
	this->shadowBufferStruct.enable = enable;
}

void ShadowMap::SetVariance(BOOL enable)
{
	this->variance = enable;
	this->shadowBufferStruct.mode = variance;
}

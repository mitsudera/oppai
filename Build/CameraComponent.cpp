//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#pragma once
#include "CameraComponent.h"
#include "Renderer.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "Scene.h"
#include "CBufferManager.h"
#include "ShaderSet.h"
#include "primitivecomponent.h"
#include "Material.h"
#include "AssetsManager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// ビュー平面の視野角
#define	VIEW_ASPECT		(16.0f/ 9.0f)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(100000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// カメラの回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int				ViewPortType = TYPE_FULL_SCREEN;


CameraComponent::CameraComponent()
{
	for (int i = 0; i < Layer::LayerMax; i++)
	{
		layerCulling[i] = FALSE;
	}

}

CameraComponent::CameraComponent(GameObject* gameObject)
{

	pRenderer = gameObject->GetScene()->GetGameEngine()->GetRenderer();

	for (int i = 0; i < Layer::LayerMax; i++)
	{
		layerCulling[i] = FALSE;
	}

	// カメラバッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(CameraCBuffer);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	pRenderer->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->cameraBuffer);

	this->pGameObject = gameObject;
	this->pRenderer = gameObject->GetScene()->GetGameEngine()->GetRenderer();
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Init(void)
{
	TransformComponent::Init();

	this->attribute = Attribute::Camera;

	this->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f));

	this->at = { 0.0f, 0.0f, 0.0f };
	this->up = { 0.0f, 1.0f, 0.0f };

	// 視点と注視点の距離を計算
	float vx, vz;
	XMFLOAT3 location = { 0.0f,0.0f,10.0f };
	vx = location.x - this->at.x;
	vz = location.z - this->at.z;
	this->len = sqrtf(vx * vx + vz * vz);



	// 仮
	this->aspect = VIEW_ASPECT;	// アスペクト比 
	this->angle = VIEW_ANGLE;	// 視野角
	this->nearZ = VIEW_NEAR_Z;
	this->farZ = VIEW_FAR_Z;

	this->len = 50.0f;

	this->mode = MODE::WORLD;
	for (int i = 0; i < Layer::LayerMax; i++)
	{
		layerCulling[i] = FALSE;
	}

	this->clearColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// ビューポートタイプの初期化
	SetViewPort(VIEWPORT_TYPE::TYPE_FULL_SCREEN);
}

void CameraComponent::Update(void)
{
	TransformComponent::Update();



}

void CameraComponent::Draw(void)
{
	TransformComponent::Draw();
	Renderer* renderer = GetGameObject()->GetScene()->GetGameEngine()->GetRenderer();




	SetViewPort(ViewPortType);
	SetCamera();


	//SetCameraAT(XMFLOAT3(0, 0, 1));
}

void CameraComponent::Uninit(void)
{
	TransformComponent::Uninit();


}

void CameraComponent::Render(void)
{


	//描画ターゲットのクリア
	const float cc[4] = { 1.0f,1.0f,1.0f,1.0f };
	pRenderer->GetDeviceContext()->ClearRenderTargetView(this->renderTarget, cc);
	pRenderer->GetDeviceContext()->ClearDepthStencilView(this->depthTarget, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//ビューポートセット
	pRenderer->GetDeviceContext()->RSSetViewports(1, &vp);

	//描画ターゲットのセット
	pRenderer->GetDeviceContext()->OMSetRenderTargets(1, &this->renderTarget, this->depthTarget);

	pGameObject->GetScene()->GetGameEngine()->GetCBufferManager()->SetViewMtx(&this->mtxView);
	pGameObject->GetScene()->GetGameEngine()->GetCBufferManager()->SetCameraBuffer(cameraBuffer);
	pGameObject->GetScene()->GetGameEngine()->GetCBufferManager()->SetProjectionMtx(&this->mtxProj);

	for (int i = 0; i < ShaderSet::ShaderIndex::MAX; i++)
	{
		pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->SetShader((ShaderSet::ShaderIndex)i);


		//描画処理
		for (GameObject* gameObject : pGameObject->GetScene()->GetGameObject())
		{
			//カリングがtrueの場合は描画しない
			if (layerCulling[gameObject->GetLayer()] == TRUE)
				continue;

			for (Component* component : gameObject->GetComponentList())
			{
				if (component->GetAttribute() != Component::Attribute::Primitive)
					continue;

				PrimitiveComponent* primitiveComponent = static_cast<PrimitiveComponent*>(component);

				//現在セットしてるシェーダーを使っている場合描画
				if (primitiveComponent->GetMaterial()->GetShaderSet()->GetShaderIndex() != i)
					continue;

				primitiveComponent->Draw();

			}

		}

	}

}



void CameraComponent::SetMode(MODE mode)
{
	this->mode = mode;

}

XMMATRIX CameraComponent::GetView(void)
{
	return this->mtxView;
}

ID3D11RenderTargetView* CameraComponent::GetRenderTarget(void)
{
	return this->renderTarget;
}

void CameraComponent::SetRenderTarget(ID3D11RenderTargetView* rtv)
{
	this->renderTarget = rtv;
}

ID3D11DepthStencilView* CameraComponent::GetDepthStencilView(void)
{
	return this->depthTarget;
}

void CameraComponent::SetDepthStencilView(ID3D11DepthStencilView* dsv)
{
	this->depthTarget = dsv;
}

//=============================================================================
// カメラの更新
//=============================================================================
void CameraComponent::SetCamera(void)
{
	Renderer* renderer = GetGameObject()->GetScene()->GetGameEngine()->GetRenderer();
	// ビューマトリックス設定
	XMMATRIX mtxView;


	SetCameraAT(this->lookObject->GetTransFormComponent()->GetPosition());


	XMFLOAT3 pos = {.0f,.0f,.0f};




	switch (this->mode)
	{
	case MODE::TRACKING:

		XMMATRIX pMtx = this->lookObject->GetTransFormComponent()->GetWorldMtx();
		XMFLOAT3 lPos = this->pos;
		XMVECTOR wPos = XMLoadFloat3(&lPos);
		wPos = XMVector3Transform(wPos, pMtx);
		XMStoreFloat3(&lPos, wPos);
		pos = lPos;
		break;
	case MODE::TRACKING_SKY:

		
		XMStoreFloat3(&this->up, lookObject->GetTransFormComponent()->GetAxisY());


		XMFLOAT3 pPos = lookObject->GetTransFormComponent()->GetPosition();

		XMVECTOR pPosVec = XMLoadFloat3(&pPos);
		this->at = pPos;
		

		XMVECTOR lxVec = lookObject->GetTransFormComponent()->GetAxisX();
		XMVECTOR lyVec = lookObject->GetTransFormComponent()->GetAxisY();
		XMVECTOR lzVec = lookObject->GetTransFormComponent()->GetAxisZ();


		lxVec *= this->pos.x;
		lyVec *= this->pos.y;
		lzVec *= this->pos.z;

		XMVECTOR v = (lxVec+lyVec+lzVec);

		v = v + pPosVec;

		XMStoreFloat3(&pos, v);

		break;
	case MODE::WORLD:
		
		pos = this->pos;
		break;

	default:
		break;
	}

	XMFLOAT3 At = this->at;
	XMFLOAT3 Up = this->up;


	//mtxView = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&this->at), XMLoadFloat3(&this->up));

	XMVECTOR posv = XMLoadFloat3(&pos);

	mtxView = XMMatrixLookAtLH(posv, XMLoadFloat3(&At), XMLoadFloat3(&Up));
	this->mtxView = mtxView;

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);


	// プロジェクションマトリックス設定
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

}

//=============================================================================
// ビューポートの設定
//=============================================================================
void CameraComponent::SetViewPort(int m_type)
{

	ViewPortType = m_type;

	XMFLOAT2 screenSize = GetGameObject()->GetScene()->GetGameEngine()->GetWindowSize();


	// ビューポート設定
	switch (ViewPortType)
	{
	case TYPE_FULL_SCREEN:
		vp.Width = (FLOAT)screenSize.x;
		vp.Height = (FLOAT)screenSize.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_LEFT_HALF_SCREEN:
		vp.Width = (FLOAT)screenSize.x / 2;
		vp.Height = (FLOAT)screenSize.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_RIGHT_HALF_SCREEN:
		vp.Width = (FLOAT)screenSize.x / 2;
		vp.Height = (FLOAT)screenSize.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)screenSize.x / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_UP_HALF_SCREEN:
		vp.Width = (FLOAT)screenSize.x;
		vp.Height = (FLOAT)screenSize.y / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_DOWN_HALF_SCREEN:
		vp.Width = (FLOAT)screenSize.x;
		vp.Height = (FLOAT)screenSize.y / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)screenSize.x / 2;
		break;
	case TYPE_VIEWPORT:
		vp.Width = (FLOAT)screenSize.x * 0.6f;
		vp.Height = (FLOAT)screenSize.y * 0.6f + GetSystemMetrics(SM_CYCAPTION);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)screenSize.x * 0.2f;
		vp.TopLeftY = (FLOAT)screenSize.y * 0.1f;
		break;
	}
}




int CameraComponent::GetViewPortType(void)
{
	return ViewPortType;
}



// カメラの視点と注視点をセット
void CameraComponent::SetCameraAT(XMFLOAT3 pos)
{
	this->at = pos;

	
}

void CameraComponent::SetCameraUp(XMFLOAT3 up)
{
	this->up = up;
}

void CameraComponent::SetLookObject(GameObject* gameObject)
{
	this->lookObject = gameObject;
}


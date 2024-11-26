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


}

CameraComponent::CameraComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Init(void)
{
	PrimitiveComponent::Init();

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


	this->clearColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// ビューポートタイプの初期化
	//SetViewPort(g_ViewPortType);
}

void CameraComponent::Update(void)
{
	PrimitiveComponent::Update();



}

void CameraComponent::Draw(void)
{
	PrimitiveComponent::Draw();
	Renderer* renderer = GetGameObject()->GetScene()->GetGameEngine()->GetRenderer();

	const float cc[4] = { 1.0f,1.0f,1.0f,1.0f };


	renderer->GetDeviceContext()->ClearRenderTargetView(this->renderTarget, cc);



	SetViewPort(ViewPortType);
	SetCamera();


	//SetCameraAT(XMFLOAT3(0, 0, 1));
}

void CameraComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();


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
	renderer->SetViewMatrix(&mtxView);
	this->mtxView = mtxView;

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);


	// プロジェクションマトリックス設定
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	renderer->SetProjectionMatrix(&mtxProjection);

	renderer->SetShaderCamera(GetWorldPos());
}

//=============================================================================
// ビューポートの設定
//=============================================================================
void CameraComponent::SetViewPort(int m_type)
{
	Renderer* renderer = GetGameObject()->GetScene()->GetGameEngine()->GetRenderer();
	ID3D11DeviceContext *ImmediateContext = renderer->GetDeviceContext();
	D3D11_VIEWPORT vp;

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
	ImmediateContext->RSSetViewports(1, &vp);

	vpSize.x = vp.Width;
	vpSize.y = vp.Height;
	vpPos.x = vp.TopLeftX;
	vpPos.y = vp.TopLeftY;


}


void CameraComponent::GetViewPort(XMFLOAT2& size, XMFLOAT2& pos)
{
	size = vpSize;// カメラが表示するスクリーンのサイズをセット
	pos = vpPos;
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


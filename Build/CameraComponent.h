//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#pragma once
#include "TransformComponent.h"
#include "gameobject.h"

class Renderer;

//*****************************************************************************
// マクロ定義
//*****************************************************************************


enum VIEWPORT_TYPE
{
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_VIEWPORT,
	TYPE_NONE,

};


class CameraComponent : public TransformComponent
{
public:

	struct CameraCBuffer
	{
		XMFLOAT4 camPos;
	};

	CameraComponent();
	CameraComponent(GameObject* gameObject);
	~CameraComponent();


	void SetCameraAT(XMFLOAT3 pos);
	void SetCameraUp(XMFLOAT3 up);
	void SetLookObject(GameObject* gameObject);


	void SetViewPort(int m_type);
	void GetViewPort(XMFLOAT2& size, XMFLOAT2& pos);
	int GetViewPortType(void);

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Uninit(void) override;

	void Render(void);

	enum class MODE
	{
		TRACKING,
		TRACKING_SKY,
		
		WORLD,
	};

	void SetMode(MODE mode);

	XMMATRIX GetView(void);

	ID3D11RenderTargetView* GetRenderTarget(void);
	void SetRenderTarget(ID3D11RenderTargetView* rtv);
	ID3D11DepthStencilView* GetDepthStencilView(void);
	void SetDepthStencilView(ID3D11DepthStencilView* dsv);


private:
	void SetCamera(void);

	D3D11_VIEWPORT vp;

	XMMATRIX mtxView;
	XMMATRIX mtxProj;

	XMFLOAT3			at;				// カメラの注視点
	XMFLOAT3			up;				// カメラの上方向ベクトル

	float				angle;			// 視野角
	float				aspect;			// アスペクト比
	float				len;			// カメラの視点と注視点の距離
	float				nearZ;			// カメラのクリッピング最小値Z
	float				farZ;			// カメラのクリッピング最大値Z


	BOOL layerCulling[Layer::LayerMax];

	GameObject*			lookObject;
	Renderer* pRenderer;

	MODE				mode;
	ID3D11Buffer* cameraBuffer;

	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthTarget;
	XMFLOAT4 clearColor;	// 背景色

};
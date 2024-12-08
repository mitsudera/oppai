//=============================================================================
//
// �J�������� [camera.h]
// Author : 
//
//=============================================================================
#pragma once
#include "Component.h"
#include "gameobject.h"

class Renderer;

class MeshComponent;

//*****************************************************************************
// �}�N����`
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


class CameraComponent : public Component
{
public:

	enum class ClearMode
	{
		None,
		Color,
		SkySphere,
	};

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
	int GetViewPortType(void);

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	void Render(void);

	enum class MODE
	{
		TRACKING_PARENT,
		TRACKING_SKY,
		
		WORLD,
	};

	void SetMode(MODE mode);

	XMMATRIX GetView(void);

	ID3D11RenderTargetView* GetRenderTarget(void);
	void SetRenderTarget(ID3D11RenderTargetView* rtv);
	ID3D11DepthStencilView* GetDepthStencilView(void);
	void SetDepthStencilView(ID3D11DepthStencilView* dsv);

	void SetClearMode(ClearMode mode);
	void SetClearColor(XMFLOAT4 color);

	void SetSky(GameObject* sky);

private:
	void SetCamera(void);

	D3D11_VIEWPORT vp;

	XMMATRIX mtxView;
	XMMATRIX mtxProj;

	XMFLOAT3			at;				// �J�����̒����_
	XMFLOAT3			up;				// �J�����̏�����x�N�g��

	float				angle;			// ����p
	float				aspect;			// �A�X�y�N�g��
	float				len;			// �J�����̎��_�ƒ����_�̋���
	float				nearZ;			// �J�����̃N���b�s���O�ŏ��lZ
	float				farZ;			// �J�����̃N���b�s���O�ő�lZ


	BOOL layerCulling[Layer::LayerMax];

	GameObject*			lookObject;
	Renderer* pRenderer;

	MODE				mode;
	ID3D11Buffer* cameraBuffer;

	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthTarget;
	XMFLOAT4 clearColor;	// �w�i�F

	ClearMode clearMode;

	GameObject* sky;

};
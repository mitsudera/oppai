//=============================================================================
//
// �J�������� [camera.cpp]
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
#include "component.h"
#include "transformcomponent.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// �r���[���ʂ̎���p
#define	VIEW_ASPECT		(16.0f/ 9.0f)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(100000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// �J�����̉�]��

//*****************************************************************************
// �O���[�o���ϐ�
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

	gameObject->GetScene()->AddCamera(this);
	for (int i = 0; i < Layer::LayerMax; i++)
	{
		layerCulling[i] = FALSE;
	}

	// �J�����o�b�t�@����
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
	Component::Init();

	this->attribute = Attribute::Camera;

	this->mode = MODE::WORLD;


	this->at = { 0.0f, 0.0f, -1.0f };
	this->up = { 0.0f, 1.0f, 0.0f };


	clearMode = ClearMode::Color;

	// ��
	this->aspect = 16.0f / 9.0f;	// �A�X�y�N�g�� 
	this->angle = XMConvertToRadians(90.0f);	// ����p
	this->nearZ = 10.0f;
	this->farZ = 10000.0f;

	this->len = 50.0f;

	this->mode = MODE::WORLD;
	for (int i = 0; i < Layer::LayerMax; i++)
	{
		layerCulling[i] = FALSE;
	}

	this->clearColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	// �r���[�|�[�g�^�C�v�̏�����
	SetViewPort(VIEWPORT_TYPE::TYPE_FULL_SCREEN);
	this->mtxProj = XMMatrixPerspectiveFovLH(this->angle, this->aspect, this->nearZ, this->farZ);
}

void CameraComponent::Update(void)
{
	Component::Update();


}


void CameraComponent::Uninit(void)
{
	Component::Uninit();


}

void CameraComponent::Render(void)
{

	switch (this->mode)
	{
	case MODE::TRACKING_PARENT:


		this->at = this->pGameObject->GetParent()->GetTransFormComponent()->GetWorldPos();
		this->mtxView = XMMatrixLookAtLH(XMLoadFloat3(&this->GetWorldPos()), XMLoadFloat3(&this->at), XMLoadFloat3(&this->up));

		break;
	case MODE::TRACKING_SKY:


		this->mtxView = XMMatrixLookAtLH(XMLoadFloat3(&this->GetWorldPos()), XMLoadFloat3(&this->at), XMLoadFloat3(&this->up));

		break;
	case MODE::WORLD:

		this->mtxView = XMMatrixLookToLH(XMLoadFloat3(&this->GetWorldPos()), this->GetTransFormComponent()->GetAxisZ(), this->GetTransFormComponent()->GetAxisY());
		break;

	default:
		break;
	}



	//�r���[�|�[�g�Z�b�g
	pRenderer->GetDeviceContext()->RSSetViewports(1, &vp);

	//�`��^�[�Q�b�g�̃Z�b�g
	pRenderer->GetDeviceContext()->OMSetRenderTargets(1, &this->renderTarget, this->depthTarget);

	pGameObject->GetScene()->GetGameEngine()->GetCBufferManager()->SetViewMtx(&this->mtxView);

	pGameObject->GetScene()->GetGameEngine()->GetCBufferManager()->SetCameraBuffer(cameraBuffer);

	pGameObject->GetScene()->GetGameEngine()->GetCBufferManager()->SetProjectionMtx(&this->mtxProj);


	const float cc[4] = { clearColor.x,clearColor.y,clearColor.z,clearColor.w };

	//�`��^�[�Q�b�g�̃N���A
	switch (clearMode)
	{

	case ClearMode::None:


		break;
	case ClearMode::Color:



		pRenderer->GetDeviceContext()->ClearRenderTargetView(this->renderTarget, cc);
		pRenderer->GetDeviceContext()->ClearDepthStencilView(this->depthTarget, D3D11_CLEAR_DEPTH, 1.0f, 0);

		break;

	case ClearMode::SkySphere:


		//�X�J�C�X�t�B�A�Ŕw�i�N���A������ꍇ

		layerCulling[Layer::Sky] = TRUE;
		pRenderer->SetDepthEnable(FALSE);
		this->sky->GetTransFormComponent()->SetPosition(this->GetWorldPos());

		this->sky->GetTransFormComponent()->UpdateMatrix();

		for (int j = 0; j < ShaderSet::ShaderIndex::MAX; j++)
		{
			pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->SetShader((ShaderSet::ShaderIndex)j);


			sky->Draw((ShaderSet::ShaderIndex)j);




		}
		pRenderer->SetDepthEnable(TRUE);


	}





	//�V�F�[�_�[���ɕ`��
	for (int i = 0; i < ShaderSet::ShaderIndex::MAX; i++)
	{
		pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->SetShader((ShaderSet::ShaderIndex)i);
		//�`�揈��
		for (GameObject* gameObject : pGameObject->GetScene()->GetGameObject())
		{
			//���C���[�̃J�����O�`�F�b�N
			if (layerCulling[gameObject->GetLayer()])
				continue;

			gameObject->Draw((ShaderSet::ShaderIndex)i);
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

void CameraComponent::SetClearMode(ClearMode mode)
{
	this->clearMode = mode;
}

void CameraComponent::SetClearColor(XMFLOAT4 color)
{
	this->clearColor = color;
}

void CameraComponent::SetSky(GameObject* sky)
{
	this->sky = sky;
	clearMode = ClearMode::SkySphere;
}



//=============================================================================
// �r���[�|�[�g�̐ݒ�
//=============================================================================
void CameraComponent::SetViewPort(int m_type)
{

	ViewPortType = m_type;

	XMFLOAT2 screenSize = GetGameObject()->GetScene()->GetGameEngine()->GetWindowSize();


	// �r���[�|�[�g�ݒ�
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


void CameraComponent::SetLookObject(GameObject* gameObject)
{
	this->lookObject = gameObject;
}


//=============================================================================
//
// �����_�����O���� [Renderer.cpp]
// Author : 
//
//=============================================================================
#include "Renderer.h"
#include "CameraComponent.h"
#include "GameEngine.h"
//�f�o�b�O�p��ʃe�L�X�g�o�͂�L���ɂ���
#define DEBUG_DISP_TEXTOUT
//�V�F�[�_�[�f�o�b�O�ݒ��L���ɂ���
//#define DEBUG_SHADER

//*********************************************************
// �\����
//*********************************************************




Renderer::Renderer(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;


	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	m_D3DDevice = NULL;
	m_ImmediateContext = NULL;
	SwapChain = NULL;
	RenderTargetView = NULL;
	DepthStencilView = NULL;


	m_VertexShader = NULL;
	m_PixelShader = NULL;
	VertexLayout = NULL;


	//WorldBuffer = NULL;
	//ViewBuffer = NULL;
	//ProjectionBuffer = NULL;
	//MaterialBuffer = NULL;
	//LightBuffer = NULL;
	//FogBuffer = NULL;
	//FuchiBuffer = NULL;
	//CameraBuffer = NULL;

	DepthStateEnable;
	DepthStateDisable;

	BlendStateNone;
	BlendStateAlphaBlend;
	BlendStateAdd;
	BlendStateSubtract;
	BlendStateParam;


	RasterStateCullOff;
	RasterStateCullCW;
	RasterStateCullCCW;
	RasterStateFillSOLID;
	RasterStateFillWIRE;


	Material;
	Fog;

	Fuchi;

	// �t�F�[�h�Ɠ����F�ɂ��Ȃ��ƃ`�J�`�J����̂ō���
	ClearColor[0] = 0.0f;	// �w�i�F
	ClearColor[1] = 0.0f;	// �w�i�F
	ClearColor[2] = 0.0f;	// �w�i�F
	ClearColor[3] = 1.0f;	// �w�i�F


}

Renderer::~Renderer()
{

}


//=============================================================================
// ���e�s��̎擾
//=============================================================================
XMMATRIX Renderer::GetProjectionMatrix(void)
{
	return projection;
}

//=============================================================================
// ���e�s��̃Z�b�g
//=============================================================================
void Renderer::SetProjectionMatrix(XMMATRIX* ProjectionMatrix)
{
	projection = *ProjectionMatrix;
	projection = XMMatrixTranspose(projection);

	m_ProjectionBuffer->SetToBuffer(m_ImmediateContext, &projection);
	//GetDeviceContext()->UpdateSubresource(ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

//=============================================================================
// �r���[�s��̎擾
//=============================================================================
XMMATRIX Renderer::GetViewMatrix(void)
{
	return view;
}

//=============================================================================
// �r���[�s��̃Z�b�g
//=============================================================================
void Renderer::SetViewMatrix(XMMATRIX* ViewMatrix)
{
	view = *ViewMatrix;
	view = XMMatrixTranspose(view);

	m_ViewBuffer->SetToBuffer(GetDeviceContext(), &view);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ID3D11Device* Renderer::GetDevice( void )
{
	return m_D3DDevice;
}


ID3D11DeviceContext* Renderer::GetDeviceContext( void )
{
	return m_ImmediateContext;
}


void Renderer::SetDepthEnable( BOOL Enable )
{
	if( Enable )
		m_ImmediateContext->OMSetDepthStencilState( DepthStateEnable, NULL );
	else
		m_ImmediateContext->OMSetDepthStencilState( DepthStateDisable, NULL );

}


void Renderer::SetBlendState(BLEND_MODE bm)
{
	BlendStateParam = bm;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (BlendStateParam)
	{
	case BLEND_MODE_NONE:
		m_ImmediateContext->OMSetBlendState(BlendStateNone, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ALPHABLEND:
		m_ImmediateContext->OMSetBlendState(BlendStateAlphaBlend, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ADD:
		m_ImmediateContext->OMSetBlendState(BlendStateAdd, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_SUBTRACT:
		m_ImmediateContext->OMSetBlendState(BlendStateSubtract, blendFactor, 0xffffffff);
		break;
	}
}

void Renderer::SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		m_ImmediateContext->RSSetState(RasterStateCullOff);
		break;
	case CULL_MODE_FRONT:
		m_ImmediateContext->RSSetState(RasterStateCullCW);
		break;
	case CULL_MODE_BACK:
		m_ImmediateContext->RSSetState(RasterStateCullCCW);
		break;
	}
}

void Renderer::SetFillMode(FILL_MODE fm)
{
	switch (fm)
	{
	case FILL_MODE_SOLID:
		m_ImmediateContext->RSSetState(RasterStateFillSOLID);
		break;
	case FILL_MODE_WIREFRAME:
		m_ImmediateContext->RSSetState(RasterStateFillWIRE);
		break;
	}
}

void Renderer::SetAlphaTestEnable(BOOL flag)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	if (flag)
		blendDesc.AlphaToCoverageEnable = TRUE;
	else
		blendDesc.AlphaToCoverageEnable = FALSE;

	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	switch (BlendStateParam)
	{
	case BLEND_MODE_NONE:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ALPHABLEND:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ADD:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_SUBTRACT:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	if (blendState != NULL)
		blendState->Release();
}


void Renderer::SetWorldViewProjection2D( void )
{
	XMMATRIX world;
	world = XMMatrixTranspose(XMMatrixIdentity());
	m_WorldBuffer->SetToBuffer(m_ImmediateContext, &world);
	//GetDeviceContext()->UpdateSubresource(WorldBuffer, 0, NULL, &world, 0, 0);

	XMMATRIX view;
	view = XMMatrixTranspose(XMMatrixIdentity());
	m_ViewBuffer->SetToBuffer(m_ImmediateContext, &view);
	//GetDeviceContext()->UpdateSubresource(ViewBuffer, 0, NULL, &view, 0, 0);

	XMFLOAT2 screen = pGameEngine->GetWindowSize();

	XMMATRIX worldViewProjection;
	worldViewProjection = XMMatrixOrthographicOffCenterLH(0.0f, screen.x, screen.y, 0.0f, 0.0f, 1.0f);
	worldViewProjection = XMMatrixTranspose(worldViewProjection);

	m_ProjectionBuffer->SetToBuffer(m_ImmediateContext, &worldViewProjection);
	//GetDeviceContext()->UpdateSubresource(ProjectionBuffer, 0, NULL, &worldViewProjection, 0, 0);
}


void Renderer::SetWorldMatrix( XMMATRIX *WorldMatrix )
{
	XMMATRIX world;
	world = *WorldMatrix;
	world = XMMatrixTranspose(world);

	m_WorldBuffer->SetToBuffer(m_ImmediateContext,&world);
	//GetDeviceContext()->UpdateSubresource(WorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetMaterial( MATERIAL material )
{
	Material.Diffuse = material.Diffuse;
	Material.Ambient = material.Ambient;
	Material.Specular = material.Specular;
	Material.Emission = material.Emission;
	Material.Shininess = material.Shininess;
	Material.noDiffuseTex = material.noDiffuseTex;
	Material.noNormalTex = material.noNormalTex;
	Material.noArmTex = material.noArmTex;
	Material.phong = material.phong;

	m_MaterialBuffer->SetToBuffer(m_ImmediateContext, &Material);
	//GetDeviceContext()->UpdateSubresource( MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLightBuffer(void)
{
	m_LightBuffer->SetToBuffer(m_ImmediateContext,&m_CbLight);
}

//���C�g�g�p����H
void Renderer::SetLightEnable(BOOL useFlag){
	m_CbLight.m_Enable = useFlag;
	SetLightBuffer();
}

void Renderer::SetLight(int index,LIGHT_PARAM param)
{
	this->m_CbLight.m_lightParam[index] = param;
	this->SetLightBuffer();
}

//void Renderer::SetFogBuffer(void)
//{
//	m_FogBuffer->SetToBuffer(m_ImmediateContext, &Fog);
//	//GetDeviceContext()->UpdateSubresource(FogBuffer, 0, NULL, &Fog, 0, 0);
//}

//void Renderer::SetFogEnable(BOOL flag)
//{
//	// �t���O���X�V����
//	Fog.Enable = flag;
//
//	SetFogBuffer();
//}

//void Renderer::SetFog(FOG* pFog)
//{
//	Fog.Fog.x = pFog->FogStart;
//	Fog.Fog.y = pFog->FogEnd;
//	Fog.FogColor = pFog->FogColor;
//
//	SetFogBuffer();
//}

void Renderer::SetFuchi(int flag)
{
	Fuchi.fuchi = flag;
	m_FuchiBuffer->SetToBuffer(m_ImmediateContext,&Fuchi);
	GetDeviceContext()->UpdateSubresource(m_FuchiBuffer->GetBuffer(), 0, NULL, &Fuchi, 0, 0);
}


void Renderer::SetShaderCamera(XMFLOAT3 pos)
{
	XMFLOAT4 tmp = XMFLOAT4( pos.x, pos.y, pos.z, 0.0f );

	m_CameraBuffer->SetToBuffer(m_ImmediateContext, &tmp);
	//GetDeviceContext()->UpdateSubresource(CameraBuffer, 0, NULL, &tmp, 0, 0);
}

void Renderer::SetShadow(SHADOWMAP_CBUFFER* shadow)
{
	m_ShadowBuffer->SetToBuffer(m_ImmediateContext, shadow);
}

void Renderer::SetGausBuffer(void)
{
	GaussianCBuffer gaus;

	ZeroMemory(&gaus, sizeof(gaus));
	float total = 0;
	constexpr float disperision = 40.0f;
	for (int i = 0; i < 8; i++) {
		float pos = 1.0f + 2.0f * (float)i;
		gaus.weight[i] = expf(-0.5f * pos * pos / disperision);
		if (i == 0) {
			total += gaus.weight[i];
		}
		else {
			total += 2.0f * gaus.weight[i];
		}
	}

	for (int i = 0; i < 8; i++) {
		gaus.weight[i] /= total;
	}


	m_GausBuffer->SetToBuffer(m_ImmediateContext, &gaus);
}

ID3D11RenderTargetView* Renderer::GetBackBuffer(void)
{
	return this->RenderTargetView;
}


void Renderer::SetTessFacter(float facter)
{
	TessellationCBuffer tess;
	
	tess.TessellationFacter = facter;

	m_TessellationBuffer->SetToBuffer(m_ImmediateContext, &tess);
}




//=============================================================================
// ����������
//=============================================================================
HRESULT Renderer::InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;
	XMFLOAT2 screen = pGameEngine->GetWindowSize();

	// �f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g����
	DWORD deviceFlags = 0;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = (UINT)screen.x;
	sd.BufferDesc.Height = (UINT)screen.y;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = bWindow;

	// �����o�͗p�ݒ�
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
	deviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;




	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&SwapChain,
		&m_D3DDevice,
		&FeatureLevel,
		&m_ImmediateContext);
	if (FAILED(hr))
		return hr;

	// �����o�͗p�ݒ�
	hr = SwapChain->ResizeBuffers(0, (UINT)screen.x, (UINT)screen.y, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE); // N.B. the GDI compatible flag
	if (FAILED(hr))
		return hr;

	// �����_�[�^�[�Q�b�g�r���[�����A�ݒ�
	ID3D11Texture2D* pBackBuffer = NULL;
	SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	m_D3DDevice->CreateRenderTargetView( pBackBuffer, NULL, &RenderTargetView );
	pBackBuffer->Release();



	//�X�e���V���p�e�N�X�`���[�쐬
	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof(td) );
	td.Width			= sd.BufferDesc.Width;
	td.Height			= sd.BufferDesc.Height;
	td.MipLevels		= 1;
	td.ArraySize		= 1;
	td.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc		= sd.SampleDesc;
	td.Usage			= D3D11_USAGE_DEFAULT;
	td.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags	= 0;
    td.MiscFlags		= 0;
	m_D3DDevice->CreateTexture2D( &td, NULL, &depthTexture );

	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format			= td.Format;
	dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags			= 0;
	m_D3DDevice->CreateDepthStencilView( depthTexture, &dsvd, &DepthStencilView );


	m_ImmediateContext->OMSetRenderTargets( 1, &RenderTargetView, DepthStencilView );


	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)screen.x;
	vp.Height = (FLOAT)screen.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports( 1, &vp );



	// ���X�^���C�U�X�e�[�g�쐬
	D3D11_RASTERIZER_DESC rd; 
	ZeroMemory( &rd, sizeof( rd ) );
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE; 
	rd.DepthClipEnable = TRUE; 
	rd.MultisampleEnable = FALSE; 
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateCullOff);
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateFillSOLID);

	rd.CullMode = D3D11_CULL_FRONT;
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateCullCW);

	rd.CullMode = D3D11_CULL_BACK;
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateCullCCW);

	rd.FillMode = D3D11_FILL_WIREFRAME;
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateFillWIRE);

	// �J�����O���[�h�ݒ�iCCW�j
	SetCullingMode(CULL_MODE_BACK);
	// �t�B�����[�h�ݒ�
	SetFillMode(FILL_MODE_SOLID);



	// �u�����h�X�e�[�g�̍쐬
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState( &blendDesc, &BlendStateAlphaBlend );

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateNone);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateSubtract);

	// �A���t�@�u�����h�ݒ�
	SetBlendState(BLEND_MODE_ALPHABLEND);




	// �[�x�X�e���V���X�e�[�g�쐬
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &DepthStateEnable );//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	m_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &DepthStateDisable );//�[�x�����X�e�[�g

	// �[�x�X�e���V���X�e�[�g�ݒ�
	SetDepthEnable(TRUE);



	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_D3DDevice->CreateSamplerState( &samplerDesc, &samplerState );

	m_ImmediateContext->PSSetSamplers( 0, 1, &samplerState );

	// Border�T���v���[�X�e�[�g�ݒ�
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;

	samplerState = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);
	m_ImmediateContext->PSSetSamplers(1, 1, &samplerState);



	
	// ���_�V�F�[�_�R���p�C���E����
 
	ID3DBlob* pVSBlob = this->CreateVSFile("shaders/shader.hlsl","VSmain", & m_VertexShader);

	// ���̓��C�A�E�g����
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE( layout );

	m_D3DDevice->CreateInputLayout( layout,
		numElements,
		(pVSBlob)->GetBufferPointer(),
		(pVSBlob)->GetBufferSize(),
		&VertexLayout );

	(pVSBlob)->Release();

	// �s�N�Z���V�F�[�_�R���p�C���E����
	this->CreatePSFile("shaders/shader.hlsl","PSmain", &m_PixelShader);


	//this->CreateVSFile("shaders/particleVP.hlsl" , "VSmain", &m_ParticleVS);
	//this->CreatePSFile("shaders/particleVP.hlsl", "PSmain", &m_ParticlePS);

	this->CreateVSFile("shaders/UIShader.hlsl", "VSmain", &m_UIVS);
	this->CreatePSFile("shaders/UIShader.hlsl", "PSmain", &m_UIPS);

	this->CreateHSFile("shaders/TerrainShader.hlsl", "HS_Main", &m_TerrainHS);
	this->CreateDSFile("shaders/TerrainShader.hlsl", "DS_Main", &m_TerrainDS);
	this->CreateVSFile("shaders/TerrainShader.hlsl", "VS_Main", &m_TerrainVS);
	this->CreatePSFile("shaders/TerrainShader.hlsl", "PS_Main", &m_TerrainPS);


	// �萔�o�b�t�@����
	InitConstantBuffers();

	SetShaderBuffersMode(DEFAULT_SMode);


	// ���̓��C�A�E�g�ݒ�
	m_ImmediateContext->IASetInputLayout( VertexLayout );

	// �V�F�[�_�ݒ�
	SetShaderFile(DEFAULT_SMode);
	//m_ImmediateContext->VSSetShader( m_VertexShader, NULL, 0 );
	//m_ImmediateContext->PSSetShader( m_PixelShader, NULL, 0 );


	{//���C�g������
		ZeroMemory(&m_CbLight, sizeof(LIGHT_CBUFFER));
		SetLightBuffer();
	}

	{//�}�e���A��������
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);
	}

	//�K�E�V�A���u���[�p�o�b�t�@�Z�b�g
	SetGausBuffer();

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void Renderer::UninitRenderer(void)
{
	// �I�u�W�F�N�g���
	if (DepthStateEnable)		DepthStateEnable->Release();
	if (DepthStateDisable)	DepthStateDisable->Release();
	if (BlendStateNone)		BlendStateNone->Release();
	if (BlendStateAlphaBlend)	BlendStateAlphaBlend->Release();
	if (BlendStateAdd)		BlendStateAdd->Release();
	if (BlendStateSubtract)	BlendStateSubtract->Release();
	if (RasterStateCullOff)	RasterStateCullOff->Release();
	if (RasterStateCullCW)	RasterStateCullCW->Release();
	if (RasterStateCullCCW)	RasterStateCullCCW->Release();
	if (RasterStateFillSOLID) RasterStateFillSOLID->Release();
	if (RasterStateFillWIRE) RasterStateFillWIRE->Release();

	delete m_WorldBuffer;
	delete m_ViewBuffer;
	delete m_ProjectionBuffer;
	delete m_MaterialBuffer;
	delete m_LightBuffer;
	delete m_FogBuffer;
	delete m_FuchiBuffer;
	delete m_CameraBuffer;
	delete m_ShadowBuffer;
	delete m_GausBuffer;

	if (VertexLayout)			VertexLayout->Release();
	if (m_VertexShader)			m_VertexShader->Release();
	if (m_PixelShader)			m_PixelShader->Release();
	if (m_UIVS)					m_UIVS->Release();
	if (m_UIPS)					m_UIPS->Release();

	if (m_ImmediateContext)		m_ImmediateContext->ClearState();
	if (RenderTargetView)		RenderTargetView->Release();
	if (SwapChain)				SwapChain->Release();
	if (m_ImmediateContext)		m_ImmediateContext->Release();
	if (m_D3DDevice)			m_D3DDevice->Release();
}



//=============================================================================
// �o�b�N�o�b�t�@�N���A
//=============================================================================
void Renderer::Clear(void)
{
	// �o�b�N�o�b�t�@�N���A
	m_ImmediateContext->ClearRenderTargetView( RenderTargetView, ClearColor );
	m_ImmediateContext->ClearDepthStencilView( DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void Renderer::SetClearColor(float* color4)
{
	ClearColor[0] = color4[0];
	ClearColor[1] = color4[1];
	ClearColor[2] = color4[2];
	ClearColor[3] = color4[3];
}


//=============================================================================
// �v���[���g
//=============================================================================
void Renderer::Present(void)
{
	SwapChain->Present( 0, 0 );
}



//=============================================================================
// �f�o�b�O�p�e�L�X�g�o��
//=============================================================================
void Renderer::DebugTextOut(char* text, int x, int y)
{
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	HRESULT hr;

	//�o�b�N�o�b�t�@����T�[�t�F�X���擾����
	IDXGISurface1* pBackSurface = NULL;
	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);

	if (SUCCEEDED(hr))
	{
		//�擾�����T�[�t�F�X����f�o�C�X�R���e�L�X�g���擾����
		HDC hdc;
		hr = pBackSurface->GetDC(FALSE, &hdc);

		if (SUCCEEDED(hr))
		{
			//�����F�𔒂ɕύX
			SetTextColor(hdc, RGB(255, 255, 255));
			//�w�i�𓧖��ɕύX
			SetBkMode(hdc, TRANSPARENT);

			XMFLOAT2 screen = pGameEngine->GetWindowSize();

			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = (LONG)screen.x;
			rect.bottom = (LONG)screen.y;

			//�e�L�X�g�o��
			DrawText(hdc, text, (int)strlen(text), &rect, DT_LEFT);

			//�f�o�C�X�R���e�L�X�g���������
			pBackSurface->ReleaseDC(NULL);
		}
		//�T�[�t�F�X���������
		pBackSurface->Release();

		//�����_�����O�^�[�Q�b�g�����Z�b�g�����̂ŃZ�b�g���Ȃ���
		m_ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
	}
#endif
}




IDXGISwapChain* Renderer::GetSwapChain(void)
{
	return SwapChain;
}



//Create Vertex Shader �t�@�C��
ID3DBlob* Renderer::CreateVSFile(const char* shaderName, char* fName, ID3D11VertexShader** VS){

	// ���_�V�F�[�_�R���p�C���E����
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pVSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) && defined(DEBUG_SHADER)
	shFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DX11CompileFromFile(shaderName, NULL, NULL, fName, "vs_4_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	}

	m_D3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, VS);



	return pVSBlob;
}

//Create P Shader �t�@�C��
void Renderer::CreatePSFile(char* shaderName, char* fName, ID3D11PixelShader** PS) {

	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pPSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr = D3DX11CompileFromFile(shaderName, NULL, NULL, fName, "ps_4_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}

	m_D3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, PS);
	//���
	pPSBlob->Release();

}
//Create Compute Shader �t�@�C��
void Renderer::CreateCSFile(char* shaderName, char* fName, ID3D11ComputeShader** CS) {

	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* csBlob = NULL;

	hr = D3DX11CompileFromFile(shaderName, 0, 0, fName, "cs_5_0", 0, 0, 0, &csBlob, &pErrorBlob, 0);
	if (FAILED(hr)){
		if (pErrorBlob){
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
	}
	GetDevice()->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), NULL, CS);
	//���
	csBlob->Release();
}

void Renderer::CreateHSFile(char* shaderName, char* fName, ID3D11HullShader** HS)
{

	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* hsBlob = NULL;

	hr = D3DX11CompileFromFile(shaderName, 0, 0, fName, "hs_5_0", 0, 0, 0, &hsBlob, &pErrorBlob, 0);
	if (FAILED(hr)) {
		if (pErrorBlob) {
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			string c=(char*)pErrorBlob->GetBufferPointer();
			pErrorBlob->Release();
		}
	}
	GetDevice()->CreateHullShader(hsBlob->GetBufferPointer(), hsBlob->GetBufferSize(), NULL, HS);
	//���
	hsBlob->Release();

}

void Renderer::CreateDSFile(char* shaderName, char* fName, ID3D11DomainShader** DS)
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* dsBlob = NULL;

	hr = D3DX11CompileFromFile(shaderName, 0, 0, fName, "ds_5_0", 0, 0, 0, &dsBlob, &pErrorBlob, 0);
	if (FAILED(hr)) {
		if (pErrorBlob) {
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			string c = (char*)pErrorBlob->GetBufferPointer();

			pErrorBlob->Release();
		}
	}
	GetDevice()->CreateDomainShader(dsBlob->GetBufferPointer(), dsBlob->GetBufferSize(), NULL, DS);
	//���
	dsBlob->Release();

}

void Renderer::CreateUAV(ID3D11Buffer* pBF, ID3D11UnorderedAccessView* pUav, UINT BFnumElements)
{
	// UAV���쐬
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = BFnumElements;

	HRESULT hr = GetDevice()->CreateUnorderedAccessView(pBF, &uavDesc, &pUav);
}

void Renderer::SetShaderRender(Shader_Mode mode) {

	this->SetShaderBuffersMode(mode);
	this->SetShaderFile(mode);


}

//�o�b�t�@�̏���������
void Renderer::InitConstantBuffers(void) {

	m_WorldBuffer = new Buffer<XMMATRIX>(GetDevice());
	m_ViewBuffer = new Buffer<XMMATRIX>(GetDevice());
	m_ProjectionBuffer = new Buffer<XMMATRIX>(GetDevice());
	m_MaterialBuffer = new Buffer<MATERIAL_CBUFFER>(GetDevice());
	m_LightBuffer = new Buffer<LIGHT_CBUFFER>(GetDevice());
	m_FogBuffer = new Buffer<FOG_CBUFFER>(GetDevice());
	m_FuchiBuffer = new Buffer<FUCHI>(GetDevice());
	m_CameraBuffer = new Buffer<XMFLOAT4>(GetDevice());
	m_ShadowBuffer = new Buffer<SHADOWMAP_CBUFFER>(GetDevice());
	m_GausBuffer = new Buffer<GaussianCBuffer>(GetDevice());
	m_TessellationBuffer = new Buffer<TessellationCBuffer>(GetDevice());

	
}

void Renderer::SetShaderBuffersMode(Shader_Mode bfMode) {

	// null �o�b�t�@�̔z����쐬
	static const UINT MaxSlots = D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;
	ID3D11Buffer* nullBuffers[MaxSlots] = { nullptr };

	// ���_�V�F�[�_�[�̂��ׂẴX���b�g�� null �ɐݒ�
	GetDeviceContext()->VSSetConstantBuffers(0, MaxSlots, nullBuffers);
	// �s�N�Z���V�F�[�_�[�̂��ׂẴX���b�g�� null �ɐݒ�
	GetDeviceContext()->PSSetConstantBuffers(0, MaxSlots, nullBuffers);


	switch (bfMode) {
	case DEFAULT_SMode:
	case UI_SMode:
	{//�o�b�t�@�Đݒ�
		//���[���h�}�g���N�X
		m_WorldBuffer->SetVS(m_ImmediateContext, 0);
		m_WorldBuffer->SetPS(m_ImmediateContext, 0);

		//�r���[�}�g���N�X
		m_ViewBuffer->SetVS(m_ImmediateContext, 1);
		m_ViewBuffer->SetPS(m_ImmediateContext, 1);

		//�v���W�F�N�V�����}�g���N�X
		m_ProjectionBuffer->SetVS(m_ImmediateContext, 2);
		m_ProjectionBuffer->SetPS(m_ImmediateContext, 2);

		//�}�e���A�����
		m_MaterialBuffer->SetVS(m_ImmediateContext, 3);
		m_MaterialBuffer->SetPS(m_ImmediateContext, 3);

		//���C�g���
		m_LightBuffer->SetVS(m_ImmediateContext, 4);
		m_LightBuffer->SetPS(m_ImmediateContext, 4);

		//�t�H�O���
		m_FogBuffer->SetVS(m_ImmediateContext, 5);
		m_FogBuffer->SetPS(m_ImmediateContext, 5);

		//�����
		m_FuchiBuffer->SetVS(m_ImmediateContext, 6);
		m_FuchiBuffer->SetPS(m_ImmediateContext, 6);

		//�J����
		m_CameraBuffer->SetVS(m_ImmediateContext, 7);
		m_CameraBuffer->SetPS(m_ImmediateContext, 7);

		//�e
		m_ShadowBuffer->SetVS(m_ImmediateContext, 8);
		m_ShadowBuffer->SetPS(m_ImmediateContext, 8);
		//�e
		m_GausBuffer->SetVS(m_ImmediateContext, 9);
		m_GausBuffer->SetPS(m_ImmediateContext, 9);

	}
	break;
	case PARTICAL_SMode:
	{
		//���[���h�}�g���N�X
		m_WorldBuffer->SetVS(m_ImmediateContext, 0);
		m_WorldBuffer->SetPS(m_ImmediateContext, 0);

		//�r���[�}�g���N�X
		m_ViewBuffer->SetVS(m_ImmediateContext, 1);
		m_ViewBuffer->SetPS(m_ImmediateContext, 1);

		//�v���W�F�N�V�����}�g���N�X
		m_ProjectionBuffer->SetVS(m_ImmediateContext, 2);
		m_ProjectionBuffer->SetPS(m_ImmediateContext, 2);

		////�G�~�b�^�[�o�b�t�@
		//m_EmitterParamBF->SetVS(m_ImmediateContext, 3);
		//m_EmitterParamBF->SetPS(m_ImmediateContext, 3);

		//�}�e���A���o�b�t�@
		m_MaterialBuffer->SetVS(m_ImmediateContext, 4);
		m_MaterialBuffer->SetPS(m_ImmediateContext, 4);
	}
	break;
	case TERRAIN_SMode:
	{
		//���[���h�}�g���N�X
		m_WorldBuffer->SetVS(m_ImmediateContext, 0);
		m_WorldBuffer->SetPS(m_ImmediateContext, 0);

		//�r���[�}�g���N�X
		m_ViewBuffer->SetVS(m_ImmediateContext, 1);
		m_ViewBuffer->SetPS(m_ImmediateContext, 1);

		//�v���W�F�N�V�����}�g���N�X
		m_ProjectionBuffer->SetVS(m_ImmediateContext, 2);
		m_ProjectionBuffer->SetPS(m_ImmediateContext, 2);

		//�}�e���A�����
		m_MaterialBuffer->SetVS(m_ImmediateContext, 3);
		m_MaterialBuffer->SetPS(m_ImmediateContext, 3);

		//���C�g���
		m_LightBuffer->SetVS(m_ImmediateContext, 4);
		m_LightBuffer->SetPS(m_ImmediateContext, 4);

		//�t�H�O���
		m_FogBuffer->SetVS(m_ImmediateContext, 5);
		m_FogBuffer->SetPS(m_ImmediateContext, 5);

		//�����
		m_FuchiBuffer->SetVS(m_ImmediateContext, 6);
		m_FuchiBuffer->SetPS(m_ImmediateContext, 6);

		//�J����
		m_CameraBuffer->SetVS(m_ImmediateContext, 7);
		m_CameraBuffer->SetPS(m_ImmediateContext, 7);

		//�e
		m_ShadowBuffer->SetVS(m_ImmediateContext, 8);
		m_ShadowBuffer->SetPS(m_ImmediateContext, 8);
		//�e
		m_GausBuffer->SetVS(m_ImmediateContext, 9);
		m_GausBuffer->SetPS(m_ImmediateContext, 9);
		//Tessellation
		m_TessellationBuffer->SetVS(m_ImmediateContext, 10);
		m_TessellationBuffer->SetPS(m_ImmediateContext, 10);
		m_TessellationBuffer->SetHS(m_ImmediateContext, 10);
		m_TessellationBuffer->SetDS(m_ImmediateContext, 10);




	}
	default:
	{

	}
	break;

	}
}

void Renderer::SetShaderFile(Shader_Mode bfMode) {

	// ���_�V�F�[�_�[�� null �ɐݒ�
	GetDeviceContext()->VSSetShader(nullptr, NULL, 0);
	// �s�N�Z���V�F�[�_�[�� null �ɐݒ�
	GetDeviceContext()->PSSetShader(nullptr, NULL, 0);
	// �R���s���[�g�V�F�[�_�[�� null �ɐݒ�
	GetDeviceContext()->CSSetShader(nullptr, NULL, 0);

	// �V�F�[�_�ݒ�
	switch (bfMode)
	{
	case DEFAULT_SMode:
	{
		m_ImmediateContext->VSSetShader(m_VertexShader, NULL, 0);
		m_ImmediateContext->PSSetShader(m_PixelShader, NULL, 0);
	}
	break;
	case PARTICAL_SMode:
	{
		//m_ImmediateContext->VSSetShader(m_ParticleVS, NULL, 0);
		//m_ImmediateContext->PSSetShader(m_ParticlePS, NULL, 0);
		//m_ImmediateContext->CSsSetShader(m_ParticleCS, NULL, 0);
	}
	break;
	case UI_SMode:
	{
		m_ImmediateContext->VSSetShader(m_UIVS, NULL, 0);
		m_ImmediateContext->PSSetShader(m_UIPS, NULL, 0);
	}
	break;
	case TERRAIN_SMode:
	{
		m_ImmediateContext->HSSetShader(m_TerrainHS, NULL, 0);
		m_ImmediateContext->DSSetShader(m_TerrainDS, NULL, 0);
		m_ImmediateContext->VSSetShader(m_TerrainVS, NULL, 0);
		m_ImmediateContext->PSSetShader(m_TerrainPS, NULL, 0);
	}
	default:
	{

	}
	break;

	}
}

void Renderer::DrawStringText(string text, float fontSize, XMFLOAT4 color, XMFLOAT2 pos, XMFLOAT2 size, TEXT_ANCHOR anchor, string font)
{
	HRESULT hr;

	//�o�b�N�o�b�t�@����T�[�t�F�X���擾����
	IDXGISurface1* pBackSurface = NULL;
	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);

	if (SUCCEEDED(hr))
	{
		//�擾�����T�[�t�F�X����f�o�C�X�R���e�L�X�g���擾����
		HDC hdc;
		hr = pBackSurface->GetDC(FALSE, &hdc);

		if (SUCCEEDED(hr))
		{
			//�����F��ύX
			SetTextColor(hdc, RGB(color.x * 255, color.y * 255, color.z * 255));

			//�w�i�𓧖��ɕύX
			SetBkMode(hdc, TRANSPARENT);
			XMFLOAT2 vpSize;
			XMFLOAT2 vpPos;
			CameraComponent* camComp = pGameEngine->GetMainCamera();
			if (camComp != nullptr)
			{
				camComp->GetViewPort(vpSize, vpPos);


				XMFLOAT2 resolution = pGameEngine->GetWindowSize();

				XMFLOAT2 absoluteSize, absolutePos;
				absoluteSize.x = vpSize.x / resolution.x;
				absoluteSize.y = vpSize.y / resolution.y;
				absolutePos.x = vpPos.x / resolution.x;
				absolutePos.y = vpPos.y / resolution.y;


				//pos.x *= absolutePos.x;
				//pos.y *= absolutePos.y;
				//size.x *= absoluteSize.x;
				//size.y *= absoluteSize.y;

				RECT screenRect;
				screenRect.left = (LONG)(pos.x - size.x / 2.0f) * absoluteSize.x + vpPos.x;
				screenRect.top = (LONG)(pos.y - size.y / 2.0f) * absoluteSize.y + vpPos.y;
				screenRect.right = (LONG)(pos.x + size.x / 2.0f) * absoluteSize.x + vpPos.x;
				screenRect.bottom = (LONG)(pos.y + size.y / 2.0f) * absoluteSize.y + vpPos.y;


				int rectDT = DT_SINGLELINE;	//�c��RECT���L��������

				switch (anchor % 3)
				{	//����RECT
				case 0:
					rectDT |= DT_LEFT;
					break;
				case 1:
					rectDT |= DT_CENTER;
					break;
				case 2:
					rectDT |= DT_RIGHT;
					break;
				default:
					break;
				}

				switch (anchor / 3)
				{	//�c��RECT
				case 0:
					rectDT |= DT_TOP;
					break;
				case 1:
					rectDT |= DT_VCENTER;
					break;
				case 2:
					rectDT |= DT_BOTTOM;
					break;
				default:
					break;
				}

				LPCSTR lpcwstrFontName = font.c_str();
				//Font�쐬 Showcard Gothic
				HFONT font = CreateFont((int)fontSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, lpcwstrFontName);//�t�H���g�̎w��@�����ɂł���
				HGDIOBJ hgdi = SelectObject(hdc, font);
				//rect.top += DrawText(hdc, str, -1, &rect, DT_WORDBREAK); //�c�������H

				//�e�L�X�g�o��
				DrawText(hdc, const_cast<char*>(text.c_str()), text.length(), &screenRect, rectDT);

				SelectObject(hdc, hgdi); //�t�H���g�����ɖ߂�
				DeleteObject(font);	//�I�u�W�F�N�g�폜
			}
				//�f�o�C�X�R���e�L�X�g���������
				pBackSurface->ReleaseDC(NULL);
			
		}
		//�T�[�t�F�X���������
		pBackSurface->Release();

		//�����_�����O�^�[�Q�b�g�����Z�b�g�����̂ŃZ�b�g���Ȃ���
		m_ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
	}
}
//=============================================================================
//
// �����_�����O���� [Renderer.h]
// Author : 
//
//=============================================================================
#pragma once
#include "CoreMinimal.h"
#include "Main.h"
#include "Buffer.h"
#include "LightComponent.h"
#include "ShadowMapping.h"
//*********************************************************
// �}�N����`
//*********************************************************

enum LIGHT_TYPE
{
	LIGHT_TYPE_NONE,		//���C�g����
	LIGHT_TYPE_DIRECTIONAL,	//�f�B���N�V���i�����C�g
	LIGHT_TYPE_POINT,		//�|�C���g���C�g

	LIGHT_TYPE_NUM
};

enum BLEND_MODE
{
	BLEND_MODE_NONE,		//�u�����h����
	BLEND_MODE_ALPHABLEND,	//���u�����h
	BLEND_MODE_ADD,			//���Z�u�����h
	BLEND_MODE_SUBTRACT,	//���Z�u�����h

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//�J�����O����
	CULL_MODE_FRONT,		//�\�̃|���S����`�悵�Ȃ�(CW)
	CULL_MODE_BACK,			//���̃|���S����`�悵�Ȃ�(CCW)

	CULL_MODE_NUM
};


//*********************************************************
// �\����
//*********************************************************

// ���_�\����
struct VERTEX_3D
{
    XMFLOAT3	Position;
    XMFLOAT3	Normal;
    XMFLOAT4	Diffuse;
    XMFLOAT2	TexCoord;
};

// �}�e���A���\����
struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			phong;				//0=lambart,1=phong
};


// �t�H�O�\����
struct FOG {
	float		FogStart;	// �t�H�O�̊J�n����
	float		FogEnd;		// �t�H�O�̍ő勗��
	XMFLOAT4	FogColor;	// �t�H�O�̐F
};

// �}�e���A���p�萔�o�b�t�@�\����
struct MATERIAL_CBUFFER
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			phong;					//0=lambart,1=phong
};


// ���C�g�p�t���O�\����
struct LIGHTFLAGS
{
	int			Type;		//���C�g�^�C�v�ienum LIGHT_TYPE�j
	int         OnOff;		//���C�g�̃I��or�I�t�X�C�b�`
	int			Dummy[2];
};

// ���C�g�p�萔�o�b�t�@�\����
struct LIGHT_CBUFFER
{
	XMFLOAT4	direction[LIGHT_MAX];	// ���C�g�̕���
	XMFLOAT4	Position[LIGHT_MAX];	// ���C�g�̈ʒu
	XMFLOAT4	Diffuse[LIGHT_MAX];		// �g�U���̐F
	XMFLOAT4	Ambient[LIGHT_MAX];		// �����̐F
	XMFLOAT4	Attenuation[LIGHT_MAX];	// ������
	LIGHTFLAGS	Flags[LIGHT_MAX];		// ���C�g���
	int			Enable;					// ���C�e�B���O�L���E�����t���O
	int			Dummy[15];				// 16byte���E�p
};

// �t�H�O�p�萔�o�b�t�@�\����
struct FOG_CBUFFER
{
	XMFLOAT4	Fog;					// �t�H�O��
	XMFLOAT4	FogColor;				// �t�H�O�̐F
	int			Enable;					// �t�H�O�L���E�����t���O
	float		Dummy[3];				// 16byte���E�p
};

// �����p�o�b�t�@
struct FUCHI
{
	int			fuchi;
	int			fill[3];
};

struct GaussianCBuffer {
	float weight[8];
};



class Main;

class Renderer
{
public:

	enum ShaderBF_MODE
	{
		DEFAULT_BF,
		PARTICAL_BF,
	};

	Renderer(Main*main);
	~Renderer();

	ID3DBlob* CreateVSFile(const char* shaderName, char* fName , ID3D11VertexShader** VS);
	void CreatePSFile(char* shaderName, char* fName , ID3D11PixelShader** PS);
	void CreateCSFile(char* shaderName, char* fName , ID3D11ComputeShader** CS);

	XMMATRIX GetViewMatrix(void);
	XMMATRIX GetProjectionMatrix(void);

	HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void UninitRenderer(void);

	void Clear(void);
	void Present(void);

	ID3D11Device *GetDevice( void );
	ID3D11DeviceContext *GetDeviceContext( void );

	void SetDepthEnable( BOOL Enable );
	void SetBlendState(BLEND_MODE bm);
	void SetCullingMode(CULL_MODE cm);
	void SetAlphaTestEnable(BOOL flag);

	void SetWorldViewProjection2D( void );
	void SetWorldMatrix( XMMATRIX *WorldMatrix );
	void SetViewMatrix( XMMATRIX *ViewMatrix );
	void SetProjectionMatrix( XMMATRIX *ProjectionMatrix );

	void SetMaterial( MATERIAL material );

	void SetShaderBuffersMode(ShaderBF_MODE bfMode);

	void SetLightBuffer(void); // static
	void SetLightEnable(BOOL flag);
	void SetLight(LightComponent* light);

	void SetFogBuffer(void);
	void SetFogEnable(BOOL flag);
	void SetFog(FOG* fog);

	void DebugTextOut(char* text, int x, int y);

	void SetFuchi(int flag);
	void SetShaderCamera(XMFLOAT3 pos);

	void SetShadow(SHADOWMAP_CBUFFER* shadow);
	
	void SetGausBuffer(void);
	void SetClearColor(float* color4);

	ID3D11InputLayout** GetVertexLayout(void);

	void SetShaderDefault(void);


	IDXGISwapChain* GetSwapChain(void);

private:

	Main* main;

	D3D_FEATURE_LEVEL       FeatureLevel;

	ID3D11Device* m_D3DDevice ;
	ID3D11DeviceContext* m_ImmediateContext;
	IDXGISwapChain* SwapChain;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
	ID3D11Texture2D* depthTexture;

	D3D11_VIEWPORT defaultViewPort;

	void	InitConstantBuffers(void);



	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* VertexLayout;

	Buffer<XMMATRIX>			*m_WorldBuffer;
	Buffer<XMMATRIX>			*m_ViewBuffer;
	Buffer<XMMATRIX>			*m_ProjectionBuffer;
	Buffer<MATERIAL_CBUFFER>	*m_MaterialBuffer;
	Buffer<LIGHT_CBUFFER>		*m_LightBuffer;
	Buffer<FOG_CBUFFER>			*m_FogBuffer;
	Buffer<FUCHI>				*m_FuchiBuffer;
	Buffer<XMFLOAT4>			*m_CameraBuffer;
	Buffer<SHADOWMAP_CBUFFER>	*m_ShadowBuffer;
	Buffer<GaussianCBuffer>		*m_GausBuffer;
	

	ID3D11DepthStencilState* DepthStateEnable;
	ID3D11DepthStencilState* DepthStateDisable;

	ID3D11BlendState* BlendStateNone;
	ID3D11BlendState* BlendStateAlphaBlend;
	ID3D11BlendState* BlendStateAdd;
	ID3D11BlendState* BlendStateSubtract;
	BLEND_MODE				BlendStateParam;


	ID3D11RasterizerState* RasterStateCullOff;
	ID3D11RasterizerState* RasterStateCullCW;
	ID3D11RasterizerState* RasterStateCullCCW;

	MATERIAL_CBUFFER	Material;
	LIGHT_CBUFFER		Light;
	FOG_CBUFFER			Fog;

	FUCHI				Fuchi;

	GaussianCBuffer		Gaus;

	float ClearColor[4] ;	// �w�i�F

	//////////////////////////////////////////
	// �ύX���܂���							//
	//////////////////////////////////////////
	//��
	XMMATRIX projection;
	XMMATRIX view;


};



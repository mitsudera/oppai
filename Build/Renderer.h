//=============================================================================
//
// レンダリング処理 [Renderer.h]
// Author : 
//
//=============================================================================
#pragma once
#include "CoreMinimal.h"

#include "Buffer.h"
#include "Light.h"
#include "ShadowMapping.h"


//*********************************************************
// マクロ定義
//*********************************************************
#define MAX_LIGHT (8)


enum LIGHT_TYPE
{
	LIGHT_TYPE_NONE,		//ライト無し
	LIGHT_TYPE_DIRECTIONAL,	//ディレクショナルライト
	LIGHT_TYPE_POINT,		//ポイントライト

	LIGHT_TYPE_NUM
};

enum BLEND_MODE
{
	BLEND_MODE_NONE,		//ブレンド無し
	BLEND_MODE_ALPHABLEND,	//αブレンド
	BLEND_MODE_ADD,			//加算ブレンド
	BLEND_MODE_SUBTRACT,	//減算ブレンド

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//カリング無し
	CULL_MODE_FRONT,		//表のポリゴンを描画しない(CW)
	CULL_MODE_BACK,			//裏のポリゴンを描画しない(CCW)

	CULL_MODE_NUM
};

enum FILL_MODE
{
	FILL_MODE_SOLID,		// 通常(ソリッド表示)
	FILL_MODE_WIREFRAME,	// ワイヤーフレーム

	FILL_MODE_NUM
};

enum TEXT_ANCHOR
{
	TEXT_ANCHOR_TOP_LEFT,		// 左上揃え
	TEXT_ANCHOR_TOP_CENTER,		// 中上揃え
	TEXT_ANCHOR_TOP_RIGHT,		// 右上揃え
	TEXT_ANCHOR_CENTER_LEFT,	// 左中央揃え
	TEXT_ANCHOR_CENTER_CENTER,	// 中中央揃え
	TEXT_ANCHOR_CENTER_RIGHT,	// 右中央揃え
	TEXT_ANCHOR_BOTTOM_LEFT,	// 左下揃え
	TEXT_ANCHOR_BOTTOM_CENTER,	// 中央下揃え
	TEXT_ANCHOR_BOTTOM_RIGHT,	// 右下揃え
	TEXT_ANCHOR_MAX				// 最大
};

//*********************************************************
// 構造体
//*********************************************************

// 頂点構造体
struct VERTEX_3D
{
    XMFLOAT3	Position;
    XMFLOAT3	Normal;
    XMFLOAT4	Diffuse;
    XMFLOAT2	TexCoord;
	XMFLOAT3	Tangent;
	XMFLOAT3	BiNormal;
};

// マテリアル構造体
struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;
	int			phong;				//0=lambart,1=phong
};


struct LIGHT_PARAM
{
	XMFLOAT4	m_Position;	    // ライトの位置
	XMFLOAT4	m_Direction;	    // ライトの方向
	XMFLOAT4	m_Diffuse;	        // 拡散光の色
	XMFLOAT4	m_Ambient;		    // 環境光の色
	XMFLOAT4	m_Attenuation;	    // 減衰率    
	XMFLOAT4    m_intensity;       // ライトの強度
	int     	m_Flags;		    // ライト種別
	int			m_Enable;
	int         dummy[2];
};

// ライト用定数バッファ構造体
struct LIGHT_CBUFFER
{
	LIGHT_PARAM  m_lightParam[MAX_LIGHT];
	int			m_Enable;					            // ライティング有効・無効フラグ
	int			m_Dummy[3];				                // 16byte境界用
};

// フォグ構造体
struct FOG {
	float		FogStart;	// フォグの開始距離
	float		FogEnd;		// フォグの最大距離
	XMFLOAT4	FogColor;	// フォグの色
};

// マテリアル用定数バッファ構造体
struct MATERIAL_CBUFFER
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;
	int			phong;					//0=lambart,1=phong
	int			dummy[3];
};

// フォグ用定数バッファ構造体
struct FOG_CBUFFER
{
	XMFLOAT4	Fog;					// フォグ量
	XMFLOAT4	FogColor;				// フォグの色
	int			Enable;					// フォグ有効・無効フラグ
	float		Dummy[3];				// 16byte境界用
};

// 縁取り用バッファ
struct FUCHI
{
	int			fuchi;
	int			fill[3];
};

struct GaussianCBuffer {
	float weight[8];
};

struct TimeCBuffer {
	UINT fps;
	float padding[3];
};
struct TessellationCBuffer
{
	float TessellationFacter;
	float dummy[3];
};

struct EmitterParam_CBuffer {
	XMFLOAT4 m_respawnPos;              //Particleの生成座標
	XMFLOAT4 m_direction;               //Particleの生成向き
	XMFLOAT4 m_color;                   //Color

	UINT     m_type;                    //EmitterSystemのパーティクルタイプ（アルゴリズム）
	UINT     m_particleAmountMax;       //Particle数MAX
	float    m_particleLifeMax;         //Particle寿命MAX
	float    m_emissionRate;            //毎秒Particleを生成する数

	float    m_emissionAccumulator;     //emissionの蓄積容器
	float    m_particleSize;            //ParticleのSize倍率
	float    m_acc;                     //加速度
	BOOL     m_isGravity;               //重力使用？

	float    m_gravity;                 //重力倍率
	BOOL     m_pause;                   //再生するかい？
	float    m_LifeMax;                 //エミッター自身の寿命
	float    m_elapsedTime;             //経過時間
};

class GameEngine;

class Renderer
{
public:

	enum Shader_Mode
	{
		DEFAULT_SMode,
		PARTICAL_SMode,
		UI_SMode,
		TERRAIN_SMode,
	};

	Renderer(GameEngine* pGameEngine);
	~Renderer();

	ID3DBlob* CreateVSFile(const char* shaderName, char* fName , ID3D11VertexShader** VS);
	void CreatePSFile(char* shaderName, char* fName , ID3D11PixelShader** PS);
	void CreateCSFile(char* shaderName, char* fName , ID3D11ComputeShader** CS);
	void CreateHSFile(char* shaderName, char* fName , ID3D11HullShader** HS);
	void CreateDSFile(char* shaderName, char* fName , ID3D11DomainShader** DS);
	void CreateUAV(ID3D11Buffer* pBF, ID3D11UnorderedAccessView* pUav, UINT BFnumElements);

	XMMATRIX GetViewMatrix(void);
	XMMATRIX GetProjectionMatrix(void);

	HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void UninitRenderer(void);

	void SetShaderRender(const Shader_Mode mode);

	void Clear(void);
	void Present(void);

	ID3D11Device *GetDevice( void );
	ID3D11DeviceContext *GetDeviceContext( void );

	void SetDepthEnable( BOOL Enable );
	void SetBlendState(BLEND_MODE bm);
	void SetCullingMode(CULL_MODE cm);
	void SetFillMode(FILL_MODE fm);
	void SetAlphaTestEnable(BOOL flag);

	void SetWorldViewProjection2D( void );
	void SetWorldMatrix( XMMATRIX *WorldMatrix );
	void SetViewMatrix( XMMATRIX *ViewMatrix );
	void SetProjectionMatrix( XMMATRIX *ProjectionMatrix );

	void SetMaterial( MATERIAL material );

	// ライト
	void SetLight(int index, LIGHT_PARAM param);
	void SetLightBuffer(void); // static
	void SetLightEnable( BOOL useFlag);				

	//void SetTimeBuffer(void);
	//void SetTime(TimeCBuffer* time);

	void SetTessFacter(float facter);

	void DebugTextOut(char* text, int x, int y);

	void SetFuchi(int flag);
	void SetShaderCamera(XMFLOAT3 pos);

	void SetClearColor(float* color4);



	IDXGISwapChain* GetSwapChain(void);

	void DrawStringText(string text, float fontSize, XMFLOAT4 color, XMFLOAT2 pos, XMFLOAT2 size, TEXT_ANCHOR anchor, string font);
	void SetShadow(SHADOWMAP_CBUFFER* shadow);

	void SetGausBuffer(void);

	ID3D11RenderTargetView* GetBackBuffer(void);
	ID3D11DepthStencilView* GetBackBufferDSV(void);

private:

	GameEngine* pGameEngine;

	D3D_FEATURE_LEVEL			FeatureLevel;

	ID3D11Device*				m_D3DDevice ;
	ID3D11DeviceContext*		m_ImmediateContext;
	IDXGISwapChain*				SwapChain;
	ID3D11RenderTargetView*		RenderTargetView;
	ID3D11DepthStencilView*		DepthStencilView;
	ID3D11VertexShader*			m_VertexShader;
	ID3D11PixelShader*			m_PixelShader;
	ID3D11InputLayout*			VertexLayout;

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
	Buffer<TimeCBuffer>			*m_TimeBuffer;
	Buffer<TessellationCBuffer>* m_TessellationBuffer;

	ID3D11DepthStencilState*	DepthStateEnable;
	ID3D11DepthStencilState*	DepthStateDisable;

	ID3D11BlendState*			BlendStateNone;
	ID3D11BlendState*			BlendStateAlphaBlend;
	ID3D11BlendState*			BlendStateAdd;
	ID3D11BlendState*			BlendStateSubtract;
	BLEND_MODE					BlendStateParam;


	ID3D11RasterizerState*		RasterStateCullOff;
	ID3D11RasterizerState*		RasterStateCullCW;
	ID3D11RasterizerState*		RasterStateCullCCW;
	ID3D11RasterizerState*		RasterStateFillSOLID;
	ID3D11RasterizerState*		RasterStateFillWIRE;

	MATERIAL_CBUFFER			Material;
	FOG_CBUFFER					Fog;
	LIGHT_CBUFFER				m_CbLight;
	FUCHI						Fuchi;
	TimeCBuffer					m_CbTime;


	float ClearColor[4] ;	// 背景色

	//////////////////////////////////////////
	// 変更しました						//
	//////////////////////////////////////////
	// 仮
	XMMATRIX projection;
	XMMATRIX view;

	//Particle用変数


	// UI
	ID3D11VertexShader* m_UIVS;
	ID3D11PixelShader* m_UIPS;

	//trrain
	ID3D11HullShader* m_TerrainHS;
	ID3D11DomainShader* m_TerrainDS;
	ID3D11VertexShader* m_TerrainVS;
	ID3D11PixelShader* m_TerrainPS;



	void InitConstantBuffers(void);

	void SetShaderBuffersMode(Shader_Mode bfMode);
	void SetShaderFile(Shader_Mode bfMode);

};



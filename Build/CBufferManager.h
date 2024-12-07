#pragma once
#include "Coreminimal.h"

#define MAX_LIGHT (8)


class Renderer;
class GameEngine;

struct LIGHT_PARAM
{
	XMFLOAT4	m_Position;	    // ライトの位置
	XMFLOAT4	m_Direction;	    // ライトの方向
	XMFLOAT4	m_Diffuse;	        // 拡散光の色
	XMFLOAT4	m_Ambient;		    // 環境光の色
	XMFLOAT4	m_Attenuation;	    // 減衰率    
	XMFLOAT4    m_intensity;       // ライトの強度
	XMINT4     	m_Flags;		    // ライト種別0=dir,1=point
	int			m_Enable;
	int         dummy[2];
};


// ライト用定数バッファ構造体
struct LIGHT_CBUFFER
{
	XMFLOAT4	m_Position[MAX_LIGHT];	    // ライトの位置
	XMFLOAT4	m_Direction[MAX_LIGHT];	    // ライトの方向
	XMFLOAT4	m_Diffuse[MAX_LIGHT];	        // 拡散光の色
	XMFLOAT4	m_Ambient[MAX_LIGHT];		    // 環境光の色
	XMFLOAT4	m_Attenuation[MAX_LIGHT];	    // 減衰率    
	XMFLOAT4    m_intensity[MAX_LIGHT];       // ライトの強度
	XMINT4     	m_Flags[MAX_LIGHT];		    // x=enable,y=ライト種別0=dir,1=point
	int			m_Enable;
	int         dummy[3];
};


class CBufferManager
{
public:
	CBufferManager(GameEngine* gameEngine);
	~CBufferManager();

	enum class BufferSlot :unsigned int
	{
		World,
		View,
		Projection,
		Material,
		Light,
		Camera,
		Shadow,
		Free1,
		Free2,
		Free3,
		Free4,
		Free5,
		Free6,
		Free7,
	};

	void SetCBufferOtherCS(ID3D11Buffer* buffer, BufferSlot slot);//コンピュートシェーダー以外の定数バッファ
	void SetCBufferVSPS(ID3D11Buffer* buffer, BufferSlot slot);//頂点シェーダーとピ
	void SetCBufferCS(ID3D11Buffer* buffer, BufferSlot slot);//コンピュートシェーダーの定数バッファ

	void SetWorldMtx(XMMATRIX* world);
	void SetViewMtx(XMMATRIX* view);
	void SetProjectionMtx(XMMATRIX* projection);
	void SetWorldViewProjection2D(void);



	//バッファを直接セットする
	void SetWorldBuffer(ID3D11Buffer* world);
	void SetViewBuffer(ID3D11Buffer* view);
	void SetProjectionBuffer(ID3D11Buffer* projection);
	void SetMaterialBuffer(ID3D11Buffer* material);
	void SetLightBuffer(ID3D11Buffer* light);
	void SetCameraBuffer(ID3D11Buffer* camera);
	void SetShadowBuffer(ID3D11Buffer* shadow);

private:
	GameEngine* pGameEngine;
	Renderer* pRenderer;
	ID3D11DeviceContext* pDeviceContext;

	ID3D11Buffer* WorldBuffer;
	ID3D11Buffer* ViewBuffer;
	ID3D11Buffer* ProjectionBuffer;


};


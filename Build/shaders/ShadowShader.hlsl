

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}

cbuffer ViewBuffer : register(b1)
{
    matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    int noDiffuseTex;
    int noNormalTex;
    int noAmbientTex;
    int phong;
    int dummy[3];
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}
// ライト用バッファ
struct LIGHT
{
    float4 Direction[5];
    float4 Position[5];
    float4 Diffuse[5];
    float4 Ambient[5];
    float4 Attenuation[5];
    float4 Intensity[5];
    int4 Flags[5];
    int Enable;
    int Dummy[3]; //16byte境界用
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}

struct FOG
{
    float4 Distance;
    float4 FogColor;
    int Enable;
    float Dummy[3]; //16byte境界用
};

// フォグ用バッファ
cbuffer FogBuffer : register(b5)
{
    FOG Fog;
};

// 縁取り用バッファ
cbuffer Fuchi : register(b6)
{
    int fuchi;
    int fill[3];
};


cbuffer CameraBuffer : register(b7)
{
    float4 Camera;
}

struct SHADOW
{
    matrix wvp;
    matrix view;
    matrix proj;
    int enable;
    int mode;
    int2 dummy;
};

cbuffer ShadowBuffer : register(b8)
{
    SHADOW Shadow;
}



struct GAUSSIAN
{
    float4 weight1;
    float4 weight2;

};

cbuffer GaussianBuffer : register(b9)
{
    GAUSSIAN gaus;

}




//=============================================================================
// 頂点シェーダ
//=============================================================================


Texture2D Texture : register(t0);
SamplerState smpWrap : register(s0);
SamplerState smpBorder : register(s1);

void VS_SM(
		in float4 inPosition : POSITION0,
		in float4 inNormal : NORMAL0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,

		out float4 outPosition : SV_POSITION,
		out float2 outTexCoord : TEXCOORD0,
		out float4 outDiffuse : COLOR0,
		out float4 outWorldPos : POSITION0
)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    outPosition = mul(inPosition, wvp);


    outTexCoord = inTexCoord;

    outWorldPos = mul(inPosition, World);

    outDiffuse = inDiffuse;
}


//シャドウマップ用ピクセルシェーダー
void PS_SM(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		in float4 inWorldPos : POSITION0,


		out float4 outDiffuse : SV_Target
		)
{
    float4 color;
    if (Material.noDiffuseTex == 0)
    {
        color = Texture.Sample(smpWrap, inTexCoord);

        color *= inDiffuse;
    }
    else
    {
        
        color = inDiffuse;
    }
    
    color = color * Material.Diffuse;

    color.r = inPosition.z;
    color.g = color.r * color.r;
    
    outDiffuse = color;
}

void VS_2D(
		in float4 inPosition : POSITION0,
		in float4 inNormal : NORMAL0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,

		out float4 outPosition : SV_POSITION,
		out float2 outTexCoord : TEXCOORD0,
		out float4 outDiffuse : COLOR0
)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    outPosition = mul(inPosition, wvp);


    outTexCoord = inTexCoord;


    outDiffuse = inDiffuse;
}

//const float sValue = 0.8f;

void xpass(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		

		out float4 outDiffuse : SV_Target)
{
    float x;
    float y;
    Texture.GetDimensions(x, y);
    float4 col;
    col = gaus.weight1.x * Texture.Sample(smpWrap, float2(inTexCoord));
    col += gaus.weight1.y * (Texture.Sample(smpWrap, inTexCoord + float2(+1.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-1.0f  / x, 0)));
    col += gaus.weight1.z * (Texture.Sample(smpWrap, inTexCoord + float2(+3.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-3.0f  / x, 0)));
    col += gaus.weight1.w * (Texture.Sample(smpWrap, inTexCoord + float2(+5.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-5.0f  / x, 0)));
    col += gaus.weight2.x * (Texture.Sample(smpWrap, inTexCoord + float2(+7.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-7.0f  / x, 0)));
    col += gaus.weight2.y * (Texture.Sample(smpWrap, inTexCoord + float2(+9.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-9.0f  / x, 0)));
    col += gaus.weight2.z * (Texture.Sample(smpWrap, inTexCoord + float2(+11.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-11.0f / x, 0)));
    col += gaus.weight2.w * (Texture.Sample(smpWrap, inTexCoord + float2(+13.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-13.0f / x, 0)));


    
    
    outDiffuse = col;


}
void ypass(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		

		out float4 outDiffuse : SV_Target)
{
    float x;
    float y;
    Texture.GetDimensions(x, y);
    float4 col;
    col = gaus.weight1.x * Texture.Sample(smpWrap, float2(inTexCoord));
    col += gaus.weight1.y * (Texture.Sample(smpWrap, inTexCoord + float2(0, +1.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -1.0f  / y)));
    col += gaus.weight1.z * (Texture.Sample(smpWrap, inTexCoord + float2(0, +3.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -3.0f  / y)));
    col += gaus.weight1.w * (Texture.Sample(smpWrap, inTexCoord + float2(0, +5.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -5.0f  / y)));
    col += gaus.weight2.x * (Texture.Sample(smpWrap, inTexCoord + float2(0, +7.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -7.0f  / y)));
    col += gaus.weight2.y * (Texture.Sample(smpWrap, inTexCoord + float2(0, +9.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -9.0f  / y)));
    col += gaus.weight2.z * (Texture.Sample(smpWrap, inTexCoord + float2(0, +11.0f / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -11.0f / y)));
    col += gaus.weight2.w * (Texture.Sample(smpWrap, inTexCoord + float2(0, +13.0f / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -13.0f / y)));


    
    
    outDiffuse = col;


}
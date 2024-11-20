
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

struct EmitterParam
{
    float4 m_spawnPos;
    float4 m_direction; //Particleの生成向き
    float4 m_color; //Color

    uint m_type; //EmitterSystemのパーティクルタイプ（アルゴリズム）
    uint m_particleAmountMax; //Particle数MAX
    float m_LifetimeMax; //Particle寿命MAX
    float m_emissionRate; //毎秒Particleを生成する数

    float m_emissionAccumulator; //emissionの蓄積容器
    float m_particleSize; //ParticleのSize倍率
    float m_acc; //加速度
    int m_isGravity; //重力
    
    float m_gravity; //重力倍率
    int m_pause;     //再生するかい？
    float m_lifeMax; //エミッター自身の寿命
    float m_elapsedTime; //経過時間
};

cbuffer EmitterParamBuffer : register(b3)
{
    EmitterParam emitterparam;
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
    int phong;
};

cbuffer MaterialBuffer : register(b4)
{
    MATERIAL Material;
}

struct VS_IN_PARTICLE
{
    float4 Position : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};
struct PS_IN_PARTICLE
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WorldPos : POSITION0;
};

void VSmain(in VS_IN_PARTICLE In, out PS_IN_PARTICLE Out)
{

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    //位置
    Out.Position = mul(In.Position, wvp);
    Out.WorldPos = mul(In.Position, World);
    //UV
    Out.TexCoord = In.TexCoord;
    //法線
    Out.Normal = normalize(mul(float4(In.Normal.xyz, 0.0f), World));
    //色
    Out.Diffuse = In.Diffuse;
    
}

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void PSmain(in PS_IN_PARTICLE In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= emitterparam.m_color * Material.Diffuse;

}

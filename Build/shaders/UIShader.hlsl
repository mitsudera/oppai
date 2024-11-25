
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
    float4 color;

    color = g_Texture.Sample(g_SamplerState, In.TexCoord);
    color *= In.Diffuse;
    outDiffuse = color;
}

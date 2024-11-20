
// �}�g���N�X�o�b�t�@
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
    float4 m_direction; //Particle�̐�������
    float4 m_color; //Color

    uint m_type; //EmitterSystem�̃p�[�e�B�N���^�C�v�i�A���S���Y���j
    uint m_particleAmountMax; //Particle��MAX
    float m_LifetimeMax; //Particle����MAX
    float m_emissionRate; //���bParticle�𐶐����鐔

    float m_emissionAccumulator; //emission�̒~�ϗe��
    float m_particleSize; //Particle��Size�{��
    float m_acc; //�����x
    int m_isGravity; //�d��
    
    float m_gravity; //�d�͔{��
    int m_pause;     //�Đ����邩���H
    float m_lifeMax; //�G�~�b�^�[���g�̎���
    float m_elapsedTime; //�o�ߎ���
};

cbuffer EmitterParamBuffer : register(b3)
{
    EmitterParam emitterparam;
}

// �}�e���A���o�b�t�@
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

    //�ʒu
    Out.Position = mul(In.Position, wvp);
    Out.WorldPos = mul(In.Position, World);
    //UV
    Out.TexCoord = In.TexCoord;
    //�@��
    Out.Normal = normalize(mul(float4(In.Normal.xyz, 0.0f), World));
    //�F
    Out.Diffuse = In.Diffuse;
    
}

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void PSmain(in PS_IN_PARTICLE In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= emitterparam.m_color * Material.Diffuse;

}

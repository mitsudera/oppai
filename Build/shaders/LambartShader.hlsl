

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
	float4		Diffuse;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;
    int         dummy;
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ライト用バッファ
struct LIGHT
{
    float4 Position;
    float4 Direction;

    float4 Diffuse;
    float4 Ambient;
    float4 Attenuation;
    float4 Intensity;
    int Flags;
    int Enable;
    int Dummy[2]; //16byte境界用
};

#define MAX_LIGHT (8)


cbuffer LightBuffer : register(b4)
{
    LIGHT Light[MAX_LIGHT];
    
    int LightEnable;
    int Dummy[3]; //16byte境界用

    
}


cbuffer CameraBuffer : register(b5)
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

cbuffer ShadowBuffer : register(b6)
{
    SHADOW Shadow;
}






//=============================================================================
// 頂点シェーダ
//=============================================================================
void VSmain( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,
                          in float4 inTangent       : TANGENT0,
                          in float4 inBiNoramal    : BINORMAL0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0,
						  out float4 outPosSM		: POSITION1,
						  out float4 outTangent     : TANGENT0,
						  out float4 outBiNormal    : BINORMAL0
)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));
	outTangent = normalize(mul(float4(inTangent.xyz, 0.0f), World));
	outBiNormal = normalize(mul(float4(inBiNoramal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;
	
	
	 //頂点座標　モデル座標系→透視座標系(シャドウマップ)
    matrix SMWorldViewProj = mul(World, Shadow.wvp);
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    float4 pos4 = mul(inPosition, SMWorldViewProj);
    pos4.xyz = pos4.xyz / pos4.w;
    outPosSM.x = (pos4.x + 1.0) / 2.0;
    outPosSM.y = (-pos4.y + 1.0) / 2.0;
    outPosSM.z = pos4.z;


}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		DiffuseTexture       : register(t0);
Texture2D		NormalTex       : register(t1);
Texture2D       armTex           : register(t2);
Texture2D       ShadowMap       : register(t3);
Texture2D       ShadowMapTex    : register(t4);

SamplerState	WrapSampler : register( s0 );
SamplerState BorderSampler : register(s1);


//=============================================================================
// ピクセルシェーダ
//=============================================================================


float GetVarianceDirectionalShadowFactor(float4 shadowCoord)
{
    
    float2 depth = ShadowMapTex.Sample(BorderSampler, shadowCoord.xy).xy;
    float depth_sq = depth.x * depth.x; // E(x)^2
    float variance = depth.y - depth_sq; // σ^2 = E(x^2) - E(x^2)
    variance = saturate(variance + 0.0001); // 0.0001を追加して安定性を向上

    float fragDepth = shadowCoord.z;
    float md = fragDepth - depth.x; // t - μ
    float p = variance / (variance + (md * md)); // σ^2 / (σ^2 + (t - μ)^2)

    return saturate(max(p, fragDepth <= depth.x)); // P(x >= t)を満たすときのみ
}

float VSM_Filter(float2 texcoord, float fragDepth)
{

    float4 depth = ShadowMapTex.Sample(BorderSampler, texcoord);
  

    float depth_sq = depth.x * depth.x;

    float variance = depth.y - depth_sq;
    variance = min(1.0f, max(0.0f, variance + 0.0001f));

    float md = fragDepth - depth.x;
    float p = variance / (variance + (md * md));
  
    return saturate(max(p, depth.x <= fragDepth));
}

void PSmain(in float4 inPosition : SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos      : POSITION0,
						 in  float4 inPosSM			: POSITION1,
						 in  float4 inTangent		: TANGENT0,
						 in  float4 inBiNormal		: BINORMAL0,

						 out float4 outDiffuse		: SV_Target )
{
    float4 color;

    float sma = 1.0;
    bool shadow;
    
    
    float4 normal = inNormal;
    
    if (Material.noNormalTex == 0)
    {
        // Sample the normal map
        float3 normalMap = NormalTex.Sample(WrapSampler, inTexCoord).rgb;
        
        normalMap.x = 1.0 - normalMap.x;
        normalMap.y = 1.0 - normalMap.y;
        //normalMap.z = 1.0 - normalMap.z;
       
        normalMap = normalize(normalMap * 2.0 - 1.0);
        
        
        
        float3x3 TBN = float3x3(inTangent.xyz, inBiNormal.xyz, inNormal.xyz);
        normal.xyz = mul(normalMap, TBN);
        
        
    }
    
    
    
    
    
    					//影
    if (Shadow.enable == 1)
    {

		
        if (inPosSM.z > 1.0)
        {
            sma = 1.0;
        }
        else if (Shadow.mode == 0)
        {
            float sm0 = ShadowMap.Sample(BorderSampler, inPosSM.xy);

            
            if (inPosSM.z - 0.0002 > sm0)
            {
                sma = 0.5;


            }
            
            
            
        }
        else if (Shadow.mode == 1)
        {
            sma = GetVarianceDirectionalShadowFactor(inPosSM);
        
 
        
            if (sma < 0.99f)
            {
                sma = sma * sma;

            }

        }
        
 

    }
    else
    {
        sma = 1.0;
    }

	
    if (Material.noDiffuseTex == 0)
    {
        color = DiffuseTexture.Sample(WrapSampler, inTexCoord);

        color *= inDiffuse;
    }
    else
    {
        color = inDiffuse;
    }

    float alpha = color.a;
    
    if (LightEnable == 0)
    {
        color = color * Material.Diffuse * sma;
    }
    else
    {
        float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

        for (int i = 0; i < MAX_LIGHT; i++)
        {
            float3 lightDir;
            float light;

            if (Light[i].Enable == 1)
            {
                if (Light[i].Flags == 0)
                {
                    lightDir = normalize(Light[i].Direction.xyz);
                    light = dot(lightDir, normal.xyz);

                    light = (0.5 - 0.5 * light) * sma;
                    tempColor = color * Material.Diffuse * light * Light[i].Diffuse;
                }
                else if (Light[i].Flags == 1)
                {
                    lightDir = normalize(Light[i].Position.xyz - inWorldPos.xyz);
                    light = dot(lightDir, normal.xyz);

                    tempColor = color * Material.Diffuse * light * Light[i].Diffuse;

                    float distance = length(inWorldPos - Light[i].Position);

                    float att = saturate((Light[i].Attenuation.x - distance) / Light[i].Attenuation.x);
                    tempColor *= att;
                }
                else
                {
                    tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
                }

                outColor += tempColor;
            }
            

        }

        color = outColor;

        
    }
		

    color.a = alpha;
    
    
    outDiffuse = color;
}


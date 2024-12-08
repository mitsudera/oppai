

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
    int noArmTex;
    int phong;
    int dummy[3];
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

#define MAX_LIGHT (8)

// ライト用バッファ
struct LIGHT
{
    float4 Position[MAX_LIGHT];
    float4 Direction[MAX_LIGHT];
    float4 Diffuse[MAX_LIGHT];
    float4 Ambient[MAX_LIGHT];
    
    float4 Attenuation[MAX_LIGHT];
    float4 Intensity[MAX_LIGHT];
    //float4 other[MAX_LIGHT];
    int4 flags[MAX_LIGHT]; //x=enable,y=pointORdirection
    int enable;
    int dummy[3];

    
};



cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
    
    
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
void VSmain(in float4 inPosition : POSITION0,
						  in float4 inNormal : NORMAL0,
						  in float4 inDiffuse : COLOR0,
						  in float2 inTexCoord : TEXCOORD0,
                          in float4 inTangent : TANGENT0,
                          in float4 inBiNoramal : BINORMAL0,

						  out float4 outPosition : SV_POSITION,
						  out float4 outNormal : NORMAL0,
						  out float2 outTexCoord : TEXCOORD0,
						  out float4 outDiffuse : COLOR0,
						  out float4 outWorldPos : POSITION0,
						  out float4 outPosSM : POSITION1,
						  out float4 outTangent : TANGENT0,
						  out float4 outBiNormal : BINORMAL0
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
Texture2D DiffuseTexture : register(t0);
Texture2D NormalTex : register(t1);
Texture2D armTex : register(t2);
Texture2D ShadowMap : register(t3);
Texture2D ShadowMapTex : register(t4);

SamplerState WrapSampler : register(s0);
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
						 in float4 inNormal : NORMAL0,
						 in float2 inTexCoord : TEXCOORD0,
						 in float4 inDiffuse : COLOR0,
						 in float4 inWorldPos : POSITION0,
						 in float4 inPosSM : POSITION1,
						 in float4 inTangent : TANGENT0,
						 in float4 inBiNormal : BINORMAL0,

						 out float4 outDiffuse : SV_Target)
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
    
    if (Light.enable == 0)
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
            float3 iA;
            float3 iD;
            float3 iS;
                
            if (Light.flags[i].x == 1)
            {
                lightDir = normalize(Light.Direction[i].xyz);
                light = dot(lightDir, normal.xyz);
                light = (0.5 - 0.5 * light);

                if (Light.flags[i].y == 0)
                {
           

                    float3 r = 2.0 * normal.xyz * light - lightDir;

                    float3 v = normalize(Camera.xyz - inWorldPos.xyz);
                        
                        
                    iA = color.xyz * Material.Ambient.xyz * Light.Ambient[i].xyz;
                        
                        
                        
                    iD = color * Material.Diffuse * light * Light.Diffuse[i];
                    iS = color.xyz * pow(saturate(dot(r, v)), Material.Shininess) * Material.Specular.xyz;

                    if (light > 0.5)
                    {
                        iD *= sma;
                    }
                        

                       
                        
                        
                    tempColor = float4(saturate((iA + iD + iS)), 1.0f);

                }
                else if (Light.flags[i].y== 1)
                {
                    //ポイントライトのphongshdinig
                }
                else
                {
                    tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
                }

                outColor += tempColor;
            }

        }

        
		

        color = outColor;
        color.a = alpha;
    }

    outDiffuse = color;

}



//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
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

// �}�e���A���o�b�t�@
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;
	int			phong;
    int         dummy[3];
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ���C�g�p�o�b�t�@
struct LIGHT
{
    float4      Position[5];
    float4      Direction[5];

	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
    float4		Intensity[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte���E�p
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

struct FOG
{
	float4		Distance;
	float4		FogColor;
	int			Enable;
	float		Dummy[3];//16byte���E�p
};

// �t�H�O�p�o�b�t�@
cbuffer FogBuffer : register( b5 )
{
	FOG			Fog;
};

// �����p�o�b�t�@
cbuffer Fuchi : register(b6)
{
	int			fuchi;
	int			fill[3];
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
// ���_�V�F�[�_
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
	
	
	 //���_���W�@���f�����W�n���������W�n(�V���h�E�}�b�v)
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
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture       : register(t0);
Texture2D		NormalTex       : register(t1);
Texture2D       ShadowMap       : register(t2);
Texture2D       ShadowMapTex    : register(t3);
Texture2D       armTex           : register(t4);

SamplerState	g_SamplerState : register( s0 );
SamplerState smpBorder : register(s1);


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================


float GetVarianceDirectionalShadowFactor(float4 shadowCoord)
{
    
    float2 depth = ShadowMapTex.Sample(smpBorder, shadowCoord.xy).xy;
    float depth_sq = depth.x * depth.x; // E(x)^2
    float variance = depth.y - depth_sq; // ��^2 = E(x^2) - E(x^2)
    variance = saturate(variance + 0.0001); // 0.0001��ǉ����Ĉ��萫������

    float fragDepth = shadowCoord.z;
    float md = fragDepth - depth.x; // t - ��
    float p = variance / (variance + (md * md)); // ��^2 / (��^2 + (t - ��)^2)

    return saturate(max(p, fragDepth <= depth.x)); // P(x >= t)�𖞂����Ƃ��̂�
}

float VSM_Filter(float2 texcoord, float fragDepth)
{

    float4 depth = ShadowMapTex.Sample(smpBorder, texcoord);
  

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
    
    if(Material.noNormalTex==0)
    {
        // Sample the normal map
        float3 normalMap = NormalTex.Sample(g_SamplerState, inTexCoord).rgb;
        
        normalMap.x = 1.0 - normalMap.x;
        normalMap.y = 1.0 - normalMap.y;
        //normalMap.z = 1.0 - normalMap.z;
       
        normalMap = normalize(normalMap * 2.0 - 1.0);
        
        
        
        float3x3 TBN = float3x3(inTangent.xyz, inBiNormal.xyz, inNormal.xyz);
        normal.xyz = mul(normalMap, TBN);
        
        
    }
    
    
    
    
    
    					//�e
    if (Shadow.enable == 1)
    {

		
		if(inPosSM.z>1.0)
        {
            sma = 1.0;
        }
        else if (Shadow.mode == 0)
        {
            float sm0 = ShadowMap.Sample(smpBorder, inPosSM.xy);

            
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
                sma = sma*sma ;

            }

        }
        
 

    }
    else
    {
        sma = 1.0;
    }

	
	if (Material.noDiffuseTex == 0)
	{
		color = g_Texture.Sample(g_SamplerState, inTexCoord);

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
	}

    float alpha = color.a;
    
	if (Light.Enable == 0)
	{
		color = color * Material.Diffuse*sma;
	}
	else
	{
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

        if(Material.phong == false)
        {
            for (int i = 0; i < 5; i++)
            {
                float3 lightDir;
                float light;

                if (Light.Flags[i].y == 1)
                {
                    if (Light.Flags[i].x == 1)
                    {
                        lightDir = normalize(Light.Direction[i].xyz);
                        light = dot(lightDir, normal.xyz);

                        light = (0.5 - 0.5 * light)*sma;
                        tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
                    }
                    else if (Light.Flags[i].x == 2)
                    {
                        lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
                        light = dot(lightDir, normal.xyz);

                        tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

                        float distance = length(inWorldPos - Light.Position[i]);

                        float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
                        tempColor *= att;
                    }
                    else
                    {
                        tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
                    }

                    outColor += tempColor;
                }
            }

        }
        else
        {
            for (int i = 0; i < 5; i++)
            {
                float3 lightDir;
                float light;
                float3 iA;
                float3 iD;
                float3 iS;
                
                if (Light.Flags[i].y == 1)
                {
                    lightDir = normalize(Light.Direction[i].xyz);
                    light = dot(lightDir, normal.xyz);
                    light = (0.5 - 0.5 * light);

                    if (Light.Flags[i].x == 1)
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
                    else
                    {
                        tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
                    }

                    outColor += tempColor;
                }

            }

        }
		

		color = outColor;
		color.a = alpha;
	}

    //color.xyz = normal.xyz;
    
	//�t�H�O
	if (Fog.Enable == 1)
	{
		float z = inPosition.z*inPosition.w;
		float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
		f = saturate(f);
		outDiffuse = f * color + (1 - f)*Fog.FogColor;
		outDiffuse.a = color.a;
	}
	else
	{
		outDiffuse = color;
	}

	//�����
	if (fuchi == 1)
	{
		float angle = dot(normalize(inWorldPos.xyz - Camera.xyz), normalize(inNormal));
		//if ((angle < 0.5f)&&(angle > -0.5f))
		if (angle > -0.3f)
		{
			outDiffuse.rb  = 1.0f;
			outDiffuse.g = 0.0f;			
		}
	}
}
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



//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void VSmain(              in float4 inPosition : POSITION0,
						  in float4 inNormal : NORMAL0,
						  in float4 inDiffuse : COLOR0,
						  in float2 inTexCoord : TEXCOORD0,
                          in float4 inTangent : TANGENT0,
                          in float4 inBiNoramal : BINORMAL0,

						  out float4 outPosition : SV_POSITION,
						  out float2 outTexCoord : TEXCOORD0,
						  out float4 outDiffuse : COLOR0)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    outPosition = mul(inPosition, wvp);

    outTexCoord = inTexCoord;
    outDiffuse = inDiffuse;
	
}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D DiffuseTexture : register(t0);

SamplerState WrapSampler : register(s0);
SamplerState BorderSampler : register(s1);


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================



void PSmain(             in float4 inPosition : SV_POSITION,
						 in float2 inTexCoord : TEXCOORD0,
						 in float4 inDiffuse : COLOR0,

						 out float4 outDiffuse : SV_Target)
{
    float4 color;

	
    color = DiffuseTexture.Sample(WrapSampler, inTexCoord);
    color *= inDiffuse;
	
    outDiffuse = color;

}

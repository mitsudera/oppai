

void VS_Main(
		in float4 inPosition : POSITION0,
		in float4 inNormal : NORMAL0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inTangent : TANGENT0,
		in float4 inBiNoramal : BINORMAL0,


		out float4 outPosition : SV_POSITION,
		out float2 outTexCoord : TEXCOORD0,
		out float4 outDiffuse : COLOR0,
		out float4 outWorldPos : POSITION0
)
{
    outPosition = inPosition;

    outTexCoord = inTexCoord;
    outDiffuse = inDiffuse;
}



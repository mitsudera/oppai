
cbuffer ConstantBuffer : register(b0)
{
    matrix BoneMatrices[100]; // ボーンの変換行列
}
cbuffer ConstantBuffer : register(b1)
{
    int boneNum;
    int dummy[3];
}

struct ControlPoint
{
    float3 pos; // 12バイト
    float dummy1; // 4バイト (アライメントのためのパディング)
    float weights[100]; // 400バイト
    int showBoneNum; // 4バイト
    float dummy2[2];
    int showBoneIndex[100]; // 400バイト
};
StructuredBuffer<ControlPoint> ControlPoints : register(t0);
RWStructuredBuffer<float3> OutputPositions : register(u0);
// CS・ｽﾝ抵ｿｽ
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};



[numthreads(32, 1, 1)]
void CSFunc(uint3 DTid : SV_DispatchThreadID)
{

    ControlPoint cp = ControlPoints[DTid.x];
    float3 pos = cp.pos;
    float4 cpoint = float4(pos, 1.0f);

    matrix lasttrans = matrix(0, 0, 0, 0,
                           0, 0, 0, 0,
                           0, 0, 0, 0,
                           0, 0, 0, 0);
    
    for (int k = 0; k < cp.showBoneNum; k++)
    {
        int n = cp.showBoneIndex[k];
        matrix transmtx = BoneMatrices[n];
            
        lasttrans += transmtx * cp.weights[n];
    }
    
    float4 transformedPos = mul(cpoint, lasttrans);

    OutputPositions[DTid.x].xy = transformedPos.xy;
    OutputPositions[DTid.x].z = -transformedPos.z;
}


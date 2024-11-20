
cbuffer ConstantBuffer : register(b0)
{
    matrix BoneMatrices[100]; // �{�[���̕ϊ��s��
}
cbuffer ConstantBuffer : register(b1)
{
    int boneNum;
    int dummy[3];
}

struct ControlPoint
{
    float3 pos; // 12�o�C�g
    float dummy1; // 4�o�C�g (�A���C�����g�̂��߂̃p�f�B���O)
    float weights[100]; // 400�o�C�g
    int showBoneNum; // 4�o�C�g
    float dummy2[2];
    int showBoneIndex[100]; // 400�o�C�g
};
StructuredBuffer<ControlPoint> ControlPoints : register(t0);
RWStructuredBuffer<float3> OutputPositions : register(u0);
// CS�E�ݒ�
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


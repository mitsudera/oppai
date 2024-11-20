// ParticleCS.hlsl

struct EmitterParam
{
    float4 m_spawnPos;            //Particle�̐������W
    float4 m_direction;             //Particle�̐�������
    float4 m_color;                 //Color

    uint m_type;                    //EmitterSystem�̃p�[�e�B�N���^�C�v�i�A���S���Y���j
    uint m_particleAmountMax;       //Particle��MAX
    float m_LifetimeMax;            //Particle����MAX
    float m_emissionRate;           //���bParticle�𐶐����鐔

    float m_emissionAccumulator;    //emission�̒~�ϗe��
    float m_particleSize;           //Particle��Size�{��
    float m_acc;                    //�����x
    int m_isGravity;                //�d��
    
    float m_gravity;                //�d�͔{��
    int m_pause;                    //�Đ����邩���H
    float m_lifeMax;                //�G�~�b�^�[���g�̎���
    float m_elapsedTime;            //�o�ߎ���
};

cbuffer EmitterParamBuffer : register(b0)
{
    EmitterParam emitterparam;
}

//cbuffer DeltaTimeBuffer : register(b1)
//{
//    float deltaTime;
//    float padding[3];
//}

struct ParticleData
{
    float4 m_position;      // �p�[�e�B�N���̈ʒu
    float4 m_velocity;      // �p�[�e�B�N���̑��x
    float4 m_color;         // �p�[�e�B�N���̐F
    
    float m_lifeTime;       // �p�[�e�B�N���̎���
    float m_age;            // �p�[�e�B�N���̌o�ߎ���
    float m_size;           // �p�[�e�B�N���̃T�C�Y
    float m_acc;            // �����x
                
    uint  m_isGravity;      // �d��
    uint  m_use;            // �p�[�e�B�N�����A�N�e�B�u���ǂ���
    float m_padding[2];     // �p�f�B���O
};


void EmitParticle(inout ParticleData particle, uint3 DTid);
float generateRandom(float minValue, float maxValue, uint3 DTid);

RWStructuredBuffer<ParticleData> particles : register(u0);


[numthreads(256, 1, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x;
    float deltaTime = 0.016f;
    // �p�[�e�B�N�����𒴂���X���b�h�͖���
    if (index >= emitterparam.m_particleAmountMax)
        return;

    ParticleData particle = particles[index];

    if (particle.m_use)
    {
        // �d�͂̓K�p
        if (particle.m_isGravity)
        {
            particle.m_velocity.y += emitterparam.m_gravity * -9.8f * deltaTime;
        }

        // �ʒu�̍X�V
        particle.m_position += particle.m_velocity * deltaTime;

        // �����̌���
        particle.m_lifeTime -= deltaTime;
        if (particle.m_lifeTime <= 0.0f)
        {
            particle.m_use = false; // �������s�����疳����
        }
    }
    else
    {
        // �p�[�e�B�N������A�N�e�B�u�ȏꍇ�A�V�����p�[�e�B�N�����G�~�b�g
        EmitParticle(particle, DTid);
    }

    // �X�V���ꂽ�p�[�e�B�N���f�[�^�������߂�
    particles[index] = particle;
}

// ���������p�֐��i�ȈՓI�ȗ�j
float generateRandom(float minValue, float maxValue, uint3 DTid)
{
    return lerp(minValue, maxValue, frac(sin(dot(float2(DTid.xy), float2(12.9898, 78.233))) * 43758.5453));
}

// �p�[�e�B�N���̐�������
void EmitParticle(inout ParticleData particle,uint3 DTid)
{
    // �p�[�e�B�N���̏������W�i��: �G�~�b�^�[�̈ʒu�j
    particle.m_position = emitterparam.m_spawnPos;

    // �����_���Ȏ���
    particle.m_lifeTime = emitterparam.m_lifeMax * generateRandom(0.8f, 1.2f, DTid); // 80%�`120%�̃����_������

    // �����x
    float particleAcc = emitterparam.m_acc * generateRandom(0.8f, 1.2f, DTid);

    int type = emitterparam.m_type;
    if (type == 1)
    { // BURST�^�C�v�̃p�[�e�B�N��
        // ����ɕ��˂�������̌v�Z
        float phi = acos(2.0f * generateRandom(0.0f, 1.0f, DTid) - 1.0f); // -1����1��0����pi�Ƀ}�b�s���O
        float theta = 2.0f * 3.14159f * generateRandom(0.0f, 1.0f, DTid); // 0����2pi�܂�

        float4 burstDirection = float4(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi) , 0.0f);
        particle.m_velocity = burstDirection * particleAcc;
        particle.m_size = emitterparam.m_particleSize * generateRandom(0.9f, 1.1f, DTid); // �T�C�Y�������_���ɕϓ�
    }
    else if (type == 2)
    { // DIRECTION�^�C�v�̃p�[�e�B�N��
        // �w�肳�ꂽ�����ɉ������p�[�e�B�N��
        float3 dir = emitterparam.m_direction.xyz + float3(generateRandom(-0.1f, 0.1f, DTid), generateRandom(-0.1f, 0.1f, DTid), generateRandom(-0.1f, 0.1f, DTid));
        particle.m_velocity.xyz = normalize(dir) * particleAcc; //�����̌v�Z
        particle.m_velocity.w = 0.0f;
        particle.m_size = emitterparam.m_particleSize * generateRandom(0.9f, 1.1f, DTid); // �T�C�Y�������_���ɕϓ�
    }

    // �p�[�e�B�N���̐F (�����ő����ω���������)
    float colorRandom = generateRandom(0.8f, 1.0f, DTid);
    particle.m_color = float4(emitterparam.m_color.rgb * colorRandom, 1.0f);

    // �d�͂̓K�p
    particle.m_isGravity = emitterparam.m_isGravity;

    // �p�[�e�B�N�����A�N�e�B�u�ɂ���
    particle.m_use = true;
}
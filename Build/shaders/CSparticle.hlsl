// ParticleCS.hlsl

struct EmitterParam
{
    float4 m_spawnPos;            //Particleの生成座標
    float4 m_direction;             //Particleの生成向き
    float4 m_color;                 //Color

    uint m_type;                    //EmitterSystemのパーティクルタイプ（アルゴリズム）
    uint m_particleAmountMax;       //Particle数MAX
    float m_LifetimeMax;            //Particle寿命MAX
    float m_emissionRate;           //毎秒Particleを生成する数

    float m_emissionAccumulator;    //emissionの蓄積容器
    float m_particleSize;           //ParticleのSize倍率
    float m_acc;                    //加速度
    int m_isGravity;                //重力
    
    float m_gravity;                //重力倍率
    int m_pause;                    //再生するかい？
    float m_lifeMax;                //エミッター自身の寿命
    float m_elapsedTime;            //経過時間
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
    float4 m_position;      // パーティクルの位置
    float4 m_velocity;      // パーティクルの速度
    float4 m_color;         // パーティクルの色
    
    float m_lifeTime;       // パーティクルの寿命
    float m_age;            // パーティクルの経過時間
    float m_size;           // パーティクルのサイズ
    float m_acc;            // 加速度
                
    uint  m_isGravity;      // 重力
    uint  m_use;            // パーティクルがアクティブかどうか
    float m_padding[2];     // パディング
};


void EmitParticle(inout ParticleData particle, uint3 DTid);
float generateRandom(float minValue, float maxValue, uint3 DTid);

RWStructuredBuffer<ParticleData> particles : register(u0);


[numthreads(256, 1, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x;
    float deltaTime = 0.016f;
    // パーティクル数を超えるスレッドは無視
    if (index >= emitterparam.m_particleAmountMax)
        return;

    ParticleData particle = particles[index];

    if (particle.m_use)
    {
        // 重力の適用
        if (particle.m_isGravity)
        {
            particle.m_velocity.y += emitterparam.m_gravity * -9.8f * deltaTime;
        }

        // 位置の更新
        particle.m_position += particle.m_velocity * deltaTime;

        // 寿命の減少
        particle.m_lifeTime -= deltaTime;
        if (particle.m_lifeTime <= 0.0f)
        {
            particle.m_use = false; // 寿命が尽きたら無効化
        }
    }
    else
    {
        // パーティクルが非アクティブな場合、新しいパーティクルをエミット
        EmitParticle(particle, DTid);
    }

    // 更新されたパーティクルデータを書き戻す
    particles[index] = particle;
}

// 乱数生成用関数（簡易的な例）
float generateRandom(float minValue, float maxValue, uint3 DTid)
{
    return lerp(minValue, maxValue, frac(sin(dot(float2(DTid.xy), float2(12.9898, 78.233))) * 43758.5453));
}

// パーティクルの生成処理
void EmitParticle(inout ParticleData particle,uint3 DTid)
{
    // パーティクルの初期座標（例: エミッターの位置）
    particle.m_position = emitterparam.m_spawnPos;

    // ランダムな寿命
    particle.m_lifeTime = emitterparam.m_lifeMax * generateRandom(0.8f, 1.2f, DTid); // 80%～120%のランダム寿命

    // 加速度
    float particleAcc = emitterparam.m_acc * generateRandom(0.8f, 1.2f, DTid);

    int type = emitterparam.m_type;
    if (type == 1)
    { // BURSTタイプのパーティクル
        // 球状に放射する方向の計算
        float phi = acos(2.0f * generateRandom(0.0f, 1.0f, DTid) - 1.0f); // -1から1を0からpiにマッピング
        float theta = 2.0f * 3.14159f * generateRandom(0.0f, 1.0f, DTid); // 0から2piまで

        float4 burstDirection = float4(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi) , 0.0f);
        particle.m_velocity = burstDirection * particleAcc;
        particle.m_size = emitterparam.m_particleSize * generateRandom(0.9f, 1.1f, DTid); // サイズもランダムに変動
    }
    else if (type == 2)
    { // DIRECTIONタイプのパーティクル
        // 指定された方向に沿ったパーティクル
        float3 dir = emitterparam.m_direction.xyz + float3(generateRandom(-0.1f, 0.1f, DTid), generateRandom(-0.1f, 0.1f, DTid), generateRandom(-0.1f, 0.1f, DTid));
        particle.m_velocity.xyz = normalize(dir) * particleAcc; //向きの計算
        particle.m_velocity.w = 0.0f;
        particle.m_size = emitterparam.m_particleSize * generateRandom(0.9f, 1.1f, DTid); // サイズもランダムに変動
    }

    // パーティクルの色 (乱数で多少変化を加える)
    float colorRandom = generateRandom(0.8f, 1.0f, DTid);
    particle.m_color = float4(emitterparam.m_color.rgb * colorRandom, 1.0f);

    // 重力の適用
    particle.m_isGravity = emitterparam.m_isGravity;

    // パーティクルをアクティブにする
    particle.m_use = true;
}
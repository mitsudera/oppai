#pragma once
#include "Coreminimal.h"

class Light
{
public:
    Light();
    ~Light();

private:
    XMFLOAT4	m_Position;	        // ライトの位置
    XMFLOAT4	m_Direction;	    // ライトの方向
    XMFLOAT4	m_Diffuse;	        // 拡散光の色
    XMFLOAT4	m_Ambient;		    // 環境光の色
    XMFLOAT4	m_Attenuation;	    // 減衰率    
    XMFLOAT4    m_intensity;        // ライトの強度
    int     	m_Flags;		    // ライト種別+OnOff,(0=Off,1=Directional,2=Point)

};


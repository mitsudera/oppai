#pragma once
#include "Coreminimal.h"

class Light
{
public:
    Light();
    ~Light();

private:
    XMFLOAT4	m_Position;	        // ���C�g�̈ʒu
    XMFLOAT4	m_Direction;	    // ���C�g�̕���
    XMFLOAT4	m_Diffuse;	        // �g�U���̐F
    XMFLOAT4	m_Ambient;		    // �����̐F
    XMFLOAT4	m_Attenuation;	    // ������    
    XMFLOAT4    m_intensity;        // ���C�g�̋��x
    int     	m_Flags;		    // ���C�g���+OnOff,(0=Off,1=Directional,2=Point)

};


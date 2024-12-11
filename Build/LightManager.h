#pragma once
#include "Coreminimal.h"
#include "CBufferManager.h"


#define MAX_DIREC_LIGHT (4)
#define MAX_POINT_LIGHT (8)


class DirectionalLightComponent;
class PointLightComponent;
class GameEngine;



struct DIREC_LIGHT_PARAM
{
	XMFLOAT4	m_Direction;	    // ���C�g�̕���
	XMFLOAT4	m_Diffuse;	        // �g�U���̐F
	XMFLOAT4	m_Ambient;		    // �����̐F
	int			m_Enable;
};
// ���C�g�p�萔�o�b�t�@�\����
struct DIREC_LIGHT_CBUFFER
{
	XMFLOAT4	m_Direction[MAX_DIREC_LIGHT];	    // ���C�g�̕���
	XMFLOAT4	m_Diffuse[MAX_DIREC_LIGHT];	        // �g�U���̐F
	XMFLOAT4	m_Ambient[MAX_DIREC_LIGHT];		    // �����̐F
	XMINT4		m_Enable[MAX_DIREC_LIGHT];
	XMINT4		allEnable;

};


struct POINT_LIGHT_PARAM
{
	XMFLOAT4	m_Position;	    // ���C�g�̕���
	XMFLOAT4	m_Diffuse;	        // �g�U���̐F
	XMFLOAT4	m_Ambient;		    // �����̐F
	XMFLOAT4	m_Attenuation;	    // ������    
	XMFLOAT4    m_intensity;       // ���C�g�̋��x
	int			m_Enable;
};

// ���C�g�p�萔�o�b�t�@�\����
struct POINT_LIGHT_CBUFFER
{
	XMFLOAT4	m_Position[MAX_POINT_LIGHT];	    // ���C�g�̈ʒu
	XMFLOAT4	m_Diffuse[MAX_POINT_LIGHT];	        // �g�U���̐F
	XMFLOAT4	m_Ambient[MAX_POINT_LIGHT];		    // �����̐F
	XMFLOAT4	m_Attenuation[MAX_POINT_LIGHT];	    // ������    
	XMFLOAT4    m_intensity[MAX_POINT_LIGHT];       // ���C�g�̋��x
	XMINT4		m_Enable[MAX_POINT_LIGHT];
	XMINT4		allEnable;

};




class LightManager
{

public:


    LightManager(GameEngine* pGameEngine);
    ~LightManager();

    void Init();
	void Update();

    void SetLightNear(XMFLOAT3 pos);//�w��̃|�W�V�����ɋ߂����C�g���I���ɂ���

	int AddLight(DirectionalLightComponent* com);
	int AddLight(PointLightComponent* com);

	void SetPointLight(PointLightComponent* pointlightComponent, int index);
	void SetDirecLight(DirectionalLightComponent* direclightComponent, int index);

	DirectionalLightComponent* GetMainLight(void);

	void SetAllEnable(BOOL flag);
private:
    GameEngine* pGameEngine;
	CBufferManager* cBufferManager;

    DIREC_LIGHT_CBUFFER direcLightCBufferStruct;
    POINT_LIGHT_CBUFFER pointLightCBufferStruct;


    vector<DirectionalLightComponent*> direcLightList;
    vector<PointLightComponent*> pointLightList;

    ID3D11Buffer* direcLightBuffer;
    ID3D11Buffer* pointLightBuffer;



};


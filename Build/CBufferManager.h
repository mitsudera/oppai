#pragma once
#include "Coreminimal.h"

#define MAX_LIGHT (8)


class Renderer;
class GameEngine;

struct LIGHT_PARAM
{
	XMFLOAT4	m_Position;	    // ���C�g�̈ʒu
	XMFLOAT4	m_Direction;	    // ���C�g�̕���
	XMFLOAT4	m_Diffuse;	        // �g�U���̐F
	XMFLOAT4	m_Ambient;		    // �����̐F
	XMFLOAT4	m_Attenuation;	    // ������    
	XMFLOAT4    m_intensity;       // ���C�g�̋��x
	XMINT4     	m_Flags;		    // ���C�g���0=dir,1=point
	int			m_Enable;
	int         dummy[2];
};


// ���C�g�p�萔�o�b�t�@�\����
struct LIGHT_CBUFFER
{
	XMFLOAT4	m_Position[MAX_LIGHT];	    // ���C�g�̈ʒu
	XMFLOAT4	m_Direction[MAX_LIGHT];	    // ���C�g�̕���
	XMFLOAT4	m_Diffuse[MAX_LIGHT];	        // �g�U���̐F
	XMFLOAT4	m_Ambient[MAX_LIGHT];		    // �����̐F
	XMFLOAT4	m_Attenuation[MAX_LIGHT];	    // ������    
	XMFLOAT4    m_intensity[MAX_LIGHT];       // ���C�g�̋��x
	XMINT4     	m_Flags[MAX_LIGHT];		    // x=enable,y=���C�g���0=dir,1=point
	int			m_Enable;
	int         dummy[3];
};


class CBufferManager
{
public:
	CBufferManager(GameEngine* gameEngine);
	~CBufferManager();

	enum class BufferSlot :unsigned int
	{
		World,
		View,
		Projection,
		Material,
		Light,
		Camera,
		Shadow,
		Free1,
		Free2,
		Free3,
		Free4,
		Free5,
		Free6,
		Free7,
	};

	void SetCBufferOtherCS(ID3D11Buffer* buffer, BufferSlot slot);//�R���s���[�g�V�F�[�_�[�ȊO�̒萔�o�b�t�@
	void SetCBufferVSPS(ID3D11Buffer* buffer, BufferSlot slot);//���_�V�F�[�_�[�ƃs
	void SetCBufferCS(ID3D11Buffer* buffer, BufferSlot slot);//�R���s���[�g�V�F�[�_�[�̒萔�o�b�t�@

	void SetWorldMtx(XMMATRIX* world);
	void SetViewMtx(XMMATRIX* view);
	void SetProjectionMtx(XMMATRIX* projection);
	void SetWorldViewProjection2D(void);



	//�o�b�t�@�𒼐ڃZ�b�g����
	void SetWorldBuffer(ID3D11Buffer* world);
	void SetViewBuffer(ID3D11Buffer* view);
	void SetProjectionBuffer(ID3D11Buffer* projection);
	void SetMaterialBuffer(ID3D11Buffer* material);
	void SetLightBuffer(ID3D11Buffer* light);
	void SetCameraBuffer(ID3D11Buffer* camera);
	void SetShadowBuffer(ID3D11Buffer* shadow);

private:
	GameEngine* pGameEngine;
	Renderer* pRenderer;
	ID3D11DeviceContext* pDeviceContext;

	ID3D11Buffer* WorldBuffer;
	ID3D11Buffer* ViewBuffer;
	ID3D11Buffer* ProjectionBuffer;


};


#pragma once
#include "Coreminimal.h"



class Renderer;
class GameEngine;

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
		DirectionalLight,
		PointLight,
		Camera,
		Shadow,
		Free1,
		Free2,
		Free3,
		Free4,
		Free5,
		Free6,
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
	void SetDirectionalLightBuffer(ID3D11Buffer* light);
	void SetPointLightBuffer(ID3D11Buffer* light);
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


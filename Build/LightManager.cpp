#include "LightManager.h"
#include "GameEngine.h"
#include "LightComponent.h"
#include "renderer.h"

LightManager::LightManager(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
}

LightManager::~LightManager()
{
	
}

void LightManager::Init()
{

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(LIGHT_CBUFFER);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	pGameEngine->GetRenderer()->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->lightBuffer);
	pGameEngine->GetCBufferManager()->SetLightBuffer(this->lightBuffer);

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		activeLightIndex[i] = -1;
	}
}

void LightManager::Update()
{


	//int setNum = 0;
	//for (int i = 0; i < lightList.size(); i++)
	//{

	//	if (lightList[i]->GetActive() && lightList[i]->GetLightParam().m_Flags == 0)
	//	{
	//		activeLightIndex[setNum] = i;

	//	}
	//	if (setNum > MAX_LIGHT-1) break;
	//}


	//ZeroMemory(&this->lightCBufferStruct, sizeof(LIGHT_CBUFFER));

	//for (int i = 0; i < setNum; i++)
	//{
	//	SetLight(lightList[activeLightIndex[i]], i);
	//}


	//pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(this->lightBuffer, 0, NULL, &this->lightCBufferStruct, 0, 0);
}

void LightManager::Draw()
{
	Renderer* renderer = pGameEngine->GetRenderer();

}

void LightManager::Uninit()
{
}


void LightManager::SetLight(LightComponent* lightComponent,int index)
{
	this->lightCBufferStruct.m_Position[index] = lightComponent->GetLightParam().m_Position;
	this->lightCBufferStruct.m_Direction[index] = lightComponent->GetLightParam().m_Direction;
	this->lightCBufferStruct.m_Diffuse[index] = lightComponent->GetLightParam().m_Diffuse;
	this->lightCBufferStruct.m_Ambient[index] = lightComponent->GetLightParam().m_Ambient;
	this->lightCBufferStruct.m_Attenuation[index] = lightComponent->GetLightParam().m_Attenuation;
	this->lightCBufferStruct.m_intensity[index] = lightComponent->GetLightParam().m_intensity;
	this->lightCBufferStruct.m_Flags[index] = lightComponent->GetLightParam().m_Flags;
}

#pragma once
#include "ShaderSet.h"
#include "Coreminimal.h"

class Renderer;

class LambartShader :public ShaderSet
{
public:
	LambartShader(Renderer* renderer);
	~LambartShader();

	struct MaterialCBuffer
	{
		XMFLOAT4 diffuse;
		int noDiffuseTex;
		int noNormalTex;
		int noArmTex;
		int dummy;

	};
	void LambartShader::SetMaterialCbuffer(LambartShader::MaterialCBuffer data);

private:

	ID3D11Buffer* materialBuffer;
	int materialSlot;
};


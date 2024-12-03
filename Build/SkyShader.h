#pragma once
#include "ShaderSet.h"
#include "Coreminimal.h"

class Renderer;

class SkyShader :public ShaderSet
{
public:
	SkyShader(Renderer* renderer);
	~SkyShader();

	struct MaterialCBuffer
	{
		XMFLOAT4 diffuse;
		int noDiffuseTex;
		int noNormalTex;
		int noArmTex;
		int dummy;

	};
	void SetMaterialCbuffer(MaterialCBuffer data);

private:

};


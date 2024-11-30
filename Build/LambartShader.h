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
	void SetMaterialCbuffer(MaterialCBuffer data);

private:

};


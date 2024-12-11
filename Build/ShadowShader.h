#pragma once
#include "ShaderSet.h"
class ShadowShader :public ShaderSet
{
public:
	ShadowShader(Renderer* renderer);
	~ShadowShader();

	struct MaterialCBuffer
	{
		XMFLOAT4 diffuse;
		int noDiffuseTex;
		int dummy[3];

	};
	void SetMaterialCbuffer(MaterialCBuffer data);


};


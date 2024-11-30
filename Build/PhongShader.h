#pragma once
#include "ShaderSet.h"

class Renderer;


class PhongShader : public ShaderSet
{
public:

	PhongShader(Renderer* renderer);
	~PhongShader();

	struct MaterialCBuffer:public ShaderSet::MaterialCBuffer
	{
		XMFLOAT4	ambient;
		XMFLOAT4	diffuse;
		XMFLOAT4	specular;
		XMFLOAT4	emission;
		float		shininess;
		int			noDiffuseTex;
		int			noNormalTex;
		int			noArmTex;

	};
	void SetMaterialCbuffer(MaterialCBuffer data);


private:
	Renderer* pRenderer;



};


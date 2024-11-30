#pragma once
#include "Material.h"

class PhongShader;

class DX11Texture;

class PhongMaterial :public Material
{
public:
	PhongMaterial(PhongShader* phongShader);
	~PhongMaterial();

	// Material ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void SetBufferMaterial(void) override;


private:
	PhongShader* pPhongShader;

	XMFLOAT4	ambient;
	XMFLOAT4	diffuse;
	XMFLOAT4	specular;
	XMFLOAT4	emission;
	float		shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;



	DX11Texture* diffuseTex;
	DX11Texture* normalTex;
	DX11Texture* armTex;



};


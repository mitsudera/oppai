#pragma once
#include "Material.h"

class LambartShader;
class DX11Texture;
class LambartMaterial :public Material
{
public:
	LambartMaterial(LambartShader* lambartShader);
	~LambartMaterial();
	// Material を介して継承されました
	virtual void SetBufferMaterial(void) override;



private:

	XMFLOAT4	diffuse;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;
	
	DX11Texture* diffuseTex;
	DX11Texture* normalTex;
	DX11Texture* armTex;



	LambartShader* pLambartShader;
};


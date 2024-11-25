#pragma once
#include "Material.h"

class LambartShader;
class LambartMaterial :public Material
{
public:
	LambartMaterial(LambartShader* lambartShader);
	~LambartMaterial();

private:

	LambartShader* pLambartShader;
	XMFLOAT4 diffuse;
	int noDiffuseTex;
	int noNormalTex;
	int noArmTex;


	// Material を介して継承されました
	virtual void SetBufferMaterial(void) override;

};


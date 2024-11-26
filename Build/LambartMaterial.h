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


	// Material ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void SetBufferMaterial(void) override;

};


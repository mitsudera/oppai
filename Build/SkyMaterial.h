#pragma once
#include "Material.h"

class SkyShader;
class DX11Texture;
class SkyMaterial :public Material
{
public:
	SkyMaterial(AssetsManager* assetsManager);
	SkyMaterial(SkyMaterial* sky);
	~SkyMaterial();
	// Material を介して継承されました
	virtual void SetBufferMaterial(void) override;


	void LoadDiffuseTex(string fName);
	void LoadNormalTex(string fName);
	void LoadArmTex(string fName);

	void LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial);
	void SetDiffuse(XMFLOAT4 diff);

private:




	SkyShader* pSkyShader;
};


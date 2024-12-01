#pragma once
#include "Material.h"

class LambartShader;
class DX11Texture;
class LambartMaterial :public Material
{
public:
	LambartMaterial(AssetsManager* assetsManager);
	LambartMaterial(LambartMaterial* lambart);
	~LambartMaterial();
	// Material ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void SetBufferMaterial(void) override;


	void LoadDiffuseTex(string fName);
	void LoadNormalTex(string fName);
	void LoadArmTex(string fName);

	void LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial);

private:

	XMFLOAT4	diffuse;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;
	
	int textureDiffuseIndex;
	int textureNormalIndex;
	int textureArmIndex;



	LambartShader* pLambartShader;
};


#pragma once
#include "Material.h"

class PhongShader;

class DX11Texture;

class PhongMaterial :public Material
{
public:
	PhongMaterial(AssetsManager* assetsManager);
	PhongMaterial(PhongMaterial* phong);
	~PhongMaterial();

	// Material ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void SetBufferMaterial(void) override;

	void LoadDiffuseTex(string fName);
	void LoadNormalTex(string fName);
	void LoadArmTex(string fName);

	void LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial);


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


	int textureDiffuseIndex;
	int textureNormalIndex;
	int textureArmIndex;



};


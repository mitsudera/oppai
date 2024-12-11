#pragma once
#include "Coreminimal.h"

class Renderer;

class ShaderSet;

class AssetsManager;



class Material
{
public:
	Material();
	~Material();

	void SetShaderRenderer(void);

	virtual void SetBufferMaterial(void) = 0;

	void SetBufferShadowMapping(void);

	ShaderSet* GetShaderSet(void);


	void LoadDiffuseTex(string fName);
	void LoadNormalTex(string fName);
	void LoadArmTex(string fName);

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


private:

protected:






	ShaderSet* pShader;
	AssetsManager* pAssetsManager;

};


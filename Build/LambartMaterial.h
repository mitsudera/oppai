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
	// Material ����Čp������܂���
	virtual void SetBufferMaterial(void) override;



	void LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial);

private:



	LambartShader* pLambartShader;
};


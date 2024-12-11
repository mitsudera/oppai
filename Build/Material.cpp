#include "Material.h"
#include "renderer.h"
#include "ShaderSet.h"
#include "AssetsManager.h"

Material::Material()
{
}

Material::~Material()
{
}


void Material::SetShaderRenderer(void)
{
	pShader->SetShaderRenderer();

}

void Material::SetBufferShadowMapping(void)
{
}

ShaderSet* Material::GetShaderSet(void)
{
	return this->pShader;
}


void Material::LoadDiffuseTex(string fName)
{
	textureDiffuseIndex = pAssetsManager->LoadTexture(fName);
}

void Material::LoadNormalTex(string fName)
{
	textureNormalIndex = pAssetsManager->LoadTexture(fName);
}

void Material::LoadArmTex(string fName)
{
	textureArmIndex = pAssetsManager->LoadTexture(fName);
}

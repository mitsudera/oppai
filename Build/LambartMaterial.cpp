#include "LambartMaterial.h"
#include "LambartShader.h"
#include "DX11Texture.h"

LambartMaterial::LambartMaterial(LambartShader* lambartShader)
{
	this->pShader= lambartShader;

	this->pLambartShader = lambartShader;
}

LambartMaterial::~LambartMaterial()
{
}

void LambartMaterial::SetBufferMaterial(void)
{
	LambartShader::MaterialCBuffer mCBuffer;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	mCBuffer.noNormalTex = this->noNormalTex;
	mCBuffer.noArmTex = this->noArmTex;
	this->pLambartShader->SetMaterialCbuffer(mCBuffer);

	if (!noDiffuseTex) diffuseTex->SetShaderResource(0);
	if (!noNormalTex) normalTex->SetShaderResource(1);
	if (!noArmTex) armTex->SetShaderResource(2);

}

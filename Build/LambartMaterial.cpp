#include "LambartMaterial.h"
#include "LambartShader.h"

LambartMaterial::LambartMaterial(LambartShader* lambartShader)
{
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
}

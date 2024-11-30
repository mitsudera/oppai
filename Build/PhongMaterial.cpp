#include "PhongMaterial.h"
#include "PhongShader.h"
#include "DX11Texture.h"

PhongMaterial::PhongMaterial(PhongShader* phongShader)
{
	this->pPhongShader = phongShader;
	this->pShader = phongShader;

}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::SetBufferMaterial(void)
{
	PhongShader::MaterialCBuffer mCBuffer;
	mCBuffer.ambient = this->ambient;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.emission = this->emission;
	mCBuffer.shininess = this->shininess;
	mCBuffer.specular = this->specular;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	mCBuffer.noNormalTex = this->noNormalTex;
	mCBuffer.noArmTex = this->noArmTex;
	this->pPhongShader->SetMaterialCbuffer(mCBuffer);

	if (!noDiffuseTex) diffuseTex->SetShaderResource(0);
	if (!noNormalTex) normalTex->SetShaderResource(1);
	if (!noArmTex) armTex->SetShaderResource(2);


}

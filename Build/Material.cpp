#include "Material.h"
#include "renderer.h"
#include "ShaderSet.h"


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

ShaderSet* Material::GetShaderSet(void)
{
	return this->pShader;
}

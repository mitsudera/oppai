#pragma once
#include "Coreminimal.h"

class Renderer;

class ShaderSet;

class Material
{
public:
	Material();
	~Material();

	void SetShaderRenderer(void);

	virtual void SetBufferMaterial(void) = 0;

	ShaderSet* GetShaderSet(void);
private:

protected:

	ShaderSet* pShader;


};


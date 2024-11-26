#pragma once
#include "Coreminimal.h"

class Renderer;

class Material
{
public:
	Material();
	~Material();

	void SetShaderRenderer(void);

	virtual void SetBufferMaterial(void) = 0;
private:
	Renderer* pRenderer;



};


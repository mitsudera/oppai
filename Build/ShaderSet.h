#pragma once
#include "Coreminimal.h"

class Renderer;

class ShaderSet
{
public:
	ShaderSet();
	~ShaderSet();
	void SetShaderRenderer(void);

	void ShaderRelease(void);

	void CreateVS(string filePath,string shaderName);
	void CreateHS(string filePath,string shaderName);
	void CreateDS(string filePath,string shaderName);
	void CreateGS(string filePath,string shaderName);
	void CreatePS(string filePath,string shaderName);

private:


	ID3D11VertexShader* VS;
	ID3D11HullShader* HS;
	ID3D11DomainShader* DS;
	ID3D11GeometryShader* GS;
	ID3D11PixelShader* PS;

	ID3D11InputLayout* VertexLayout;

protected:
	Renderer* pRenderer;

	string name;
};


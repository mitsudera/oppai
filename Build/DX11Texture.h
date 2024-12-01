#pragma once

#include "CoreMinimal.h"

class AssetsManager;

class DX11Texture
{
	AssetsManager* pManager;
	ID3D11ShaderResourceView* SRV;
	string filepath;

public:
	DX11Texture();
	~DX11Texture();

	ID3D11ShaderResourceView* GetTexture(void);
	void CreateSRV(char* path);
	void CreateSRV(string path);

	string GetFilePath(void);

	void SetShaderResourcePS(int n);

	void SetManager(AssetsManager* Manager);

private:

};

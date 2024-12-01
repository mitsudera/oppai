#include "DX11Texture.h"

#include "AssetsManager.h"
#include "renderer.h"
#include "GameEngine.h"

DX11Texture::DX11Texture()
{
	pManager = nullptr;
	this->SRV = nullptr;

}

DX11Texture::~DX11Texture()
{
	if (this->SRV)	SRV->Release();
}


ID3D11ShaderResourceView* DX11Texture::GetTexture(void)
{
	return this->SRV;
}
void DX11Texture::CreateSRV(char* path)
{

	D3DX11CreateShaderResourceViewFromFile(
		this->pManager->GetGameEngine()->GetRenderer()->GetDevice(),
		path,
		NULL,
		NULL,
		&SRV,
		NULL);
	this->filepath = path;

}
void DX11Texture::CreateSRV(string path)
{

	D3DX11CreateShaderResourceViewFromFile(
		this->pManager->GetGameEngine()->GetRenderer()->GetDevice(),
		path.c_str(),
		NULL,
		NULL,
		&SRV,
		NULL);
	this->filepath = path;

}
string DX11Texture::GetFilePath(void)
{
	return this->filepath;
}
void DX11Texture::SetShaderResourcePS(int n)
{
	this->pManager->GetGameEngine()->GetRenderer()->GetDeviceContext()->PSSetShaderResources(n, 1, &SRV);
}

void DX11Texture::SetManager(AssetsManager* Manager)
{
	this->pManager = Manager;
}


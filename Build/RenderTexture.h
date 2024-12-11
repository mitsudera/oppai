#pragma once
#include "Coreminimal.h"

class AssetsManager;
class Renderer;
//�����_�����O�\�e�N�X�`���N���X
class RenderTexture
{
public:
	enum class BindMode :int
	{
		Texture,
		DepthTexture,
		BOTH,
	};


	RenderTexture(AssetsManager* am);
	~RenderTexture();

	void SetPSSRV(BindMode mode, int slot);

	void SetRTV(BindMode mode, int slot);

	string GetName(void);

	void CreateRenderTexture(int widht, int height, string name);

	ID3D11RenderTargetView* GetRenderTargetView(void);
	ID3D11DepthStencilView* GetDepthStencilView(void);

	void ClearRTV(XMFLOAT4 clearColor);
	void ClearDSV(float f);

private:

	AssetsManager* pAssetsManager;
	ID3D11Texture2D* texture;			// �V���h�E�E�}�b�v
	ID3D11RenderTargetView* rtv;	// �[�x/�X�e���V���E�r���[
	ID3D11ShaderResourceView* srv;	// �V�F�[�_�E���\�[�X�E�r���[

	ID3D11Texture2D* depthTexture;			// �V���h�E�E�}�b�v
	ID3D11DepthStencilView* dsv;	// �[�x/�X�e���V���E�r���[
	ID3D11ShaderResourceView* dssrv;	// �V�F�[�_�E���\�[�X�E�r���[

	string name;

	Renderer* pRenderer;
};


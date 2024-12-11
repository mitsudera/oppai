#include "RenderTexture.h"
#include "AssetsManager.h"
#include "GameEngine.h"
#include "renderer.h"
RenderTexture::RenderTexture(AssetsManager* am)
{
	pAssetsManager = am;
	pRenderer = pAssetsManager->GetGameEngine()->GetRenderer();
}

RenderTexture::~RenderTexture()
{
}

void RenderTexture::SetPSSRV(BindMode mode,int slot)
{
	if (mode == BindMode::BOTH || mode == BindMode::Texture)
	{
		pRenderer->GetDeviceContext()->PSSetShaderResources(slot, 1, &srv);
	}
	if (mode == BindMode::BOTH || mode == BindMode::DepthTexture)
	{
		pRenderer->GetDeviceContext()->PSSetShaderResources(slot+1, 1, &dssrv);
	}
}

void RenderTexture::SetRTV(BindMode mode,int slot)
{
	if (mode == BindMode::BOTH)
	{
		pRenderer->GetDeviceContext()->OMSetRenderTargets(1, &rtv, dsv);
	}
	if (mode == BindMode::Texture)
	{
		pRenderer->GetDeviceContext()->OMSetRenderTargets(1, &rtv, nullptr);
	}
	if (mode == BindMode::DepthTexture)
	{
		pRenderer->GetDeviceContext()->OMSetRenderTargets(1, nullptr, dsv);
	}

}

string RenderTexture::GetName(void)
{
	return this->name;
}

void RenderTexture::CreateRenderTexture(int widht, int height, string name)
{
	this->name = name;

	HRESULT hr;
	// �V���h�E �}�b�v�̍쐬
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = (UINT)widht;   // ��
	texDesc.Height = (UINT)height;  // ����
	texDesc.MipLevels = 1;       // �~�b�v�}�b�v ���x����
	texDesc.ArraySize = 1;       // �z��T�C�Y
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;  // �t�H�[�}�b�g
	texDesc.SampleDesc.Count = 1;  // �}���`�T���v�����O�̐ݒ�
	texDesc.SampleDesc.Quality = 0;  // �}���`�T���v�����O�̕i��
	texDesc.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; //
	texDesc.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
	texDesc.MiscFlags = 0;   // ���̑��̐ݒ�Ȃ�



	hr = pRenderer->GetDevice()->CreateTexture2D(
		&texDesc,         // �쐬����2D�e�N�X�`���̐ݒ�
		nullptr,               // 
		&texture);     // �쐬�����e�N�X�`�����󂯎��ϐ�


	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = pRenderer->GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &rtv);




	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �t�H�[�}�b�g
	srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srvDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srvDesc.Texture2D.MipLevels = -1;  // ���ׂẴ~�b�v�}�b�v ���x��

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = pRenderer->GetDevice()->CreateShaderResourceView(
		texture,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&srvDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&srv);  // �󂯎��ϐ�

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (UINT)widht;   // ��
	descDepth.Height = (UINT)height;  // ����
	descDepth.MipLevels = 1;       // �~�b�v�}�b�v ���x����
	descDepth.ArraySize = 1;       // �z��T�C�Y
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // �t�H�[�}�b�g
	descDepth.SampleDesc.Count = 1;  // �}���`�T���v�����O�̐ݒ�
	descDepth.SampleDesc.Quality = 0;  // �}���`�T���v�����O�̕i��
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL; // �[�x/�X�e���V���A�V�F�[�_ ���\�[�X �r���[�Ƃ��Ďg�p
	descDepth.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
	descDepth.MiscFlags = 0;   // ���̑��̐ݒ�Ȃ�
	hr = pRenderer->GetDevice()->CreateTexture2D(
		&descDepth,         // �쐬����2D�e�N�X�`���̐ݒ�
		nullptr,               // 
		&depthTexture);     // �쐬�����e�N�X�`�����󂯎��ϐ�



		// �[�x/�X�e���V�� �r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // �r���[�̃t�H�[�}�b�g
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pRenderer->GetDevice()->CreateDepthStencilView(
		depthTexture,         // �[�x/�X�e���V���E�r���[�����e�N�X�`��
		&descDSV,             // �[�x/�X�e���V���E�r���[�̐ݒ�
		&dsv); // �쐬�����r���[���󂯎��ϐ�

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srDesc.Texture2D.MipLevels = -1;  // ���ׂẴ~�b�v�}�b�v ���x��

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = pRenderer->GetDevice()->CreateShaderResourceView(
		depthTexture,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&srDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&dssrv);  // �󂯎��ϐ�



}

ID3D11RenderTargetView* RenderTexture::GetRenderTargetView(void)
{
	return this->rtv;
}

ID3D11DepthStencilView* RenderTexture::GetDepthStencilView(void)
{
	return this->dsv;
}

void RenderTexture::ClearRTV(XMFLOAT4 clearColor)
{
	float cc[4] = { clearColor.x,clearColor.y,clearColor.z,clearColor.w };
	pRenderer->GetDeviceContext()->ClearRenderTargetView(
		rtv, // �N���A����`��^�[�Q�b�g
		cc);         // �N���A����l


}

void RenderTexture::ClearDSV(float f)
{
	pRenderer->GetDeviceContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, f, 0);

}


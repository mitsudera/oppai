#include "ShadowMapping.h"
#include "level.h"
#include "gameobject.h"
#include "Main.h"
#include "Renderer.h"

ShadowMapping::ShadowMapping(Level* level)
{
	this->pLevel = level;
	this->quarity = 1024.0f*4.0f;
	this->quarityblur = 1024.0f*4.0f;
	this->hw = 128.0f;

	pos = XMFLOAT3(100.0f, 100.0f, 100.0f);
	at = XMFLOAT3(0.0f, 0.0f, 0.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	this->ShadowMap.Enable = TRUE;
	SetShadowMode(SHADOW_MODE::VARIANCE);
	m_VertexShaderShadow = nullptr;
	m_PixelShaderShadow = nullptr;

	m_VertexShaderShadow2D = nullptr;
	m_PixelShaderShadowX = nullptr;
	m_PixelShaderShadowY = nullptr;

	this->ShadowTarget = nullptr;

	this->dir=XMFLOAT3(0.0f, 1.0f, 0.0f);
	this->len = 100.0f;
	this->vNear = 10.0f;
	this->vFar = 1000.0f;
}

ShadowMapping::~ShadowMapping()
{
}

void ShadowMapping::Init(void)
{
	Renderer* renderer = this->pLevel->GetMain()->GetRenderer();

	HRESULT hr;

	renderer->CreateVSFile("shader.hlsl", "VS_SM", &m_VertexShaderShadow);
	renderer->CreatePSFile("shader.hlsl", "PS_SM", &m_PixelShaderShadow);

	renderer->CreateVSFile("shader.hlsl", "VS_2D", &m_VertexShaderShadow2D);
	renderer->CreatePSFile("shader.hlsl", "xpass", &m_PixelShaderShadowX);
	renderer->CreatePSFile("shader.hlsl", "ypass", &m_PixelShaderShadowY);


	// �V���h�E �}�b�v�̍쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (UINT)quarity;   // ��
	descDepth.Height = (UINT)quarity;  // ����
	descDepth.MipLevels = 1;       // �~�b�v�}�b�v ���x����
	descDepth.ArraySize = 1;       // �z��T�C�Y
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // �t�H�[�}�b�g
	descDepth.SampleDesc.Count = 1;  // �}���`�T���v�����O�̐ݒ�
	descDepth.SampleDesc.Quality = 0;  // �}���`�T���v�����O�̕i��
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL; // �[�x/�X�e���V���A�V�F�[�_ ���\�[�X �r���[�Ƃ��Ďg�p
	descDepth.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
	descDepth.MiscFlags = 0;   // ���̑��̐ݒ�Ȃ�
	hr = renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // �쐬����2D�e�N�X�`���̐ݒ�
		nullptr,               // 
		&ShadowMapDS);     // �쐬�����e�N�X�`�����󂯎��ϐ�

	// �[�x/�X�e���V�� �r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // �r���[�̃t�H�[�}�b�g
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = renderer->GetDevice()->CreateDepthStencilView(
		ShadowMapDS,         // �[�x/�X�e���V���E�r���[�����e�N�X�`��
		&descDSV,             // �[�x/�X�e���V���E�r���[�̐ݒ�
		&ShadowMapDSView); // �쐬�����r���[���󂯎��ϐ�

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srDesc.Texture2D.MipLevels = -1;  // ���ׂẴ~�b�v�}�b�v ���x��

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapDS,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&srDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&ShadowMapDSSRView);  // �󂯎��ϐ�





	// �V���h�E �}�b�v�̍쐬
	descDepth.Format = DXGI_FORMAT_R32G32_TYPELESS;  // �t�H�[�}�b�g
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //�V�F�[�_ ���\�[�X �r���[�Ƃ��Ďg�p
	hr = renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // �쐬����2D�e�N�X�`���̐ݒ�
		NULL,               // 
		&ShadowMapingTexture);     // �쐬�����e�N�X�`�����󂯎��ϐ�



	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	// �����_�[�^�[�Q�b�g�r���[�̐���


	hr = renderer->GetDevice()->CreateRenderTargetView(ShadowMapingTexture, &rtvDesc, &RenderTargetShadow);

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	srDesc.Format = DXGI_FORMAT_R32G32_FLOAT; // �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srDesc.Texture2D.MipLevels = -1;  // ���ׂẴ~�b�v�}�b�v ���x��
	
	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapingTexture,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&srDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&ShadowMapSRView);  // �󂯎��ϐ�




	// X�V���h�E �}�b�v�̍쐬
	descDepth.Width = quarityblur;
	descDepth.Height = quarityblur;

	descDepth.Format = DXGI_FORMAT_R32G32_TYPELESS;  // �t�H�[�}�b�g
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //�V�F�[�_ ���\�[�X �r���[�Ƃ��Ďg�p
	hr = renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // �쐬����2D�e�N�X�`���̐ݒ�
		NULL,               // 
		&ShadowMapingTextureX);     // �쐬�����e�N�X�`�����󂯎��ϐ�



	// �����_�[�^�[�Q�b�g�r���[�̐���


	hr = renderer->GetDevice()->CreateRenderTargetView(ShadowMapingTextureX, &rtvDesc, &RenderTargetShadowX);


	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapingTextureX,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&srDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&ShadowMapSRViewX);  // �󂯎��ϐ�


	// Y�V���h�E �}�b�v�̍쐬
	descDepth.Width = quarityblur;
	descDepth.Height = quarityblur;

	descDepth.Format = DXGI_FORMAT_R32G32_TYPELESS;  // �t�H�[�}�b�g
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //�V�F�[�_ ���\�[�X �r���[�Ƃ��Ďg�p
	hr = renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // �쐬����2D�e�N�X�`���̐ݒ�
		NULL,               // 
		&ShadowMapingTextureY);     // �쐬�����e�N�X�`�����󂯎��ϐ�



	

	// �����_�[�^�[�Q�b�g�r���[�̐���


	hr = renderer->GetDevice()->CreateRenderTargetView(ShadowMapingTextureY, &rtvDesc, &RenderTargetShadowY);

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapingTextureY,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&srDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&ShadowMapSRViewY);  // �󂯎��ϐ�




		// �r���[�|�[�g�̐ݒ�
	ViewPortShadowMap[0].TopLeftX = 0.0f;		// �r���[�|�[�g�̈�̍���X���W�B
	ViewPortShadowMap[0].TopLeftY = 0.0f;		// �r���[�|�[�g�̈�̍���Y���W�B
	ViewPortShadowMap[0].Width = quarity;	// �r���[�|�[�g�̈�̕�
	ViewPortShadowMap[0].Height = quarity;	// �r���[�|�[�g�̈�̍���
	ViewPortShadowMap[0].MinDepth = 0.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	ViewPortShadowMap[0].MaxDepth = 1.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ő�l


	ViewPortShadowMap[1].TopLeftX = 0.0f;		// �r���[�|�[�g�̈�̍���X���W�B
	ViewPortShadowMap[1].TopLeftY = 0.0f;		// �r���[�|�[�g�̈�̍���Y���W�B
	ViewPortShadowMap[1].Width = quarityblur;	// �r���[�|�[�g�̈�̕�
	ViewPortShadowMap[1].Height = quarityblur;	// �r���[�|�[�g�̈�̍���
	ViewPortShadowMap[1].MinDepth = 0.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	ViewPortShadowMap[1].MaxDepth = 1.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ő�l




	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	renderer->GetDevice()->CreateBuffer(&bd, NULL, &this->VertexBuffer);

	D3D11_MAPPED_SUBRESOURCE msr;
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();
	renderer->GetDeviceContext()->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	// �w�肳�ꂽ���W�𒆐S�ɐݒ肷��v���O����

	// ���_�O�ԁi����̒��_�j
	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f,0.0f);

	// ���_�P�ԁi�E��̒��_�j
	vertex[1].Position = XMFLOAT3(quarityblur, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	// ���_�Q�ԁi�����̒��_�j
	vertex[2].Position = XMFLOAT3(0.0f, quarityblur, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	// ���_�R�ԁi�E���̒��_�j
	vertex[3].Position = XMFLOAT3(quarityblur, quarityblur, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	
	renderer->GetDeviceContext()->Unmap(VertexBuffer, 0);



}

void ShadowMapping::Uninit(void)
{

	if (m_VertexShaderShadow)			m_VertexShaderShadow->Release();
	if (m_PixelShaderShadow)			m_PixelShaderShadow->Release();
	if (m_VertexShaderShadow2D)			m_VertexShaderShadow2D->Release();
	if (m_PixelShaderShadowX)			m_PixelShaderShadowX->Release();
	if (m_PixelShaderShadowY)			m_PixelShaderShadowY->Release();
	if (ShadowMapDS) ShadowMapDS->Release();
	if (ShadowMapDSView) ShadowMapDSView->Release();
	if (ShadowMapDSSRView) ShadowMapDSSRView->Release();
	if (ShadowMapingTexture) ShadowMapingTexture->Release();
	if (RenderTargetShadow) RenderTargetShadow->Release();
	if (ShadowMapSRView) ShadowMapSRView->Release();
	if (ShadowMapingTextureX) ShadowMapingTextureX->Release();
	if (RenderTargetShadowX) RenderTargetShadowX->Release();
	if (ShadowMapSRViewX) ShadowMapSRViewX->Release();
	if (ShadowMapingTextureY) ShadowMapingTextureY->Release();
	if (RenderTargetShadowY) RenderTargetShadowY->Release();
	if (ShadowMapSRViewY) ShadowMapSRViewY->Release();
	if (VertexBuffer) VertexBuffer->Release();

}

void ShadowMapping::Update(void)
{
	Renderer* renderer= this->pLevel->GetMain()->GetRenderer();
	if (ShadowTarget!=nullptr)
	{
		this->at = ShadowTarget->GetTransFormComponent()->GetPosition();
		this->pos.x = at.x + dir.x * len;
		this->pos.y = at.y + dir.y * len;
		this->pos.z = at.z + dir.z * len;
	}
	
	
}

void ShadowMapping::Draw(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();
	this->SetShaderShadow();
	
	// �`��^�[�Q�b�g�̃N���A
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	// �w�i�F
	renderer->GetDeviceContext()->ClearRenderTargetView(
		RenderTargetShadow, // �N���A����`��^�[�Q�b�g
		ClearColor);         // �N���A����l

	ClearColor[0] = 0.0f;	// �w�i�F
	ClearColor[1] = 0.0f;	// �w�i�F


	// �[�x/�X�e���V���̃N���A
	renderer->GetDeviceContext()->ClearDepthStencilView(ShadowMapDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// RS�Ƀr���[�|�[�g��ݒ�
	renderer->GetDeviceContext()->RSSetViewports(1, ViewPortShadowMap);


	//�A���t�@�l���g���ꍇ
	renderer->GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetShadow, ShadowMapDSView);

	//�f�v�X�X�e���V���o�b�t�@�݂̂ŃV���h�E�}�b�s���O����ꍇ
	//ID3D11RenderTargetView* pRender[1] = { NULL };
	//renderer->GetDeviceContext()->OMSetRenderTargets(1, pRender, ShadowMapDSView);

	XMMATRIX mtxShadowMapView = XMMatrixLookAtLH(XMLoadFloat3(&this->pos), XMLoadFloat3(&this->at), XMLoadFloat3(&this->up));
	renderer->SetViewMatrix(&mtxShadowMapView);
	XMMATRIX mtxShadowMapProj = XMMatrixOrthographicLH(hw, hw, vNear, vFar);
	renderer->SetProjectionMatrix(&mtxShadowMapProj);

	XMMATRIX smwvp = XMMatrixTranspose(XMMatrixIdentity() * mtxShadowMapView * mtxShadowMapProj);

	XMStoreFloat4x4(&this->ShadowMap.SmView, mtxShadowMapView);
	XMStoreFloat4x4(&this->ShadowMap.SmProj, mtxShadowMapProj);
	XMStoreFloat4x4(&this->ShadowMap.SMWorldViewProj, smwvp);

	renderer->SetShadow(&this->ShadowMap);

	pLevel->DrawShadowObject();


	

	////�����_�[�^�[�Q�b�g����O���Ȃ��ƃV�F�[�_�[���\�[�X�Ƀo�C���h�ł��Ȃ�
	renderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);


	renderer->GetDeviceContext()->PSSetShaderResources(1, 1, &this->ShadowMapDSSRView);


	// RS�Ƀr���[�|�[�g��ݒ�
	renderer->GetDeviceContext()->RSSetViewports(1, &ViewPortShadowMap[1]);

	this->SetWorldViewProjection2D();


	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	renderer->SetCullingMode(CULL_MODE_NONE);


	for (int i = 0; i < 1; i++)
	{

		// �`��^�[�Q�b�g�̃N���A
		renderer->GetDeviceContext()->ClearRenderTargetView(
			RenderTargetShadowX, // �N���A����`��^�[�Q�b�g
			ClearColor);         // �N���A����l



		this->SetShaderXpass();




		if (i == 0)
		{
			renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &this->ShadowMapSRView);

		}
		else
		{

			renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &this->ShadowMapSRViewY);

		}
		renderer->GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetShadowX, nullptr);

		renderer->GetDeviceContext()->Draw(4, 0);

		this->SetShaderYpass();

		// �`��^�[�Q�b�g�̃N���A
		renderer->GetDeviceContext()->ClearRenderTargetView(
			RenderTargetShadowY, // �N���A����`��^�[�Q�b�g
			ClearColor);         // �N���A����l


		renderer->GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetShadowY, nullptr);


		renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &this->ShadowMapSRViewX);




		renderer->GetDeviceContext()->Draw(4, 0);



		////�����_�[�^�[�Q�b�g����O���Ȃ��ƃV�F�[�_�[���\�[�X�Ƀo�C���h�ł��Ȃ�
		renderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);


	}





	////�����_�[�^�[�Q�b�g����O���Ȃ��ƃV�F�[�_�[���\�[�X�Ƀo�C���h�ł��Ȃ�
	renderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);

	renderer->GetDeviceContext()->PSSetShaderResources(2, 1, &this->ShadowMapSRViewY);

}

void ShadowMapping::SetShadowMap(XMFLOAT3 pos, XMFLOAT3 at, XMFLOAT3 up)
{
	this->pos = pos;
	this->at = at;
	this->up = up;
}

void ShadowMapping::SetPos(XMFLOAT3 pos)
{
	this->pos = pos;
}

void ShadowMapping::SetShadowBuffer(void)
{
	
	this->pLevel->GetMain()->GetRenderer()->SetShadow(&this->ShadowMap);


}
void ShadowMapping::SetShaderShadow(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();

	renderer->GetDeviceContext()->VSSetShader(m_VertexShaderShadow, NULL, 0);
	renderer->GetDeviceContext()->PSSetShader(m_PixelShaderShadow, NULL, 0);

}
void ShadowMapping::SetShaderXpass(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();

	renderer->GetDeviceContext()->VSSetShader(m_VertexShaderShadow2D, NULL, 0);
	renderer->GetDeviceContext()->PSSetShader(m_PixelShaderShadowX, NULL, 0);

}
void ShadowMapping::SetShaderYpass(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();

	renderer->GetDeviceContext()->VSSetShader(m_VertexShaderShadow2D, NULL, 0);
	renderer->GetDeviceContext()->PSSetShader(m_PixelShaderShadowY, NULL, 0);

}
void ShadowMapping::SetWorldViewProjection2D(void)
{
	Renderer* renderer = this->pLevel->GetMain()->GetRenderer();

	XMMATRIX world = XMMatrixIdentity();
	renderer->SetWorldMatrix(&world);

	XMMATRIX view = XMMatrixIdentity();
	renderer->SetViewMatrix(&view);


	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, this->quarityblur, this->quarityblur, 0.0f, 0.0f, 1.0f);
	renderer->SetProjectionMatrix(&projection);

}

void ShadowMapping::SetShadowMode(SHADOW_MODE mode)
{
	this->ShadowMap.mode = mode;
}

void ShadowMapping::SetTarget(GameObject* gameObject)
{
	this->ShadowTarget = gameObject;
}

void ShadowMapping::SetForward(XMFLOAT3 dir)
{

	this->dir = XMFLOAT3Normalize(dir);
	

}

void ShadowMapping::SetLen(float len)
{
	this->len = len;
}

void ShadowMapping::SetNear(float f)
{
	this->vNear = f;
}

void ShadowMapping::SetFar(float f)
{
	this->vFar = f;

}

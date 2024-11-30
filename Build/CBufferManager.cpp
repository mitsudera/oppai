#include "CBufferManager.h"
#include "renderer.h"

CBufferManager::CBufferManager(Renderer* renderer)
{
	pRenderer = renderer;
	pDeviceContext = renderer->GetDeviceContext();
	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	renderer->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->WorldBuffer);
	SetWorldBuffer(this->WorldBuffer);

	renderer->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->ViewBuffer);
	SetViewBuffer(this->ViewBuffer);

	renderer->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->ProjectionBuffer);
	SetProjectionBuffer(this->ProjectionBuffer);
}

CBufferManager::~CBufferManager()
{
	if (WorldBuffer) WorldBuffer->Release();
	if (ViewBuffer) ViewBuffer->Release();
	if (ProjectionBuffer) ProjectionBuffer->Release();

}


void CBufferManager::SetCBufferOtherCS(ID3D11Buffer* buffer, BufferSlot slot)
{
	pDeviceContext->VSSetConstantBuffers((UINT)slot, 1, &buffer);
	pDeviceContext->HSSetConstantBuffers((UINT)slot, 1, &buffer);
	pDeviceContext->DSSetConstantBuffers((UINT)slot, 1, &buffer);
	pDeviceContext->GSSetConstantBuffers((UINT)slot, 1, &buffer);
	pDeviceContext->PSSetConstantBuffers((UINT)slot, 1, &buffer);

}

void CBufferManager::SetCBufferVSPS(ID3D11Buffer* buffer, BufferSlot slot)
{
	pDeviceContext->VSSetConstantBuffers((UINT)slot, 1, &buffer);
	pDeviceContext->PSSetConstantBuffers((UINT)slot, 1, &buffer);

}

void CBufferManager::SetCBufferCS(ID3D11Buffer* buffer, BufferSlot slot)
{
	pDeviceContext->CSSetConstantBuffers((UINT)slot, 1, &buffer);

}

void CBufferManager::SetWorldMtx(XMMATRIX* world)
{
	XMMATRIX mtx = XMMatrixTranspose(*world);

	pDeviceContext->UpdateSubresource(WorldBuffer, 0, NULL, &mtx, 0, 0);
}

void CBufferManager::SetViewMtx(XMMATRIX* view)
{
	XMMATRIX mtx = XMMatrixTranspose(*view);

	pDeviceContext->UpdateSubresource(ViewBuffer, 0, NULL, &mtx, 0, 0);

}

void CBufferManager::SetProjectionMtx(XMMATRIX* projection)
{
	XMMATRIX mtx = XMMatrixTranspose(*projection);

	pDeviceContext->UpdateSubresource(ProjectionBuffer, 0, NULL, &mtx, 0, 0);

}

void CBufferManager::SetWorldBuffer(ID3D11Buffer* world)
{
	SetCBufferVSPS(world, BufferSlot::World);
}

void CBufferManager::SetViewBuffer(ID3D11Buffer* view)
{
	SetCBufferVSPS(view, BufferSlot::View);

}

void CBufferManager::SetProjectionBuffer(ID3D11Buffer* projection)
{
	SetCBufferVSPS(projection, BufferSlot::Projection);

}

void CBufferManager::SetMaterialBuffer(ID3D11Buffer* material)
{
	SetCBufferVSPS(material, BufferSlot::Material);

}

void CBufferManager::SetLightBuffer(ID3D11Buffer* light)
{
	SetCBufferVSPS(light, BufferSlot::Light);

}

void CBufferManager::SetCameraBuffer(ID3D11Buffer* camera)
{
	SetCBufferVSPS(camera, BufferSlot::Camera);

}

void CBufferManager::SetShadowBuffer(ID3D11Buffer* shadow)
{
	SetCBufferVSPS(shadow, BufferSlot::Shadow);

}

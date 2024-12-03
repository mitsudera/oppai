#include "SkyShader.h"
#include "renderer.h"

SkyShader::SkyShader(Renderer* renderer)
{
	pRenderer = renderer;
	name = "SkyShader";
	CreateVS("shaders/SkyShader.hlsl", "VSmain");
	CreatePS("shaders/SkyShader.hlsl", "PSmain");

    shaderIndex = ShaderIndex::Sky;
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.ByteWidth = sizeof(MaterialCBuffer);
    bufferDesc.StructureByteStride = sizeof(float);

    HRESULT hr = pRenderer->GetDevice()->CreateBuffer(&bufferDesc, NULL, &materialBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create buffer");
    }

}

SkyShader::~SkyShader()
{
	ShaderRelease();
    if (materialBuffer) materialBuffer->Release();
}

void SkyShader::SetMaterialCbuffer(SkyShader::MaterialCBuffer data)
{
	pRenderer->GetDeviceContext()->UpdateSubresource(this->materialBuffer, 0, nullptr, &data, 0, 0);
}

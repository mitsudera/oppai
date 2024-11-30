#include "ShaderSet.h"
#include "renderer.h"
#include "CBufferManager.h"

ShaderSet::ShaderSet()
{
}

ShaderSet::~ShaderSet()
{
	ShaderRelease();
}

void ShaderSet::SetShaderRenderer(void)
{

	if (!pRenderer) return;
	//���_�V�F�[�_�[�� null �ɐݒ�
	pRenderer->GetDeviceContext()->VSSetShader(nullptr, NULL, 0);
	//�n���V�F�[�_�[�� null �ɐݒ�
	pRenderer->GetDeviceContext()->HSSetShader(nullptr, NULL, 0);
	//�h���C���V�F�[�_�[�� null �ɐݒ�
	pRenderer->GetDeviceContext()->DSSetShader(nullptr, NULL, 0);
	//�W�I���g���V�F�[�_�[�� null �ɐݒ�
	pRenderer->GetDeviceContext()->GSSetShader(nullptr, NULL, 0);
	//�s�N�Z���V�F�[�_�[�� null �ɐݒ�
	pRenderer->GetDeviceContext()->PSSetShader(nullptr, NULL, 0);


	//���_���̓��C�A�E�g���Z�b�g
	if (this->VertexLayout) pRenderer->GetDeviceContext()->IASetInputLayout(this->VertexLayout);
	//���_�V�F�[�_�[���Z�b�g
	if (this->VS) pRenderer->GetDeviceContext()->VSSetShader(this->VS, NULL, 0);
	//�n���V�F�[�_�[���Z�b�g
	if (this->HS) pRenderer->GetDeviceContext()->HSSetShader(this->HS, NULL, 0);
	//�h���C���V�F�[�_�[���Z�b�g
	if (this->DS) pRenderer->GetDeviceContext()->DSSetShader(this->DS, NULL, 0);
	//�W�I���g���V�F�[�_�[���Z�b�g
	if (this->GS) pRenderer->GetDeviceContext()->GSSetShader(this->GS, NULL, 0);
	//�s�N�Z���V�F�[�_�[���Z�b�g
	if (this->PS) pRenderer->GetDeviceContext()->PSSetShader(this->PS, NULL, 0);

	pRenderer->GetDeviceContext()->PSSetConstantBuffers((UINT)CBufferManager::BufferSlot::Material, 1, &this->materialBuffer);
}

void ShaderSet::ShaderRelease(void)
{
	if (VS) VS->Release();
	if (HS) HS->Release();
	if (DS) DS->Release();
	if (GS) GS->Release();
	if (PS) PS->Release();

}

void ShaderSet::CreateVS(string filePath, string shaderName)
{
	// ���_�V�F�[�_�R���p�C���E����
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pVSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) && defined(DEBUG_SHADER)
	shFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DX11CompileFromFile(shaderName.c_str(), NULL, NULL, filePath.c_str(), "vs_4_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	}

	pRenderer->GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &this->VS);


	// ���̓��C�A�E�g����
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	pRenderer->GetDevice()->CreateInputLayout(layout,
		numElements,
		(pVSBlob)->GetBufferPointer(),
		(pVSBlob)->GetBufferSize(),
		&VertexLayout);

	(pVSBlob)->Release();

}

void ShaderSet::CreateHS(string filePath, string shaderName)
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pHSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr = D3DX11CompileFromFile(shaderName.c_str(), NULL, NULL, filePath.c_str(), "hs_4_0", shFlag, 0, NULL, &pHSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "HS", MB_OK | MB_ICONERROR);
	}

	pRenderer->GetDevice()->CreateHullShader(pHSBlob->GetBufferPointer(), pHSBlob->GetBufferSize(), NULL, &this->HS);
	//���
	pHSBlob->Release();

}

void ShaderSet::CreateDS(string filePath, string shaderName)
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pDSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr = D3DX11CompileFromFile(shaderName.c_str(), NULL, NULL, filePath.c_str(), "ds_4_0", shFlag, 0, NULL, &pDSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "DS", MB_OK | MB_ICONERROR);
	}

	pRenderer->GetDevice()->CreateDomainShader(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), NULL, &this->DS);
	//���
	pDSBlob->Release();

}

void ShaderSet::CreateGS(string filePath, string shaderName)
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pGSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr = D3DX11CompileFromFile(shaderName.c_str(), NULL, NULL, filePath.c_str(), "gs_4_0", shFlag, 0, NULL, &pGSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "GS", MB_OK | MB_ICONERROR);
	}

	pRenderer->GetDevice()->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), NULL, &this->GS);
	//���
	pGSBlob->Release();

}

void ShaderSet::CreatePS(string filePath, string shaderName)
{

	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pPSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr = D3DX11CompileFromFile(shaderName.c_str(), NULL, NULL, filePath.c_str(), "ps_4_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}

	pRenderer->GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &this->PS);
	//���
	pPSBlob->Release();

}

ShaderSet::ShaderIndex ShaderSet::GetShaderIndex(void)
{
	return this->shaderIndex;
}


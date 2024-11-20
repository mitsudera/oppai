#include "SquareParticle.h"
#include "CameraComponent.h"
#include "camera.h"

#define PART_MAX (8192)

#define TEXTUE_PATH "data/PARTICLE/partical.png"

SquareParticle::SquareParticle(Level* level)
{

	this->pLevel = level;
	maxPart = PART_MAX;
	vertNum = maxPart * 4;
	indexNum = maxPart * 6;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	squareInfoArray = new SquareInfo[maxPart];
	indexArray = new unsigned int[indexNum];
	vertexArray = new VERTEX_3D[vertNum];
	texture = nullptr;
}

SquareParticle::~SquareParticle()
{
	delete[] squareInfoArray;
	delete[] indexArray;
	delete[] vertexArray;
}

void SquareParticle::Init(void)
{
	blend = BLEND_MODE::BLEND_MODE_ADD;

	D3DX11CreateShaderResourceViewFromFile(GetLevel()->GetMain()->GetRenderer()->GetDevice(),
		TEXTUE_PATH,
		NULL,
		NULL,
		&texture,
		NULL);


	for (unsigned int i = 0; i < maxPart; i++)
	{
		squareInfoArray[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		squareInfoArray[i].size = 0.0f;
		squareInfoArray[i].use = FALSE;

		squareInfoArray[i].vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
		squareInfoArray[i].vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
		squareInfoArray[i].vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
		squareInfoArray[i].vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


		for (int j = 0; j < 4; j++)
		{
			squareInfoArray[i].vertex[j].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			squareInfoArray[i].vertex[j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			squareInfoArray[i].vertex[j].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

			

			vertexArray[i * 4 + j] = squareInfoArray[i].vertex[j];


		}

	}




	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetLevel()->GetMain()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);


	D3D11_MAPPED_SUBRESOURCE msrV;
	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrV);

	VERTEX_3D* pvtx = (VERTEX_3D*)msrV.pData;

	memcpy(pvtx, vertexArray, sizeof(VERTEX_3D) * vertNum);


	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);




	// �C���f�b�N�X�o�b�t�@����
	
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * indexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetLevel()->GetMain()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->indexBuffer);


	for (unsigned int i = 0; i < maxPart; i++)
	{
		indexArray[i * 6] = i * 4;
		indexArray[i * 6 + 1] = i * 4 + 1;
		indexArray[i * 6 + 2] = i * 4 + 2;
		indexArray[i * 6 + 3] = i * 4 + 1;
		indexArray[i * 6 + 4] = i * 4 + 2;
		indexArray[i * 6 + 5] = i * 4 + 3;
	}


	D3D11_MAPPED_SUBRESOURCE msrI;
	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrI);

	unsigned int* pIdx = (unsigned int*)msrI.pData;

	memcpy(pIdx, indexArray, sizeof(unsigned int) * indexNum);

	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->indexBuffer, 0);





}

void SquareParticle::Uninit(void)
{
	if (vertexBuffer) vertexBuffer->Release();
	vertexBuffer = nullptr;
	if (indexBuffer) indexBuffer->Release();
	indexBuffer = nullptr;
}

void SquareParticle::Update(void)
{
	

	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view,GetLevel()->GetCamera()->GetView());

	XMFLOAT3 vx = XMFLOAT3(view._11, view._21, view._31);
	XMVECTOR axx = XMLoadFloat3(&vx);
	XMFLOAT3 vy = XMFLOAT3(view._12, view._22, view._32);
	XMVECTOR axy = XMLoadFloat3(&vy);
	

	for (unsigned int i = 0; i < maxPart; i++)
	{
		float size = squareInfoArray[i].size;
		XMVECTOR vvec[4];
		XMVECTOR posv = XMLoadFloat3(&squareInfoArray[i].pos);





		vvec[0] = axy * size;
		vvec[1] = axx * size;
		vvec[2] = -axx * size;
		vvec[3] = -axy * size;
		


		XMFLOAT3 vpos[4];


		for (int j = 0; j < 4; j++)
		{
			vvec[j] += posv;
			XMStoreFloat3(&vpos[j], vvec[j]);

		}





		for (int j = 0; j < 4; j++)
		{
			squareInfoArray[i].vertex[j].Position = vpos[j];
		}


	}


	for (unsigned int i = 0; i < maxPart; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			int n = i * 4 + j;
			vertexArray[i * 4 + j] = squareInfoArray[i].vertex[j];
		}
	}

	D3D11_MAPPED_SUBRESOURCE msrV;
	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrV);

	VERTEX_3D* pvtx = (VERTEX_3D*)msrV.pData;

	memcpy(pvtx, vertexArray, sizeof(VERTEX_3D) * vertNum);


	this->GetLevel()->GetMain()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);


}

void SquareParticle::Draw(void)
{
	Renderer* renderer = GetLevel()->GetMain()->GetRenderer();

	renderer->SetCullingMode(CULL_MODE_NONE);

	// ���C�e�B���O�𖳌���
	renderer->SetLightEnable(FALSE);

	// ���Z�����ɐݒ�
	renderer->SetBlendState(this->blend);

	renderer->SetAlphaTestEnable(FALSE);

	// Z��r����
	renderer->SetDepthEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	renderer->GetDeviceContext()->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f);
	renderer->SetMaterial(material);

	// �e�N�X�`���ݒ�
	renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);




	// ���[���h�}�g���b�N�X�̏�����
	XMMATRIX mtxWorld = XMMatrixIdentity();




	// ���[���h�}�g���b�N�X�̐ݒ�
	renderer->SetWorldMatrix(&mtxWorld);



	// �|���S���`��
	renderer->GetDeviceContext()->DrawIndexed(indexNum, 0, 0);	


		// ���C�e�B���O��L����
	renderer->SetLightEnable(TRUE);

	// �ʏ�u�����h�ɖ߂�
	renderer->SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	renderer->SetDepthEnable(TRUE);

}

int SquareParticle::AddParticle(XMFLOAT3 pos, float size,float alpha)
{
	for (unsigned int i = 0; i < maxPart; i++)
	{
		if (squareInfoArray[i].use == FALSE)
		{
			squareInfoArray[i].pos = pos;
			squareInfoArray[i].size = size;
			squareInfoArray[i].vertex[0].Diffuse.w = alpha;
			squareInfoArray[i].vertex[1].Diffuse.w = alpha;
			squareInfoArray[i].vertex[2].Diffuse.w = alpha;
			squareInfoArray[i].vertex[3].Diffuse.w = alpha;
			squareInfoArray[i].use = TRUE;
			return i;
		}
	}
	return -1;
}

void SquareParticle::DeleteParticle(int n)
{
	squareInfoArray[n].pos =XMFLOAT3(0.0f,0.0f,0.0f);
	squareInfoArray[n].size = 0.0f;
	squareInfoArray[n].use = FALSE;
}

void SquareParticle::UpdatePositon(int n, XMFLOAT3 pos)
{
	squareInfoArray[n].pos = pos;
}

void SquareParticle::UpdateSize(int n, float size)
{
	squareInfoArray[n].size = size;
}

void SquareParticle::SetBlend(BLEND_MODE blend)
{
	this->blend = blend;
}

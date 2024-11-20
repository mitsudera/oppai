#include "MeshComponent.h"
#include "GameEngine.h"
#include "Renderer.h"
#include "AssetsManager.h"
#include "SkinMeshDataList.h"
#include "World.h"
#include "SkinMeshComponent.h"
#include "SkeletonAnimData.h"
#include "DX11Texture.h"



SkinMeshComponent::SkinMeshComponent()
{
	this->blendBoneMtx = nullptr;
	this->boneCnt = 0;
	this->meshVertex = nullptr;
	this->boneMtx = nullptr;
	this->boneBuffer = nullptr;
	this->skinMeshMathBuffer = nullptr;

}

SkinMeshComponent::~SkinMeshComponent()
{
}

void SkinMeshComponent::Init(void)
{
	MeshComponent::Init();
	SetName("SkinMesh");

}

void SkinMeshComponent::Update(void)
{
	PrimitiveComponent::Update();
	float dTime = GetWorld()->GetGameEngine()->GetDeltaTime();//1.0=1秒
	float dFrame = dTime * 60.0f;


	if (animstate == ANIM_STATE::NO_ANIM)
	{



	}
	if (animstate == ANIM_STATE::DATA_ANIM)
	{
		
		int frame1 = (int)(framecnt);
		int frame2 = (int)(framecnt)+1;
		if (frame2 >= framenum)
		{
			frame2 = 0;
		}
		float w2 = framecnt - (float)(int)(framecnt);
		float w1 = 1.0f - w2;


		for (int i = 0; i < this->boneCnt; i++)
		{

			XMMATRIX mtx1= this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex])->GetFrameSkeleton(frame1)->GetBone(i)->GetMtx();
			XMMATRIX mtx2= this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex])->GetFrameSkeleton(frame2)->GetBone(i)->GetMtx();
			this->boneMtx[i] = (mtx1 * w1) + (mtx2 * w2);


		}

		UpdateSkinMeshForCS();

		framecnt += animSpeed*dFrame;
		if (framecnt >= framenum)
		{
			framecnt = framecnt - (float)framenum;

			if (isOneTime)
			{
				SwichAnimIndex(defaultAnimIndex);
			}
			else if (frameBlendMode)
			{
				SwichAnimIndex(this->animindex);
			}


		}


	}
	else if (animstate == ANIM_STATE::BLEND_ANIM)
	{
		float blendweight1 = (1.0f / blendcntmax) * blendcnt;
		float blendweight2 = 1.0f - blendweight1;


		int frame1 = (int)(framecnt);
		int frame2 = (int)(framecnt)+1;
		if (frame2 >= framenum)
		{
			frame2 = 0;
		}
		float w2 = framecnt - (float)(int)(framecnt);
		float w1 = 1.0f - w2;


		for (int i = 0; i < this->boneCnt; i++)
		{
			XMMATRIX fmtx1= this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex])->GetFrameSkeleton(frame1)->GetBone(i)->GetMtx() * blendweight1;
			XMMATRIX fmtx2= this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex])->GetFrameSkeleton(frame2)->GetBone(i)->GetMtx() * blendweight1;

			XMMATRIX mtx1 = (fmtx1 * w1) + (fmtx2 * w2);
			XMMATRIX mtx2 = this->blendBoneMtx[i] * blendweight2;
			
			this->boneMtx[i] = mtx1 + mtx2;




		}


		UpdateSkinMeshForCS();



		blendcnt += dFrame;
		if (blendcnt >= blendcntmax)
		{
			animstate = ANIM_STATE::DATA_ANIM;
		}


	}
	else if (animstate == ANIM_STATE::CROSSFADE_ANIM)
	{
		float blendweight1 = (1.0f / blendcntmax) * blendcnt;
		float blendweight2 = 1.0f - blendweight1;


		int frame1 = (int)(framecnt);
		int frame2 = (int)(framecnt)+1;
		if (frame2 >= framenum)
		{
			frame2 = 0;
		}
		float w2 = framecnt - (float)(int)(framecnt);
		float w1 = 1.0f - w2;


		for (int i = 0; i < this->boneCnt; i++)
		{
			XMMATRIX fmtx1= this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex])->GetFrameSkeleton(frame1)->GetBone(i)->GetMtx() * blendweight1;
			XMMATRIX fmtx2= this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[animindex])->GetFrameSkeleton(frame2)->GetBone(i)->GetMtx() * blendweight1;

			XMMATRIX mtx1 = (fmtx1 * w1) + (fmtx2 * w2);
			XMMATRIX mtx2 = this->blendBoneMtx[i] * blendweight2;
			
			this->boneMtx[i] = mtx1 + mtx2;




		}


		UpdateSkinMeshForCS();



		blendcnt += dFrame;
		if (blendcnt >= blendcntmax)
		{
			animstate = ANIM_STATE::DATA_ANIM;
		}
		framecnt += animSpeed * dFrame;
		if (framecnt >= framenum)
		{
			framecnt = framecnt - (float)framenum;
		}


	}


}

void SkinMeshComponent::Uninit(void)
{
	MeshComponent::Uninit();
	if (boneBuffer) boneBuffer->Release();
	if (skinMeshMathBuffer) skinMeshMathBuffer->Release();
	if (meshVertex) delete[] meshVertex;


}

void SkinMeshComponent::BeginPlay(void)
{
	MeshComponent::BeginPlay();
}

void SkinMeshComponent::UpdatePlay(void)
{
	MeshComponent::UpdatePlay();
}

void SkinMeshComponent::EndPlay(void)
{
	MeshComponent::EndPlay();
}

void SkinMeshComponent::Draw(void)
{
	PrimitiveComponent::Draw();
	this->GetWorld()->GetGameEngine()->GetRenderer()->SetLightEnable(TRUE);
	this->GetWorld()->GetGameEngine()->GetRenderer()->SetCullingMode(CULL_MODE_NONE);


	for (int i = 0; i < this->meshNum; i++)
	{
		DrawSkinMesh(i);
	}

	//this->GetWorld()->GetGameEngine()->GetRenderer()->SetLightEnable(FALSE);
}

void SkinMeshComponent::DrawSkinMesh(int n)
{
	MeshDataList* list = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex);
	
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->meshVertex[n].m_vertexBuffer, &stride, &offset);


	this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].BufferSetIndex();

	XMMATRIX world = XMMatrixIdentity();
	world = XMMatrixMultiply(world, this->MeshMtxArray[n]);
	world = XMMatrixMultiply(world, GetWorldMatrix());
	this->GetWorld()->GetGameEngine()->GetRenderer()->SetWorldMatrix(&world);


	//マテリアルテクスチャ設定
	int subsetnum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].GetSubsetNum();

	for (unsigned short i = 0; i < subsetnum; i++)
	{
		MATERIAL m = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].GetSubset()[0].GetMaterial();


		this->GetWorld()->GetGameEngine()->GetRenderer()->SetMaterial(m);


		this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].GetSubset()[i].SetShaderResouce();
		
		if (isOriginalDiffuse)
		{
			this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetTexture(diffuseIndex)->SetShaderResource(0);
		}


		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->DrawIndexed(this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[n].GetIndexNum(), 0, 0);
	}

}

void SkinMeshComponent::SetSkinMeshStatus(string meshfilepath, vector<string> animfilepatharray, BOOL blend, int blendframe)
{
	
	SetMeshFilePath(meshfilepath);
	SetAnimFilePathArray(animfilepatharray);


	motionblend = blend;
	blendcnt = 0;
	blendcntmax = blendframe;
	animation = TRUE;
	animstate = ANIM_STATE::DATA_ANIM;
	animindex = 0;

}
void SkinMeshComponent::SetSkinMeshStatus(string meshfilepath)
{
	
	SetMeshFilePath(meshfilepath);


	blendcnt = 0;
	animation = FALSE;
	animstate = ANIM_STATE::NO_ANIM;
	animindex = 0;

}

void SkinMeshComponent::SetSkinMesh(void)
{

	ID3D11Device* device = GetWorld()->GetGameEngine()->GetRenderer()->GetDevice();


	this->MeshDataListIndex = this->GetWorld()->GetGameEngine()->GetAssetsManager()->LoadSkinMesh(this->meshFilePath);
	this->CreateMeshMtxArray(this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetMeshDataNum());

	this->meshVertex = new MeshVertex[this->meshNum];
	this->boneCnt = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkeleton()->GetBoneCount();
	this->boneMtx = new XMMATRIX[boneCnt];
	this->blendBoneMtx = new XMMATRIX[boneCnt];
	for (int i = 0; i < boneCnt; i++)
	{
		this->boneMtx[i] = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkeleton()->GetBone(i)->GetMtx();

	}
	SetBlendBone();

	for (int i = 0; i < this->meshNum; i++)
	{

		MeshMtxArray[i] = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[i].GetOffset();
		int vertNum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[i].GetVertNum();
		this->meshVertex[i].vertexArray = new VERTEX_3D[vertNum];
		this->meshVertex[i].vertNum = vertNum;
		for (int j = 0; j < vertNum; j++)
		{
			this->meshVertex[i].vertexArray[j] = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData()[i].GetVertex(j);
		}



		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * vertNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->meshVertex[i].m_vertexBuffer);

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->meshVertex[i].m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, this->meshVertex[i].vertexArray, sizeof(VERTEX_3D) * vertNum);

		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->meshVertex[i].m_vertexBuffer, 0);

		

		SkinMeshData* skinMesh = GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)[i].GetSkinMeshData();


		CP* cp = new CP[skinMesh->GetControlNum()];
		for (int j = 0; j < skinMesh->GetControlNum(); j++)
		{
			cp[j].pos = skinMesh->GetControlPoint(j)->pos;
			for (int k = 0; k < BONE_MAX; k++)
			{
				cp[j].weight[k] = 0.0f;
			}
			for (int k = 0; k < this->boneCnt; k++)
			{
				cp[j].weight[k] = skinMesh->GetControlPoint(j)->weight[k];
			}
			
			int cnt = 0;
			for (int k = 0; k < this->boneCnt; k++)
			{
				if (cp[j].weight[k]>0.00001f)
				{
					cp[j].showBoneIndex[cnt] = k;
					cnt++;
				}
			}

			cp[j].showBoneNum = cnt;

		}


		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.BindFlags =
			D3D11_BIND_UNORDERED_ACCESS |                               // アンオーダード アクセス リソースをバインドする
			D3D11_BIND_SHADER_RESOURCE;                                 // バッファーをシェーダー ステージにバインドする
		bufferDesc.ByteWidth = sizeof(CP) * skinMesh->GetControlNum();                  // バッファサイズ
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;  // 構造化バッファーとしてリソースを作成する
		bufferDesc.StructureByteStride = sizeof(CP);                 // 構造化バッファーのサイズ (バイト単位)

		D3D11_SUBRESOURCE_DATA initData;

		initData.pSysMem = cp;
		device->CreateBuffer(&bufferDesc, &initData, &meshVertex[i].controlPointBuffer);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.NumElements = skinMesh->GetControlNum();

		device->CreateShaderResourceView(meshVertex[i].controlPointBuffer, &srvDesc, &meshVertex[i].controlPointSRV);




		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(XMFLOAT3) * skinMesh->GetControlNum();
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;  // 構造化バッファーとしてリソースを作成する
		bufferDesc.StructureByteStride = sizeof(XMFLOAT3);                 // 構造化バッファーのサイズ (バイト単位)
		device->CreateBuffer(&bufferDesc, nullptr, &meshVertex[i].outputBuffer);

		// UAVの作成
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = skinMesh->GetControlNum();

		HRESULT hr= device->CreateUnorderedAccessView(meshVertex[i].outputBuffer, &uavDesc, &meshVertex[i].outputUAV);

	}
	this->SetBlendMtxArray();

	D3D11_BUFFER_DESC bufferDesc = {};

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(XMMATRIX) * BONE_MAX;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	device->CreateBuffer(&bufferDesc, nullptr, &boneBuffer);



	SkinMeshMath smm;
	ZeroMemory(&smm, sizeof(SkinMeshMath));

	smm.BoneNum = boneCnt;

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(SkinMeshMath);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	device->CreateBuffer(&bufferDesc, nullptr, &skinMeshMathBuffer);
	GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->UpdateSubresource(skinMeshMathBuffer, 0, NULL, &smm, 0, 0);


	if (animation==TRUE)
	{
		for (int i = 0; i < this->animFilePath.size(); i++)
		{
			int n = this->GetWorld()->GetGameEngine()->GetAssetsManager()->LoadSkeletonAnimData(this->animFilePath[i]);
			this->AnimDataIndexArray.push_back(n);
		}
		framecnt = 0;
		framenum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(this->AnimDataIndexArray[this->animindex])->GetFrameNum();

	}




}

void SkinMeshComponent::SetBlendBone(void)
{

	for (int i = 0; i < boneCnt; i++)
	{
		this->blendBoneMtx[i] = this->boneMtx[i];
	}
}

void SkinMeshComponent::SwichAnimIndex(int n)
{
	animindex = n;
	framecnt = 0;
	framenum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(AnimDataIndexArray[n])->GetFrameNum();
	if (motionblend)
	{
		SetBlendMtxArray();
		SetBlendBone();
		blendcnt = 0;
		if (crossFadeMode)
		{
			animstate = ANIM_STATE::CROSSFADE_ANIM;
		}
		else
		{
			animstate = ANIM_STATE::BLEND_ANIM;

		}

	}

}

void SkinMeshComponent::StartOneTimeAnimIndex(int n)
{

	animindex = n;
	framecnt = 0;
	framenum = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkeletonAnimData(AnimDataIndexArray[n])->GetFrameNum();
	SetBlendMtxArray();
	SetBlendBone();
	blendcnt = 0;
	isOneTime = TRUE;

	animstate = ANIM_STATE::BLEND_ANIM;


}

void SkinMeshComponent::UpdateSkinMeshForCS(void)
{
	XMMATRIX* bone = new XMMATRIX[BONE_MAX];


	ZeroMemory(bone, sizeof(XMMATRIX) * BONE_MAX);

	for (int i = 0; i < boneCnt; i++)
	{
		bone[i] = XMMatrixMultiply(this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkeleton()->GetBone(i)->GetInitMtxInverse(), boneMtx[i]);

		bone[i] = XMMatrixTranspose(bone[i]);
	}
	
	ID3D11DeviceContext* context = this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext();

	context->UpdateSubresource(boneBuffer, 0, NULL, bone, 0, 0);
	context->CSSetConstantBuffers(0, 1, &boneBuffer);
	context->CSSetConstantBuffers(1, 1, &skinMeshMathBuffer);

	delete[] bone;

	for (int i = 0; i < this->meshNum; i++)
	{
		SkinMeshData* skinmeshdata = this->GetWorld()->GetGameEngine()->GetAssetsManager()->GetSkinMeshDataList(this->MeshDataListIndex)->GetSkinMeshData();

		int controlN = skinmeshdata[i].GetControlNum();

		// シェーダーリソースのバインド
		this->GetWorld()->GetGameEngine()->GetAssetsManager()->SetSkinMeshCompute();
		context->CSSetShaderResources(0, 1, &meshVertex[i].controlPointSRV);
		context->CSSetUnorderedAccessViews(0, 1, &meshVertex[i].outputUAV, nullptr);

		// コンピュートシェーダーの実行
		context->Dispatch(controlN, 1, 1);

		ID3D11Buffer* resultBuf = NULL;

		// バッファの記述
		D3D11_BUFFER_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
		meshVertex[i].outputBuffer->GetDesc(&BufferDesc);
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;  // CPUからの読み取りを許可
		BufferDesc.Usage = D3D11_USAGE_STAGING;             // GPUからCPUへのデータ転送をサポート
		BufferDesc.BindFlags = 0;
		BufferDesc.MiscFlags = 0;

		// ステージングバッファの作成
		HRESULT hr = this->GetWorld()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&BufferDesc, NULL, &resultBuf);
		if (FAILED(hr)) {
			// エラー処理
		}

		// リソースのコピー
		context->CopyResource(resultBuf, meshVertex[i].outputBuffer);

		XMFLOAT3* cpPos = nullptr;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context->Map(resultBuf, 0, D3D11_MAP_READ, 0, &mappedResource);
		cpPos = (XMFLOAT3*)mappedResource.pData;




		for (int j = 0; j < this->meshVertex[i].vertNum; j++)
		{
			this->meshVertex[i].vertexArray[j].Position = cpPos[skinmeshdata[i].GetCPIndexArray()[j]];

		}

		context->Unmap(resultBuf, 0);
		resultBuf->Release();

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->meshVertex[i].m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, this->meshVertex[i].vertexArray, sizeof(VERTEX_3D) * this->meshVertex[i].vertNum);

		this->GetWorld()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->meshVertex[i].m_vertexBuffer, 0);



		this->meshVertex[i].vertexArray;

	}

}

int SkinMeshComponent::GetAnimIndex(void)
{
	return animindex;
}


MeshVertex::MeshVertex()
{
	this->controlPointSRV = nullptr;
	this->controlPointBuffer = nullptr;
	this->m_vertexBuffer = nullptr;
	this->outputBuffer = nullptr;
	this->outputUAV = nullptr;
	this->vertNum = 0;
	this->vertexArray = nullptr;

}

MeshVertex::~MeshVertex()
{
	if (controlPointBuffer) controlPointBuffer->Release();
	if (m_vertexBuffer) m_vertexBuffer->Release();
	if (vertexArray) delete vertexArray;
	if (outputBuffer) outputBuffer->Release();
	if (outputUAV) outputUAV->Release();
	if (controlPointSRV) controlPointSRV->Release();
}

#pragma once
#include "MeshComponent.h"
#include "GameEngine.h"
#include "Renderer.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "MeshAnimData.h"
#include "DX11Texture.h"
#include "gameobject.h"
#include "GameScene.h"
#include "CBufferManager.h"
#include "Material.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "transformcomponent.h"

#define TEXTURE_PATH	"data/TEXTURE/"


MeshComponent::MeshComponent()
{
	animation = FALSE;
	animindex = 0;
	animstate = ANIM_STATE::NO_ANIM;
	blendcnt = 0;
	blendcntmax = 0;
	framecnt = 0;
	framenum = 0;
	lastanimindex = 0;
	meshNum = 0;
	MeshDataIndex = 0;
	frameBlendMode = TRUE;
	cullMode = CULL_MODE::CULL_MODE_FRONT;
	defaultAnimIndex = 0;

	crossFadeMode = TRUE;
	motionblend = FALSE;


	//shader
	this->alphaTest = FALSE;
	m_fuchi = FALSE;

}

MeshComponent::MeshComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
	attribute = Attribute::Primitive;

	animation = FALSE;
	animindex = 0;
	animstate = ANIM_STATE::NO_ANIM;
	blendcnt = 0;
	blendcntmax = 0;
	framecnt = 0;
	framenum = 0;
	lastanimindex = 0;
	meshNum = 0;
	MeshDataIndex = 0;
	frameBlendMode = TRUE;
	cullMode = CULL_MODE::CULL_MODE_FRONT;
	defaultAnimIndex = 0;

	crossFadeMode = TRUE;
	motionblend = FALSE;


	//shader
	this->alphaTest = FALSE;
	m_fuchi = FALSE;


}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::Init(void)
{
	PrimitiveComponent::Init();

	//SetMeshDataList();
	//if (animation==TRUE)
	//{
	//	SetAnimationArray();
	//}

	//this->SetpGameEngine(this->GetWorld()->GetGameEngine());
	//this->SetpMeshDataList();
	isShow = true;
	animSpeed = 1.0f;
	diffuseIndex = -1;
	defaultAnimIndex = 0;
	crossFadeMode = TRUE;
	hasShadow = TRUE;


}

void MeshComponent::Update(void)
{
	PrimitiveComponent::Update();

	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();

	float dTime = pGameEngine->GetDeltaTime();//1.0=1秒
	float dFrame = dTime * 60.0f;

	if (animstate==ANIM_STATE::NO_ANIM)
	{

	}
	else if (animstate == ANIM_STATE::DATA_ANIM)
	{
		for (int i = 0; i < meshNum; i++)
		{
			int frame1 = (int)(framecnt);
			int frame2 = (int)(framecnt)+1;
			if (frame2 >= framenum)
			{
				frame2 = 0;
			}
			float w2 = framecnt - (float)(int)(framecnt);
			float w1 = 1.0f - w2;

			XMMATRIX mtx1 = pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(animindex)->GetFrameMeshMtx(frame1, i);
			XMMATRIX mtx2 = pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(animindex)->GetFrameMeshMtx(frame2, i);


			SetMtx((mtx1 * w1) + (mtx2 * w2));


		}

		framecnt += animSpeed*dFrame;
		if (framecnt >= framenum)
		{


			framecnt = framecnt-(float)framenum;



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
		float blendweight1= (1.0f / blendcntmax) * blendcnt;
		float blendweight2= 1.0f - blendweight1;


		

		for (int i = 0; i < meshNum; i++)
		{


			int frame1 = (int)(framecnt);
			int frame2 = (int)(framecnt)+1;
			if (frame2>=framenum)
			{
				frame2 = 0;
			}
			float w2 = framecnt - (float)(int)(framecnt);
			float w1 = 1.0f - w2;

			XMMATRIX mtx1= (pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(animindex)->GetFrameMeshMtx(frame1, i) * blendweight1) + (this->GetBlendMtx() * blendweight2);
			XMMATRIX mtx2= (pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(animindex)->GetFrameMeshMtx(frame2, i) * blendweight1) + (this->GetBlendMtx() * blendweight2);


			SetMtx((mtx1 * w1) + (mtx2 * w2));




		}
		blendcnt += dFrame;
		if (blendcnt >= blendcntmax)
		{
			animstate = ANIM_STATE::DATA_ANIM;
		}

	}
	else if (animstate == ANIM_STATE::CROSSFADE_ANIM)
	{
		float blendweight1= (1.0f / blendcntmax) * blendcnt;
		float blendweight2= 1.0f - blendweight1;


		

		for (int i = 0; i < meshNum; i++)
		{


			int frame1 = (int)(framecnt);
			int frame2 = (int)(framecnt)+1;
			if (frame2>=framenum)
			{
				frame2 = 0;
			}
			float w2 = framecnt - (float)(int)(framecnt);
			float w1 = 1.0f - w2;

			XMMATRIX mtx1= (pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(animindex)->GetFrameMeshMtx(frame1, i) * blendweight1) + (this->GetBlendMtx() * blendweight2);
			XMMATRIX mtx2= (pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(animindex)->GetFrameMeshMtx(frame2, i) * blendweight1) + (this->GetBlendMtx() * blendweight2);



			SetMtx((mtx1* w1) + (mtx2 * w2));



		}
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

void MeshComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();
	if (mesh) delete[] mesh;



}


void MeshComponent::Draw(void)
{
	PrimitiveComponent::Draw();

	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();

	Renderer* renderer = pGameEngine->GetRenderer();



	if (this->alphaTest==TRUE)
	{
		renderer->SetAlphaTestEnable(TRUE);
	}
	
	MeshData* meshData = pGameEngine->GetAssetsManager()->GetMeshData(this->MeshDataIndex);


	renderer->SetCullingMode((CULL_MODE)cullMode);
	meshData->BufferSetVertex();
	meshData->BufferSetIndex();


	TransformComponent* trans = this->pGameObject->GetTransFormComponent();

	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	pGameEngine->GetCBufferManager()->SetWorldMtx(&world);


	pGameEngine->GetAssetsManager()->GetMaterial(this->materialIndex)->SetBufferMaterial();

	renderer->GetDeviceContext()->DrawIndexed(meshData->GetIndexNum(), 0, 0);

	renderer->SetAlphaTestEnable(FALSE);

}


void MeshComponent::SetCullingMode(int cullMode)
{
	this->cullMode = cullMode;
}

int MeshComponent::GetCullingMode(void)
{
	return this->cullMode;
}

void MeshComponent::SetAlphaTest(BOOL enable)
{
	this->alphaTest = enable;
}

BOOL MeshComponent::GetAlphaTest(void)
{
	return this->alphaTest;
}

void MeshComponent::SetFuchi(BOOL enable)
{
	m_fuchi = enable;
}

BOOL MeshComponent::GetFuchi(void)
{
	return m_fuchi;
}

void MeshComponent::SetDiffuseTex(string texName)
{
	isOriginalDiffuse = TRUE;

	string path = TEXTURE_PATH + texName;

	diffuseIndex = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->LoadTexture(path);
	
}

string MeshComponent::GetDiffuseTex(void)
{
	if (diffuseIndex < 0)
	{
		return "\0";
	}
	return pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->GetTexture(diffuseIndex)->GetFilePath();
}



void MeshComponent::SetAnimationSpeed(float speed)
{
	animSpeed = speed;
}

float MeshComponent::GetAnimationSpeed(void)
{
	return animSpeed;
}

void MeshComponent::SetShow(bool show)
{
	isShow = show;
}

bool MeshComponent::GetShow(void)
{
	return isShow;
}

void MeshComponent::SetOcclusionCulling(bool occlusionCulling)
{
	m_occlusionCulling = occlusionCulling;
}

bool MeshComponent::GetOcclusionCulling(void)
{
	return m_occlusionCulling;
}

void MeshComponent::SetMeshDataIndex(int index)
{
	this->MeshDataIndex = index;
	animation = FALSE;

	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();

	Renderer* renderer = pGameEngine->GetRenderer();

	MeshData* meshData = pGameEngine->GetAssetsManager()->GetMeshData(index);

	this->materialIndex = meshData->GetMaterialIndex();

	this->GetTransFormComponent()->SetPosition(meshData->GetPosOffset());
	this->GetTransFormComponent()->SetRotation(meshData->GetRotOffset());
	this->GetTransFormComponent()->SetScale(meshData->GetSclOffset());

	this->GetTransFormComponent()->UpdateMatrix();

	//SetMtx(meshData->GetLocalOffset());
	//this->SetBlendMtx();
	isOriginalDiffuse = FALSE;


}


BOOL MeshComponent::GetFrameBlendMode(void)
{
	return frameBlendMode;
}

void MeshComponent::SetFrameBlendMode(BOOL flag)
{
	frameBlendMode = flag;
}

BOOL MeshComponent::GetCrossFadeMode(void)
{
	return crossFadeMode;
}

void MeshComponent::SetCrossFadeMode(BOOL flag)
{
	crossFadeMode = flag;
}

int MeshComponent::GetDefaultAnimIndex(void)
{
	return defaultAnimIndex;
}

void MeshComponent::SetDefaultAnimIndex(int n)
{
	defaultAnimIndex = n;
}


void MeshComponent::CreatepAnimDataArray(int n)
{

}
void MeshComponent::SetpKeyFrameAnimData(int n, KeyFrameAnimData* p)
{

}

string MeshComponent::GetMeshFilePath(void)
{
	return meshFilePath;
}

void MeshComponent::SetMeshFilePath(string path)
{
	meshFilePath = path;
}

void MeshComponent::SetAnimFilePath(string path)
{
	animFilePath.push_back(path);
}

void MeshComponent::SetAnimFilePathArray(vector<string> path)
{
	for (int i = 0; i < path.size(); i++)
	{
		animFilePath.push_back(path[i]);

	}
}



void MeshComponent::SetMeshComponent(
	string meshFilePath,		//メッシュデータのパス
	vector<string> animFilePath,//アニメーションさせるか？
	BOOL blend,					//モーションブレンドするか？
	int blendN)					//モーションブレンディングのフレーム数
{


	SetMeshFilePath(meshFilePath);
	SetAnimFilePathArray(animFilePath);


	motionblend = blend;
	blendcnt = 0;
	blendcntmax = blendN;
	animation = TRUE;
	animstate = ANIM_STATE::DATA_ANIM;

	//SetMeshDataList();
	SetAnimationArray();
}


void MeshComponent::SetMeshComponent(string meshFilePath)		//メッシュデータのパス
{
	SetMeshFilePath(meshFilePath);
	animation = FALSE;
	//SetMeshDataList();

}

void MeshComponent::SetBlendFrame(int frame)
{
	blendcntmax = frame;
}




//void MeshComponent::SetMeshDataList(void)
//{
//
//	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();
//
//	Renderer* renderer = pGameEngine->GetRenderer();
//
//	this->MeshDataIndex = pGameEngine->GetAssetsManager()->LoadMeshTree(this->meshFilePath);
//	MeshDataList* l = pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex);
//
//	MeshData* md=nullptr;
//
//
//	DX11_SUBSET* sub = nullptr;
//	int rootNum;
//
//	for (int i = 0; i < l->GetMeshDataNum(); i++)
//	{
//		md = &l->GetMeshData()[i];
//		sub = &md->GetSubset()[0];
//
//		if (sub != nullptr)
//		{
//			rootNum = i;
//			break;
//		}
//	}
//
//	if (md->GetSubset()->GetMaterial()->GetShaderSet()->GetShaderIndex() == ShaderSet::ShaderIndex::Lambart)
//	{
//		this->material = new LambartMaterial(dynamic_cast<LambartMaterial*>( pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()->GetSubset()->GetMaterial()));
//	}
//	else if (md->GetSubset()->GetMaterial()->GetShaderSet()->GetShaderIndex() == ShaderSet::ShaderIndex::Phong)
//	{
//		this->material = new PhongMaterial(dynamic_cast<PhongMaterial*>(pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()->GetSubset()->GetMaterial()));
//
//	}
//
//	this->CreateMeshArray(pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshDataNum());
//
//	for (int i = 0; i < this->meshNum; i++)
//	{
//		if (pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()[i].GetSubset() == nullptr)
//			continue;
//
//		mesh[i].SetMtx(pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()[i].GetWorldOffset());
//		mesh[i].SetMaterial(pGameEngine->GetAssetsManager()->GetMeshDataList(this->MeshDataIndex)->GetMeshData()[i].GetSubset()->GetMaterial());
//	}
//	this->SetBlendMtx();
//	isOriginalDiffuse = FALSE;
//}

void MeshComponent::SetAnimationArray(void)
{
	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();

	Renderer* renderer = pGameEngine->GetRenderer();

	for (int i = 0; i < this->animFilePath.size(); i++)
	{
		int n = pGameEngine->GetAssetsManager()->LoadMeshAnim(this->animFilePath[i]);
		this->AnimDataIndexArray.push_back(n);
	}
	framecnt = 0;
	framenum = pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(AnimDataIndexArray[0])->GetFrameNum();
	

}



void MeshComponent::CreateMeshArray(int n)
{
	meshNum = n;
	mesh = new Mesh[n];
}

void MeshComponent::SwichAnimIndex(int n)
{
	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();

	Renderer* renderer = pGameEngine->GetRenderer();

	animindex = n;
	framecnt = 0;
	framenum = pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(AnimDataIndexArray[n])->GetFrameNum();
	if (motionblend)
	{
		SetBlendMtx();
		blendcnt = 0;
		animstate = ANIM_STATE::BLEND_ANIM;

	}
}

void MeshComponent::StartOneTimeAnimIndex(int n)
{
	GameEngine* pGameEngine = pGameObject->GetScene()->GetGameEngine();

	Renderer* renderer = pGameEngine->GetRenderer();

	animindex = n;
	framecnt = 0;
	framenum = pGameEngine->GetAssetsManager()->GetKeyFrameAnimData(AnimDataIndexArray[n])->GetFrameNum();
	SetBlendMtx();
	blendcnt = 0;

	isOneTime = TRUE;
	
	animstate = ANIM_STATE::BLEND_ANIM;


}

void MeshComponent::SetMtx(XMMATRIX mtx)
{
	this->mtx = mtx;
}

void MeshComponent::SetBlendMtx(void)
{
	this->blendMtx = this->mtx;
}

XMMATRIX MeshComponent::GetMtx(void)
{
	return this->mtx;
}

XMMATRIX MeshComponent::GetBlendMtx(void)
{
	return this->blendMtx;
}


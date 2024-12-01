#include "AssetsManager.h"
#include "MeshData.h"
#include "GameEngine.h"
#include "MeshAnimData.h"
//#include "SkinMeshDataList.h"
//#include "SkeletonAnimData.h"
#include "DX11Texture.h"
#include "LambartShader.h"
#include "PhongShader.h"
#include "UIShader.h"

#define MESH_PATH "data/MODEL/mesh/"
#define SKINMESH_PATH "data/MODEL/skinmesh/"

#define MESH_ANIM_PATH "data/animation/mesh/"
#define SKINMESH_ANIM_PATH "data/animation/skinmesh/"

AssetsManager::AssetsManager()
{
	this->pGameEngine = nullptr;
	this->skinMeshCompute = nullptr;
}

AssetsManager::AssetsManager(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
	this->skinMeshCompute = nullptr;

}

AssetsManager::~AssetsManager()
{


}

void AssetsManager::Init(void)
{
	//pGameEngine->GetRenderer()->CreateCSFile("shaders/CSskinmesh.hlsl", "CSFunc", &skinMeshCompute);
	
	CreateAllShader();

	
}

void AssetsManager::Uninit(void)
{

	for (int i = 0; i < this->MeshDataListArray.size(); i++)
	{
		delete this->MeshDataListArray[i];

	}
	this->MeshDataListArray.clear();

	for (int i = 0; i < this->KeyFrameAnimDataArray.size(); i++)
	{
		delete this->KeyFrameAnimDataArray[i];
	}
	this->KeyFrameAnimDataArray.clear();

	//for (int i = 0; i < this->SkinMeshDataListArray.size(); i++)
	//{
	//	delete this->SkinMeshDataListArray[i];
	//}
	//this->SkinMeshDataListArray.clear();


	//for (int i = 0; i < this->SkeletonAnimDataArray.size(); i++)
	//{
	//	delete this->SkeletonAnimDataArray[i];
	//}
	//this->SkeletonAnimDataArray.clear();


	for (int i = 0; i < this->TextureArray.size(); i++)
	{
		delete this->TextureArray[i];
	}
	this->TextureArray.clear();


	if (skinMeshCompute) skinMeshCompute->Release();
}

int AssetsManager::LoadMesh(string filepath)
{
	int p = -1;
	BOOL find = FALSE;
	for (int i = 0; i < MeshDataListArray.size(); i++)
	{
		 
		string fp = MeshDataListArray[i]->GetFilePath();
		if ((MESH_PATH+ filepath) == MeshDataListArray[i]->GetFilePath())
		{
			p = i;
			find = TRUE;
			break;
		}
	}

	if (find == FALSE)
	{

		MeshDataList* meshdatalist = new MeshDataList;


		string path = MESH_PATH+filepath;

		
		meshdatalist->LoadFbxFile(path, this);

		this->MeshDataListArray.push_back(meshdatalist);
		p = (int)MeshDataListArray.size() - 1;

	}

	return p;
}
MeshDataList* AssetsManager::GetMeshDataList(int n)
{
	return this->MeshDataListArray[n];
}

KeyFrameAnimData* AssetsManager::GetKeyFrameAnimData(int n)
{
	return this->KeyFrameAnimDataArray[n];
}


GameEngine* AssetsManager::GetGameEngine(void)
{
	return this->pGameEngine;
}


int AssetsManager::LoadMeshAnim(string filepath)
{
	int p = -1;
	BOOL find = FALSE;
	for (int i = 0; i < KeyFrameAnimDataArray.size(); i++)
	{
		if ((MESH_ANIM_PATH + filepath) == KeyFrameAnimDataArray[i]->GetFilePath())
		{
			p = i;
			find = TRUE;
			break;
		}
	}

	if (find == FALSE)
	{

		string path = MESH_ANIM_PATH + filepath;

		KeyFrameAnimData* animdata = new KeyFrameAnimData;
		animdata->LoadKeyFrameAnim(path);

		this->KeyFrameAnimDataArray.push_back(animdata);
		p = (int)MeshDataListArray.size() - 1;
	}

	return p;
}

//int AssetsManager::LoadSkinMesh(string filepath)
//{
//	int p = -1;
//	BOOL find = FALSE;
//	for (int i = 0; i < SkinMeshDataListArray.size(); i++)
//	{
//		if ((SKINMESH_PATH + filepath) == SkinMeshDataListArray[i]->GetFilePath())
//		{
//			p = i;
//			find = TRUE;
//			break;
//		}
//	}
//
//	if (find == FALSE)
//	{
//		string path = SKINMESH_PATH + filepath;
//
//
//		SkinMeshDataList* skinmeshdatalist = new SkinMeshDataList;
//
//		skinmeshdatalist->LoadSkinMeshDataList(path, this);
//
//		this->SkinMeshDataListArray.push_back(skinmeshdatalist);
//		p = (int)SkinMeshDataListArray.size() - 1;
//
//	}
//
//	return p;
//}
//
//SkinMeshDataList* AssetsManager::GetSkinMeshDataList(int n)
//{
//	return this->SkinMeshDataListArray[n];
//}
//
//SkeletonAnimData* AssetsManager::GetSkeletonAnimData(int n)
//{
//	return this->SkeletonAnimDataArray[n];
//}
//int AssetsManager::LoadSkeletonAnimData(string filepath)
//{
//	int p = -1;
//	BOOL find = FALSE;
//	for (int i = 0; i < SkeletonAnimDataArray.size(); i++)
//	{
//		if ((SKINMESH_ANIM_PATH + filepath) == SkeletonAnimDataArray[i]->GetFilePath())
//		{
//			p = i;
//			find = TRUE;
//			break;
//		}
//	}
//
//	if (find == FALSE)
//	{
//		string path = SKINMESH_ANIM_PATH + filepath;
//
//
//		SkeletonAnimData* skeletonAnimdata = new SkeletonAnimData;
//
//		
//		skeletonAnimdata->LoadSkeletonAnimData(path);
//
//		this->SkeletonAnimDataArray.push_back(skeletonAnimdata);
//		p = (int)SkeletonAnimDataArray.size() - 1;
//
//	}
//
//	return p;
//}



DX11Texture* AssetsManager::GetTexture(int n)
{


	return this->TextureArray[n];
}
int AssetsManager::LoadTexture(string filepath)
{
	int p = -1;
	BOOL find = FALSE;
	for (int i = 0; i < TextureArray.size(); i++)
	{
		string  fp = TextureArray[i]->GetFilePath();
		if (filepath == TextureArray[i]->GetFilePath())
		{
			p = i;
			find = TRUE;
			break;
		}
	}

	if (find == FALSE)
	{

		DX11Texture* tex = new DX11Texture;
		tex->SetManager(this);

		tex->CreateSRV(filepath);

		this->TextureArray.push_back(tex);
		p = (int)TextureArray.size() - 1;

	}

	return p;
}

void AssetsManager::SetSkinMeshCompute(void)
{
	GetGameEngine()->GetRenderer()->GetDeviceContext()->CSSetShader(skinMeshCompute, nullptr, 0);
}

void AssetsManager::CreateAllShader(void)
{
	lambartShader = new LambartShader(this->pGameEngine->GetRenderer());
	phongShader = new PhongShader(this->pGameEngine->GetRenderer());
	uiShader = new UIShader(this->pGameEngine->GetRenderer());
}

LambartShader* AssetsManager::GetLambartShader(void)
{
	return this->lambartShader;
}

PhongShader* AssetsManager::GetPhongShader(void)
{
	return this->phongShader;
}

UIShader* AssetsManager::GetUIShader(void)
{
	return this->uiShader;
}

void AssetsManager::SetShader(ShaderSet::ShaderIndex index)
{
	switch (index)
	{

	case ShaderSet::Lambart:

		this->lambartShader->SetShaderRenderer();
		break;

	case ShaderSet::Phong:

		this->phongShader->SetShaderRenderer();
		break;

	case ShaderSet::UI:

		this->uiShader->SetShaderRenderer();
		break;

	}
}


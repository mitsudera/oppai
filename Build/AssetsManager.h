#pragma once
#include "CoreMinimal.h"
#include "ShaderSet.h"


class MeshData;

class KeyFrameAnimData; 

//class SkinMeshDataList;
//
//class SkeletonAnimData;

class DX11Texture;

class GameEngine;

class Material;

//shader
class LambartShader;
class PhongShader;
class UIShader;
class SkyShader;

class AssetsManager
{


public:
	AssetsManager();
	AssetsManager(GameEngine* pGameEngine);
	~AssetsManager();

	void Init(void);
	void Uninit(void);




	int LoadMeshNode(string filepath);

	MeshData* GetMeshTree(int n);

	int AddMesh(MeshData* data);


	MeshData* GetMeshData(int n);

	KeyFrameAnimData* GetKeyFrameAnimData(int n);


	GameEngine* GetGameEngine(void);

	int LoadMeshAnim(string filepath);

	//SkinMeshDataList* GetSkinMeshDataList(int n);
	//int LoadSkinMesh(string filepath);

	//SkeletonAnimData* GetSkeletonAnimData(int n);
	//int LoadSkeletonAnimData(string filepath);


	DX11Texture* GetTexture(int n);
	int LoadTexture(string filepath);

	void SetSkinMeshCompute(void);

	void CreateAllShader(void);

	LambartShader* GetLambartShader(void);
	PhongShader* GetPhongShader(void);
	UIShader* GetUIShader(void);
	SkyShader* GetSkyShader(void);

	void SetShader(ShaderSet::ShaderIndex index);

	int LoadMaterial(Material* material);

	Material* GetMaterial(int index);

private:

	GameEngine* pGameEngine;
	vector<MeshData*> MeshDataTree;//メッシュデータのルートの配列
	vector<MeshData*> MeshDataArray;
	vector<KeyFrameAnimData*>  KeyFrameAnimDataArray;
	//vector<SkinMeshDataList*> SkinMeshDataListArray;
	//vector<SkeletonAnimData*> SkeletonAnimDataArray;
	vector<Material*> MaterialArray;

	vector<DX11Texture*> TextureArray;

	ID3D11ComputeShader* skinMeshCompute;

	LambartShader* lambartShader;
	PhongShader* phongShader;
	UIShader* uiShader;
	SkyShader* skyShader;
};


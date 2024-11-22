#pragma once
#include "CoreMinimal.h"


class MeshDataList;

class KeyFrameAnimData; 

class SkinMeshDataList;

class SkeletonAnimData;

class DX11Texture;


class GameEngine;

class AssetsManager
{


public:
	AssetsManager();
	AssetsManager(GameEngine* gameEngine);
	~AssetsManager();

	void Init(void);
	void Uninit(void);




	int LoadMesh(string filepath);

	MeshDataList* GetMeshDataList(int n);

	KeyFrameAnimData* GetKeyFrameAnimData(int n);


	GameEngine* GetGameEngine(void);

	int LoadMeshAnim(string filepath);

	SkinMeshDataList* GetSkinMeshDataList(int n);
	int LoadSkinMesh(string filepath);

	SkeletonAnimData* GetSkeletonAnimData(int n);
	int LoadSkeletonAnimData(string filepath);


	DX11Texture* GetTexture(int n);
	int LoadTexture(string filepath);

	void SetSkinMeshCompute(void);

private:

	GameEngine* gameEngine;
	vector<MeshDataList*> MeshDataListArray;
	vector<KeyFrameAnimData*>  KeyFrameAnimDataArray;
	vector<SkinMeshDataList*> SkinMeshDataListArray;
	vector<SkeletonAnimData*> SkeletonAnimDataArray;

	vector<DX11Texture*> TextureArray;

	ID3D11ComputeShader* skinMeshCompute;

};


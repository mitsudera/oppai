#pragma once
#include "CoreMinimal.h"
#include "MeshData.h"
class DX11_SUBSET;
class MeshData;
class MeshDataList;
class AssetsManager;



struct CONTROLPOINT
{
	XMFLOAT3 pos;
	float* weight;


};



class Bone
{
public:
	Bone();
	~Bone();
	void LoadBone(FbxCluster* cluster);
	void LoadBone(FbxNode* node, Bone* parent);
	void LoadBoneFrame(FbxCluster* cluster,FbxTime time);
	void SetInitMtx(XMMATRIX Mtx);
	XMMATRIX GetMtx(void);
	XMMATRIX GetInitMtxInverse(void);
	void CreateChildArray(int n);
	int GetIndex(void);
	void SetIndex(int n);

private:
	XMMATRIX mtx;
	XMMATRIX initMtxInverse;
	int childCnt;
	Bone* child;
	Bone* parent;
	BOOL isRootBone;
	int index;

};

class Skeleton
{
public:
	Skeleton();
	~Skeleton();
	void CreateBoneArray(int n);
	void LoadSkeleton(FbxScene* scene);
	void LoadSkeletonFrame(FbxSkin* skin, int boneCnt, FbxTime time);
	Bone* GetBone(int n);
	int GetBoneCount(void);

private:
	Bone* bone;
	int boneCnt;
};


class SkinMeshData:public MeshData
{
public:
	SkinMeshData();
	~SkinMeshData();
	void LoadSkinMeshData(FbxMesh* mesh,Skeleton* sp,AssetsManager* ap);
	int GetVertNum(void);
	VERTEX_3D GetVertex(int n);
	CONTROLPOINT GetControlPointVertex(int n);//頂点番号からコントロールポイント取得
	CONTROLPOINT* GetControlPoint(int n);
	CONTROLPOINT* GetControlPointArray(void);
	int GetControlNum(void);
	int* GetCPIndexArray(void);


private:

	Skeleton *pSkeleton;
	int controlPointNum;
	CONTROLPOINT* controlPoint;
	VERTEX_3D* VertexArray;
	int* controlPointIndexArray;
	int indexNum;
	int vertNum;

};

class SkinMeshDataList:public MeshDataList
{

public:
	SkinMeshDataList();
	~SkinMeshDataList();
	

	void LoadSkinMeshDataList(string filepath,AssetsManager* ap);
	void CreateSkinMeshDataArray(int n);
	SkinMeshData* GetSkinMeshData(void);
	Skeleton* GetSkeleton(void);


private:
	SkinMeshData* skinMeshData;
	
	Skeleton* skeleton;

};

XMMATRIX GetParentTransFormFbx(FbxNode* boneNode, XMMATRIX mtx);

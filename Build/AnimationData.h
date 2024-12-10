#pragma once
#include "Coreminimal.h"

class AssetsManager;
class AnimationData;

class MtxNode
{
public:

	enum class Attribute :int
	{
		Root,
		Mesh,
		Bone,
	};

	MtxNode();
	~MtxNode();

	void LoadAnimation(FbxNode* node, MtxNode* parent, AnimationData* animdata);

	
	void CreateFrameMtxArray(int n);

	XMMATRIX GetFrameMtx(float frame);

	int GetChildCnt(void);
	MtxNode* GetChild(int n);

private:

	XMMATRIX* frameMtxArray;
	vector<MtxNode*> childArray;
	MtxNode* parent;

	Attribute attribute;



	string name;
	BOOL isRoot;

	AnimationData* pAnimData;

};

class AnimationData
{
public:
	AnimationData();
	~AnimationData();

	int GetFrameNum(void);
	int GetFrameRate(void);

	LONGLONG GetStartTime(void);
	LONGLONG GetOneFrameValue(void);

	string GetFileName(void);
	MtxNode* GetMtxTreeRoot(void);

	void LoadAnimation(string fileName, AssetsManager* assetsManager);


private:
	int frameNum;
	int frameRate;
	LONGLONG startTime;
	LONGLONG oneFrameValue;


	string fileName;
	
	AssetsManager* pAssetsManager;
	MtxNode* mtxTreeRoot;

};


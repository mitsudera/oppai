﻿#pragma once
#include "PrimitiveComponent.h"

class MeshDataList;

class KeyFrameAnimData;

class GameEngine;

class DX11Texture;

class MeshComponent :public PrimitiveComponent
{



public:

	enum class ANIM_STATE
	{
		NO_ANIM,
		DATA_ANIM,
		BLEND_ANIM,
		CROSSFADE_ANIM, // クロスフェード補間を行うステート
	};

	MeshComponent();
	MeshComponent(GameObject* gameObject);
	~MeshComponent();

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	virtual void Draw(void) override;

	//void SetpGameEngine(GameEngine* gameEngine);

	string GetMeshFilePath(void);
	void SetMeshFilePath(string path);
	void SetAnimFilePath(string path);
	void SetAnimFilePathArray(vector<string> path);


	void SetMeshComponent(
		string meshFilePath,		
		vector<string> animFilePath,
		BOOL blend,					
		int blendN);				
	void SetMeshComponent(string meshFilePath);	

	void SetBlendFrame(int frame);

	virtual void SetMeshDataList(void);
	virtual void SetAnimationArray(void);

	void CreatepAnimDataArray(int n);
	void SetpKeyFrameAnimData(int n, KeyFrameAnimData* p);

	void CreateMeshMtxArray(int n);
	void SetMeshMtxArray(int n, XMMATRIX mtx);
	void SetBlendMtxArray(void);
	virtual void SwichAnimIndex(int n);
	virtual void StartOneTimeAnimIndex(int n);



	virtual void DrawMesh(int n);

	void SetCullingMode(int cullMode);
	int GetCullingMode(void);

	void SetAlphaTest(BOOL enable);
	BOOL GetAlphaTest(void);

	void SetFuchi(BOOL enable);
	BOOL GetFuchi(void);

	virtual void SetDiffuseTex(string texName);
	virtual string GetDiffuseTex(void);
	virtual void SetAnimationSpeed(float speed);
	virtual float GetAnimationSpeed(void);

	BOOL GetFrameBlendMode(void);
	void SetFrameBlendMode(BOOL flag);

	BOOL GetCrossFadeMode(void);
	void SetCrossFadeMode(BOOL flag);

	int GetDefaultAnimIndex(void);
	void SetDefaultAnimIndex(int n);
	void SetShow(bool show);
	bool GetShow(void);
	
	
	void SetOcclusionCulling(bool occlusionCulling);
	bool GetOcclusionCulling(void);



protected:

	string meshFilePath;
	vector<string> animFilePath;
	
	int MeshDataListIndex;
	vector<int> AnimDataIndexArray;

	//GameEngine* pGameEngine;


	XMMATRIX* MeshMtxArray;
	int meshNum;

	XMMATRIX* BlendMeshMtxArray;
	int animindex;
	int lastanimindex;
	float framecnt;
	int framenum;
	float blendcnt;
	int blendcntmax;
	ANIM_STATE animstate;
	BOOL motionblend;
	BOOL animation;
	
	int cullMode;
	BOOL alphaTest;


	BOOL isOriginalDiffuse;
	int diffuseIndex;

	float animSpeed;
	BOOL isOneTime;

	BOOL frameBlendMode;

	int defaultAnimIndex;

	BOOL crossFadeMode;
	//shader
	BOOL m_fuchi;

	bool isShow;

	bool m_occlusionCulling;
};


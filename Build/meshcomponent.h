#pragma once
#include "PrimitiveComponent.h"

class MeshDataList;

class KeyFrameAnimData;

class GameEngine;

class DX11Texture;


class Mesh
{

};

class MeshComponent :public PrimitiveComponent
{



public:

	MeshComponent();
	MeshComponent(GameObject* gameObject);
	~MeshComponent();

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	virtual void Draw(void) override;

	virtual void ShadowMapping(void) override;


	void SetCullingMode(int cullMode);
	int GetCullingMode(void);

	void SetAlphaTest(BOOL enable);
	BOOL GetAlphaTest(void);
	void SetMeshDataIndex(int index);



protected:

	string meshFilePath;
	
	int MeshDataIndex;




	
	int cullMode;


	BOOL isRoot;

};


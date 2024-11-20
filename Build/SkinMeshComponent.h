#pragma once
#include "MeshComponent.h"
#include "Renderer.h"
#define BONE_MAX (100)

struct CP
{
	XMFLOAT3 pos;
	float dummy;
	float weight[BONE_MAX];
	int showBoneNum;
	float dummy2[2];
	int showBoneIndex[BONE_MAX];
	
};


struct SkinMeshMath
{
	int BoneNum;
	int dummy[3];

};

class MeshVertex
{
public:

	MeshVertex();
	~MeshVertex();

	ID3D11Buffer* m_vertexBuffer;
	VERTEX_3D* vertexArray;
	int vertNum;

	ID3D11Buffer* controlPointBuffer;
	ID3D11Buffer* outputBuffer;
	ID3D11UnorderedAccessView* outputUAV;
	ID3D11ShaderResourceView* controlPointSRV;

};

class SkinMeshComponent :public MeshComponent
{

public:
	SkinMeshComponent();
	~SkinMeshComponent();


	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;
	virtual void BeginPlay(void) override;
	virtual void UpdatePlay(void) override;
	virtual void EndPlay(void) override;
	
	virtual void Draw(void) override;
	virtual void DrawSkinMesh(int n) ;

	void SetSkinMeshStatus(
		string meshfilepath,
		vector<string> animfilepatharray,
		BOOL blend,
		int blendframe);

	void SetSkinMeshStatus(string meshfilepath);


	virtual void SetSkinMesh(void);

	void SetBlendBone(void);

	virtual void SwichAnimIndex(int n) override;
	virtual void StartOneTimeAnimIndex(int n) override;//çƒê∂èIóπå„ÇÕÇOî‘Ç…à⁄çs

	void UpdateSkinMeshForCS(void);
	int GetAnimIndex(void);


protected:



	XMMATRIX* boneMtx;
	int boneCnt;
	XMMATRIX* blendBoneMtx;
	MeshVertex* meshVertex;
	ID3D11Buffer* boneBuffer;
	ID3D11Buffer* skinMeshMathBuffer;

};


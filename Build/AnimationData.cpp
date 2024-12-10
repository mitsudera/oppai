#include "AnimationData.h"

MtxNode::MtxNode()
{
}

MtxNode::~MtxNode()
{
}

void MtxNode::LoadAnimation(FbxNode* node, MtxNode* parent, AnimationData* animdata)
{
	using namespace fbxsdk;
	name = node->GetName();
	pAnimData = animdata;
	this->parent = parent;
	if (parent == nullptr)
	{
		this->isRoot = TRUE;
		this->attribute = Attribute::Root;

		
	}
	else
	{

		if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			this->attribute = Attribute::Mesh;
		}
		else if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			this->attribute = Attribute::Bone;
		}

		this->isRoot = FALSE;


		CreateFrameMtxArray(pAnimData->GetFrameNum());

		for (int i = 0; i < pAnimData->GetFrameNum(); i++)
		{
			//FbxMatrix framemtx = node->EvaluateGlobalTransform(pAnimData->GetStartTime() + pAnimData->GetOneFrameValue() * i);
			FbxMatrix framemtx = node->EvaluateLocalTransform(pAnimData->GetStartTime() + pAnimData->GetOneFrameValue() * i);
			XMMATRIX frameMtx = FbxMatrixConvertToXMMATRIX(framemtx);


			//FbxVector4 pos = node->EvaluateLocalTranslation(pAnimData->GetStartTime() + pAnimData->GetOneFrameValue() * i);
			//FbxVector4 scl = node->EvaluateLocalScaling(pAnimData->GetStartTime() + pAnimData->GetOneFrameValue() * i);
			//FbxVector4 rot = node->EvaluateLocalRotation(pAnimData->GetStartTime() + pAnimData->GetOneFrameValue() * i);

			//XMMATRIX posMtx = XMMatrixTranslation(pos[0], pos[1], pos[2]);
			//XMMATRIX sclMtx = XMMatrixTranslation(scl[0], scl[1], scl[2]);
			//XMMATRIX rotMtx = XMMatrixTranslation(rot[0], rot[1], rot[2]);

			//XMMATRIX localFrameMtx = XMMatrixIdentity();
			//localFrameMtx = XMMatrixMultiply(localFrameMtx, sclMtx);
			//localFrameMtx = XMMatrixMultiply(localFrameMtx, rotMtx);
			//localFrameMtx = XMMatrixMultiply(localFrameMtx, posMtx);

			this->frameMtxArray[i] = frameMtx;
		}
	}


	for (int i = 0; i < node->GetChildCount(); i++)
	{
		FbxNode* child = node->GetChild(i);

		if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			MtxNode* childNode = new MtxNode();
			childNode->LoadAnimation(child, this, pAnimData);
			childArray.push_back(childNode);
		}
		else if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			MtxNode* childNode = new MtxNode();
			childNode->LoadAnimation(child, this, pAnimData);
			childArray.push_back(childNode);
		}
	}




}


void MtxNode::CreateFrameMtxArray(int n)
{
	this->frameMtxArray = new XMMATRIX[n];

}

XMMATRIX MtxNode::GetFrameMtx(float frame)
{

	float frame2weight = frame - (float)(int)frame;
	float frame1weight = 1.0f - frame2weight;;



	int frame1 = int(frame);
	
	if (frame1 > (float)pAnimData->GetFrameNum())
	{
		frame1 %= pAnimData->GetFrameNum();
	}
	int frame2 = frame1+1;


	if (frame2 >= pAnimData->GetFrameNum())
	{
		frame2 = 0;
	}

	XMMATRIX mtx1 = this->frameMtxArray[frame1];
	XMMATRIX mtx2 = this->frameMtxArray[frame2];

	return (mtx1 * frame1weight) + (mtx2 * frame2weight);

}

int MtxNode::GetChildCnt(void)
{
	return this->childArray.size();
}

MtxNode* MtxNode::GetChild(int n)
{
	return this->childArray[n];
}


AnimationData::AnimationData()
{
}

AnimationData::~AnimationData()
{
}

int AnimationData::GetFrameNum(void)
{
	return frameNum;
}

int AnimationData::GetFrameRate(void)
{
	return frameRate;
}

LONGLONG AnimationData::GetStartTime(void)
{
	return startTime;
}

LONGLONG AnimationData::GetOneFrameValue(void)
{
	return oneFrameValue;
}

string AnimationData::GetFileName(void)
{
	return fileName;
}

MtxNode* AnimationData::GetMtxTreeRoot(void)
{
	return mtxTreeRoot;
}

void AnimationData::LoadAnimation(string fileName, AssetsManager* assetsManager)
{

	this->fileName = fileName;

	FbxManager* manager;
	FbxIOSettings* ioSettings;

	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);



	// Importerを生成
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(fileName.c_str(), -1, manager->GetIOSettings()) == false)
	{
		// インポートエラー
	}


	//SceneオブジェクトにFBXファイル内の情報を流し込む
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);

	

	int meshcount = scene->GetSrcObjectCount<FbxMesh>();

	//アニメーション名一覧を格納する配列の宣言
	FbxArray<FbxString*> animation_names;

	//シーンからアニメーション一覧を取得＆格納
	scene->FillAnimStackNameArray(animation_names);

	int animcount = animation_names.GetCount();
	int framenum = 0;
	FbxTime start_time = 0;
	FbxTime end_time = 0;
	FbxLongLong oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	FbxTime totalframe = 0;

	if (animcount > 0)
	{
		//アニメ番号0番を元にFbxTakeInfoを取得
		auto take_info = scene->GetTakeInfo(animation_names[0]->Buffer());
		//開始時間
		start_time = take_info->mLocalTimeSpan.GetStart();
		//終了時間
		end_time = take_info->mLocalTimeSpan.GetStop();

		oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

		totalframe = (end_time - start_time) / oneFrameValue;

		framenum = (int)totalframe.Get();//アニメーションの合計フレーム数



	}

	this->startTime = start_time.Get();
	this->oneFrameValue = oneFrameValue;

	this->frameNum = framenum;

	this->mtxTreeRoot = new MtxNode();

	this->mtxTreeRoot->LoadAnimation(scene->GetRootNode(), nullptr, this);


}

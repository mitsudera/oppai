#include "SkeletonAnimData.h"

SkeletonAnimData::SkeletonAnimData()
{
	frameSkeleton = nullptr;
	frameNum = 0;
}

SkeletonAnimData::~SkeletonAnimData()
{
	delete[] frameSkeleton;
}

void SkeletonAnimData::CreateSkeleton(int frameN)
{
	this->frameSkeleton = new Skeleton[frameN];
	frameNum = frameN;
}

Skeleton* SkeletonAnimData::GetFrameSkeleton(int frame)
{
	return &this->frameSkeleton[frame];
}


void SkeletonAnimData::LoadSkeletonAnimData(string path)
{
	this->filePath = path;
	FbxManager* manager;
	FbxIOSettings* ioSettings;

	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);



	// Importer�𐶐�
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(path.c_str(), -1, manager->GetIOSettings()) == false)
	{
		// �C���|�[�g�G���[
	}


	//Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);



	int meshcount = scene->GetSrcObjectCount<FbxMesh>();

	//�A�j���[�V�������ꗗ���i�[����z��̐錾
	FbxArray<FbxString*> animation_names;

	//�V�[������A�j���[�V�����ꗗ���擾���i�[
	scene->FillAnimStackNameArray(animation_names);

	int animcount = animation_names.GetCount();
	int framenum = 0;
	FbxTime start_time = 0;
	FbxTime end_time = 0;
	FbxLongLong oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	FbxTime totalframe = 0;

	if (animcount > 0)
	{
		//�A�j���ԍ�0�Ԃ�����FbxTakeInfo���擾
		auto take_info = scene->GetTakeInfo(animation_names[0]->Buffer());
		//�J�n����
		start_time = take_info->mLocalTimeSpan.GetStart();
		//�I������
		end_time = take_info->mLocalTimeSpan.GetStop();

		oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

		totalframe = (end_time - start_time) / oneFrameValue;

		framenum = (int)totalframe.Get();//�A�j���[�V�����̍��v�t���[����



	}

	int n = scene->GetSrcObjectCount<FbxDeformer>();
	FbxSkin* skin = (FbxSkin*)scene->GetSrcObject<FbxDeformer>(0);
	int bonecount = skin->GetClusterCount();

	this->CreateSkeleton(framenum);


	for (int i = 0; i < framenum; i++)
	{

		FbxTime time = start_time + oneFrameValue * i;

		frameSkeleton[i].LoadSkeletonFrame(skin, bonecount, time);


	}
	XMMATRIX a= frameSkeleton[0].GetBone(10)->GetMtx();
	XMMATRIX b= frameSkeleton[15].GetBone(10)->GetMtx();
	a.r->m128_f32;
	b.r->m128_f32;

}

string SkeletonAnimData::GetFilePath(void)
{
	return this->filePath;
}

int SkeletonAnimData::GetFrameNum(void)
{
	return this->frameNum;
}


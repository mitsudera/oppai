#include "meshanimdata.h"

KeyFrameAnimData* g_keyframeanimdata;

int keyframeanimfilenum;

std::vector<std::string> file_names;


//*****************************************************************************
// �N���X�֐�
//*****************************************************************************

FrameMtxList::FrameMtxList()
{
	meshframemtx = nullptr;
}

FrameMtxList::~FrameMtxList()
{
	if (meshframemtx) delete[]meshframemtx;

}
void  FrameMtxList::CreateMeshMtxArray(int n)
{
	meshframemtx = new XMMATRIX[n];
}
void FrameMtxList::SetMeshFrameMtx(int meshN, XMMATRIX mtx)
{
	meshframemtx[meshN] = mtx;
}
XMMATRIX FrameMtxList::GetMeshFrameMtx(int meshN)
{
	XMMATRIX mtx= this->meshframemtx[meshN];
	return mtx;
}

KeyFrameAnimData::KeyFrameAnimData()
{
	framenum = 0;
	meshnum = 0;
	framemtxlist = nullptr;
}

KeyFrameAnimData::~KeyFrameAnimData()
{
	if (framemtxlist)delete[]framemtxlist;
}

int KeyFrameAnimData::GetFrameNum(void)
{
	return framenum;
}

int KeyFrameAnimData::GetMeshNum(void)
{
	return meshnum;
}

void KeyFrameAnimData::CreateFrameMeshMtxListArray(int frameN, int meshN)
{
	framenum = frameN;
	meshnum = meshN;
	framemtxlist = new FrameMtxList[frameN];
	for (int i = 0; i < frameN; i++)
	{
		framemtxlist[i].CreateMeshMtxArray(meshN);
	}

}

void KeyFrameAnimData::SetFrameMeshMtx(int frameN, int meshN, XMMATRIX mtx)
{
	framemtxlist[frameN].SetMeshFrameMtx(meshN, mtx);
}
XMMATRIX KeyFrameAnimData::GetFrameMeshMtx(int frameN, int meshN)
{
	XMMATRIX mtx = framemtxlist[frameN].GetMeshFrameMtx(meshN);
	return mtx;
}

string KeyFrameAnimData::GetFilePath(void)
{
	return filepath;
}



//*****************************************************************************
// fbx�t�@�C�����烁�b�V���f�[�^�̓ǂݍ���
//*****************************************************************************
void KeyFrameAnimData::LoadKeyFrameAnim(string fpath)
{

	this->filepath = fpath;

	FbxManager* manager;
	FbxIOSettings* ioSettings;

	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);

	

	// Importer�𐶐�
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings()) == false)
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

	this->CreateFrameMeshMtxListArray(framenum, meshcount);


	for (int i = 0; i < meshcount; i++)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(i);
		FbxNode* node = mesh->GetNode();

		for (int j = 0; j < framenum; j++)
		{
			FbxMatrix framemtx = node->EvaluateGlobalTransform(start_time + oneFrameValue * j);

			XMMATRIX mtx = FbxMatrixConvertToXMMATRIX(framemtx);

			this->SetFrameMeshMtx(j, i, mtx);
		}


	}


	scene->Destroy();
	importer->Destroy(); // �V�[���𗬂����񂾂�Importer�͉��

	manager->Destroy();//fbx�t�@�C���̓ǂݍ��݂��I�������}�l�[�W���[�͉������
}



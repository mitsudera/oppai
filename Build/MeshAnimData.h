//=============================================================================
//
// ���b�V���̏��� [meshdata.h]
// Author :�O�c���M�I
//
//=============================================================================
#pragma once
#include "CoreMinimal.h"
#include "renderer.h"
#include "meshdata.h"


//*********************************************************
//�}�N����`
//*********************************************************



//*********************************************************
//�N���X��`
//*********************************************************
 class FrameMtxList
 {

	 
	 XMMATRIX* meshframemtx;//���b�V�����Ŕz��m��
 public:
	 FrameMtxList();
	 ~FrameMtxList();

	 void CreateMeshMtxArray(int n);
	 void SetMeshFrameMtx(int meshN,XMMATRIX mtx);
	 XMMATRIX GetMeshFrameMtx(int meshN);

 private:

 };


 class KeyFrameAnimData
 {
	 int framenum;
	 int meshnum;
	 FrameMtxList* framemtxlist;//�t���[�����̊e���b�V���̍s��̃��X�g�t���[�����Ŕz��m��
	 string filepath;

 public:
	 KeyFrameAnimData();
	 ~KeyFrameAnimData();

	 int GetFrameNum(void);

	 int GetMeshNum(void);

	 void CreateFrameMeshMtxListArray(int frameN,int meshN);

	 void SetFrameMeshMtx(int frameN, int meshN, XMMATRIX mtx);
	 XMMATRIX GetFrameMeshMtx(int frameN, int meshN);


	 string GetFilePath(void);
	 

	 void LoadKeyFrameAnim(string fpath);


 private:

 };

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


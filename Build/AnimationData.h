#pragma once
#include "Coreminimal.h"

class AssetsManager;

class MtxNode
{
public:
	MtxNode();
	~MtxNode();


private:
	XMMATRIX* FrameMtx;
	string name;
	BOOL isRoot;

};

class AnimationData
{
public:
	AnimationData();
	~AnimationData();

private:
	string fileName;
	


};


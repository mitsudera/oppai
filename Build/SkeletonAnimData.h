#pragma once
#include "SkinMeshDataList.h"
class SkeletonAnimData
{

public:
	SkeletonAnimData();
	~SkeletonAnimData();
	void CreateSkeleton(int frameN);
	Skeleton* GetFrameSkeleton(int frame);
	void LoadSkeletonAnimData(string path);
	string GetFilePath(void);
	int GetFrameNum(void);

private:
	Skeleton* frameSkeleton;
	int frameNum;
	string filePath;
	
};


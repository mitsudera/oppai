#pragma once
#include "CoreMinimal.h"
#include "FbxConvertDX11.h"
XMMATRIX mtxrotX(float x);
XMMATRIX mtxrotY(float y);
XMMATRIX mtxrotZ(float z);


XMMATRIX mtxrotZYX(float x, float y, float z);
XMMATRIX mtxrotZXY(float x, float y, float z);
XMMATRIX mtxrotYXZ(float x, float y, float z);

XMMATRIX mtxrotXYZ(float x, float y, float z);

XMVECTOR xonevec(void);
XMVECTOR yonevec(void);
XMVECTOR zonevec(void);

float XMFLOAT3Length(XMFLOAT3 p1, XMFLOAT3 p2);

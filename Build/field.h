//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.h]
// Author : 
//
//=============================================================================
#pragma once
#include"main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitField(XMFLOAT3 pos, XMFLOAT3 rot,
	int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ);
void UninitField(void);
void UpdateField(void);
void DrawField(void);


void SetFieldHight(int xl, int xr, int zd, int zu, int n, float tl, float tr, float bl, float br);

float GetRandomFieldHightPM(int n);

//XMFLOAT3 GetVertexField(int x, int y);
BOOL RayHitField(XMFLOAT3 pos, XMFLOAT3* HitPosition, XMFLOAT3* Normal);

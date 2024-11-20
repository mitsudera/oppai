//=============================================================================
//
// �^�C�g����ʏ��� [battleui.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


struct HIT
{
	BOOL use;
	int life;
	int texno;

};
struct DESTROY
{
	BOOL use;
	int life;
	int texno;


};

struct ALERT
{
	BOOL use;
	int texno;
	int cnt;


};

struct SPEED
{
	int speed;
	int texno;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBattleui(void);
void UninitBattleui(void);
void UpdateBattleui(void);
void DrawBattleui(void);

void SetHit(void);

void SetDestroy(void);

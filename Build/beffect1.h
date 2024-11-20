//=============================================================================
//
// �p�[�e�B�N������ [beffect1.h]
// Author : 
//
//====================================================BEFFECT1=========================
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBeffect1(void);
void UninitBeffect1(void);
void UpdateBeffect1(void);
void DrawBeffect1(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type);
void SetColorBeffect1(int nIdxBeffect1, XMFLOAT4 col);

//void SetBombEffect1(XMFLOAT3 pos);



struct BEFFECT1
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scale;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	int				nLife;			// ����
	BOOL			use;			// �g�p���Ă��邩�ǂ���
	int				type;			// �^�C�v
};

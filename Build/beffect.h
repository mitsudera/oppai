//=============================================================================
//
// �p�[�e�B�N������ [beffect.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBeffect(void);
void UninitBeffect(void);
void UpdateBeffect(void);
void DrawBeffect(void);

int SetBeffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type);
void SetColorBeffect(int nIdxBeffect, XMFLOAT4 col);

void SetBombEffect(XMFLOAT3 pos);



struct BEFFECT
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

//=============================================================================
//
// �~�T�C�����f������ [misairu.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MISAIRU		(100)					// �~�T�C���̐�

#define	MISAIRU_SIZE		(5.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct MISAIRU
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	XMVECTOR			vec;
	bool				use;
	bool				load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	float				spd;				// �ړ��X�s�[�h
	float				size;				// �����蔻��̑傫��
	int muki;
	int move;
	int obj;//�ڕW
	int targettype;
	int pore;//�v���C���[���ڕW���G�l�~�[���ڕW���Hp=0,e=1,none=-1
	int life;

	float hseido;

	XMFLOAT3 oldpos;
	XMMATRIX			mtxrot;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMisairu(void);
void UninitMisairu(void);
void UpdateMisairu(void);
void DrawMisairu(void);

MISAIRU* GetMisairu(void);

void SetMisairu(XMFLOAT3 pos, XMVECTOR vec, XMMATRIX rot, float spd, int obj, int pore);

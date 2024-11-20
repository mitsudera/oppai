//=============================================================================
//
// ���f������ [b36.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_B36		(1)					// �v���C���[�̐�

#define	B36_SIZE		(5.0f)				// �����蔻��̑傫��
#define B36_PARTS_MAX	(6)								// �v���C���[�̃p�[�c�̐�


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct B36
{
	XMFLOAT3		pos;		// �|���S���̈ʒu
	XMFLOAT3		rot;		// �|���S���̌���(��])
	XMFLOAT3		scl;		// �|���S���̑傫��(�X�P�[��)
	float			plegth;
	XMFLOAT3		wpos;
	XMFLOAT3		ec;
	XMFLOAT4X4		mtxWorld;	// ���[���h�}�g���b�N�X

	int				hp;
	BOOL			load;
	DX11_MODEL		model;		// ���f�����

	float			spd;		// �ړ��X�s�[�h
	float			dir;		// ����
	float			size;		// �����蔻��̑傫��
	BOOL			use;

	XMVECTOR		axisZ;

	float oldrot;

	BOOL			breaked;

	XMFLOAT4		diffuse[MODEL_MAX_MATERIAL];

	// �e�́ANULL�A�q���͐e�̃A�h���X������(�Z�[�u�����[�h�̎��́������C�����鎖)
	B36			*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��b36�A�h���X

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitB36(void);
void UninitB36(void);
void UpdateB36(void);
void DrawB36(void);

B36 *GetB36(void);

void SetB36(XMFLOAT3 pos);
B36* GetB36Parts(void);
HRESULT MakeVertexCir1(void);
HRESULT MakeVertexCirRed1(void);

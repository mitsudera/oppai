//=============================================================================
//
// �G�l�~�[���f������ [sky.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "sky.h"
#include"player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_SKY			"data/MODEL/sky1.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define SKY_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define SKY_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static SKY			g_Sky[MAX_SKY];				// �G�l�~�[
int g_Sky_load = 0;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitSky(void)
{
	for (int i = 0; i < MAX_SKY; i++)
	{
		LoadModel(MODEL_SKY, &g_Sky[i].model);
		g_Sky[i].load = true;

		g_Sky[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Sky[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Sky[i].scl = XMFLOAT3(10000000.0f, 10000000.0f, 10000000.0f);

		g_Sky[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Sky[i].size = SKY_SIZE;	// �����蔻��̑傫��

		int muki = 0;
		int move = 1;


		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Sky[0].model, &g_Sky[0].diffuse[0]);




		g_Sky[i].use = true;		// true:�����Ă�

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSky(void)
{

	for (int i = 0; i < MAX_SKY; i++)
	{
		if (g_Sky[i].load)
		{
			UnloadModel(&g_Sky[i].model);
			g_Sky[i].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSky(void)
{
	PLAYER* g_Player = GetPlayer();

	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_SKY; i++)
	{
		if (g_Sky[i].use == true)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes
			
			switch (i)
			{
			case 0:	
				break;

			case 1:
				break;

			case 2:
				break;

			default:
				break;
			}

		}
	}

	//cnt += 1;


#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// ���f���̐F��ύX�ł����I�������ɂ��ł����B
	//	for (int j = 0; j < g_Sky[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Sky[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// ���f���̐F�����ɖ߂��Ă���
	//	for (int j = 0; j < g_Sky[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Sky[0].model, j, g_Sky[0].diffuse[j]);
	//	}
	//}
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSky(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_SKY; i++)
	{
		if (g_Sky[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Sky[i].scl.x, g_Sky[i].scl.y, g_Sky[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Sky[i].rot.x, g_Sky[i].rot.y+XM_PI , g_Sky[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Sky[i].pos.x, g_Sky[i].pos.y, g_Sky[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Sky[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Sky[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
SKY *GetSky()
{
	return &g_Sky[0];
}

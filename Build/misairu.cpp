//=============================================================================
//
// �~�T�C�����f������ [misairu.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "misairu.h"
#include"player.h"
#include"sound.h"
#include"enemy.h"
#include"beffect1.h"
#include"b36.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_MISAIRU			"data/MODEL/misairu.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(50.0f)						// �ړ���
#define	VALUE_ROTATEX		(XM_PI * 0.02f)				// 1f�̉�]��X
#define	VALUE_ROTATEY		(XM_PI * 0.02f)				// 1f�̉�]��Y

#define MISAIRU_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define MISAIRU_OFFSET_Y		(7.0f)						// �~�T�C���̑��������킹��
#define MAX_SPEED				(200.0f)
#define SMOKE_POP				(10)
#define PART_LIFE				(15)
#define SMOKE_SIZE				(1.0f)
#define SMOKE_RAND				(3)
#define SMOKE_MOVE				(0.1f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MISAIRU			g_Misairu[MAX_MISAIRU];				// �~�T�C��
int g_Misairu_load = 0;
static float hseido = XM_PI / 180;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMisairu(void)
{
	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		LoadModel(MODEL_MISAIRU, &g_Misairu[i].model);
		g_Misairu[i].load = true;

		g_Misairu[i].pos = XMFLOAT3(-50.0f + i * 30.0f, 7.0f, 20.0f);
		g_Misairu[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Misairu[i].scl = XMFLOAT3(2.0f, 2.0f, 2.0f);

		g_Misairu[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Misairu[i].size = MISAIRU_SIZE;	// �����蔻��̑傫��

		int muki = 0;
		int move = 1;


		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Misairu[0].model, &g_Misairu[0].diffuse[0]);




		g_Misairu[i].use = false;		//�ŏ��͖��g�p

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMisairu(void)
{

	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (g_Misairu[i].load)
		{
			UnloadModel(&g_Misairu[i].model);
			g_Misairu[i].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMisairu(void)
{
	PLAYER* g_Player = GetPlayer();
	ENEMY* g_Enemy = GetEnemy();
	B36* b36parts = GetB36Parts();
	XMVECTOR qton;
	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (g_Misairu[i].use == true)		// ���̃~�T�C�����g���Ă���H
		{								// Yes
			if (g_Misairu[i].spd < MAX_SPEED)
			{
				g_Misairu[i].spd += 1.0f;
			}


			XMVECTOR temp;
			XMFLOAT3 en ;
			XMVECTOR misvec ;
			XMVECTOR envec ;
			XMVECTOR em ;
			XMVECTOR anglev;
			float angle;
			XMVECTOR ziku;


			switch (g_Misairu[i].pore)
			{
			case -1:


				break;

			case 0:
				en = g_Player[0].pos;
				misvec = XMLoadFloat3(&g_Misairu[i].pos);
				envec = XMLoadFloat3(&en);
				em = misvec - envec;
				em = XMVector3Normalize(em);
				anglev = XMVector3AngleBetweenNormals(g_Misairu[i].vec, em);
				angle;
				XMStoreFloat(&angle, anglev);
				ziku = XMVector3Cross(g_Misairu[i].vec, em);
				ziku = XMVector3Normalize(ziku);
				if (angle < hseido)
				{
					qton = XMQuaternionRotationNormal(ziku, angle);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;
				}
				else
				{
					qton = XMQuaternionRotationNormal(ziku, hseido);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;
				}

				g_Misairu[i].mtxrot = XMMatrixMultiply(g_Misairu[i].mtxrot, XMMatrixRotationQuaternion(qton));




				break;

			case 1:

				en = g_Enemy[g_Misairu[i].obj].pos;
				misvec = XMLoadFloat3(&g_Misairu[i].pos);
				envec = XMLoadFloat3(&en);
				em =  misvec-envec;
				em = XMVector3Normalize(em);
				anglev=XMVector3AngleBetweenNormals(g_Misairu[i].vec, em);
				angle;
				XMStoreFloat(&angle, anglev);
				ziku = XMVector3Cross(g_Misairu[i].vec, em);

				if (angle< hseido)
				{
					qton = XMQuaternionRotationAxis(ziku, angle);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;
				}
				else
				{

					qton = XMQuaternionRotationAxis(ziku, hseido);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;


				}

				g_Misairu[i].mtxrot=XMMatrixMultiply(g_Misairu[i].mtxrot, XMMatrixRotationQuaternion(qton));
				//g_Misairu[i].mtxrot = XMMatrixRotationQuaternion(qton);
				

				break;

			case 2:

				en = b36parts[g_Misairu[i].obj].wpos;
				misvec = XMLoadFloat3(&g_Misairu[i].pos);
				envec = XMLoadFloat3(&en);
				em =  misvec-envec;
				em = XMVector3Normalize(em);
				anglev=XMVector3AngleBetweenNormals(g_Misairu[i].vec, em);
				angle;
				XMStoreFloat(&angle, anglev);
				ziku = XMVector3Cross(g_Misairu[i].vec, em);

				if (angle< hseido)
				{
					qton = XMQuaternionRotationAxis(ziku, angle);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;
				}
				else
				{

					qton = XMQuaternionRotationAxis(ziku, hseido);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;


				}

				g_Misairu[i].mtxrot=XMMatrixMultiply(g_Misairu[i].mtxrot, XMMatrixRotationQuaternion(qton));
				//g_Misairu[i].mtxrot = XMMatrixRotationQuaternion(qton);
				

				break;





			default:
				break;
			}





			g_Misairu[i].pos.x -= g_Misairu[i].vec.m128_f32[0] * g_Misairu[i].spd;
			g_Misairu[i].pos.y -= g_Misairu[i].vec.m128_f32[1] * g_Misairu[i].spd;
			g_Misairu[i].pos.z -= g_Misairu[i].vec.m128_f32[2] * g_Misairu[i].spd;

			g_Misairu[i].life -= 1;
			if (g_Misairu[i].life<=0)
			{
				g_Misairu[i].use = FALSE;
			}


			XMFLOAT3 diff;

			XMFLOAT3 pos;
			diff.x = g_Misairu[i].pos.x - g_Misairu[i].oldpos.x;
			diff.y = g_Misairu[i].pos.y - g_Misairu[i].oldpos.y;
			diff.z = g_Misairu[i].pos.z - g_Misairu[i].oldpos.z;



			for (int j = 0; j < 10; j++)
			{

				XMFLOAT3 enginepos = XMFLOAT3(0.0f, 0.1f, -28.0f);
				XMVECTOR envec = XMLoadFloat3(&enginepos);
				XMVECTOR enqton = XMQuaternionRotationMatrix(g_Misairu[i].mtxrot);
				envec = XMVector3Rotate(envec, enqton);
				XMStoreFloat3(&enginepos, envec);



				XMFLOAT2 moveangle;

				moveangle.x = (rand() % 618) * 0.01f;
				moveangle.y = (rand() % 618) * 0.01f;

				XMFLOAT3 move;

				move.x = sinf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.z = cosf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.y = sinf(moveangle.x) * SMOKE_MOVE;


				pos.x = g_Misairu[i].oldpos.x + j * (diff.x / SMOKE_POP) + enginepos.x;
				pos.y = g_Misairu[i].oldpos.y + j * (diff.y / SMOKE_POP) + enginepos.y;
				pos.z = g_Misairu[i].oldpos.z + j * (diff.z / SMOKE_POP) + enginepos.z;
				//float fSize = (float)((rand() % 30 + 20) * SMOKE_SIZE);
				float fSize = SMOKE_SIZE;
				SetParticle(pos, move, XMFLOAT4(0.9f, 0.9f, 0.9f, 0.85f), fSize, fSize, PART_LIFE + (rand() % PART_LIFE), 1);
			}

			g_Misairu[i].oldpos = g_Misairu[i].pos;


		}

	}


	//cnt += 1;


#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// ���f���̐F��ύX�ł����I�������ɂ��ł����B
	//	for (int j = 0; j < g_Misairu[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Misairu[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// ���f���̐F�����ɖ߂��Ă���
	//	for (int j = 0; j < g_Misairu[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Misairu[0].model, j, g_Misairu[0].diffuse[j]);
	//	}
	//}

#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMisairu(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (g_Misairu[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Misairu[i].scl.x, g_Misairu[i].scl.y, g_Misairu[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxWorld = XMMatrixMultiply(mtxWorld, g_Misairu[i].mtxrot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Misairu[i].pos.x, g_Misairu[i].pos.y, g_Misairu[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Misairu[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Misairu[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �~�T�C���̎擾
//=============================================================================
MISAIRU *GetMisairu()
{
	return &g_Misairu[0];
}

//=============================================================================
// �~�T�C���̃Z�b�g
//=============================================================================
void SetMisairu(XMFLOAT3 pos, XMVECTOR vec,XMMATRIX rot,float spd,int obj,int pore)
{
	for (int i = 0; i < MAX_MISAIRU; i++)
	{

		if (g_Misairu[i].use == FALSE)
		{
		PlaySound(SOUND_LABEL_SE_misairu001);

			g_Misairu[i].use = TRUE;
			g_Misairu[i].pos = pos;
			g_Misairu[i].vec = vec;
			g_Misairu[i].mtxrot = rot;
			g_Misairu[i].spd = spd+50.0f;
			g_Misairu[i].obj = obj;
			g_Misairu[i].pore = pore;
			g_Misairu[i].life = 180;
			return;
		}
	}


}




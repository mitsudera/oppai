//=============================================================================
//
// ���f������ [b36.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "b36.h"
#include "light.h"
#include"player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	MODEL_B36		"data/MODEL/b36.obj"			// �ǂݍ��ރ��f����
//#define	MODEL_B36		"data/MODEL/cone.obj"			// �ǂݍ��ރ��f����
//#define	MODEL_B36_PARTS	"data/MODEL/torus.obj"			// �ǂݍ��ރ��f����


#define	MODEL_B36		"data/MODEL/b-36.obj"	// �ǂݍ��ރ��f����
#define	MODEL_FUN		"data/MODEL/fun.obj"	// �ǂݍ��ރ��f����
#define	MODEL_HOUDAI	"data/MODEL/houdai.obj"	// �ǂݍ��ރ��f����
#define	MODEL_HOUTOU	"data/MODEL/houtou.obj"	// �ǂݍ��ރ��f����



#define TEXTURE_MAX			(3)


#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define B36_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define B36_OFFSET_Y		(7.0f)							// �v���C���[�̑��������킹��

#define MOTION_BLUR		(3)								//���[�V�����u���[�̐�
#define FUN_SPEED		(XM_PI*0.2)					//�v���y���̉�]���x
#define FUN_MAX			(6)
#define CANON_MAX		(6)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static B36		g_B36;						// �v���C���[

static B36		g_Parts[B36_PARTS_MAX];		// �v���C���[�̃p�[�c�p


static ID3D11Buffer* g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11Buffer* g_VertexBufferRed = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����


static char* g_TextureName[] =
{
	"data/TEXTURE/siromaru.png",
	"data/TEXTURE/sirowaku.png",
	"data/TEXTURE/sirohisi.png",
};
static MATERIAL g_cirmat;
static MATERIAL g_redmat;




//=============================================================================
// ����������
//=============================================================================
HRESULT InitB36(void)
{
	LoadModel(MODEL_B36, &g_B36.model);
	g_B36.load = TRUE;

	g_B36.pos = { 0.0f, B36_OFFSET_Y, 0.0f };
	g_B36.rot = { 0.0f, 0.0f, 0.0f };
	g_B36.scl = { 1.0f, 1.0f, 1.0f };

	g_B36.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	g_B36.size = B36_SIZE;	// �����蔻��̑傫��

	g_B36.use = FALSE;

	g_B36.axisZ = zonevec();

	g_B36.breaked = FALSE;


	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}



	MakeVertexCir1();
	MakeVertexCirRed1();

	ZeroMemory(&g_cirmat, sizeof(g_cirmat));
	g_cirmat.Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	ZeroMemory(&g_redmat, sizeof(g_redmat));
	g_redmat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);



	// �K�w�A�j���[�V�����p�̏���������
	g_B36.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	// �p�[�c�̏�����
	for (int i = 0; i < B36_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Parts[i].breaked = FALSE;
		g_Parts[i].hp = 100;

		// �e�q�֌W
		g_Parts[i].parent = &g_B36;		// �� �����ɐe�̃A�h���X������
	//	g_Parts[�r].parent= &g_B36;		// �r��������e�͖{�́i�v���C���[�j
	//	g_Parts[��].parent= &g_Paerts[�r];	// �w���r�̎q���������ꍇ�̗�

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		g_Parts[i].load = 0;
	}

	g_Parts[0].use = FALSE;
	g_Parts[0].parent = &g_B36;	// �e���Z�b�g
	g_Parts[0].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[0].model);
	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_Parts[0].model, &g_Parts[0].diffuse[0]);

	g_Parts[1].use = FALSE;
	g_Parts[1].parent = &g_B36;	// �e���Z�b�g
	g_Parts[1].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[1].model);
	GetModelDiffuse(&g_Parts[1].model, &g_Parts[1].diffuse[0]);


	g_Parts[2].use = FALSE;
	g_Parts[2].parent = &g_B36;	// �e���Z�b�g
	g_Parts[2].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[2].model);
	GetModelDiffuse(&g_Parts[2].model, &g_Parts[2].diffuse[0]);


	g_Parts[3].use = FALSE;
	g_Parts[3].parent = &g_B36;	// �e���Z�b�g
	g_Parts[3].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[3].model);
	GetModelDiffuse(&g_Parts[3].model, &g_Parts[3].diffuse[0]);


	g_Parts[4].use = FALSE;
	g_Parts[4].parent = &g_B36;	// �e���Z�b�g
	g_Parts[4].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[4].model);
	GetModelDiffuse(&g_Parts[4].model, &g_Parts[4].diffuse[0]);


	g_Parts[5].use = FALSE;
	g_Parts[5].parent = &g_B36;	// �e���Z�b�g
	g_Parts[5].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[5].model);
	GetModelDiffuse(&g_Parts[5].model, &g_Parts[5].diffuse[0]);

	//g_Parts[6].use = FALSE;
	//g_Parts[6].parent = &g_B36;	// �e���Z�b�g
	//g_Parts[6].load = 1;
	//g_Parts[6].scl = { 0.5f,0.5f,0.5f };
	//LoadModel(MODEL_HOUDAI, &g_Parts[6].model);
	//GetModelDiffuse(&g_Parts[6].model, &g_Parts[6].diffuse[0]);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitB36(void)
{
	// ���f���̉������
	if (g_B36.load)
	{
		UnloadModel(&g_B36.model);
		g_B36.load = FALSE;
	}


	// �p�[�c�̉������
	for (int i = 0; i < B36_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// �p�[�c�̉������
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateB36(void)
{
	CAMERA *camera = GetCamera();
	PLAYER* player = GetPlayer();

	XMFLOAT3 pl = player[0].pos;
	XMVECTOR plvec = XMLoadFloat3(&pl);
	XMVECTOR camvec = XMLoadFloat3(&camera->pos);


	for (int i = 0; i < B36_PARTS_MAX; i++)
	{

		g_Parts[i].wpos = XMFLOAT3(g_B36.pos.x+ g_Parts[i].pos.x*g_B36.scl.x, g_B36.pos.y+ g_Parts[i].pos.y*g_B36.scl.y, g_B36.pos.z+ g_Parts[i].pos.z*g_B36.scl.z);

		XMVECTOR envec = XMLoadFloat3(&g_Parts[i].wpos);
		XMVECTOR ep = envec - plvec;

		XMVECTOR eplen = XMVector3Length(ep);
		XMStoreFloat(&g_Parts[i].plegth, eplen);
		XMVECTOR ec = envec - camvec;
		ec = XMVector3Normalize(ec);
		XMStoreFloat3(&g_Parts[i].ec, ec);




	}





#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_B36.pos.z = g_B36.pos.x = 0.0f;
		g_B36.rot.y = g_B36.dir = 0.0f;
		g_B36.spd = 0.0f;
	}
#endif

	BOOL destroy = TRUE;
	for (int i = 0; i < B36_PARTS_MAX; i++)
	{
		if (g_Parts[i].breaked == FALSE)
		{
			destroy = FALSE;



			g_Parts[i].oldrot = g_Parts[i].rot.z;
			if (i < 3)
			{
				g_Parts[i].rot.z += FUN_SPEED;

			}
			else if (i >= 3 && i < 6)
			{
				g_Parts[i].rot.z -= FUN_SPEED;


			}
		}
	}

	if (destroy == TRUE)
	{
		g_B36.pos.y -= 100.0f;

		if (g_B36.pos.y<0.0f)
		{
			g_B36.use = FALSE;
		}

	}
	g_B36.pos.z += 60.0f;


#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("B36:�� �� �� ���@Space\n");
	PrintDebugProc("B36:X:%f Y:%f Z:%f\n", g_Parts[0].wpos.x,g_Parts[0].wpos.y,g_Parts[0].wpos.z);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawB36(void)
{
	CAMERA* camera = GetCamera();

	PLAYER* player = GetPlayer();
	if (g_B36.use == FALSE) return;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �܂��͊�{�ƂȂ�a�n�c�x��`�悷��

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_B36.scl.x, g_B36.scl.y, g_B36.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_B36.rot.x, g_B36.rot.y + XM_PI, g_B36.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_B36.pos.x, g_B36.pos.y, g_B36.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	// ������`�悵���Ƃ��ɂ��悤�����}�g���N�X��ۑ����Ă���
	XMStoreFloat4x4(&g_B36.mtxWorld, mtxWorld);


	// ���f���`��
	DrawModel(&g_B36.model);



	// �p�[�c�̊K�w�A�j���[�V����
	for (int i = 0; i < B36_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();



		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ��
			// g_B36.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
		if (g_Parts[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		// ���f���`��
		DrawModel(&g_Parts[i].model);

		for (int j = 0; j < MOTION_BLUR; j++)
		{


			if (g_Parts[i].breaked == FALSE)
			{


				// ���[���h�}�g���b�N�X�̏�����
				mtxWorld = XMMatrixIdentity();



				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				float rotdif = g_Parts[i].rot.z - g_Parts[i].oldrot;
				float zrot = g_Parts[i].oldrot + (rotdif / MOTION_BLUR) * j;

				// ��]�𔽉f
				mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, zrot);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

				if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
				{
					mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
					// ��
					// g_B36.mtxWorld���w���Ă���
				}

				XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

				// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
				if (g_Parts[i].use == FALSE) continue;

				// ���[���h�}�g���b�N�X�̐ݒ�
				SetWorldMatrix(&mtxWorld);

				//for (int j = 0; j < g_Parts[0].model.SubsetNum; j++)
				//{
				//	SetModelDiffuse(&g_Parts[0].model, j, XMFLOAT4(g_Parts[i].diffuse[0].x, g_Parts[i].diffuse[0].y, g_Parts[i].diffuse[0].z, 0.1f));
				//}

				//// ���f���`��
				DrawModel(&g_Parts[i].model);


				//for (int j = 0; j < g_Parts[0].model.SubsetNum; j++)
				//{
				//	SetModelDiffuse(&g_Parts[0].model, j, g_Parts[i].diffuse[0]);
				//}

			}



							// ���e�X�g�ݒ�
		// ���e�X�g��L����
			SetAlphaTestEnable(TRUE);

			SetCullingMode(CULL_MODE_NONE);

			// ���C�e�B���O�𖳌�
			SetLightEnable(FALSE);


			if (player->target == i && player->targettype == 2)
			{
				// ���_�o�b�t�@�ݒ�
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBufferRed, &stride, &offset);

			}
			else
			{
				// ���_�o�b�t�@�ݒ�
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);


			}


			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			XMMATRIX mtxView = XMLoadFloat4x4(&camera->mtxView);




			// �����s��i�����s��j��]�u�s�񂳂��ċt�s�������Ă��(����)
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];




			//// �X�P�[���𔽉f
			//float sclk = (1 / g_Enemy[i].plegth);

			mtxScl = XMMatrixScaling(0.1f, 0.1f, 0.1f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			//mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);

			//mtxRot = mtxrotYXZ(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);

			//mtxWorld = XMMatrixMultiply(mtxWorld, g_Enemy[i].eprot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(camera[0].pos.x + g_Parts[i].ec.x * 50.0f, camera[0].pos.y + g_Parts[i].ec.y * 50.0f, camera[0].pos.z + g_Parts[i].ec.z * 50.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);




			// �}�e���A���ݒ�
			SetMaterial(g_cirmat);
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);

			if (player->target == i && player->targettype == 2)
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

				// �|���S���̕`��
				GetDeviceContext()->Draw(4, 0);


			}


			//DrawModel(&torus.model);



			// ���C�e�B���O��L����
			SetLightEnable(TRUE);


			SetCullingMode(CULL_MODE_BACK);


			// ���e�X�g�𖳌���
			SetAlphaTestEnable(FALSE);



		}



	}


	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
B36 *GetB36(void)
{
	return &g_B36;
}

void SetB36(XMFLOAT3 pos)
{
	g_B36.pos = pos;
	g_B36.rot = { 0.0f, 0.0f, 0.0f };
	g_B36.scl = { 100.0f, 100.0f, 100.0f };

	g_B36.spd = 0.0f;			// �ړ��X�s�[�h�N���A

	g_B36.use = TRUE;




	// �K�w�A�j���[�V�����p�̏���������
	g_B36.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������


	g_Parts[0].use = TRUE;
	g_Parts[0].pos = { 8.6f,-0.1f,3.5f };
	g_Parts[0].rot = { 0.0f,-0.05f,0.0f };
	g_Parts[0].breaked = FALSE;

	g_Parts[1].use = TRUE;
	g_Parts[1].pos = { 15.3f,-0.1f,3.5f };
	g_Parts[1].rot = { 0.0f,-0.05f,0.0f };
	g_Parts[1].breaked = FALSE;

	g_Parts[2].use = TRUE;
	g_Parts[2].pos = { 22.2f,-0.1f,3.5f };
	g_Parts[2].rot = { 0.0f,-0.05f,0.0f };
	g_Parts[2].breaked = FALSE;

	g_Parts[3].use = TRUE;
	g_Parts[3].pos = { -8.6f,-0.1f,3.5f };
	g_Parts[3].rot = { 0.0f,0.05f,0.0f };
	g_Parts[3].breaked = FALSE;

	g_Parts[4].use = TRUE;
	g_Parts[4].pos = { -15.3f,-0.1f,3.5f };
	g_Parts[4].rot = { 0.0f,0.05f,0.0f };
	g_Parts[4].breaked = FALSE;

	g_Parts[5].use = TRUE;
	g_Parts[5].pos = { -22.2f,-0.1f,3.5f };
	g_Parts[5].rot = { 0.0f,0.05f,0.0f };
	g_Parts[5].breaked = FALSE;

	//g_Parts[6].use = TRUE;
	//g_Parts[6].pos = { 0.0f,0.0f,0.0f };
	//g_Parts[6].rot = { 0.0f,0.05f,0.0f };
	//g_Parts[6].breaked = FALSE;






}

B36* GetB36Parts(void)
{
	return &g_Parts[0];
}

HRESULT MakeVertexCir1(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = 20.0f;
	float fHeight = 20.0f;

	// ���_���W�̐ݒ�
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight / 2, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight / 2, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, -fHeight / 2, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, -fHeight / 2, 0.0f);

	// �@���̐ݒ�
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// �g�U���̐ݒ�
	vertex[0].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}
HRESULT MakeVertexCirRed1(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBufferRed);

	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBufferRed, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = 20.0f;
	float fHeight = 20.0f;

	// ���_���W�̐ݒ�
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight / 2, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight / 2, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, -fHeight / 2, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, -fHeight / 2, 0.0f);

	// �@���̐ݒ�
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// �g�U���̐ݒ�
	vertex[0].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBufferRed, 0);

	return S_OK;
}

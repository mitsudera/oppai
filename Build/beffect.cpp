//=============================================================================
//
// �p�[�e�B�N������ [beffect.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "light.h"
#include "beffect.h"
#include "player.h"
#include"renderer.h"
#include"debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)			// �e�N�X�`���̐�
//
//#define	g_aBeffect[nCntBeffect].fSizeX		(1.0f)		// ���_�T�C�Y
//#define	g_aBeffect[nCntBeffect].fSizeY		(1.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_BEFFECT	(5.0f)		// �ړ����x

#define	MAX_BEFFECT		(1024)		// �p�[�e�B�N���ő吔

#define	DISP_SHADOW						// �e�̕\��

#define MAX_POLYGON			(MAX_BEFFECT*2)
#define MAX_VERTEX			(MAX_POLYGON*6)
#define BEFFECT_SIZE		(1.0f)
#define POLYGON_VERTEX		(6)
#define CLOUD_ALPHA			(0.8f)
#define BOMB_MOVE			(10.0f)
#define BOMB_POP			(100)
#define BOMB_LIFE			(60)
#define BOMB_SIZE			(100.0f)
//#undef DISP_SHADOW

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBeffect(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_�o�b�t�@
// ���_�z��
static VERTEX_3D g_VertexArray[MAX_BEFFECT][POLYGON_VERTEX];




static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;					// �e�N�X�`���ԍ�

static BEFFECT					g_aBeffect[MAX_BEFFECT];		// �p�[�e�B�N�����[�N
static XMFLOAT3					g_posBase;						// �r���{�[�h�����ʒu
static float					g_fWidthBase = 5.0f;			// ��̕�
static float					g_fHeightBase = 10.0f;			// ��̍���
static float					g_roty = 0.0f;					// �ړ�����
static float					g_spd = 0.0f;					// �ړ��X�s�[�h
static int						g_partcnt = 0;

static XMFLOAT3 g_vpos[4];


static char* g_TextureName[] =
{
	//"data/TEXTURE/effect000.jpg",
	//"data/TEXTURE/siro.png",
	"data/TEXTURE/particle_smoke.png",
	//"data/TEXTURE/beffect_sakura.png",
};

static BOOL							g_Load = FALSE;

// �R���s���[�g�V�F�[�_�[
static ID3D11ComputeShader* mComputeShader = NULL;

// �p�[�e�B�N��
//static BEFFECT* mbeffect;
// �o�b�t�@
//static ID3D11Buffer* mVertexBuffer;
static ID3D11Buffer* mpBeffectBuffer;
static ID3D11Buffer* mpResultBuffer;
static ID3D11Buffer* mpPositionBuffer;
// SRV
static ID3D11ShaderResourceView* mpBeffectSRV;
static ID3D11ShaderResourceView* mpPositionSRV;
// UAV
static ID3D11UnorderedAccessView* mpResultUAV;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBeffect(void)
{
	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// �p�[�e�B�N�����[�N�̏�����
	for (int nCntBeffect = 0; nCntBeffect < MAX_BEFFECT; nCntBeffect++)
	{
		g_aBeffect[nCntBeffect].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aBeffect[nCntBeffect].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aBeffect[nCntBeffect].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aBeffect[nCntBeffect].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_aBeffect[nCntBeffect].material, sizeof(g_aBeffect[nCntBeffect].material));
		g_aBeffect[nCntBeffect].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		//g_aBeffect[nCntBeffect].fSizeX = g_aBeffect[nCntBeffect].fSizeX;
		//g_aBeffect[nCntBeffect].fSizeY = g_aBeffect[nCntBeffect].fSizeY;
		g_aBeffect[nCntBeffect].nLife = 0;
		g_aBeffect[nCntBeffect].use = FALSE;
		g_aBeffect[nCntBeffect].type = 1;



		for (int i = 0; i < 4; i++)
		{
			g_vpos[i] = g_aBeffect[nCntBeffect].pos;
		}

		g_vpos[0].x -= g_aBeffect[nCntBeffect].fSizeX / 2;
		g_vpos[0].y += g_aBeffect[nCntBeffect].fSizeY / 2;


		g_vpos[1].x += g_aBeffect[nCntBeffect].fSizeX / 2;
		g_vpos[1].y += g_aBeffect[nCntBeffect].fSizeY / 2;


		g_vpos[2].x -= g_aBeffect[nCntBeffect].fSizeX / 2;
		g_vpos[2].y -= g_aBeffect[nCntBeffect].fSizeY / 2;


		g_vpos[3].x += g_aBeffect[nCntBeffect].fSizeX / 2;
		g_vpos[3].y -= g_aBeffect[nCntBeffect].fSizeY / 2;



		g_VertexArray[nCntBeffect][0] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
		g_VertexArray[nCntBeffect][1] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
		g_VertexArray[nCntBeffect][2] = { g_vpos[1], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };
		g_VertexArray[nCntBeffect][3] = { g_vpos[2], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
		g_VertexArray[nCntBeffect][4] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
		g_VertexArray[nCntBeffect][5] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };




	}

	MakeVertexBeffect();








	//�o�b�t�@����
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	hBufferDesc.ByteWidth = sizeof(BEFFECT);
	hBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	hBufferDesc.StructureByteStride = (UINT)MAX_BEFFECT;
	hBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	hBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	//// �萔�o�b�t�@����
	//D3D11_BUFFER_DESC hBufferDesc;
	//hBufferDesc.ByteWidth = sizeof(BEFFECT);
	//hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//hBufferDesc.CPUAccessFlags = 0;
	//hBufferDesc.MiscFlags = 0;
	//hBufferDesc.StructureByteStride = (UINT)MAX_BEFFECT;


	GetDevice()->CreateBuffer(&hBufferDesc, NULL, &mpBeffectBuffer);
	GetDevice()->CreateBuffer(&hBufferDesc, NULL, &mpResultBuffer);


	hBufferDesc.ByteWidth = sizeof(XMFLOAT3);

	GetDevice()->CreateBuffer(&hBufferDesc, NULL, &mpPositionBuffer);


	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srDesc.Texture2D.MipLevels = -1;  // ���ׂẴ~�b�v�}�b�v ���x��



	// SRV����
	GetDevice()->CreateShaderResourceView(
		mpBeffectBuffer,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&srDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&mpBeffectSRV);  // �󂯎��ϐ�




	// SRV����
	GetDevice()->CreateShaderResourceView(
		mpPositionBuffer,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&srDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&mpPositionSRV);  // �󂯎��ϐ�



	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;

	ZeroMemory(&uavDesc, sizeof(uavDesc));

	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 0;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;


	// UAV����
	GetDevice()->CreateUnorderedAccessView(
		mpResultBuffer,          // �A�N�Z�X����e�N�X�`�� ���\�[�X
		&uavDesc,               // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
		&mpResultUAV);  // �󂯎��ϐ�



	//D3DX11CompileFromFile("shader.hlsl",
	//	NULL,
	//	NULL,
	//	"CSFunc",
	//	"cs_4_0",
	//	shFlag,
	//	0,
	//	NULL,
	//	&pPSBlob,
	//	&pErrorBlob,
	//	NULL);





	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBeffect(void)
{
	if (g_Load == FALSE) return;

	//�e�N�X�`���̉��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	if (mComputeShader)		mComputeShader->Release();

	if (mpBeffectBuffer)	mpBeffectBuffer->Release();
	if (mpResultBuffer)		mpResultBuffer->Release();
	if (mpPositionBuffer)	mpPositionBuffer->Release();
	if (mpBeffectSRV)		mpResultBuffer->Release();
	if (mpPositionSRV)		mpBeffectBuffer->Release();
	if (mpResultUAV)		mpResultBuffer->Release();




	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBeffect(void)
{

	CAMERA* cam = GetCamera();

	for (int nCntBeffect = 0; nCntBeffect < MAX_BEFFECT; nCntBeffect++)
	{
		if (g_aBeffect[nCntBeffect].use)
		{// �g�p��
			switch (g_aBeffect[nCntBeffect].type)
			{
			case 0:
				g_aBeffect[nCntBeffect].pos.x += g_aBeffect[nCntBeffect].move.x;
				g_aBeffect[nCntBeffect].pos.z += g_aBeffect[nCntBeffect].move.z;

				g_aBeffect[nCntBeffect].pos.y += g_aBeffect[nCntBeffect].move.y;
				if (g_aBeffect[nCntBeffect].pos.y <= g_aBeffect[nCntBeffect].fSizeY / 2)
				{// ���n����
					g_aBeffect[nCntBeffect].pos.y = g_aBeffect[nCntBeffect].fSizeY / 2;
					g_aBeffect[nCntBeffect].move.y = -g_aBeffect[nCntBeffect].move.y * 0.75f;
				}

				g_aBeffect[nCntBeffect].move.x += (0.0f - g_aBeffect[nCntBeffect].move.x) * 0.015f;
				g_aBeffect[nCntBeffect].move.y -= 0.25f;
				g_aBeffect[nCntBeffect].move.z += (0.0f - g_aBeffect[nCntBeffect].move.z) * 0.015f;


				g_aBeffect[nCntBeffect].nLife--;
				if (g_aBeffect[nCntBeffect].nLife <= 0)
				{
					g_aBeffect[nCntBeffect].use = FALSE;
					g_partcnt -= 1;
				}
				else
				{
					if (g_aBeffect[nCntBeffect].nLife <= 80)
					{
						g_aBeffect[nCntBeffect].material.Diffuse.x = 0.8f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.8f;
						g_aBeffect[nCntBeffect].material.Diffuse.y = 0.7f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.7f;
						g_aBeffect[nCntBeffect].material.Diffuse.z = 0.2f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.2f;
					}

					if (g_aBeffect[nCntBeffect].nLife <= 20)
					{
						// ���l�ݒ�
						g_aBeffect[nCntBeffect].material.Diffuse.w -= 0.05f;
						if (g_aBeffect[nCntBeffect].material.Diffuse.w < 0.0f)
						{
							g_aBeffect[nCntBeffect].material.Diffuse.w = 0.0f;
						}
					}
				}
			case 1:
				g_aBeffect[nCntBeffect].pos.x += g_aBeffect[nCntBeffect].move.x;
				g_aBeffect[nCntBeffect].pos.z += g_aBeffect[nCntBeffect].move.z;
				g_aBeffect[nCntBeffect].pos.y += g_aBeffect[nCntBeffect].move.y;

				g_aBeffect[nCntBeffect].nLife--;
				if (g_aBeffect[nCntBeffect].nLife <= 0)
				{
					g_aBeffect[nCntBeffect].use = FALSE;
					g_partcnt -= 1;
				}
				else
				{
					if (g_aBeffect[nCntBeffect].nLife <= 80)
					{
						//g_aBeffect[nCntBeffect].material.Diffuse.x = 0.9f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.9f;
						//g_aBeffect[nCntBeffect].material.Diffuse.y = 0.9f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.9f;
						//g_aBeffect[nCntBeffect].material.Diffuse.z = 0.9f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.9f;
					}

					if (g_aBeffect[nCntBeffect].nLife <= 20)
					{
						// ���l�ݒ�
						g_aBeffect[nCntBeffect].material.Diffuse.w -= 0.05f;
						if (g_aBeffect[nCntBeffect].material.Diffuse.w < 0.0f)
						{
							g_aBeffect[nCntBeffect].material.Diffuse.w = 0.0f;
						}
					}
				}
			case 2:
				g_aBeffect[nCntBeffect].pos.x += g_aBeffect[nCntBeffect].move.x;
				g_aBeffect[nCntBeffect].pos.z += g_aBeffect[nCntBeffect].move.z;
				g_aBeffect[nCntBeffect].pos.y += g_aBeffect[nCntBeffect].move.y;

				g_aBeffect[nCntBeffect].nLife--;
				if (g_aBeffect[nCntBeffect].nLife <= 0)
				{
					g_aBeffect[nCntBeffect].use = FALSE;
					g_partcnt -= 1;
				}
				else
				{
					if (g_aBeffect[nCntBeffect].nLife <= 80)
					{
						//g_aBeffect[nCntBeffect].material.Diffuse.x = 0.9f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.9f;
						//g_aBeffect[nCntBeffect].material.Diffuse.y = 0.9f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.9f;
						//g_aBeffect[nCntBeffect].material.Diffuse.z = 0.9f - (float)(80 - g_aBeffect[nCntBeffect].nLife) / 80 * 0.9f;
					}

					if (g_aBeffect[nCntBeffect].nLife <= 20)
					{
						// ���l�ݒ�
						g_aBeffect[nCntBeffect].material.Diffuse.w -= 0.005f;
						if (g_aBeffect[nCntBeffect].material.Diffuse.w < 0.0f)
						{
							g_aBeffect[nCntBeffect].material.Diffuse.w = 0.0f;
						}
					}
				}

			default:
				break;
			}


			XMFLOAT3 vpos[4];

			vpos[0].x = -g_aBeffect[nCntBeffect].fSizeX / 2;
			vpos[0].y = +g_aBeffect[nCntBeffect].fSizeY / 2;
			vpos[0].z = 0.0f;

			vpos[1].x = +g_aBeffect[nCntBeffect].fSizeX / 2;
			vpos[1].y = +g_aBeffect[nCntBeffect].fSizeY / 2;
			vpos[1].z = 0.0f;

			vpos[2].x = -g_aBeffect[nCntBeffect].fSizeX / 2;
			vpos[2].y = -g_aBeffect[nCntBeffect].fSizeY / 2;
			vpos[2].z = 0.0f;

			vpos[3].x = +g_aBeffect[nCntBeffect].fSizeX / 2;
			vpos[3].y = -g_aBeffect[nCntBeffect].fSizeY / 2;
			vpos[3].z = 0.0f;

			XMVECTOR vvec[4];


			//XMVECTOR axx= XMVector3Normalize(cam->axisX);

			//XMVECTOR axy= XMVector3Normalize(cam->axisY);


			XMVECTOR axx;

			XMVECTOR axy;

			XMFLOAT4X4 view = cam->mtxView;

			XMFLOAT3 vx = XMFLOAT3(view._11, view._21, view._31);
			axx = XMLoadFloat3(&vx);
			XMFLOAT3 vy = XMFLOAT3(view._12, view._22, view._32);
			axy = XMLoadFloat3(&vy);
			for (int i = 0; i < 4; i++)
			{

				vvec[i] += XMLoadFloat3(&vpos[i]);
				vvec[i] = XMVector3Normalize(vvec[i]);
				switch (i)
				{
				case 0:

					vvec[i] = axy * g_aBeffect[nCntBeffect].fSizeY;
					break;
				case 1:
					vvec[i] = axx * g_aBeffect[nCntBeffect].fSizeX;
					break;


				case 2:


					vvec[i] = axx * -g_aBeffect[nCntBeffect].fSizeX;
					break;

				case 3:

					vvec[i] = axy * -g_aBeffect[nCntBeffect].fSizeY;
					break;


				default:
					break;
				}

				//XMVector3Rotate(vvec[i],cam->qtonrot);
				XMStoreFloat3(&vpos[i], vvec[i]);

			}







			for (int i = 0; i < 4; i++)
			{
				g_vpos[i] = g_aBeffect[nCntBeffect].pos;
			}



			//g_vpos[0].x -= g_aBeffect[nCntBeffect].fSizeX / 2;
			//g_vpos[0].y += g_aBeffect[nCntBeffect].fSizeY / 2;

			//g_vpos[1].x += g_aBeffect[nCntBeffect].fSizeX / 2;
			//g_vpos[1].y += g_aBeffect[nCntBeffect].fSizeY / 2;

			//g_vpos[2].x -= g_aBeffect[nCntBeffect].fSizeX / 2;
			//g_vpos[2].y -= g_aBeffect[nCntBeffect].fSizeY / 2;

			//g_vpos[3].x += g_aBeffect[nCntBeffect].fSizeX / 2;
			//g_vpos[3].y -= g_aBeffect[nCntBeffect].fSizeY / 2;

			g_vpos[0].x += vpos[0].x;
			g_vpos[0].y += vpos[0].y;
			g_vpos[0].z += vpos[0].z;

			g_vpos[1].x += vpos[1].x;
			g_vpos[1].y += vpos[1].y;
			g_vpos[1].z += vpos[1].z;

			g_vpos[2].x += vpos[2].x;
			g_vpos[2].y += vpos[2].y;
			g_vpos[2].z += vpos[2].z;

			g_vpos[3].x += vpos[3].x;
			g_vpos[3].y += vpos[3].y;
			g_vpos[3].z += vpos[3].z;





			g_VertexArray[nCntBeffect][0] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect[nCntBeffect].material.Diffuse, XMFLOAT2(0.0f, 0.0f) };
			g_VertexArray[nCntBeffect][1] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect[nCntBeffect].material.Diffuse, XMFLOAT2(0.0f, 0.0f) };
			g_VertexArray[nCntBeffect][2] = { g_vpos[1], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect[nCntBeffect].material.Diffuse, XMFLOAT2(1.0f, 0.0f) };
			g_VertexArray[nCntBeffect][3] = { g_vpos[2], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect[nCntBeffect].material.Diffuse, XMFLOAT2(0.0f, 1.0f) };
			g_VertexArray[nCntBeffect][4] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect[nCntBeffect].material.Diffuse, XMFLOAT2(1.0f, 1.0f) };
			g_VertexArray[nCntBeffect][5] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f,1.0f), g_aBeffect[nCntBeffect].material.Diffuse, XMFLOAT2(1.0f, 1.0f) };

		}
		else
		{
			g_VertexArray[nCntBeffect][0] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
			g_VertexArray[nCntBeffect][1] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
			g_VertexArray[nCntBeffect][2] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };
			g_VertexArray[nCntBeffect][3] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
			g_VertexArray[nCntBeffect][4] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
			g_VertexArray[nCntBeffect][5] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };



		}


	}



	//// ���_�o�b�t�@�ɒl���Z�b�g����
	//D3D11_MAPPED_SUBRESOURCE msr;
	//GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	//// �S���_���𖈉�㏑�����Ă���̂�DX11�ł͂��̕�����������ł�
	//memcpy(pVtx, g_VertexArray, sizeof(VERTEX_3D)* MAX_VERTEX);

	//GetDeviceContext()->Unmap(g_VertexBuffer, 0);



	if (g_VertexBuffer)			g_VertexBuffer->Release();
	MakeVertexBeffect();


	//GetDeviceContext()->UpdateSubresource(g_VertexBuffer, 0, NULL, &g_VertexArray, 0, 0);

	// �p�[�e�B�N������
	//{
	//	XMFLOAT3 pos;
	//	XMFLOAT3 move;
	//	float fAngle, fLength;
	//	int nLife;
	//	float fSize;

	//	pos = g_posBase;

	//	fAngle = (float)(rand() % 628 - 314) / 100.0f;
	//	fLength = rand() % (int)(g_fWidthBase * 200 ) / 100.0f - g_fWidthBase;
	//	move.x = sinf(fAngle) * fLength;
	//	move.y = rand() % 300 / 100.0f + g_fHeightBase;
	//	move.z = cosf(fAngle) * fLength;

	//	nLife = rand() % 100 + 150;  

	//	fSize = (float)(rand() % 30 + 20);

	//	pos.y = fSize / 2;

	//	// �r���{�[�h�̐ݒ�
	//	SetBeffect(pos, move, XMFLOAT4(0.8f, 0.7f, 0.2f, 0.85f), fSize, fSize, nLife,0);
	//}




}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBeffect(void)
{
	//XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	//CAMERA* cam = GetCamera();

	SetCullingMode(CULL_MODE_NONE);

	// ���C�e�B���O�𖳌���
	SetLightEnable(FALSE);

	// ���Z�����ɐݒ�
	SetBlendState(BLEND_MODE_SUBTRACT);

	// Z��r����
	SetDepthEnable(FALSE);

	// �t�H�O����
	SetFogEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	CAMERA* cam = GetCamera();


	XMMATRIX mtxWorld, mtxView;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �r���[�}�g���b�N�X���擾
	mtxView = XMLoadFloat4x4(&cam->mtxView);


	// �֐��g������
	//mtxWorld = XMMatrixInverse(nullptr, mtxView);
	//mtxWorld.r[3].m128_f32[0] = 0.0f;
	//mtxWorld.r[3].m128_f32[1] = 0.0f;
	//mtxWorld.r[3].m128_f32[2] = 0.0f;


	//// �����s��i�����s��j��]�u�s�񂳂��ċt�s�������Ă��(����)
	//mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
	//mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
	//mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

	//mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
	//mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
	//mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

	//mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
	//mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
	//mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];




	//// �X�P�[���𔽉f
	//mtxScl = XMMatrixScaling(g_aBeffect[0].scale.x, g_aBeffect[0].scale.y, g_aBeffect[0].scale.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);


	//// ��]�𔽉f
	//mtxRot = XMMatrixRotationRollPitchYaw(0.0f, XM_PI, 0.0f);



	//// �ړ��𔽉f
	//mtxTranslate = XMMatrixTranslation(g_aBeffect[0].pos.x, g_aBeffect[0].pos.y, g_aBeffect[0].pos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);



	// �|���S���`��
	int cnt = sizeof(g_VertexArray) / sizeof(VERTEX_3D);
	GetDeviceContext()->Draw(cnt, 0);		// cnt���_����0�Ԗڂ̒��_�ԍ�����`��

#ifdef _DEBUG	// �f�o�b�O����\������


#endif


	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �ʏ�u�����h�ɖ߂�
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(TRUE);

	// �t�H�O�L��
	SetFogEnable(GetFogEnable());

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBeffect(void)
{



	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4 * MAX_POLYGON;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	// �O���[�o���̒��_�z�񂩂璸�_�o�b�t�@������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = g_VertexArray;

	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);


	return S_OK;
}

//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorBeffect(int nIdxBeffect, XMFLOAT4 col)
{
	g_aBeffect[nIdxBeffect].material.Diffuse = col;
}

//=============================================================================
// �p�[�e�B�N���̔�������
//=============================================================================
int SetBeffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type)
{
	int nIdxBeffect = -1;

	for (int nCntBeffect = 0; nCntBeffect < MAX_BEFFECT; nCntBeffect++)
	{
		if (!g_aBeffect[nCntBeffect].use)
		{



			g_aBeffect[nCntBeffect].pos = pos;
			g_aBeffect[nCntBeffect].rot = { 0.0f, 0.0f, 0.0f };
			g_aBeffect[nCntBeffect].scale = { fSizeX, fSizeY, fSizeY };
			g_aBeffect[nCntBeffect].move = move;
			g_aBeffect[nCntBeffect].material.Diffuse = col;
			g_aBeffect[nCntBeffect].fSizeX = fSizeX;
			g_aBeffect[nCntBeffect].fSizeY = fSizeY;
			g_aBeffect[nCntBeffect].nLife = nLife;
			g_aBeffect[nCntBeffect].use = TRUE;

			g_partcnt += 1;
			for (int i = 0; i < 4; i++)
			{
				g_vpos[i] = g_aBeffect[nCntBeffect].pos;
			}
			g_vpos[0].x -= g_aBeffect[nCntBeffect].fSizeX / 2;
			g_vpos[0].y += g_aBeffect[nCntBeffect].fSizeY / 2;

			g_vpos[1].x += g_aBeffect[nCntBeffect].fSizeX / 2;
			g_vpos[1].y += g_aBeffect[nCntBeffect].fSizeY / 2;

			g_vpos[2].x -= g_aBeffect[nCntBeffect].fSizeX / 2;
			g_vpos[2].y -= g_aBeffect[nCntBeffect].fSizeY / 2;

			g_vpos[3].x += g_aBeffect[nCntBeffect].fSizeX / 2;
			g_vpos[3].y -= g_aBeffect[nCntBeffect].fSizeY / 2;





			g_VertexArray[nCntBeffect][0].Diffuse = g_aBeffect[nCntBeffect].material.Diffuse;
			g_VertexArray[nCntBeffect][1].Diffuse = g_aBeffect[nCntBeffect].material.Diffuse;
			g_VertexArray[nCntBeffect][2].Diffuse = g_aBeffect[nCntBeffect].material.Diffuse;
			g_VertexArray[nCntBeffect][3].Diffuse = g_aBeffect[nCntBeffect].material.Diffuse;
			g_VertexArray[nCntBeffect][4].Diffuse = g_aBeffect[nCntBeffect].material.Diffuse;
			g_VertexArray[nCntBeffect][5].Diffuse = g_aBeffect[nCntBeffect].material.Diffuse;



			//if (g_VertexBuffer)			g_VertexBuffer->Release();
			//MakeVertexBeffect();

			nIdxBeffect = nCntBeffect;


			break;
		}
	}

	return nIdxBeffect;
}



//void MakeVertexBeffect(void)
//{
//	// ���_�o�b�t�@����
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//	bd.Usage = D3D11_USAGE_DYNAMIC;;
//	bd.ByteWidth = sizeof(VERTEX_3D) * 4 * MAX_POLYGON;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	// �O���[�o���̒��_�z�񂩂璸�_�o�b�t�@������
//	D3D11_SUBRESOURCE_DATA sd;
//	ZeroMemory(&sd, sizeof(sd));
//	sd.pSysMem = g_VertexArray;
//
//	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);
//
//	return;
//}


void SetBombEffect(XMFLOAT3 pos)
{


	for (int i = 0; i < BOMB_POP; i++)
	{




		XMFLOAT2 moveangle;

		moveangle.x = (rand() % 628) * 0.01f;
		moveangle.y = (rand() % 628) * 0.01f;

		XMFLOAT3 move;

		move.x = sinf(moveangle.y) * cosf(moveangle.x) * BOMB_MOVE;
		move.z = cosf(moveangle.y) * cosf(moveangle.x) * BOMB_MOVE;
		move.y = sinf(moveangle.x) * BOMB_MOVE;



		float fSize = BOMB_SIZE;
		SetBeffect(pos, move, XMFLOAT4(0.1f, 0.1f, 0.1f, 0.5f), fSize, fSize, BOMB_LIFE + (rand() % BOMB_LIFE), 2);




	}


}
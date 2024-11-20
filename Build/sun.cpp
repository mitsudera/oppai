//=============================================================================
//
// �؏��� [sun.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "sun.h"
#include"light.h"
#include"debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(5)				// �e�N�X�`���̐�

#define	SUN_WIDTH			(500.0f)			// ���_�T�C�Y
#define	SUN_HEIGHT			(500.0f)			// ���_�T�C�Y

#define	MAX_SUN			(1)			// �؍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	scl;			// �X�P�[��
	MATERIAL	material;		// �}�e���A��
	float		fWidth;			// ��
	float		fHeight;		// ����
	BOOL		use;			// �g�p���Ă��邩�ǂ���

} SUNOBJ;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexSun(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static SUNOBJ					g_Sun;	// �؃��[�N
static int					g_TexNo;			// �e�N�X�`���ԍ�
//static int					g_nAlpha;			// �A���t�@�e�X�g��臒l

static char *g_TextureName[] =
{
	"data/TEXTURE/sun.png",
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSun(void)
{
	MakeVertexSun();

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

	g_TexNo = 0;

		ZeroMemory(&g_Sun.material, sizeof(g_Sun.material));
		g_Sun.material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_Sun.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Sun.scl = XMFLOAT3(100000.0f, 100000.0f, 100000.0f);
		g_Sun.fWidth = SUN_WIDTH;
		g_Sun.fHeight = SUN_HEIGHT;
		g_Sun.use = TRUE;


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSun(void)
{
	for(int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if(g_Texture[nCntTex] != NULL)
		{// �e�N�X�`���̉��
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if(g_VertexBuffer != NULL)
	{// ���_�o�b�t�@�̉��
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSun(void)
{
	SUN sun = GetSun();
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, sun.Direction);

	g_Sun.pos = { sun.pos.x*10000.0f ,sun.pos.y * 10000.0f,sun.pos.z * 10000.0f };
	//g_Sun.pos = { pos.x*-1000000.0f ,pos.y * -1000000.0f,pos.z * -100000.0f };


#ifdef _DEBUG

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSun(void)
{

	SetCullingMode(CULL_MODE_BACK);

	SetAlphaTestEnable(FALSE);

	SetBlendState(BLEND_MODE_ADD);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView, mtxRot;
	CAMERA* cam = GetCamera();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �r���[�}�g���b�N�X���擾
	mtxView = XMLoadFloat4x4(&cam->mtxView);



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




	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Sun.scl.x, g_Sun.scl.y, g_Sun.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Sun.pos.x, g_Sun.pos.y, g_Sun.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);


	// �}�e���A���ݒ�
	SetMaterial(g_Sun.material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	// �|���S���̕`��
	GetDeviceContext()->Draw(4, 0);


	// ���C�e�B���O��L����
	SetLightEnable(TRUE);
	SetAlphaTestEnable(FALSE);
	SetBlendState(BLEND_MODE_ALPHABLEND);


	SetCullingMode(CULL_MODE_BACK);


#ifdef _DEBUG



#endif


		// ���e�X�g�𖳌���
		SetAlphaTestEnable(FALSE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexSun(void)
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

	float fWidth = 60.0f;
	float fHeight = 60.0f;

	// ���_���W�̐ݒ�
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight/2, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight/2, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, -fHeight / 2, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, -fHeight / 2, 0.0f);

	// �@���̐ݒ�
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// �g�U���̐ݒ�
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}


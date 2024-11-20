//=============================================================================
//
// �^�C�g����ʏ��� [battleui.cpp]
// Author : 
//
//=============================================================================
#include "battleui.h"
#include "input.h"
#include "fade.h"
#include"player.h"
#include"enemy.h"
#include"camera.h"
#include"misairu.h"
#include"sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100.0f)	
#define TEXTURE_HEIGHT				(100.0f)	// 
#define TEXTURE_MAX					(9)				// �e�N�X�`���̐�
#define	SPEED_DIGIT					(4)
#define TIME_DIGIT					(5)
#define HP_DIGIT					(3)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/siromaru.png",
	"data/TEXTURE/lockon.png",
	"data/TEXTURE/hit.png",
	"data/TEXTURE/destroy.png",
	"data/TEXTURE/alert.png",
	"data/TEXTURE/namberD.png",
	"data/TEXTURE/stall.png",
	"data/TEXTURE/missile1.png",
	"data/TEXTURE/missile2.png",

};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�


static BOOL						g_Load = FALSE;

static float	effect_dx;
static float	effect_dy;

static HIT		hit;
static DESTROY	destroy;
static ALERT alert;
static SPEED speed;
int stalltex;
int stallcnt;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBattleui(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_TexNo = 0;

	effect_dx = 100.0f;
	effect_dy = 100.0f;


	hit.use = FALSE;
	hit.texno = 2;
	destroy.use = FALSE;
	destroy.texno = 3;
	alert.use = FALSE;
	alert.texno = 4;
	speed.speed = 0;
	speed.texno = 5;
	stalltex = 6;




	
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBattleui(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBattleui(void)
{

	//AddTime(GetStage());
	MISAIRU* misairu = GetMisairu();
	PLAYER* player = GetPlayer();
	alert.use = FALSE;
	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (misairu[i].use==TRUE&&misairu[i].pore==0)
		{
			alert.use = TRUE;
		}
	}

	if (alert.use == TRUE)
	{
		if (alert.cnt % 10 == 0)
		{
			PlaySound(SOUND_LABEL_SE_alert001);
		}

		alert.cnt += 1;
	}



	if (hit.use == TRUE)
	{
		hit.life -= 1;
		if (hit.life<=0)
		{
			hit.use = FALSE;
		}

	}

	if (destroy.use == TRUE)
	{
		destroy.life -= 1;
		if (destroy.life <= 0)
		{
			destroy.use = FALSE;
		}

	}




	speed.speed = (int)(player->spd * 10.0f);

#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattleui(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	
	//{
	//	// �e�N�X�`���ݒ�
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	//	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	//	// �|���S���`��
	//	GetDeviceContext()->Draw(4, 0);
	//}




	
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	CAMERA* camera = GetCamera();

	XMMATRIX world, view, prj,wvp;
	view = XMLoadFloat4x4(&camera->mtxView);
	prj = XMLoadFloat4x4(&camera->mtxProjection);
	

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use==TRUE)
		{
			enemy[i].pos;
			world = XMMatrixIdentity();
			world = XMMatrixMultiply(world, XMMatrixTranslation(enemy[i].pos.x, enemy[i].pos.y, enemy[i].pos.z));
			wvp = XMMatrixMultiply(world, view);
			wvp = XMMatrixMultiply(wvp, prj);
			XMFLOAT4X4 wvp44;
			XMStoreFloat4x4(&wvp44, wvp);


		}
	}


	if (player->misairustanby[0]==TRUE)
	{



		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 50.0f, 100.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f,XMFLOAT4(0.0f,1.0f,0.0f,1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}


	}

	if (player->misairustanby[1]==TRUE)
	{



		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 70.0f, 100.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}


	}



	if (hit.use == TRUE)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[hit.texno]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 90.0f, 40.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
	if (destroy.use == TRUE)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[destroy.texno]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 110.0f, 70.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
	if (alert.use == TRUE)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[alert.texno]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y +130.0f, 70.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}

	if (speed.speed <100)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[stalltex]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X+80.0f, SCREEN_CENTER_Y +20.0f, 50.0f, 20.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}





	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[speed.texno]);
	for (int i = 0; i < SPEED_DIGIT; i++)
	{
		// ����\�����錅�̐���
		float x = (float)(speed.speed % 10);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = SCREEN_CENTER_X+100.0f - 10.0f * i - i * 5.0f;	// �X�R�A�̕\���ʒuX
		float py = SCREEN_CENTER_Y;			// �X�R�A�̕\���ʒuY
		float pw = 10.0f;				// �X�R�A�̕\����
		float ph = 15.0f;				// �X�R�A�̕\������

		float tw = 1.0f / 10;		// �e�N�X�`���̕�
		float th = 1.0f / 1;		// �e�N�X�`���̍���
		float tx = x * tw;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		// ���̌���
		speed.speed /= 10;
	}

	// �e�N�X�`���ݒ�
	int n = player->hp;
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[speed.texno]);
	for (int i = 0; i < HP_DIGIT; i++)
	{
		// ����\�����錅�̐���
		float x = (float)(n % 10);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = SCREEN_CENTER_X-100.0f - 10.0f * i - i * 5.0f;	// �X�R�A�̕\���ʒuX
		float py = SCREEN_CENTER_Y;			// �X�R�A�̕\���ʒuY
		float pw = 10.0f;				// �X�R�A�̕\����
		float ph = 15.0f;				// �X�R�A�̕\������

		float tw = 1.0f / 10;		// �e�N�X�`���̕�
		float th = 1.0f / 1;		// �e�N�X�`���̍���
		float tx = x * tw;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		// ���̌���
		n /= 10;
	}

	LONG num = GetStageTime(GetStage());
	for (int i = 0; i < TIME_DIGIT; i++)
	{
		// ����\�����錅�̐���
		float x = (float)(num % 10);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = SCREEN_WIDTH-50.0f - 20.0f * i - i * 5.0f;	// �X�R�A�̕\���ʒuX
		float py = 30.0f;			// �X�R�A�̕\���ʒuY
		float pw = 20.0f;				// �X�R�A�̕\����
		float ph = 30.0f;				// �X�R�A�̕\������

		float tw = 1.0f / 10;		// �e�N�X�`���̕�
		float th = 1.0f / 1;		// �e�N�X�`���̍���
		float tx = x * tw;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		// ���̌���
		num /= 10;
	}



}


void SetHit(void)
{
	hit.use = TRUE;
	hit.life = 30;
}

void SetDestroy(void)
{
	destroy.use = TRUE;
	destroy.life = 30;
}




//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : 
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "score.h"
#include"fade.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(6)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 
#define TIME_DIGIT					(5)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/gameover.jpg",
	"data/TEXTURE/result_logo.png",
	"data/TEXTURE/namberD.png",
	"data/TEXTURE/gameclear.png",
	"data/TEXTURE/stageclear.jpg",
	"data/TEXTURE/totalcleartime.png",



};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
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
	g_Pos   = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;

	// BGM�Đ�


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
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
void UpdateResult(void)
{

	if (GetKeyboardTrigger(DIK_RETURN)||IsButtonTriggered(0, BUTTON_M)|| IsButtonTriggered(0, BUTTON_C))
	{// Enter��������A�X�e�[�W��؂�ւ���
		//SetMode(MODE_TITLE);

		if (CheckClear() == TRUE|| GetInfMode() == TRUE || CGmaeOver() == TRUE)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
		else
		{
			SetFade(FADE_OUT, MODE_GAME);
		}



	}


#ifdef _DEBUG	// �f�o�b�O����\������
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
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

	if (GetInfMode()==TRUE||CGmaeOver()==TRUE)
	{


		// ���U���g�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// ���U���g�̃��S��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}


		// �X�R�A�\��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// ��������������
			int number = GetScore();
			for (int i = 0; i < SCORE_DIGIT; i++)
			{
				// ����\�����錅�̐���
				float x = (float)(number % 10);

				// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
				float pw = 60.0f;			// �X�R�A�̕\����
				float ph = 120.0f;			// �X�R�A�̕\������
				float px = SCREEN_CENTER_X - i * pw - i * 5.0f;	// �X�R�A�̕\���ʒuX
				float py = SCREEN_CENTER_Y+SCREEN_CENTER_Y/2;			// �X�R�A�̕\���ʒuY

				float tw = 1.0f / 10;		// �e�N�X�`���̕�
				float th = 1.0f / 1;		// �e�N�X�`���̍���
				float tx = x * tw;			// �e�N�X�`���̍���X���W
				float ty = 0.0f;			// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);

				// ���̌���
				number /= 10;
			}

		}

	}
	else if (CheckClear() == TRUE)
	{




		// ���U���g�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// ���U���g�̃��S��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y+100.0f, 400.0f, 80.0f, 0.0f, 0.0f, 1.0f, 1.0f,XMFLOAT4(1.0f,0.0f,0.0f,1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}


		// �X�R�A�\��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// ��������������
			LONG number = GetTotalTime();
			for (int i = 0; i < TIME_DIGIT; i++)
			{
				// ����\�����錅�̐���
				float x = (float)(number % 10);

				// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
				float pw = 50.0f;			// �X�R�A�̕\����
				float ph = 100.0f;			// �X�R�A�̕\������
				float px = SCREEN_CENTER_X+(TIME_DIGIT/2)*pw - i * pw - i * 5.0f;	// �X�R�A�̕\���ʒuX
				float py = SCREEN_CENTER_Y+200.0f;			// �X�R�A�̕\���ʒuY

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
				number /= 10;
			}

		}
	}
	else 
	{
		// ���U���g�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}



		// �X�R�A�\��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// ��������������
			LONG number = GetStageTime(GetStage()-1);
			for (int i = 0; i < TIME_DIGIT; i++)
			{
				// ����\�����錅�̐���
				float x = (float)(number % 10);

				// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
				float pw = 16 * 4;			// �X�R�A�̕\����
				float ph = 32 * 4;			// �X�R�A�̕\������
				float px = SCREEN_WIDTH-50.0f - i * pw - i * 5.0f;	// �X�R�A�̕\���ʒuX
				float py = 450.0f;			// �X�R�A�̕\���ʒuY

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
				number /= 10;
			}
		}


	}

}





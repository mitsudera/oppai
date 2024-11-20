//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "field.h"
#include "renderer.h"
#include"debugproc.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX		(1)				// �e�N�X�`���̐�
#define MAX_WIDTH		(256)
#define MAX_HIGHT		(256)
#define FIELD_HIGHT		(1000000)
#define SEED			(10)
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11Buffer* g_IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;				// �e�N�X�`���ԍ�

static XMFLOAT3		g_posField;								// �|���S���\���ʒu�̒��S���W
static XMFLOAT3		g_rotField;								// �|���S���̉�]�p

//static int			MAX_WIDTH, MAX_HIGHT;	// �u���b�N��
//static int			g_nNumBlockXField, g_nNumBlockZField;	// �u���b�N��

static int			g_nNumVertexField;						// �����_��	
static int			g_nNumVertexIndexField;					// ���C���f�b�N�X��
static int			g_nNumPolygonField;						// ���|���S����
static float		g_fBlockSizeXField, g_fBlockSizeZField;	// �u���b�N�T�C�Y

static char* g_TextureName[] = {
	"data/TEXTURE/rough_sand_texture.jpg",
	"data/TEXTURE/rough_sand_texture.jpg",
};



static VERTEX_3D  g_VertexArray[MAX_WIDTH][MAX_HIGHT];

// �g�̍��� = sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
static XMFLOAT3		g_Center;					// �g�̔����ꏊ
static float		g_Time = 0.0f;				// �g�̌o�ߎ���
static float		g_wave_frequency = 2.0f;	// �g�̎��g��
static float		g_wave_correction = 0.02f;	// �g�̋����␳
static float		g_wave_amplitude = 20.0f;	// �g�̐U��


//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(XMFLOAT3 pos, XMFLOAT3 rot,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	// �|���S���\���ʒu�̒��S���W��ݒ�
	g_posField = pos;
	g_posField.x -= 256*0.5 * fBlockSizeX;
	g_posField.z -= 256*0.5 * fBlockSizeZ;

	g_rotField = rot;
	g_fBlockSizeXField = fBlockSizeX;
	g_fBlockSizeZField = fBlockSizeZ;

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

	//// �u���b�N���̐ݒ�
	//MAX_WIDTH = MAX_WIDTH;
	//MAX_HIGHT = MAX_HIGHT;

	// ���_���̐ݒ�
	g_nNumVertexField = (MAX_WIDTH + 1) * (MAX_HIGHT + 1);

	// �C���f�b�N�X���̐ݒ�
	g_nNumVertexIndexField = (MAX_WIDTH + 1) * 2 * MAX_HIGHT + (MAX_HIGHT - 1) * 2;

	// �|���S�����̐ݒ�
	g_nNumPolygonField = MAX_WIDTH * MAX_HIGHT * 2 + (MAX_HIGHT - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	g_fBlockSizeXField = fBlockSizeX;
	g_fBlockSizeZField = fBlockSizeZ;


	// ���_�����������ɍ���Ă����i�g�ׁ̈j
	// �g�̏���
	// �g�̍��� = sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
	//g_Vertex = new VERTEX_3D[MAX_WIDTH][MAX_HIGHT];
	g_Center = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �g�̔����ꏊ
	g_Time = 0.0f;								// �g�̌o�ߎ���(�{�Ɓ[�Ƃœ����O���ɂȂ�)
	g_wave_frequency = 1.0f;					// �g�̎��g���i�㉺�^���̑����j
	g_wave_correction = 0.02f;					// �g�̋����␳�i�ς��Ȃ��Ă��ǂ��Ǝv���j
	g_wave_amplitude = 30.0f;					// �g�̐U��(�g�̍���)

	//for (int z = 0; z < (MAX_HIGHT + 1); z++)
	//{
	//	for (int x = 0; x < (MAX_WIDTH + 1); x++)
	//	{
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Position.x = -(MAX_WIDTH / 2.0f) * g_fBlockSizeXField + x * g_fBlockSizeXField;
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Position.y = 0.0f;
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Position.z = (MAX_HIGHT / 2.0f) * g_fBlockSizeZField - z * g_fBlockSizeZField;

	//		float dx = g_Vertex[z * (MAX_WIDTH + 1) + x].Position.x - g_Center.x;
	//		float dz = g_Vertex[z * (MAX_WIDTH + 1) + x].Position.z - g_Center.z;

	//		// �g��̒��S�_����̋����𓾂�
	//		float len = (float)sqrtf(dx * dx + dz * dz);

	//		// �g�̍������Asin�֐��œ���
	//		// �g�̍����@= sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
	//		//g_Vertex[z * (MAX_WIDTH + 1) + x].Position.y = sinf(-g_Time * g_wave_frequency + len * g_wave_correction) * g_wave_amplitude;


	//		// �@���̐ݒ�
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Normal = XMFLOAT3(0.0f, 1.0, 0.0f);

	//		// ���ˌ��̐ݒ�
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//		// �e�N�X�`�����W�̐ݒ�
	//		float texSizeX = 1.0f;
	//		float texSizeZ = 1.0f;
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].TexCoord.x = texSizeX * x;
	//		g_Vertex[z * (MAX_WIDTH + 1) + x].TexCoord.y = texSizeZ * z;
	//	}

	//}


	for (int x = 0; x < MAX_WIDTH; x++)
	{
		for (int z = 0; z < MAX_HIGHT; z++)
		{
			g_VertexArray[x][z].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
			g_VertexArray[x][z].Normal = { 0.0f,1.0f,0.0f };
			g_VertexArray[x][z].TexCoord = { 1.0f * x,1.0f * z };
			g_VertexArray[x][z].Position = { g_fBlockSizeXField *x,0.0f,g_fBlockSizeZField *z };
		}
	}
	int x1 = 0;
	int x2 = MAX_WIDTH-1;
	int z1 = 0;
	int z2 = MAX_HIGHT;



	//���_�ψʖ@�ɂ��n�`����
	 
	InitRand(SEED, FALSE);
	g_VertexArray[0][0].Position.y = GetRandomFieldHightPM(0);
	g_VertexArray[MAX_WIDTH - 1][0].Position.y = GetRandomFieldHightPM(0);
	g_VertexArray[0][MAX_HIGHT - 1].Position.y = GetRandomFieldHightPM(0);
	g_VertexArray[MAX_WIDTH - 1][MAX_HIGHT-1].Position.y = GetRandomFieldHightPM(0);
	SetFieldHight(x1, x2, z1, z2, 0, g_VertexArray[0][0].Position.y, g_VertexArray[MAX_WIDTH - 1][0].Position.y, g_VertexArray[0][MAX_HIGHT - 1].Position.y, g_VertexArray[MAX_WIDTH - 1][MAX_HIGHT - 1].Position.y);


	InitRand(0, TRUE);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * MAX_WIDTH * MAX_HIGHT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * g_nNumVertexIndexField;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);


	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, g_VertexArray, sizeof(VERTEX_3D) * MAX_WIDTH*MAX_HIGHT);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;


		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < MAX_HIGHT-1; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < MAX_WIDTH; nCntVtxX++)
			{
				pIdx[nCntIdx] = nCntVtxX  * (MAX_HIGHT) + nCntVtxZ;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxX * (MAX_HIGHT) + nCntVtxZ+1;
				nCntIdx++;
			}

			if (nCntVtxZ < (MAX_HIGHT-2 ))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = pIdx[nCntIdx-1];
				nCntIdx++;
			}



		}

		GetDeviceContext()->Unmap(g_IndexBuffer, 0);


		g_nNumVertexIndexField = nCntIdx;


	}




	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	// �C���f�b�N�X�o�b�t�@�̉��
	if (g_IndexBuffer) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{
	// �g�̏���
	float dt = 0.03f;

	//for (int z = 0; z < MAX_HIGHT; z++)
	//{
	//	for (int x = 0; x < MAX_WIDTH; x++)
	//	{
	//		float dx = g_Vertex[z * (MAX_WIDTH + 1) + x].Position.x - g_Center.x;
	//		float dz = g_Vertex[z * (MAX_WIDTH + 1) + x].Position.z - g_Center.z;

	//		// �g��̒��S�_����̋����𓾂�
	//		float len = (float)sqrtf(dx * dx + dz * dz);

	//		// �g�̍������Asin�֐��œ���
	//		//  �@�g�̍����@= sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
	//	//	g_Vertex[z * (MAX_WIDTH + 1) + x].Position.y = sinf(-g_Time * g_wave_frequency + len * g_wave_correction) * g_wave_amplitude;
	//	}

	//}
	//g_Time += dt;


	//// ���_�o�b�t�@�ɒl���Z�b�g����
	//D3D11_MAPPED_SUBRESOURCE msr;
	//GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	//// �S���_���𖈉�㏑�����Ă���̂�DX11�ł͂��̕�����������ł�
	//memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D) * g_nNumVertexField);

	//GetDeviceContext()->Unmap(g_VertexBuffer, 0);



}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);


	XMMATRIX mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_rotField.x, g_rotField.y, g_rotField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_posField.x,g_posField.y, g_posField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	//SetCullingMode(CULL_MODE_NONE);
	// �|���S���̕`��
	GetDeviceContext()->DrawIndexed(g_nNumVertexIndexField,0, 0);
	//// �|���S���̕`��
	//GetDeviceContext()->DrawIndexed(14,0, 0);
}





void SetFieldHight(int xl, int xr, int zd, int zu, int n, float tl, float tr, float bl, float br)
{
	if (n == 8)
	{
		return;
	}
	//����
	float top = (tl + tr) / 2;
	float bottom = (bl + br) / 2;
	float left = (tl + bl) / 2;
	float right = (tr + br) / 2;
	float midpoint = (tl + tr + bl + br) / 4 + GetRandomFieldHightPM(n);

	//float top = (tl + tr) / 2 + GetRandomHightPM(n+1);
	//float bottom = (bl + br) / 2 + GetRandomHightPM(n+1);
	//float left = (tl+ bl) / 2 + GetRandomHightPM(n+1);
	//float right = (tr + br) / 2 + GetRandomHightPM(n+1);
	//float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	////
	g_VertexArray[(xl + xr) / 2][zu].Position.y = top;
	g_VertexArray[(xl + xr) / 2][zd].Position.y = bottom;
	g_VertexArray[xl][(zu + zd) / 2].Position.y = left;
	g_VertexArray[xr][(zu + zd) / 2].Position.y = right;
	g_VertexArray[(xl + xr) / 2][(zu + zd) / 2].Position.y = midpoint;


	SetFieldHight(xl, (xl + xr) / 2, (zu + zd) / 2, zu, n +1, tl, top, left, midpoint);
	SetFieldHight((xl + xr) / 2, xr, (zu + zd) / 2, zu, n +1, top, tr, midpoint, right);
	SetFieldHight(xl, (xl + xr) / 2, zd, (zu + zd) / 2, n +1, left, midpoint, bl, bottom);
	SetFieldHight((xl + xr) / 2, xr, zd, (zu + zd) / 2, n +1, midpoint, right, bottom, br);
}
float GetRandomFieldHightPM(int n)
{
	InitRand(SEED,FALSE);
	float h = 0.0f;
	//h = (float)((rand() % (int)(FIELD_HIGHT / powf(2, n))) * 0.1f);
	int rl = ((LONGLONG)(FIELD_HIGHT / powf(2, n)));

	//h = ((rand() % rl)) * 1.0f;
	h = ((rand() % rl)-(rl /2)) * 1.0f;

	return h;
}

//XMFLOAT3 GetVertexField(int x,int y)
//{
//	return g_VertexArray[x][y].Position;
//}

//BOOL RayHitField(XMFLOAT3 pos, XMFLOAT3* HitPosition, XMFLOAT3* Normal)
//{
//	XMFLOAT3 start = pos;
//	XMFLOAT3 end = pos;
//
//	if (start.x == 0.0f)
//	{
//		start.x += 0.5f;
//		end.x += 0.5f;
//	}
//	if (start.z == 0.0f)
//	{
//		start.z -= 0.5f;
//		end.z -= 0.5f;
//	}
//
//	// �����ォ��A�Y�h�[���Ɖ��փ��C���΂�
//	start.y += 1000.0f;
//	end.y -= 1000.0f;
//
//	// ����������������ׂɑS�����ł͂Ȃ��āA���W����|���S��������o���Ă���
//	float fz = (MAX_HIGHT / 2.0f) * g_fBlockSizeXField;
//	float fx = (MAX_WIDTH/ 2.0f) * g_fBlockSizeZField;
//	int sz = (int)((-start.z + fz) / g_fBlockSizeZField);
//	int sx = (int)((start.x + fx) / g_fBlockSizeXField);
//	int ez = sz + 2;
//	int ex = sx + 2;
//
//
//	if ((sz < 0) || (sz > MAX_HIGHT - 1) ||
//		(sx < 0) || (sx > MAX_WIDTH - 1))
//	{
//		*Normal = { 0.0f, 1.0f, 0.0f };
//		return FALSE;
//	}
//
//
//	// �K�v�����������J��Ԃ�
//	for (int z = sz; z < ez; z++)
//	{
//		for (int x = sx; x < ex; x++)
//		{
//
//			if (x < MAX_WIDTH && z < MAX_HIGHT)
//			{
//
//
//
//				XMFLOAT3 p0 = g_VertexArray[x][z].Position;
//				XMFLOAT3 p1 = g_VertexArray[x + 1][z].Position;
//				XMFLOAT3 p2 = g_VertexArray[x][z + 1].Position;
//				XMFLOAT3 p3 = g_VertexArray[x + 1][z + 1].Position;
//
//				p0.y += g_posField.y;
//				p1.y += g_posField.y;
//				p2.y += g_posField.y;
//				p3.y += g_posField.y;
//
//
//				
//
//
//
//
//				// �O�p�|���S��������Q�����̓����蔻��
//				BOOL ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
//				if (ans)
//				{
//					return TRUE;
//				}
//
//				ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
//				if (ans)
//				{
//					return TRUE;
//				}
//
//			}
//			else
//			{
//
//				return FALSE;
//
//			}
//
//
//		}
//	}
//
//
//	return FALSE;
//}

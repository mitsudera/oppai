//=============================================================================
//
// ���f���̏��� [meshdata.cpp]
// Author : �O�c���M�I
//
//=============================================================================
#include "meshdata.h"
#include "GameEngine.h"
#include "AssetsManager.h"
#include "DX11Texture.h"
#include "Material.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_PATH	"data/TEXTURE/"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************




//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************



//*****************************************************************************
// �N���X�֐�
//*****************************************************************************


DX11_SUBSET::DX11_SUBSET()
{
	this->StartIndex = 0;
	this->IndexNum = 0;
	this->pmeshdata = nullptr;
	this->textureDiffuseIndex = -1;
	this->textureNormalIndex = -1;

}
DX11_SUBSET::~DX11_SUBSET()
{
	
}

void DX11_SUBSET::SetStartIndex(unsigned short n)
{
	this->StartIndex = n;
}
unsigned short DX11_SUBSET::GetStartIndex(void)
{
	return this->StartIndex;
}

void DX11_SUBSET::SetIndexNum(unsigned short n)
{
	this->IndexNum = n;
}
unsigned short DX11_SUBSET::GetIndexNum(void)
{
	return this->IndexNum;
}


Material* DX11_SUBSET::GetMaterial(void)
{
	return material;
}


MeshData* DX11_SUBSET::GetpMeshData(void)
{
	return this->pmeshdata;
}

void DX11_SUBSET::SetpMeshData(MeshData* meshdata)
{
	this->pmeshdata = meshdata;
}

void DX11_SUBSET::LoadDiffuseTex(string filepath)
{
	this->textureDiffuseIndex = pmeshdata->GetpAssetsManager()->LoadTexture(filepath);

}

void DX11_SUBSET::LoadNormalTex(string filepath)
{
	this->textureNormalIndex = pmeshdata->GetpAssetsManager()->LoadTexture(filepath);

}

int DX11_SUBSET::GetDiffuseIndex(void)
{
	return textureDiffuseIndex;
}

int DX11_SUBSET::GetNormalIndex(void)
{
	return textureNormalIndex;
}

void DX11_SUBSET::SetShaderResouce(void)
{

	if (material->noDiffuseTex==FALSE)
	{
		pmeshdata->GetpAssetsManager()->GetTexture(textureDiffuseIndex)->SetShaderResource(0);

	}

	if (Material.noNormalTex==FALSE)
	{
		pmeshdata->GetpAssetsManager()->GetTexture(textureNormalIndex)->SetShaderResource(1);

	}

}



MeshData::MeshData()
{
	this->nomesh = TRUE;
	this->VertexBuffer = nullptr;
	this->indexnum = 0;
	this->IndexBuffer = nullptr;
	this->offset = XMMatrixIdentity();
	this->subsetnum = 0;
	this->SubsetArray = nullptr;
	this->childcnt = 0;
	this->Child = nullptr;
	this->pAssetsManager = nullptr;
}

MeshData::~MeshData()
{
	if(VertexBuffer) VertexBuffer->Release();
	if(IndexBuffer) IndexBuffer->Release();


	delete[]SubsetArray;
}


void MeshData::SetNoMesh(BOOL no)
{
	this->nomesh = no;
}
BOOL MeshData::GetNoMesh(void)
{
	return this->nomesh;
}



void MeshData::CreateVertexBuffer(int VertNum)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * VertNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->VertexBuffer);



}

ID3D11Buffer* MeshData::GetVertexBuffer(void)
{
	return VertexBuffer;
}


void MeshData::SetIndexNum(unsigned int n)
{
	this->indexnum = n;
}
unsigned int MeshData::GetIndexNum(void)
{
	return this->indexnum;
}


void MeshData::CreateIndexBuffer(int IndexNum)
{
	// �C���f�b�N�X�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * IndexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->IndexBuffer);


}
ID3D11Buffer* MeshData::GetIndexBuffer(void)
{
	return IndexBuffer;
}

void MeshData::SetChildCnt(int n)
{
	childcnt = n;
}
int MeshData::GetChildCount(void)
{
	return childcnt;
}

void MeshData::CreateChildArray(int n)
{
	this->Child = new MeshData[n];
}



MeshData* MeshData::GetChild(int n)
{
	return &this->Child[n];
}

void MeshData::SetOffset(XMMATRIX offsetmtx)
{
	offset = offsetmtx;
}
XMMATRIX MeshData::GetOffset(void)
{
	return offset;
}



void MeshData::SetSubsetNum(int n)
{
	this->subsetnum = n;
}

unsigned short MeshData::GetSubsetNum(void)
{
	return this->subsetnum;
}

void MeshData::CreateSubsetArray(int n)
{
	this->SubsetArray = new DX11_SUBSET[n];

	for (int i = 0; i < n; i++)
	{
		this->SubsetArray[i].SetpMeshData(this);
	}
}


DX11_SUBSET* MeshData::GetSubset(void)
{
	return this->SubsetArray;
}

void MeshData::SetSubset(int n, DX11_SUBSET sub)
{
	this->SubsetArray[n] = sub;
}

void MeshData::BufferSetVertex(void)
{

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->VertexBuffer, &stride, &offset);
}

void MeshData::BufferSetIndex(void)
{

	// �v���~�e�B�u�g�|���W�ݒ�
	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetIndexBuffer(this->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

}


AssetsManager* MeshData::GetpAssetsManager(void)
{
	return this->pAssetsManager;
}



MeshDataList::MeshDataList()
{
	this->meshdatanum = 0;
	this->meshdata = nullptr;
	this->pManager = nullptr;
}

MeshDataList::~MeshDataList()
{
	if (meshdata) delete[]meshdata;
}


void MeshDataList::SetMeshDataNum(int n)
{
	this->meshdatanum = n;
}
int MeshDataList::GetMeshDataNum(void)
{
	return this->meshdatanum;
}


void MeshDataList::CreateMeshDataArray(int n)
{
	meshdata = new MeshData[n];
}

MeshData* MeshDataList::GetMeshData(void)
{
	return meshdata;
}

void MeshDataList::SetFilePath(std::string fn)
{
	filepath = fn;
}

std::string MeshDataList::GetFilePath(void)
{
	return filepath;

}
void MeshDataList::LoadFbxFile(string filepath, AssetsManager* p)
{
	
	this->pManager = p;
	SetFilePath(filepath);

	FbxManager* manager;
	FbxIOSettings* ioSettings;
	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);
	// Importer�𐶐�
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings()) == false)
	{
		// �C���|�[�g�G���[
	}

	//Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);

	int meshcount = scene->GetSrcObjectCount<FbxMesh>();
	this->SetMeshDataNum(meshcount);
	this->CreateMeshDataArray(meshcount);
	for (int i = 0; i < meshcount; i++)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(i);

		this->meshdata[i].LoadFbxMesh(mesh,p);

	}

	scene->Destroy();
	importer->Destroy(); // �V�[���𗬂����񂾂�Importer�͉��
	manager->Destroy();//fbx�t�@�C���̓ǂݍ��݂��I�������}�l�[�W���[�͉������



}
AssetsManager* MeshDataList::GetpAssetsManager(void)
{

	return this->pManager;
}



void MeshData::LoadFbxMesh(FbxMesh* mesh,AssetsManager* ap)
{
	using namespace fbxsdk;
	this->pAssetsManager = ap;
	FbxNode* node = mesh->GetNode();
	

	int PolygonNum = mesh->GetPolygonCount();               //���|���S����
	if (PolygonNum==0)
	{
		return;
	}
	
	int PolygonVertexNum = mesh->GetPolygonVertexCount();   //�|���S�����_�C���f�b�N�X��
	int controlNum = mesh->GetControlPointsCount();         // ���_���W��
	FbxVector4* src = mesh->GetControlPoints();             // ���_���W�z��
	VERTEX_3D* VertexArray = nullptr;
	VertexArray = new VERTEX_3D[PolygonVertexNum];
	ZeroMemory(VertexArray, sizeof(VERTEX_3D) * PolygonVertexNum);
	unsigned int indexnum = PolygonNum * 3;
	this->SetIndexNum(indexnum);
	
	//���_�z��𖄂߂�
	FbxStringList uvSetNameList;
	mesh->GetUVSetNames(uvSetNameList);
	FbxVector2* uv = new FbxVector2[PolygonVertexNum];


	bool bIsUnmapped = false;

	unsigned int* IndexArry = nullptr;
	IndexArry = new unsigned int[indexnum];

	ZeroMemory(IndexArry, sizeof(unsigned int) * indexnum);
	int ic = 0;


	int vcnt = 0;

	for (int p = 0; p < PolygonNum; p++)
	{



		int IndexNumInPolygon = mesh->GetPolygonSize(p);  // p�Ԗڂ̃|���S���̒��_��



		if (IndexNumInPolygon==3)
		{
			FbxVector4 positions[3];
			FbxVector4 normals[3];
			FbxVector2 uvs[3];

			for (int n = 0; n < IndexNumInPolygon; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// �|���S���ԍ�
					n,				// ���Ԗڂ̒��_
					nor);			// �@����
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = -(float)nor[2];

				normals[n] = nor;

				// UV�l�擾�B
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];

				uvs[n] = uv[vcnt];


				// �|���S��p���\������n�Ԗڂ̒��_�̃C���f�b�N�X�ԍ�
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = -(float)src[IndexNumber][2];

				positions[n] = src[IndexNumber];

				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };

				IndexArry[ic] = vcnt;
				ic++;
				vcnt++;

			}


			// �^���W�F���g�x�N�g���̌v�Z
			FbxVector4 edge1 = positions[1] - positions[0];
			FbxVector4 edge2 = positions[2] - positions[0];
			FbxVector2 deltaUV1 = uvs[1] - uvs[0];
			FbxVector2 deltaUV2 = uvs[2] - uvs[0];

			float f = 1.0f / (float)(deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

			FbxVector4 tangent;
			tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
			tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
			tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
			tangent[3] = 0.0f;

			// �e���_�Ƀ^���W�F���g�x�N�g����ݒ�
			for (int n = 0; n < IndexNumInPolygon; n++)
			{
				VertexArray[vcnt - IndexNumInPolygon + n].Tangent.x = (float)tangent[0];
				VertexArray[vcnt - IndexNumInPolygon + n].Tangent.y = (float)tangent[1];
				VertexArray[vcnt - IndexNumInPolygon + n].Tangent.z = (float)tangent[2];
				VertexArray[vcnt - IndexNumInPolygon + n].Tangent = XMFLOAT3Normalize(VertexArray[vcnt - IndexNumInPolygon + n].Tangent);
				


				XMVECTOR nv = XMLoadFloat3(&VertexArray[vcnt - IndexNumInPolygon + n].Normal);
				XMVECTOR tv = XMLoadFloat3(&VertexArray[vcnt - IndexNumInPolygon + n].Tangent);

				XMVECTOR binv = XMVector3Cross(nv, tv);
				binv = XMVector3Normalize(binv);

				XMFLOAT3 binor;

				XMStoreFloat3(&binor, binv);

				VertexArray[vcnt - IndexNumInPolygon + n].BiNormal=binor;
			
			}

		}
		else if (IndexNumInPolygon==4)
		{
			for (int n = 0; n < 3; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// int pPolyIndex
					n,				// int pVertexIndex
					nor);        // FbxVector4& pNormal
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = (float)nor[2];



				// UV�l�擾�B
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];


				// �|���S��p���\������n�Ԗڂ̒��_�̃C���f�b�N�X�ԍ�
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = -(float)src[IndexNumber][2];


				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };


				IndexArry[ic] = vcnt;
				ic++;

				vcnt++;

			}
			for (int n = 1; n < 4; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// int pPolyIndex
					n,				// int pVertexIndex
					nor);        // FbxVector4& pNormal
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = -(float)nor[2];


				


				// UV�l�擾�B
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];






				// �|���S��p���\������n�Ԗڂ̒��_�̃C���f�b�N�X�ԍ�
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = -(float)src[IndexNumber][2];


				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };

				IndexArry[ic] = vcnt;
				ic++;


				vcnt++;

			}

		}

	}
	////�C���f�b�N�X�z��𖄂߂�
	//unsigned int* IndexArry = nullptr;
	//IndexArry = new unsigned int[indexnum];
	//int ic = 0;

	//for (int i = 0; i < PolygonNum; i++)
	//{

	//	IndexArry[i * 3] = ic;
	//	ic++;
	//	IndexArry[i * 3 + 1] = ic;
	//	ic++;
	//	IndexArry[i * 3 + 2] = ic;
	//	ic++;
	//}


	// ���_�o�b�t�@����
	this->CreateVertexBuffer(ic);

	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, VertexArray, sizeof(VERTEX_3D) * PolygonVertexNum);

		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->GetVertexBuffer(), 0);
	}


	// �C���f�b�N�X�o�b�t�@����
	this->CreateIndexBuffer(indexnum);

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->GetIndexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned int* pIdx = (unsigned int*)msr.pData;

		//memcpy(pIdx, fbxIndexAry, sizeof(unsigned int) * indexnum);

		int n;
		for (unsigned int i = 0; i < indexnum; i++)
		{
			pIdx[i] = IndexArry[i];
			n = i;
		}

		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->GetIndexBuffer(), 0);
	}



	FbxMatrix offset = node->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);//�I�t�Z�b�g�s��̎擾
	this->SetOffset(FbxMatrixConvertToXMMATRIX(offset));


	// �}�e���A���̐�

	int mtlcnt = node->GetMaterialCount();

	this->SetSubsetNum(mtlcnt);
	this->CreateSubsetArray(mtlcnt);

	// �}�e���A�������擾
	for (int i = 0; i < mtlcnt; i++)
	{
		FbxSurfaceMaterial* fbxmaterial = node->GetMaterial(i);
		
		FbxClassId id = fbxmaterial->GetClassId();

		if (fbxmaterial != 0)
		{
			MATERIAL material;
			ZeroMemory(&material, sizeof(MATERIAL));

			// �}�e���A�����
			// Lambert��Phong��
			//lambart
			material.phong = 0;

			

			if (id == (FbxSurfaceLambert::ClassId))
			{

				// Lambert�Ƀ_�E���L���X�g
				FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)fbxmaterial;


				//�A���r�G���g
				FbxDouble3 amb = lambert->Ambient;

				material.Ambient.x = (float)amb[0];
				material.Ambient.y = (float)amb[1];
				material.Ambient.z = (float)amb[2];

				//�f�B�t���[�Y
				FbxDouble3 diffuse = lambert->Diffuse;

				material.Diffuse.x = (float)diffuse[0];
				material.Diffuse.y = (float)diffuse[1];
				material.Diffuse.z = (float)diffuse[2];

				// ���ߓx
				FbxDouble transparency = lambert->TransparencyFactor;
				material.Diffuse.w = (float)transparency;

				//�G�~�b�V����
				FbxDouble3 emi = lambert->Emissive;

				material.Emission.x = (float)emi[0];
				material.Emission.y = (float)emi[1];
				material.Emission.z = (float)emi[2];



			}
			//phong
			else if (id == (FbxSurfacePhong::ClassId))
			{

				// Phong�Ƀ_�E���L���X�g
				FbxSurfacePhong* phong = (FbxSurfacePhong*)fbxmaterial;
				material.phong = 1;
				//�A���r�G���g
				FbxDouble3 amb = phong->Ambient;

				material.Ambient.x = (float)amb[0];
				material.Ambient.y = (float)amb[1];
				material.Ambient.z = (float)amb[2];

				//�f�B�t���[�Y
				FbxDouble3 diffuse = phong->Diffuse;

				material.Diffuse.x = (float)diffuse[0];
				material.Diffuse.y = (float)diffuse[1];
				material.Diffuse.z = (float)diffuse[2];

				// ���ߓx
				FbxDouble transparency = phong->TransparencyFactor;
				material.Diffuse.w = (float)transparency;

				//�G�~�b�V����
				FbxDouble3 emi = phong->Emissive;

				material.Emission.x = (float)emi[0];
				material.Emission.y = (float)emi[1];
				material.Emission.z = (float)emi[2];

				//�X�y�L�����[
				FbxDouble3 spe = phong->Specular;

				material.Specular.x = (float)spe[0];
				material.Specular.y = (float)spe[1];
				material.Specular.z = (float)spe[2];

				FbxDouble shine = phong->Shininess;
				material.Shininess = (float)shine;


			}
			else
			{
				material.phong = 0;
				material.Diffuse.x = 1.0f;
				material.Diffuse.y = 1.0f;
				material.Diffuse.z = 1.0f;
				material.Diffuse.w = 1.0f;

			}

			material.noDiffuseTex = true;
			material.noNormalTex = true;


			// �v���p�e�B�擾�B
			const FbxProperty property = fbxmaterial->FindProperty(
				FbxSurfaceMaterial::sDiffuse    // const char* pName
			);                                  // bool pCaseSensitive = true



			// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N
			int layerNum = property.GetSrcObjectCount<FbxFileTexture>();

			if (layerNum>0)
			{
				FbxFileTexture* pFileTexture = property.GetSrcObject<FbxFileTexture>(0);

				FbxFileTexture::ETextureUse m_type = FbxFileTexture::ETextureUse(pFileTexture->GetTextureUse());


				//�f�B�t���[�Y�e�N�X�`���Ȃ�
				if (m_type == FbxFileTexture::ETextureUse::eStandard)
				{
					const char* fileName = pFileTexture->GetFileName();

					int slush = '/';
					char* path;
					path = new char[256];

					//�Ō��/���o�Ă���ꏊ
					const char* last = strrchr(fileName, slush);


					if (last == nullptr)
					{
						strcpy(path, "data/TEXTURE/");

						strcat(path, fileName);

					}
					else
					{
						strcpy(path, "data/TEXTURE");

						strcat(path, last);

					}

					this->GetSubset()[i].LoadDiffuseTex(path);

					delete[]path;

					// �}�e���A���ݒ�
					material.noDiffuseTex = false;
				}

			}






			// �v���p�e�B�擾�B
			const FbxProperty propertynormal = fbxmaterial->FindProperty(
				FbxSurfaceMaterial::sBump    // const char* pName
			);                                  // bool pCaseSensitive = true



			// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N
			layerNum = propertynormal.GetSrcObjectCount<FbxFileTexture>();

			if (layerNum>0)
			{
				FbxFileTexture* pFileTextureNormal = propertynormal.GetSrcObject<FbxFileTexture>(0);

				FbxFileTexture::ETextureUse m_type = FbxFileTexture::ETextureUse(pFileTextureNormal->GetTextureUse());


				if (m_type == FbxFileTexture::ETextureUse::eStandard)
				{

					const char* fileName1 = pFileTextureNormal->GetFileName();
					int slush = '/';
					char* path;
					path = new char[256];

					//�Ō��/���o�Ă���ꏊ
					const char* last = strrchr(fileName1, slush);

					strcpy(path, "data/TEXTURE");
					strcat(path, last);

					this->GetSubset()[i].LoadNormalTex(path);

					delete[]path;

					material.noNormalTex = FALSE;

				}


			}







			material.Diffuse.w = 1.0f;
			this->GetSubset()[i].SetMaterial(material);
			this->GetSubset()[i].SetpMeshData(this);


		}
	}


	delete[] uv;
	delete[] IndexArry;
	delete[] VertexArray;




}


void MeshData::SetpAssetsManager(AssetsManager* pa)
{
	this->pAssetsManager = pa;
}
void MeshDataList::SetpAssetsManager(AssetsManager* pa)
{
	this->pManager = pa;

}

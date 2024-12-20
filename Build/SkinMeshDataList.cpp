#include "SkinMeshDataList.h"
#include "AssetsManager.h"
#include "GameEngine.h"
#include "DX11Texture.h"

Bone::Bone()
{
	this->initMtxInverse = XMMatrixIdentity();
	this->mtx = XMMatrixIdentity();
	this->child = nullptr;
	this->childCnt = 0;
	this->isRootBone = FALSE;
	this->parent = nullptr;

}

Bone::~Bone()
{
}

void Bone::LoadBone(FbxCluster* cluster)
{
	FbxAMatrix initmtx;

	FbxNode* bonenode = cluster->GetLink();

	cluster->GetTransformLinkMatrix(initmtx);

	this->SetInitMtx(FbxMatrixConvertToXMMATRIX(initmtx));

	

	

}
void Bone::LoadBone(FbxNode* node,Bone* parent)
{
	if (parent==nullptr)
	{
		this->isRootBone = TRUE;
	}
	else
	{
		this->isRootBone = FALSE;

	}
	this->parent = parent;

	this->childCnt = node->GetChildCount();


	FbxMatrix initmtx = node->EvaluateGlobalTransform();

	this->SetInitMtx(FbxMatrixConvertToXMMATRIX(initmtx));

	this->child = new Bone[childCnt];
	

	for (int i = 0; i < childCnt; i++)
	{
		child->LoadBone(node->GetChild(i),this);
	}
	

}
void Bone::LoadBoneFrame(FbxCluster* cluster,FbxTime time)
{

	FbxNode* bonenode = cluster->GetLink();

	FbxMatrix framemtx = bonenode->EvaluateGlobalTransform(time);

	this->mtx = FbxMatrixConvertToXMMATRIX(framemtx);


	

}

void Bone::SetInitMtx(XMMATRIX Mtx)
{
	this->mtx = Mtx;
	this->initMtxInverse = XMMatrixInverse(nullptr, Mtx);

}

XMMATRIX Bone::GetMtx(void)
{
	return this->mtx;

}

XMMATRIX Bone::GetInitMtxInverse(void)
{
	return this->initMtxInverse;
}

void Bone::CreateChildArray(int n)
{
	this->childCnt = n;
	child = new Bone[n];
}

int Bone::GetIndex(void)
{
	return this->index;
}

void Bone::SetIndex(int n)
{
	this->index = n;
}



Skeleton::Skeleton()
{
	this->bone = nullptr;
	boneCnt = 0;
}

Skeleton::~Skeleton()
{
	if (bone) delete[]bone;
}

void Skeleton::CreateBoneArray(int n)
{
	this->boneCnt = n;
	this->bone = new Bone[n];
}

void Skeleton::LoadSkeleton(FbxScene* scene)
{
	

	int n = scene->GetSrcObjectCount<FbxSkin>();
	FbxSkin*skin = (FbxSkin*)scene->GetSrcObject<FbxDeformer>(0);
	int bonecount=skin->GetClusterCount();
	this->boneCnt = bonecount;
	this->CreateBoneArray(bonecount);



	//for (int i = 0; i < bonecount; i++)
	//{
	//	this->bone[i].LoadBone(scene->GetSrcObject<FbxCluster>(i));
	//}																																										
	//FbxSkeleton* fbxbone= scene->GetSrcObject<FbxSkeleton>(0);

	//
	//
	//FbxNode* node = fbxbone->GetNode();
	//this->bone = new BONE;

	//this->bone->LoadBone(node, nullptr);


}

void Skeleton::LoadSkeletonFrame(FbxSkin* skin,int boneCnt,FbxTime time)
{
	this->CreateBoneArray(boneCnt);
	for (int i = 0; i < boneCnt; i++)
	{
		FbxCluster* cluster = skin->GetCluster(i);


		this->bone[i].LoadBoneFrame(cluster, time);
	}


}

Bone* Skeleton::GetBone(int n)
{
	return &bone[n];
}

int Skeleton::GetBoneCount(void)
{
	return this->boneCnt;
}



SkinMeshData::SkinMeshData()
{
	this->VertexArray = nullptr;
	this->controlPoint = nullptr;
	this->controlPointIndexArray = nullptr;
	this->controlPointNum = 0;
	this->indexNum = 0;
	this->pSkeleton = nullptr;
	this->vertNum = 0;


}

SkinMeshData::~SkinMeshData()
{
	if (VertexArray) delete[]VertexArray;
	if (controlPoint) delete[]controlPoint;
	if (controlPointIndexArray) delete[]controlPointIndexArray;

}


void SkinMeshData::LoadSkinMeshData(FbxMesh* mesh, Skeleton* sp, AssetsManager* ap)
{
	this->pSkeleton = sp;
	this->SetpAssetsManager(ap);
	FbxNode* node = mesh->GetNode();


	int PolygonNum = mesh->GetPolygonCount();               //総ポリゴン数
	int PolygonVertexNum = mesh->GetPolygonVertexCount();   //ポリゴン頂点インデックス数
	int* IndexAry = mesh->GetPolygonVertices();             //ポリゴン頂点インデックス配列
	int controlNum = mesh->GetControlPointsCount();         // 頂点座標数
	FbxVector4* src = mesh->GetControlPoints();             // 頂点座標配列
	unsigned int indexnum = PolygonNum * 3;
	this->SetIndexNum(indexnum);
	this->indexNum = indexnum;
	this->vertNum = PolygonVertexNum;
	this->VertexArray = new VERTEX_3D[PolygonVertexNum];

	int n = mesh->GetElementTangentCount();
	FbxGeometryElementTangent* t = mesh->GetElementTangent();
	//int n=mesh->GetElementPolygonGroupCount();

	//頂点配列を埋める
	FbxStringList uvSetNameList;
	mesh->GetUVSetNames(uvSetNameList);
	FbxVector2* uv = new FbxVector2[PolygonVertexNum];
	this->controlPoint = new CONTROLPOINT[controlNum];
	this->controlPointNum = controlNum;
	FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(0);
	int boneNum = skin->GetClusterCount();

	//this->pSkeleton->CreateBoneArray(boneNum);

	this->controlPointIndexArray = new int[vertNum];
	for (int i = 0; i < controlNum; i++)
	{




		this->controlPoint[i].pos.x = (float)src[i][0];
		this->controlPoint[i].pos.y = (float)src[i][1];
		this->controlPoint[i].pos.z = (float)src[i][2];

		this->controlPoint[i].weight = new float[boneNum];

		for (int j = 0; j < boneNum; j++)
		{
			controlPoint[i].weight[j] = 0.0f;
		}
	}


	for (int i = 0; i < boneNum; i++)
	{
		FbxCluster* cluster = skin->GetCluster(i);


		FbxAMatrix initmat;
		cluster->GetTransformLinkMatrix(initmat);
		
		FbxNode*sk= cluster->GetLink();
		//initmat = sk->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);
		//XMMATRIX mat= GetParentTransFormFbx(sk, XMMatrixIdentity());
		//this->pSkeleton->GetBone(i)->SetInitMtx(mat);

		this->pSkeleton->GetBone(i)->SetInitMtx(FbxMatrixConvertToXMMATRIX(initmat));

		int pointNum = cluster->GetControlPointIndicesCount();
		int* pointAry = cluster->GetControlPointIndices();
		double* weightAry = cluster->GetControlPointWeights();

		for (int j = 0; j < pointNum; ++j)
		{


			// 頂点インデックスとウェイトを取得
			int   index = pointAry[j];
			float weight = (float)weightAry[j];
			this->controlPoint[index].weight[i] = weight;


		}

	}

	bool bIsUnmapped = false;

	int vcnt = 0;

	for (int p = 0; p < PolygonNum; p++)
	{

		FbxVector4 positions[3];
		FbxVector4 normals[3];
		FbxVector2 uvs[3];



		int IndexNumInPolygon = mesh->GetPolygonSize(p);  // p番目のポリゴンの頂点数
		for (int n = 0; n < IndexNumInPolygon; n++)
		{
			FbxVector4 nor;

			mesh->GetPolygonVertexNormal(
				p,				// int pPolyIndex
				n,				// int pVertexIndex
				nor);        // FbxVector4& pNormal
			VertexArray[vcnt].Normal.x = (float)nor[0];
			VertexArray[vcnt].Normal.y = (float)nor[1];
			VertexArray[vcnt].Normal.z = -(float)nor[2];

			normals[n] = nor;

			// UV値取得。
			FbxString name = uvSetNameList.GetStringAt(0);

			mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
			VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
			VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];

			uvs[n] = uv[vcnt];



			// ポリゴンpを構成するn番目の頂点のインデックス番号
			int IndexNumber = mesh->GetPolygonVertex(p, n);
			VertexArray[vcnt].Position.x = -(float)src[IndexNumber][0];
			VertexArray[vcnt].Position.y = -(float)src[IndexNumber][1];
			VertexArray[vcnt].Position.z = -(float)src[IndexNumber][2];

			positions[n] = src[IndexNumber];


			this->controlPointIndexArray[vcnt] = IndexNumber;

			VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };



			vcnt++;

		}

		// タンジェントベクトルの計算
		FbxVector4 edge1 = positions[1] - positions[0];
		FbxVector4 edge2 = positions[2] - positions[0];
		FbxVector2 deltaUV1 = uvs[1] - uvs[0];
		FbxVector2 deltaUV2 = uvs[2] - uvs[0];

		float f = 1.0f /(float) (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

		FbxVector4 tangent;
		tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
		tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
		tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
		tangent[3] = 0.0f;

		// 各頂点にタンジェントベクトルを設定
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

			VertexArray[vcnt - IndexNumInPolygon + n].BiNormal = binor;

		}




	}
	//インデックス配列を埋める
	unsigned int* IndexArry = nullptr;
	IndexArry = new unsigned int[indexnum];
	int ic = 0;

	for (int i = 0; i < PolygonNum; i++)
	{

		IndexArry[i * 3] = ic;
		ic++;
		IndexArry[i * 3 + 1] = ic;
		ic++;
		IndexArry[i * 3 + 2] = ic;
		ic++;
	}

	// 頂点バッファ生成
	this->CreateVertexBuffer(PolygonVertexNum);

	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, VertexArray, sizeof(VERTEX_3D) * PolygonVertexNum);

		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->GetVertexBuffer(), 0);
	}
	// インデックスバッファ生成
	this->CreateIndexBuffer(indexnum);

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->GetIndexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned int* pIdx = (unsigned int*)msr.pData;

		memcpy(pIdx, IndexAry, sizeof(unsigned int) * indexnum);

		int n;
		for (unsigned int i = 0; i < indexnum; i++)
		{
			pIdx[i] = IndexArry[i];
			n = i;
		}

		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->GetIndexBuffer(), 0);
	}



	FbxMatrix offset = node->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);//オフセット行列の取得
	this->SetOffset(FbxMatrixConvertToXMMATRIX(offset));


	// マテリアルの数
	//int materialNum = 1;



	int mtlcnt = node->GetMaterialCount();

	this->SetSubsetNum(mtlcnt);
	this->CreateSubsetArray(mtlcnt);

	// マテリアル情報を取得
	for (int i = 0; i < mtlcnt; i++)
	{
		FbxSurfaceMaterial* fbxmaterial = node->GetMaterial(i);

		FbxClassId id = fbxmaterial->GetClassId();


		if (fbxmaterial != 0)
		{
			MATERIAL material;
			ZeroMemory(&material, sizeof(MATERIAL));

			// マテリアル解析
			// LambertかPhongか
			//lambart
			if (id == (FbxSurfaceLambert::ClassId))
			{

				// Lambertにダウンキャスト
				FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)fbxmaterial;

				material.phong = 0;

				//アンビエント
				FbxDouble3 amb = lambert->Ambient;

				material.Ambient.x = (float)amb[0];
				material.Ambient.y = (float)amb[1];
				material.Ambient.z = (float)amb[2];

				//ディフューズ
				FbxDouble3 diffuse = lambert->Diffuse;

				material.Diffuse.x = (float)diffuse[0];
				material.Diffuse.y = (float)diffuse[1];
				material.Diffuse.z = (float)diffuse[2];

				// 透過度
				FbxDouble transparency = lambert->TransparencyFactor;
				material.Diffuse.w = (float)transparency;

				//エミッション
				FbxDouble3 emi = lambert->Emissive;

				material.Emission.x = (float)emi[0];
				material.Emission.y = (float)emi[1];
				material.Emission.z = (float)emi[2];



			}
			//phong
			else if (id == (FbxSurfacePhong::ClassId))
			{

				// Phongにダウンキャスト
				FbxSurfacePhong* phong = (FbxSurfacePhong*)fbxmaterial;
				material.phong = 1;
				//アンビエント
				FbxDouble3 amb = phong->Ambient;

				material.Ambient.x = (float)amb[0];
				material.Ambient.y = (float)amb[1];
				material.Ambient.z = (float)amb[2];

				//ディフューズ
				FbxDouble3 diffuse = phong->Diffuse;

				material.Diffuse.x = (float)diffuse[0];
				material.Diffuse.y = (float)diffuse[1];
				material.Diffuse.z = (float)diffuse[2];

				// 透過度
				FbxDouble transparency = phong->TransparencyFactor;
				material.Diffuse.w = (float)transparency;

				//エミッション
				FbxDouble3 emi = phong->Emissive;

				material.Emission.x = (float)emi[0];
				material.Emission.y = (float)emi[1];
				material.Emission.z = (float)emi[2];

				//スペキュラー
				FbxDouble3 spe = phong->Specular;

				material.Specular.x = (float)spe[0];
				material.Specular.y = (float)spe[1];
				material.Specular.z = (float)spe[2];

				FbxDouble shine = phong->Shininess;
				material.Shininess = (float)shine;


			}


			// プロパティ取得。
			const FbxProperty property = fbxmaterial->FindProperty(
				FbxSurfaceMaterial::sDiffuse    // const char* pName
			);                                  // bool pCaseSensitive = true


			int fileTextureCount = property.GetSrcObjectCount<FbxFileTexture>();
			// プロパティが持っているレイヤードテクスチャの枚数をチェック
			int layerNum = property.GetSrcObjectCount<FbxLayeredTexture>();


			material.noDiffuseTex = true;
			material.noNormalTex = true;
			for (int j = 0; j < fileTextureCount; j++)
			{

				FbxFileTexture* pFileTexture = mesh->GetScene()->GetSrcObject<FbxFileTexture>(j);

				FbxFileTexture::ETextureUse m_type = FbxFileTexture::ETextureUse(pFileTexture->GetTextureUse());


				//ディフューズテクスチャなら
				if (m_type == FbxFileTexture::ETextureUse::eStandard)
				{
					const char* fileName = pFileTexture->GetFileName();
					int slush = '/';
					char* path;
					path = new char[256];

					//最後に/が出てくる場所
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



					// マテリアル設定
					material.noDiffuseTex = false;
				}
				if (m_type == FbxFileTexture::ETextureUse::eBumpNormalMap)
				{

					const char* fileName = pFileTexture->GetFileName();
					int slush = '/';
					char* path;
					path = new char[256];

					//最後に/が出てくる場所
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



					this->GetSubset()[i].LoadNormalTex(path);

					material.noNormalTex = FALSE;
				}


				// プロパティ取得。
				const FbxProperty propertynormal = fbxmaterial->FindProperty(
					FbxSurfaceMaterial::sBump    // const char* pName
				);                                  // bool pCaseSensitive = true



				// プロパティが持っているレイヤードテクスチャの枚数をチェック
				layerNum = propertynormal.GetSrcObjectCount<FbxFileTexture>();

				if (layerNum > 0)
				{
					FbxFileTexture* pFileTextureNormal = propertynormal.GetSrcObject<FbxFileTexture>(0);

					FbxFileTexture::ETextureUse m_type = FbxFileTexture::ETextureUse(pFileTextureNormal->GetTextureUse());


					if (m_type == FbxFileTexture::ETextureUse::eStandard)
					{

						const char* fileName1 = pFileTextureNormal->GetFileName();
						int slush = '/';
						char* path;
						path = new char[256];

						//最後に/が出てくる場所
						const char* last = strrchr(fileName1, slush);

						strcpy(path, "data/TEXTURE");
						strcat(path, last);

						this->GetSubset()[i].LoadNormalTex(path);

						delete[]path;

						material.noNormalTex = FALSE;

					}


				}



			}
			this->GetSubset()[i].SetMaterial(material);
			this->GetSubset()[i].SetpMeshData(this);

			MATERIAL m = this->GetSubset()[i].GetMaterial();
		}
	}

	delete[] IndexArry;
	delete[] uv;

}

int SkinMeshData::GetVertNum(void)
{

	return this->vertNum;
}

VERTEX_3D SkinMeshData::GetVertex(int n)
{
	return this->VertexArray[n];
}

CONTROLPOINT SkinMeshData::GetControlPointVertex(int n)
{
	return this->controlPoint[this->controlPointIndexArray[n]];
}

CONTROLPOINT* SkinMeshData::GetControlPoint(int n)
{
	return &this->controlPoint[n];
}
CONTROLPOINT* SkinMeshData::GetControlPointArray(void)
{
	return this->controlPoint;
}

int SkinMeshData::GetControlNum(void)
{
	return this->controlPointNum;
}

int* SkinMeshData::GetCPIndexArray(void)
{
	return this->controlPointIndexArray;
}


SkinMeshDataList::SkinMeshDataList()
{
	skeleton = new Skeleton;
	skinMeshData = nullptr;
}

SkinMeshDataList::~SkinMeshDataList()
{
	delete skeleton;
	if (skinMeshData) delete[]skinMeshData;

}

void SkinMeshDataList::LoadSkinMeshDataList(string filepath, AssetsManager* ap)
{
	
	this->SetpAssetsManager(ap);
	SetFilePath(filepath);

	FbxManager* manager;
	FbxIOSettings* ioSettings;
	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);


	// Importerを生成
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings()) == false)
	{
		// インポートエラー
	}

	//SceneオブジェクトにFBXファイル内の情報を流し込む
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);

	int meshcount = scene->GetSrcObjectCount<FbxMesh>();
	this->SetMeshDataNum(meshcount);
	this->CreateSkinMeshDataArray(meshcount);


	this->skeleton->LoadSkeleton(scene);



	for (int i = 0; i < meshcount; i++)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(i);
		this->skinMeshData[i].LoadSkinMeshData(mesh,this->skeleton, this->GetpAssetsManager());

	}

	scene->Destroy();
	importer->Destroy(); // シーンを流し込んだらImporterは解放
	manager->Destroy();//fbxファイルの読み込みが終わったらマネージャーは解放する


}

void SkinMeshDataList::CreateSkinMeshDataArray(int n)
{
	this->skinMeshData = new SkinMeshData[n];

}

SkinMeshData* SkinMeshDataList::GetSkinMeshData(void)
{
	return this->skinMeshData;
}

Skeleton* SkinMeshDataList::GetSkeleton(void)
{
	return this->skeleton;
}
XMMATRIX GetParentTransFormFbx(FbxNode* boneNode, XMMATRIX mtx)
{
	FbxAMatrix fmtx = boneNode->EvaluateLocalTransform(FBXSDK_TIME_INFINITE);

	XMMATRIX amtx;
	amtx = XMMatrixMultiply(mtx, FbxMatrixConvertToXMMATRIX(fmtx));

	FbxNode* p = boneNode->GetParent();
	string name = p->GetName();
	string rootnode = "RootNode";

	if (name != rootnode)
	{
		FbxNodeAttribute* attr = p->GetNodeAttribute();
		FbxNodeAttribute::EType type = attr->GetAttributeType();
		if (type)
		{
			if (type == FbxNodeAttribute::EType::eSkeleton)
			{
				amtx = GetParentTransFormFbx(boneNode->GetParent(), amtx);
			}

		}


	}



	return amtx;

}
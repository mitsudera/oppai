#include "LambartMaterial.h"
#include "LambartShader.h"
#include "DX11Texture.h"
#include "AssetsManager.h"

LambartMaterial::LambartMaterial(AssetsManager* assetsManager)
{

	this->pAssetsManager = assetsManager;

	this->pShader = assetsManager->GetLambartShader();
	this->pLambartShader = assetsManager->GetLambartShader();

	this->diffuse = { 1.0f,1.0f,1.0f,1.0f };
	this->noDiffuseTex = 1;
	this->noNormalTex = 1;
	this->noArmTex = 1;
	this->textureDiffuseIndex = 0;
	this->textureNormalIndex = 0;
	this->textureArmIndex = 0;


}

LambartMaterial::LambartMaterial(LambartMaterial* lambart)
{
	this->pAssetsManager = lambart->pAssetsManager;

	this->pShader = lambart->pShader;
	this->pLambartShader = lambart->pLambartShader;

	this->diffuse = lambart->diffuse;
	this->noDiffuseTex = lambart->noDiffuseTex;
	this->noNormalTex = lambart->noNormalTex;
	this->noArmTex = lambart->noArmTex;
	this->textureDiffuseIndex = lambart->textureDiffuseIndex;
	this->textureNormalIndex = lambart->textureNormalIndex;
	this->textureArmIndex = lambart->textureArmIndex;



}

LambartMaterial::~LambartMaterial()
{
}

void LambartMaterial::SetBufferMaterial(void)
{
	LambartShader::MaterialCBuffer mCBuffer;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	mCBuffer.noNormalTex = this->noNormalTex;
	mCBuffer.noArmTex = this->noArmTex;
	this->pLambartShader->SetMaterialCbuffer(mCBuffer);

	if (!noDiffuseTex) pAssetsManager->GetTexture(textureDiffuseIndex)->SetShaderResourcePS(0);
	if (!noNormalTex) pAssetsManager->GetTexture(textureNormalIndex)->SetShaderResourcePS(1);
	if (!noArmTex) pAssetsManager->GetTexture(textureArmIndex)->SetShaderResourcePS(2);


}


void LambartMaterial::LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial)
{

	// Lambertにダウンキャスト
	FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)fbxmaterial;

	//ディフューズ
	FbxDouble3 fbxdiffuse = lambert->Diffuse;

	diffuse.x = (float)fbxdiffuse[0];
	diffuse.y = (float)fbxdiffuse[1];
	diffuse.z = (float)fbxdiffuse[2];


	FbxDouble3 fbxambient = lambert->Ambient;

	ambient.x = (float)fbxambient[0];
	ambient.y = (float)fbxambient[1];
	ambient.z = (float)fbxambient[2];


	// 透過度
	FbxDouble transparency = lambert->TransparencyFactor;
	diffuse.w = (float)transparency;



	this->noDiffuseTex = true;
	this->noNormalTex = true;
	this->noArmTex = true;


	// プロパティ取得。
	const FbxProperty property = fbxmaterial->FindProperty(
		FbxSurfaceMaterial::sDiffuse    // const char* pName
	);                                  // bool pCaseSensitive = true



	// プロパティが持っているレイヤードテクスチャの枚数をチェック
	int layerNum = property.GetSrcObjectCount<FbxFileTexture>();

	if (layerNum > 0)
	{
		FbxFileTexture* pFileTexture = property.GetSrcObject<FbxFileTexture>(0);

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

			this->LoadDiffuseTex(path);

			delete[]path;

			// マテリアル設定
			noDiffuseTex = false;
		}

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

			this->LoadNormalTex(path);

			delete[]path;

			noNormalTex = FALSE;

		}


	}




}

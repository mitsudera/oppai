#include "SkyMaterial.h"
#include "SkyShader.h"
#include "DX11Texture.h"
#include "AssetsManager.h"

SkyMaterial::SkyMaterial(AssetsManager* assetsManager)
{

	this->pAssetsManager = assetsManager;

	this->pShader = assetsManager->GetSkyShader();
	this->pSkyShader = assetsManager->GetSkyShader();


}

SkyMaterial::SkyMaterial(SkyMaterial* sky)
{
	this->pAssetsManager = sky->pAssetsManager;

	this->pShader = sky->pShader;
	this->pSkyShader = sky->pSkyShader;

	this->diffuse = sky->diffuse;
	this->noDiffuseTex = sky->noDiffuseTex;
	this->noNormalTex = sky->noNormalTex;
	this->noArmTex = sky->noArmTex;
	this->textureDiffuseIndex = sky->textureDiffuseIndex;
	this->textureNormalIndex = sky->textureNormalIndex;
	this->textureArmIndex = sky->textureArmIndex;



}

SkyMaterial::~SkyMaterial()
{
}

void SkyMaterial::SetBufferMaterial(void)
{
	SkyShader::MaterialCBuffer mCBuffer;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	mCBuffer.noNormalTex = this->noNormalTex;
	mCBuffer.noArmTex = this->noArmTex;
	this->pSkyShader->SetMaterialCbuffer(mCBuffer);

	if (!noDiffuseTex) pAssetsManager->GetTexture(textureDiffuseIndex)->SetShaderResourcePS(0);
	if (!noNormalTex) pAssetsManager->GetTexture(textureNormalIndex)->SetShaderResourcePS(1);
	if (!noArmTex) pAssetsManager->GetTexture(textureArmIndex)->SetShaderResourcePS(2);


}

void SkyMaterial::LoadDiffuseTex(string fName)
{
	this->noDiffuseTex = FALSE;
	textureDiffuseIndex = pAssetsManager->LoadTexture(fName);
}

void SkyMaterial::LoadNormalTex(string fName)
{
	this->noNormalTex = FALSE;

	textureNormalIndex = pAssetsManager->LoadTexture(fName);
}

void SkyMaterial::LoadArmTex(string fName)
{
	this->noArmTex = FALSE;

	textureArmIndex = pAssetsManager->LoadTexture(fName);
}

void SkyMaterial::LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial)
{

	// Lambert�Ƀ_�E���L���X�g
	FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)fbxmaterial;



	//�f�B�t���[�Y
	FbxDouble3 fbxdiffuse = lambert->Diffuse;

	diffuse.x = (float)fbxdiffuse[0];
	diffuse.y = (float)fbxdiffuse[1];
	diffuse.z = (float)fbxdiffuse[2];

	// ���ߓx
	FbxDouble transparency = lambert->TransparencyFactor;
	diffuse.w = (float)transparency;



	this->noDiffuseTex = true;
	this->noNormalTex = true;
	this->noArmTex = true;


	// �v���p�e�B�擾�B
	const FbxProperty property = fbxmaterial->FindProperty(
		FbxSurfaceMaterial::sDiffuse    // const char* pName
	);                                  // bool pCaseSensitive = true



	// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N
	int layerNum = property.GetSrcObjectCount<FbxFileTexture>();

	if (layerNum > 0)
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

			this->LoadDiffuseTex(path);

			delete[]path;

			// �}�e���A���ݒ�
			noDiffuseTex = false;
		}

	}






	// �v���p�e�B�擾�B
	const FbxProperty propertynormal = fbxmaterial->FindProperty(
		FbxSurfaceMaterial::sBump    // const char* pName
	);                                  // bool pCaseSensitive = true



	// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N
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

			//�Ō��/���o�Ă���ꏊ
			const char* last = strrchr(fileName1, slush);

			strcpy(path, "data/TEXTURE");
			strcat(path, last);

			this->LoadNormalTex(path);

			delete[]path;

			noNormalTex = FALSE;

		}


	}




}

void SkyMaterial::SetDiffuse(XMFLOAT4 diff)
{
	this->diffuse = diff;
}

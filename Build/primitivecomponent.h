#pragma once
#include "Component.h"

class Material;
class LambartMaterial;
class PhongMaterial;
class SkyMaterial;
class UIMaterial;
class Renderer;
class CBufferManager;
class AssetsManager;

class PrimitiveComponent : public Component
{
public:
	PrimitiveComponent();
	PrimitiveComponent(GameObject* gameObject);
	~PrimitiveComponent();

	BOOL GetHasShadow(void);
	void SetHasShadow(BOOL b);

	BOOL GetDrawShadow(void);
	void SetDrawShadow(BOOL b);

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;
	virtual void Draw(void) override;
	virtual void ShadowMapping(void);


	int GetMaterialIndex(void);
	void SetMaterialIndex(int index);
	int LoadMaterial(Material* material);

protected:
	Renderer* pRenderer;
	BOOL hasShadow;
	BOOL drawShadow;
	int materialIndex;
	int shadowMaterialIndex;
	BOOL alphaTest;
	CBufferManager* pCBufferManager;
	AssetsManager* pAssetsManager;
};


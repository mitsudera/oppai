#pragma once
#include "Component.h"

class Material;
class LambartMaterial;
class PhongMaterial;
class SkyMaterial;
class UIMaterial;


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


	int GetMaterialIndex(void);


protected:

	BOOL hasShadow;
	BOOL drawShadow;
	int materialIndex;
};


#pragma once
#include "TransformComponent.h"

class Material;
class LambartMaterial;
class PhongMaterial;
class SkyMaterial;
class UIMaterial;


class PrimitiveComponent : public TransformComponent
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

	Material* GetMaterial(void);


	template<class T>
	T* GetMaterial(void);

	void SetMaterial(Material* m);



protected:

	BOOL hasShadow;
	BOOL drawShadow;
	Material* material;
};


#pragma once
#include "TransformComponent.h"

class PrimitiveComponent : public TransformComponent
{
public:
	PrimitiveComponent();
	~PrimitiveComponent();

	BOOL GetHasShadow(void);
	void SetHasShadow(BOOL b);

	BOOL GetDrawShadow(void);
	void SetDrawShadow(BOOL b);

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;
	virtual void Draw(void) override;


protected:

	BOOL hasShadow;
	BOOL drawShadow;
};
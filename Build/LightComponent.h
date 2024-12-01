#pragma once
#include "transformcomponent.h"
#include "LightManager.h"

class LightComponent:public TransformComponent
{
public:

	LightComponent();
	LightComponent(GameObject* gameObject);
	~LightComponent();

	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual LIGHT_PARAM GetLightParam(void);

private:
	LIGHT_PARAM lightParam;

};


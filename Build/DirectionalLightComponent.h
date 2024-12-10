#pragma once
#include "Component.h"
#include "LightManager.h"
class DirectionalLightComponent :public Component
{
public:

	DirectionalLightComponent(GameObject* gameObject);
	~DirectionalLightComponent();

	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void SetLight(DIREC_LIGHT_PARAM direcLight);
	DIREC_LIGHT_PARAM& GetLightParam(void);


private:

	DIREC_LIGHT_PARAM  param;
	int index;
};


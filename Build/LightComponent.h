#pragma once
#include "component.h"
#include"renderer.h"

class LightComponent:public Component
{
public:

	// Component を介して継承されました
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;




};


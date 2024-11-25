#pragma once
#include "component.h"
#include"renderer.h"

class LightComponent:public Component
{
public:

	// Component ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;




};


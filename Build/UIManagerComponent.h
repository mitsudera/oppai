#pragma once
#include "component.h"
class UIManagerComponent :public Component
{
public:
	UIManagerComponent(GameObject* gameObject);
	~UIManagerComponent();

	// Component ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

};


#pragma once
#include "component.h"
class TitleManagerComponent :public Component
{
public:
	TitleManagerComponent(GameObject* gameObject);
	~TitleManagerComponent();

	// Component を介して継承されました
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
};


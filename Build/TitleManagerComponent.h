#pragma once
#include "component.h"
class TitleManagerComponent :public Component
{
public:
	TitleManagerComponent(GameObject* gameObject);
	~TitleManagerComponent();

	// Component ����Čp������܂���
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
};


#pragma once
#include "component.h"
class UIManagerComponent :public Component
{
public:
	UIManagerComponent(GameObject* gameObject);
	~UIManagerComponent();

	// Component ����Čp������܂���
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

};


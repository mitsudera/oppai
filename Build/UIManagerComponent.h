#pragma once
#include "component.h"
class UIManagerComponent :public Component
{
public:
	// Component ����Čp������܂���
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

};


#pragma once
#include "component.h"
class GameManagerComponent :public Component
{
public:
	GameManagerComponent(GameObject* gameObject);
	~GameManagerComponent();
	// Component ����Čp������܂���
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
private:

};


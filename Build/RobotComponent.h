#pragma once
#include "component.h"
class RobotComponent : public Component
{
public:
	RobotComponent(GameObject* gameObject);
	~RobotComponent();

	virtual void Init(void)override;
	virtual void Uninit(void)override;
	virtual void Update(void)override;

private:


};


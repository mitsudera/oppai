#pragma once
#include "ColliderComponent.h"
class SphereColliderComponent :public ColliderComponent
{
public:
	SphereColliderComponent(GameObject* gameObject);
	~SphereColliderComponent();

	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void SetRadius(float r);

protected:

};


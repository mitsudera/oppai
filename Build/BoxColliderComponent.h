#pragma once
#include "ColliderComponent.h"
class BoxColliderComponent :public ColliderComponent
{
public:
	BoxColliderComponent(GameObject* gameObject);
	~BoxColliderComponent();

	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;


	void SetBox(XMFLOAT3 size);

	XMFLOAT3 GetSize(void);

protected:

	XMFLOAT3 size;

};


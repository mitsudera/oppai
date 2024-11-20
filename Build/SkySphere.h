#pragma once
#include "gameobject.h"
class SkySphere :public GameObject
{
public:
	SkySphere(Level* level);
	~SkySphere();


	// GameObject ����Čp������܂���
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

private:
	MeshComponent* meshComponent;


};


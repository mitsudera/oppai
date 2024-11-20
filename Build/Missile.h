#pragma once
#include "gameobject.h"

class MeshComponent;
class MissileComponent;
class MissleList;

class Missile : public GameObject
{
public:
	Missile(Level* level);
	~Missile();

	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

	void Launch(XMFLOAT3 pos, XMMATRIX rot, float spd, GameObject* target, ObjectTag tag);

	MissileComponent* GetMissileComponent(void);

private:

	MeshComponent* mesh;
	MissileComponent* missileComponent;
};


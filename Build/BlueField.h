#pragma once
#include "gameobject.h"
class BlueField :public GameObject
{
public:
	BlueField(Level* level);
	~BlueField();


	// GameObject を介して継承されました
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

private:
	MeshComponent* mesh;
};


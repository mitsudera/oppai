#pragma once
#include "gameobject.h"
class MeshField :public GameObject
{
public:

	MeshField(Scene* scene);
	~MeshField();

	virtual void Init(void) override;


};


#pragma once
#include "gameobject.h"
class Tree :public GameObject
{
public:
	Tree(Scene* scene);
	~Tree();

	virtual void Init(void) override;

};


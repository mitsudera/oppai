#pragma once
#include "Coreminimal.h"

class GameEngine;
class GameObject;
class TransformComponent;

class Component
{
public:

	enum class Attribute
	{
		Component,
		Transform,
		Collider,
		Primitive,
		Light,
		Manager,
	};

	Component();
	Component(GameObject* gameObject);
	~Component();

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void);

	GameObject* GetGameObject(void);

	Component::Attribute GetAttribute(void);


protected:
	GameObject* pGameObject;
	Attribute attribute;

};


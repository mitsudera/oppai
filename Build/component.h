#pragma once
#include "Coreminimal.h"

class GameEngine;
class GameObject;
class TransformComponent;
class Input;

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
		Camera,
	};

	Component();
	Component(GameObject* gameObject);
	~Component();

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	GameObject* GetGameObject(void);

	Component::Attribute GetAttribute(void);

	void SetActive(BOOL b);
	BOOL GetActive(void);

	template<class T>
	T* GetComponent(void);


protected:
	GameObject* pGameObject;
	Attribute attribute;
	BOOL	isActive;
	GameEngine* pGameEngine;
	Input* input;

};


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

	XMMATRIX GetWorldMtx(XMMATRIX world);
	XMMATRIX GetWorldMtx(void);
	XMFLOAT3 GetWorldPos(void);

	TransformComponent* GetTransFormComponent(void);

protected:
	GameObject* pGameObject;
	Attribute attribute;
	BOOL	isActive;
	GameEngine* pGameEngine;
	Input* input;

};

template<class T>
T* Component::GetComponent(void)
{
	return pGameObject->GetComponent<T>();
}

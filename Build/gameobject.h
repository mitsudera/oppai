#pragma once

#include "Coreminimal.h"

typedef enum
{
	TagNone,
	TagPlayer,
	TagEnemy,
	TagField,
	TagPlayerBullet,
	TagPlayerMissile,
	TagEnemyMissile,

	ObjectTagMax,
}ObjectTag;


class ColliderComponent;
class Scene;
class TransformComponent;
class Component;

class GameObject
{
public:
	GameObject();
	GameObject(Scene* scene);
	GameObject(GameObject* parent);
	~GameObject();

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	Scene* GetScene(void);
	TransformComponent* GetTransFormComponent(void);
	ColliderComponent* GetCollider(void);

	ObjectTag GetTag(void);

	BOOL GetActive(void);
	void SetActive(BOOL isActive);

	GameObject* GetParent(void);
	GameObject* GetChild(int index);

protected:
	Scene* pScene;
	TransformComponent* transformComponent;
	ColliderComponent* collider;

	vector<Component*> componentList;

	ObjectTag tag;

	BOOL isActive;

	GameObject* parent;
	vector <GameObject*> child;

};


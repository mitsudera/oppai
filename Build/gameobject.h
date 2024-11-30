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

typedef enum
{
	Default,
	Field,
	Water,
	UI,
	Player,

	LayerMax,
}Layer;

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
	Layer GetLayer(void);

	BOOL GetActive(void);
	void SetActive(BOOL isActive);

	GameObject* GetParent(void);
	GameObject* GetChild(int index);

	vector<Component*>& GetComponentList(void);
protected:
	Scene* pScene;
	TransformComponent* transformComponent;
	ColliderComponent* collider;

	vector<Component*> componentList;

	ObjectTag tag;
	Layer layer;

	BOOL isActive;

	GameObject* parent;
	vector <GameObject*> child;

};


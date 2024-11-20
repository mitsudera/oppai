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


class MeshComponent;
class ColliderComponent;
class Scene;
class TransformComponent;

class GameObject
{
public:
	GameObject();
	GameObject(Scene* scene);
	~GameObject();

	virtual void Init(void)=0;
	virtual void Uninit(void)=0;
	virtual void Update(void)=0;
	virtual void Draw(void)=0;

	Scene* GetScene(void);
	TransformComponent* GetTransFormComponent(void);
	ColliderComponent* GetCollider(void);

	ObjectTag GetTag(void);

	BOOL GetUse(void);
	void SetUse(BOOL use);


protected:
	Scene* pScene;
	TransformComponent* transformComponent;
	ColliderComponent* collider;

	ObjectTag tag;

	BOOL use;


};


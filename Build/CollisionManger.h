#pragma once
#include "Coreminimal.h"
#include "ColliderComponent.h"

class ColliderComponent;
class Scene;
struct ColliderPair
{
	ColliderComponent* collider1;
	ColliderComponent* collider2;

};

class CollisionManger
{
public:
	CollisionManger(Scene* scene);
	~CollisionManger();

	void Init(void);
	void Uninit(void);
	void Update(void);

	void AddCollider(ColliderComponent* collider);

	void DeleteCllider(ColliderComponent* collider);


	BOOL CheckCillision(ColliderPair pair);


private:

	Scene* pScene;
	vector<ColliderComponent*> colliderList;
	vector<ColliderPair> pairList;

};


#pragma once
#include "Coreminimal.h"

class ColliderComponent;
class GameEngine;
struct ColliderPair
{
	ColliderComponent* collider1;
	ColliderComponent* collider2;

};

class CollisionManager
{
public:
	CollisionManager(GameEngine* gameEngine);
	~CollisionManager();

	void Update(void);

	void AddCollider(ColliderComponent* collider);

	void DeleteCllider(ColliderComponent* collider);


	BOOL CheckCillision(ColliderPair pair);


private:

	GameEngine* pGameEngine;
	vector<ColliderComponent*> colliderList;
	vector<ColliderPair> pairList;

};


#pragma once
#include "gameobject.h"

class SceneManager;
class GameEngine;
class CollisionManger;
class Scene
{
public:
	Scene();
	Scene(GameEngine* gameEngine);
	~Scene();

	virtual void Init();
	void Update();
	void Draw();
	void Uninit();

	GameEngine* GetGameEngine(void);
	CollisionManger* GetCollisionManager(void);
protected:
	GameEngine* gameEngine;
	vector <GameObject*> gameObjectList;
	CollisionManger* coliisionManager;
};


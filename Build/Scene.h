#pragma once
#include "gameobject.h"

class SceneManager;
class GameEngine;
class CollisionManger;
class Scene
{
public:
	Scene(GameEngine* gameEngine);
	~Scene();

	void Init();
	void Update();
	void Draw();
	void Uninit();

	GameEngine* GetGameEngine(void);
	CollisionManger* GetCollisionManager(void);
private:
	GameEngine* gameEngine;
	vector <GameObject*> gameObjectList;
	CollisionManger* coliisionManager;
};


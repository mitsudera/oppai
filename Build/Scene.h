#pragma once
#include "gameobject.h"

class SceneManager;
class GameEngine;
class CollisionManger;
class CameraComponent;

class Scene
{
public:
	Scene();
	Scene(GameEngine* pGameEngine);
	~Scene();

	virtual void Init();
	void Update();
	void Draw();
	void Uninit();

	GameEngine* GetGameEngine(void);
	CollisionManger* GetCollisionManager(void);

	vector <GameObject*>& GetGameObject(void);

	GameObject* GetGameObjectName(string name);

protected:
	GameEngine* pGameEngine;
	vector <GameObject*> gameObjectList;
	CollisionManger* coliisionManager;
	CameraComponent* mainCamera;
	vector<CameraComponent*> cameraArray;

};


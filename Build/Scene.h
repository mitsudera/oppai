#pragma once
#include "gameobject.h"

class SceneManager;
class GameEngine;
class CollisionManager;
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


	void InitAllObject(void);

	GameEngine* GetGameEngine(void);

	vector <GameObject*>& GetGameObject(void);

	GameObject* GetGameObjectName(string name);

	void AddCamera(CameraComponent* camera);

protected:
	GameEngine* pGameEngine;
	vector <GameObject*> gameObjectList;
	vector<CameraComponent*> cameraArray;

};


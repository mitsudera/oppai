#pragma once
#include"Coreminimal.h"

class GameEngine;
class Scene;



class SceneManager
{

	enum SCENE
	{
		TITLE,

		MAX_SCENE
	};


public:
	SceneManager(GameEngine* gameEngine);
	~SceneManager();
	void Init();
	void Update();
	void Draw();
	void Uninit();

	void SetScene(SCENE scene);

private:
	
	GameEngine* pGameEngine;
	vector<Scene*> SceneList;
	Scene* activeScene;
	
};


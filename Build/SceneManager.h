#pragma once
#include"Coreminimal.h"

class GameEngine;
class Scene;



class SceneManager
{

	enum SCENE
	{
		TITLE,
		GAME,
		MAX_SCENE,
	};


public:
	SceneManager(GameEngine* gameEngine);
	~SceneManager();

	void SetScene(SCENE scene);
	void SetDefaultScene(void);

private:
	
	
	GameEngine* pGameEngine;
	vector<Scene*> SceneList;
	SCENE defaultScene;

};


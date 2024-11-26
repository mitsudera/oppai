#pragma once
#include"Coreminimal.h"

enum SCENE
{
	TITLE,
	GAME,
	MAX_SCENE,
};


class GameEngine;
class Scene;



class SceneManager
{



public:
	SceneManager(GameEngine* pGameEngine);
	~SceneManager();

	void SetScene(SCENE scene);
	void SetDefaultScene(void);

private:
	
	
	GameEngine* pGameEngine;
	vector<Scene*> SceneList;
	SCENE defaultScene;

};


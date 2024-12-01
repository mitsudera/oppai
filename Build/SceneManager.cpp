#include "SceneManager.h"
#include "Scene.h"
#include "GameEngine.h"
#include "TitleScene.h"
#include "GameScene.h"
SceneManager::SceneManager(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;

	this->defaultScene = SCENE::TITLE;

	TitleScene* titleScene = new TitleScene(pGameEngine);

	this->SceneList.push_back(titleScene);

	GameScene* gameScene = new GameScene(pGameEngine);

	this->SceneList.push_back(gameScene);


}

SceneManager::~SceneManager()
{
}


void SceneManager::SetScene(SCENE scene)
{
	this->pGameEngine->SetActiveScene(this->SceneList[scene]);
	

}

void SceneManager::SetDefaultScene(void)
{
	SetScene(defaultScene);
}

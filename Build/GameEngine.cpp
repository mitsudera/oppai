#include "GameEngine.h"
#include "Main.h"
#include "renderer.h"
#include "AssetsManager.h"
#include "input.h"
#include "SceneManager.h"
#include "Scene.h"


GameEngine::GameEngine(Main* main)
{
	this->main = main;


}

GameEngine::~GameEngine()
{
}

void GameEngine::Init()
{
	
	this->windowSize.x = screenWidth;
	this->windowSize.y = screenHeight;


	this->renderer = new Renderer(this);

	this->renderer->InitRenderer(*main->GetInstanceHandle(), *main->GetWindowHangle(), true);

	this->assetsManager = new AssetsManager(this);

	this->input = new Input();

	this->input->Init(*main->GetInstanceHandle(), *main->GetWindowHangle());

	this->sceneManager = new SceneManager(this);
	this->sceneManager->SetDefaultScene();
}

void GameEngine::Update()
{
	this->input->Update();
	this->activeScene->Update();


}

void GameEngine::Draw()
{
	renderer->Clear();

	

	this->activeScene->Draw();


	renderer->Present();

}

void GameEngine::Uninit()
{

	this->activeScene->Uninit();

	this->input->Uninit();
	this->assetsManager->Uninit();
	this->renderer->UninitRenderer();


	delete input;
	delete assetsManager;
	delete renderer;


}

long GameEngine::GetMousePosX(void)
{
	return main->GetMousePosX();
}

long GameEngine::GetMousePosY(void)
{
	return main->GetMousePosY();
}

float GameEngine::GetDeltaTime(void)
{
	return main->GetDeltaTime();
}

XMFLOAT2 GameEngine::GetWindowSize(void)
{
	return this->windowSize;
}

AssetsManager* GameEngine::GetAssetsManager(void)
{
	return this->assetsManager;
}

Renderer* GameEngine::GetRenderer(void)
{
	return this->renderer;
}

Input* GameEngine::GetInput(void)
{
	return this->input;
}

CameraComponent* GameEngine::GetMainCamera(void)
{
	return mainCamera;
}

void GameEngine::SetMainCamera(CameraComponent* cam)
{
	this->mainCamera = cam;
}

Scene* GameEngine::GetActiveScene(void)
{
	return this->activeScene;
}

void GameEngine::SetActiveScene(Scene* scene)
{
	//this->activeScene->Uninit();
	this->activeScene = scene;
	activeScene->Init();

}

SceneManager* GameEngine::GetSceneManager(void)
{
	return this->sceneManager;
}



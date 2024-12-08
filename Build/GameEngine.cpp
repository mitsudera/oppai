#include "GameEngine.h"
#include "Main.h"
#include "renderer.h"
#include "AssetsManager.h"
#include "input.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CBufferManager.h"
#include "LightManager.h"

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

	this->cBufferManager = new CBufferManager(this);

	this->lightManager = new LightManager(this);
	lightManager->Init();

	this->assetsManager = new AssetsManager(this);
	this->assetsManager->Init();

	this->input = new Input();
	this->input->Init(*main->GetInstanceHandle(), *main->GetWindowHangle());

	this->sceneManager = new SceneManager(this);
	this->sceneManager->SetDefaultScene();
}

void GameEngine::Update()
{
	this->mouseDeltaX = input->GetMouseX() - oldMousePosX;
	this->mouseDeltaY = input->GetMouseY() - oldMousePosY;


	this->oldMousePosX = input->GetMouseX();
	this->oldMousePosY = input->GetMouseY();

	this->input->Update();
 	this->activeScene->Update();
	lightManager->Update();

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
	this->lightManager->Uninit();

	delete input;
	delete assetsManager;
	delete renderer;


}

long GameEngine::GetMouseMoveX(void)
{
	return mouseDeltaX;
}

long GameEngine::GetMouseMoveY(void)
{
	return mouseDeltaY;
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

CBufferManager* GameEngine::GetCBufferManager(void)
{
	return this->cBufferManager;
}

Input* GameEngine::GetInput(void)
{
	return this->input;
}


Scene* GameEngine::GetActiveScene(void)
{
	return this->activeScene;
}

void GameEngine::SetActiveScene(Scene* scene)
{
	if(this->activeScene) this->activeScene->Uninit();
	this->activeScene = scene;
	activeScene->Init();

}

SceneManager* GameEngine::GetSceneManager(void)
{
	return this->sceneManager;
}



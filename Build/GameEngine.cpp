#include "GameEngine.h"
#include "Main.h"
#include "renderer.h"
#include "AssetsManager.h"
#include "input.h"



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

}

void GameEngine::Update()
{
	this->input->Update();

}

void GameEngine::Draw()
{
	renderer->Clear();



	renderer->Present();

}

void GameEngine::Uninit()
{
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


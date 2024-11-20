#pragma once
#include "Coreminimal.h"

class AssetsManager;
class Renderer;
class Input;
class Main;
class CameraComponent;

class GameEngine
{
public:
	GameEngine();
	GameEngine(Main* main);
	~GameEngine();

	void Init();
	void Update();
	void Draw();
	void Uninit();

	long GetMousePosX(void);
	long GetMousePosY(void);

	// 前フレームからの経過ミリ秒を取得する
	float GetDeltaTime(void);

	XMFLOAT2 GetWindowSize(void);

	AssetsManager* GetAssetsManager(void);
	Renderer* GetRenderer(void);
	Input* GetInput(void);

	CameraComponent* GetMainCamera(void);

private:

	Main* main;
	AssetsManager* assetsManager;
	Renderer* renderer;
	Input* input;
	CameraComponent* mainCamera;

	XMFLOAT2 windowSize;
};


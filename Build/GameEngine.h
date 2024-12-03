#pragma once
#include "Coreminimal.h"

class AssetsManager;
class Renderer;
class Input;
class Main;
class CameraComponent;
class SceneManager;
class Scene;
class CBufferManager;
class LightManager;

class GameEngine
{
public:
	GameEngine(Main* main);
	~GameEngine();

	void Init();
	void Update();
	void Draw();
	void Uninit();

	long GetMouseMoveX(void);
	long GetMouseMoveY(void);

	// 前フレームからの経過ミリ秒を取得する
	float GetDeltaTime(void);

	XMFLOAT2 GetWindowSize(void);

	AssetsManager* GetAssetsManager(void);
	Renderer* GetRenderer(void);
	CBufferManager* GetCBufferManager(void);
	Input* GetInput(void);

	CameraComponent* GetMainCamera(void);
	void SetMainCamera(CameraComponent* cam);

	Scene* GetActiveScene(void);
	void SetActiveScene(Scene* scene);
	SceneManager* GetSceneManager(void);
private:

	Main* main;
	AssetsManager* assetsManager;
	Renderer* renderer;
	CBufferManager* cBufferManager;
	Input* input;
	LightManager* lightManager;

	CameraComponent* mainCamera;
	SceneManager* sceneManager;
	Scene* activeScene;

	long oldMousePosX;
	long oldMousePosY;

	long mouseDeltaX;
	long mouseDeltaY;



	XMFLOAT2 windowSize;
};


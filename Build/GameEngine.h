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
class CollisionManager;
class ShadowMap;
class SoundEngine;

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
	CollisionManager* GetCollisionManager(void);
	LightManager* GetLightmanager(void);
	ShadowMap* GetShadowMap(void);
	SoundEngine* GetSoundEngine(void);

	Scene* GetActiveScene(void);
	void SetActiveScene(Scene* scene);
	SceneManager* GetSceneManager(void);

	void SetFullScreen(BOOL flag);

	void ChengeWindowSize(int width, int height);

	void SetMainCamera(CameraComponent* camera);

	CameraComponent* GetMainCamera(void);

	HWND GetWindowHandle(void);

private:

	Main* main;
	AssetsManager* assetsManager;
	Renderer* renderer;
	CBufferManager* cBufferManager;
	Input* input;
	LightManager* lightManager;
	CollisionManager* collisionManager;
	CameraComponent* mainCamera;//バックバッファへの描画を行うカメラ
	ShadowMap* shadowMap;
	SoundEngine* soundEngine;


	SceneManager* sceneManager;
	Scene* activeScene;

	long oldMousePosX;
	long oldMousePosY;

	long mouseDeltaX;
	long mouseDeltaY;

	BOOL fullscreen;


	XMFLOAT2 windowSize;
};


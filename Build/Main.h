#pragma once
#include "Coreminimal.h"
#include "level.h"
#include "AssetsManager.h"
#include "Title.h"
#include "input.h"

#define SCREEN_WIDTH	(1920)				// �������E�B���h�E�̉���pixel
#define SCREEN_HEIGHT	(1080)				// �������E�B���h�E�̏c��pixel


class Renderer;
class AssetsManager;

class Main
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Main();
	~Main();

	HWND* GetWindowHangle(void);
	HINSTANCE* GetInstanceHandle(void);
	//void SetWindowType();
	int Run(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd);



	long GetMousePosX(void);
	long GetMousePosY(void);

	// �O�t���[������̌o�߃~���b���擾����
	float GetDeltaTime(void);

	enum SCENE
	{
		TITLE,
		STAGE1,
		RESULT,
	};
	void SetScene(SCENE scene);
	SCENE GetScene(void);

	AssetsManager* GetAssetsManager(void);
	Renderer* GetRenderer(void);
	Input* GetInput(void);

protected:
	void Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
private:
	HWND hWnd;
	HINSTANCE* hInstance;
	int fps;
	POINT mousePos;	//�}�E�X�̃L���b�V���ʒu

	float deltaTime;
	float beforeTime;

	SCENE scene;

	Title* title;
	


	Renderer* renderer;
	AssetsManager* assetsManager;
	Input* input;

};


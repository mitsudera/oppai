#include "Main.h"
#include "renderer.h"


#define CLASS_NAME		"AppClass"				// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"SkyFighter"				// �E�C���h�E�̃L���v�V������

long g_mouseX = 0;
long g_mouseY = 0;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Main* main = new Main;


	int result;
	result = main->Run(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	delete main;
	return result;
}

Main::Main()
{
}

Main::~Main()
{
}

HWND* Main::GetWindowHangle(void)
{
	return &this->hWnd;

}

HINSTANCE* Main::GetInstanceHandle(void)
{
	return this->hInstance;

}

int Main::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// �C���X�^���X�n���h���̃|�C���^�[��ێ�
	this->hInstance = &hInstance;

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;


	// �E�B���h�E�N���X�̏�����
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		Main::WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};


	MSG			msg;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW, //WS_POPUP�^�C�g���o�[����
		CW_USEDEFAULT,																		// �E�B���h�E�̍����W
		CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);


	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	Init();
	//if (FAILED(Init(hInstance, hWnd, mode)))
	//{
	//	return -1;
	//}

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;


	// �o�ߎ���
	this->deltaTime = 0;
	this->beforeTime = timeGetTime();

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000.0f)	// 1�b���ƂɎ��s
			{
				//g_CountFPS = dwFrameCount;
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000.0f / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

				this->deltaTime = timeGetTime() - this->beforeTime;
				this->beforeTime = timeGetTime();

				Update();			// �X�V����
				Draw();				// �`�揈��
				dwFrameCount++;
			}
		}
	}
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();
	return (int)msg.wParam;
}


long Main::GetMousePosX(void)
{
	return g_mouseX;
}

long Main::GetMousePosY(void)
{
	return g_mouseY;
}

float Main::GetDeltaTime(void)
{
	return this->deltaTime / 1000.0f;
}

void Main::SetScene(SCENE scene)
{
	this->title->Uninit();

	this->scene = scene;
	switch (scene)
	{
	case TITLE:
		this->title->Init();
		break;

	default:
		break;
	}

}

Main::SCENE Main::GetScene(void)
{
	return this->scene;
}

AssetsManager* Main::GetAssetsManager(void)
{
	return this->assetsManager;
}

Renderer* Main::GetRenderer(void)
{
	return this->renderer;
}

Input* Main::GetInput(void)
{
	return this->input;
}

void Main::Init(void)
{
	//�}�E�X�ʒu�̏�����
	GetCursorPos(&this->mousePos);

	srand((unsigned)time(NULL));


	//Renderer����
	this->renderer = new Renderer(this);

	this->renderer->InitRenderer(*GetInstanceHandle(), *GetWindowHangle(), true);

	this->assetsManager = new AssetsManager(this);

	this->input = new Input(this);

	this->input->Init(*GetInstanceHandle(), *GetWindowHangle());

	//�e���x���̐���

	this->title = new Title(this);

	SetScene(SCENE::TITLE);
	

}

void Main::Update(void)
{
	this->input->Update();
	switch (this->scene)
	{
	case SCENE::TITLE:
		this->title->Update();
		break;
	case SCENE::STAGE1:
		break;
	case SCENE::RESULT:
		break;
	default:
		break;
	}
}

void Main::Draw(void)
{
	renderer->Clear();


	switch (this->scene)
	{
	case SCENE::TITLE:
		this->title->Draw();
		break;
	case SCENE::STAGE1:
		break;
	case SCENE::RESULT:
		break;
	default:
		break;
	}

	renderer->Present();

}

void Main::Uninit(void)
{
	this->input->Uninit();
	this->title->Uninit();
	this->assetsManager->Uninit();
	this->renderer->UninitRenderer();


	delete input;
	delete assetsManager;
	delete title;
	delete renderer;

}

LRESULT Main::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


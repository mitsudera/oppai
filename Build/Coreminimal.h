#pragma once

// WinAPI����
#pragma warning(push)
#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#include "windows.h"

// DirectX11 ����
#pragma warning(disable:4005)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

// C++ ���C�u����
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <random>
#include <cmath> 
#include <algorithm>
#include <fstream>
#include <windows.h>
#include <stdexcept>


//FBXSDK���C�u����
#include <fbxsdk.h>

#pragma warning(pop)
//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#pragma comment (lib,"libfbxsdk-mt.lib")
#pragma comment (lib,"libxml2-mt.lib")
#pragma comment (lib,"zlib-mt.lib")



// �{���̓w�b�_�ɏ����Ȃ������ǂ�
using namespace DirectX;
using namespace std;

// �v���W�F�N�g�Ǝ�
#include "Math.h"


#pragma once

// WinAPI導入
#pragma warning(push)
#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include "windows.h"

// DirectX11 導入
#pragma warning(disable:4005)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"


// C++ ライブラリ
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <map>
#include <random>
#include <cmath> 
#include <algorithm>
#include <fstream>
#include <windows.h>
#include <stdexcept>
#include <filesystem>

//FBXSDKライブラリ
#include <fbxsdk.h>

#pragma warning(pop)
//*****************************************************************************
// ライブラリのリンク
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



// 本来はヘッダに書かない方が良い
using namespace DirectX;
using namespace std;

// プロジェクト独自
#include "Math.h"


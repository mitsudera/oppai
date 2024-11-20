//=============================================================================
//
// ���͏��� [input.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************





Input::Input()
{
}

Input::~Input()
{
}

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT Input::Init(HINSTANCE hInst, HWND hWnd)
{
	pDInput = NULL;
	pDIDevKeyboard = NULL;
	pMouse = NULL;
	for (int i = 0; i < GAMEPADMAX; i++)
	{
		pGamePad[i] = NULL;

	}
	padCount = 0;

	HRESULT hr;

	if(!pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

 	// �}�E�X�̏�����
	InitializeMouse(hInst, hWnd);
	
	// �p�b�h�̏�����
	InitializePad();

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void Input:: Uninit(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMouse();

	// �p�b�h�̏I������
	UninitPad();

	if(pDInput)
	{
		pDInput->Release();
		pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void Input::Update(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();
	
	// �}�E�X�̍X�V
	UpdateMouse();
	
	// �p�b�h�̍X�V
	UpdatePad();

}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT Input::InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIDevKeyboard, NULL);
	if(FAILED(hr) || pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void Input:: UninitKeyboard(void)
{
	if(pDIDevKeyboard)
	{
		pDIDevKeyboard->Release();
		pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V
//=============================================================================
HRESULT Input::UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = pDIDevKeyboard->GetDeviceState(sizeof(keyState), keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			keyStateTrigger[cnt] = (keyStateOld[cnt] ^ keyState[cnt]) & keyState[cnt];
			keyStateRelease[cnt] = (keyStateOld[cnt] ^ keyState[cnt]) & ~keyState[cnt];
			keyStateRepeat[cnt] = keyStateTrigger[cnt];

			if(keyState[cnt])
			{
				keyStateRepeatCnt[cnt]++;
				if(keyStateRepeatCnt[cnt] >= 20)
				{
					keyStateRepeat[cnt] = keyState[cnt];
				}
			}
			else
			{
				keyStateRepeatCnt[cnt] = 0;
				keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool Input::GetKeyboardPress(int key)
{
	return (keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool Input::GetKeyboardTrigger(int key)
{
	return (keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool Input::GetKeyboardRepeat(int key)
{
	return (keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool Input::GetKeyboardRelease(int key)
{
	return (keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// �}�E�X�֌W�̏���
//=============================================================================
// �}�E�X�̏�����
HRESULT Input::InitializeMouse(HINSTANCE hInst,HWND hWindow)
{
	HRESULT result;
	// �f�o�C�X�쐬
	result = pDInput->CreateDevice(GUID_SysMouse,&pMouse,NULL);
	if(FAILED(result) || pMouse==NULL)
	{
		MessageBox(hWindow,"No mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Can't setup mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse mode error","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	
	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;
	
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = pMouse->SetProperty(DIPROP_AXISMODE,&prop.diph);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse property error","Warning",MB_OK | MB_ICONWARNING);
		return result;	
	}
	
	// �A�N�Z�X���𓾂�
	pMouse->Acquire();
	return result;
}
//---------------------------------------------------------
void Input::UninitMouse()
{
	if(pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT Input::UpdateMouse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = mouseState;
	// �f�[�^�擾
	result = pMouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// �}�E�X�̃{�^�����
		for(int i=0;i<8;i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = pMouse->Acquire();
	}
	return result;
	
}

//----------------------------------------------
BOOL Input::IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80) ? true : false;	// �����ꂽ�Ƃ��ɗ��r�b�g������
}
BOOL Input::IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80) ? true : false;
}
BOOL Input::IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80) ? true : false;
}
BOOL Input::IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80) ? true : false;
}
BOOL Input::IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80) ? true : false;
}
BOOL Input::IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80) ? true : false;
}
//------------------
long Input::GetMouseX(void)
{
	return mouseState.lX;
}
long Input::GetMouseY(void)
{
	return mouseState.lY;
}
long Input::GetMouseZ(void)
{
	return mouseState.lZ;
}
//================================================= game pad
//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID )
{

	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}
//---------------------------------------- ������
HRESULT Input::InitializePad(void)			// �p�b�h������
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// �W���C�p�b�h��T��
	pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchPadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for ( i=0 ; i<padCount ; i++ ) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
			return FALSE; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.lMin = RANGE_MINRight;
		diprg.lMax = RANGE_MAXRight;

		// Z���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_Z;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Zr���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_RZ;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Xr���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_RX;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Yr���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_RY;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		dipdw.dwData = DEADZONER;

		//Z���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_Z;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//RZ���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_RZ;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//XR���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_RX;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//YR���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_RY;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//�W���C�X�e�B�b�N���͐���J�n
		pGamePad[i]->Acquire();
	}
		
	return S_OK;

}
//------------------------------------------- �I������
void Input::UninitPad(void)
{
	for (int i=0 ; i<GAMEPADMAX ; i++) {
		if ( pGamePad[i] )
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ �X�V
void Input::UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// ������

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* �\���L�[��
		if (dijs.rgdwPOV[0] == 0 || dijs.rgdwPOV[0] == 4500 || dijs.rgdwPOV[0] == 31500)						padState[i] |= BUTTON_UP;
		//* �\���L�[�E
		if (dijs.rgdwPOV[0] == 9000 || dijs.rgdwPOV[0] == 4500 || dijs.rgdwPOV[0] == 13500)						padState[i] |= BUTTON_RIGHT;
		//* �\���L�[��
		if (dijs.rgdwPOV[0] == 18000 || dijs.rgdwPOV[0] == 13500 || dijs.rgdwPOV[0] == 22500)					padState[i] |= BUTTON_DOWN;
		//* �\���L�[��
		if (dijs.rgdwPOV[0] == 27000 || dijs.rgdwPOV[0] == 22500 || dijs.rgdwPOV[0] == 31500)					padState[i] |= BUTTON_LEFT;
		//* �`�{�^��
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* �a�{�^��
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* �b�{�^��
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* �w�{�^��
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		//* �x�{�^��
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		//* �y�{�^��
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* �k�{�^��
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* �q�{�^��
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		//* �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		//* �l�{�^��
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;
		//* 
		if (dijs.rgbButtons[10] & 0x80)	padState[i] |= BUTTON_L3;
		//* �l�{�^��
		if (dijs.rgbButtons[11] & 0x80)	padState[i] |= BUTTON_R3;

		if (dijs.rgbButtons[12] & 0x80)	padState[i] |= BUTTON_PS;

		if (dijs.rgbButtons[13] & 0x80)	padState[i] |= BUTTON_PAD;

		LeftStickX[i] = dijs.lX;
		LeftStickY[i] = dijs.lY;
		RightStickX[i] = dijs.lZ;
		RightStickY[i] = dijs.lRz;

		VALUER2[i] = dijs.lRx;
		VALUEL2[i] = dijs.lRy;

		// Trigger�ݒ�
		padTrigger[i] = ((lastPadState ^ padState[i])	// �O��ƈ���Ă���
			& padState[i]);					// ��������ON�̂��

	}

}
//----------------------------------------------- ����
BOOL Input::IsButtonPressed(int padNo,DWORD button)
{
	return (button & padState[padNo]);
}

BOOL Input::IsButtonTriggered(int padNo,DWORD button)
{
	return (button & padTrigger[padNo]);
}



LONG Input::GetLeftStickX(int padNo)
{
	return LeftStickX[padNo];
}
LONG Input::GetLeftStickY(int padNo)
{
	return LeftStickY[padNo];
}
LONG Input::GetRightStickX(int padNo)
{
	return RightStickX[padNo];
}
LONG Input::GetRightStickY(int padNo)
{
	return RightStickY[padNo];
}
LONG Input::GetVALUER2(int padNo)
{
	return VALUER2[padNo];
}
LONG Input::GetVALUEL2(int padNo)
{
	return VALUEL2[padNo];
}
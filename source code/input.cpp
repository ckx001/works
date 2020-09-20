//=============================================================================
//
// 入力処理[input.cpp]
// Author :  
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

// game pad梡愝掕抣
#define DEADZONE		2500			// 奺幉偺25%傪柍岠僝乕儞偲偡傞
#define RANGE_MAX		1000			// 桳岠斖埻偺嵟戝抣
#define RANGE_MIN		-1000			// 桳岠斖埻偺嵟彫抣


//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // 儅僂僗偺弶婜壔
void UninitMouse();						// 儅僂僗偺廔椆張棟
HRESULT UpdateMouse();					// 儅僂僗偺峏怴張棟

HRESULT InitializePad(void);			// 僷僢僪弶婜壔
//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// 僷僢僪専嵏僐乕儖僶僢僋
void UpdatePad(void);
void UninitPad(void);

//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8僀儞僞乕僼僃乕僗傊偺億僀儞僞
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8僀儞僞乕僼僃乕僗傊偺億僀儞僞(僉乕儃乕僪)
BYTE					g_keyState[NUM_KEY_MAX];			// 僉乕儃乕僪偺忬懺傪庴偗庢傞儚乕僋
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// 僉乕儃乕僪偺忬懺傪庴偗庢傞儚乕僋
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// 僉乕儃乕僪偺忬懺傪庴偗庢傞儚乕僋
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// 僉乕儃乕僪偺忬懺傪庴偗庢傞儚乕僋
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// 僉乕儃乕僪偺儕僺乕僩僇僂儞僞

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 pMouse = NULL; // mouse

static DIMOUSESTATE2   mouseState;		// 儅僂僗偺僟僀儗僋僩側忬懺
static DIMOUSESTATE2   mouseTrigger;	// 墴偝傟偨弖娫偩偗ON

//--------------------------------- game pad

static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = {NULL,NULL,NULL,NULL};// 僷僢僪僨僶僀僗

static DWORD	padState[GAMEPADMAX];	// 僷僢僪忣曬乮暋悢懳墳乯
static DWORD	padTrigger[GAMEPADMAX];
static DWORD    padRelease[GAMEPADMAX];
static int		padCount = 0;			// 専弌偟偨僷僢僪偺悢

float DijsIxyRotAngel; //手柄右摇杆控制旋转角度
//=============================================================================
// 擖椡張棟偺弶婜壔
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInput僆僽僕僃僋僩偺嶌惉
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// 僉乕儃乕僪偺弶婜壔
	InitKeyboard(hInst, hWnd);

 	// 儅僂僗偺弶婜壔
	InitializeMouse(hInst, hWnd);
	
	// 僷僢僪偺弶婜壔
	InitializePad();

	return S_OK;
}

//=============================================================================
// 擖椡張棟偺廔椆張棟
//=============================================================================
void UninitInput(void)
{
	// 僉乕儃乕僪偺廔椆張棟
	UninitKeyboard();

	// 儅僂僗偺廔椆張棟
	UninitMouse();

	// 僷僢僪偺廔椆張棟
	UninitPad();

	if(g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// 擖椡張棟偺峏怴張棟
//=============================================================================
void UpdateInput(void)
{
	// 僉乕儃乕僪偺峏怴
	UpdateKeyboard();
	
	// 儅僂僗偺峏怴
	UpdateMouse();
	
	// 僷僢僪偺峏怴
	UpdatePad();

}

//=============================================================================
// 僉乕儃乕僪偺弶婜壔
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 僨僶僀僗僆僽僕僃僋僩傪嶌惉
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "僉乕儃乕僪偑偹偉両", "寈崘両", MB_ICONWARNING);
		return hr;
	}

	// 僨乕僞僼僅乕儅僢僩傪愝掕
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "僉乕儃乕僪偺僨乕僞僼僅乕儅僢僩傪愝掕偱偒傑偣傫偱偟偨丅", "寈崘両", MB_ICONWARNING);
		return hr;
	}

	// 嫤挷儌乕僪傪愝掕乮僼僅傾僌儔僂儞僪仌旕攔懠儌乕僪乯
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "僉乕儃乕僪偺嫤挷儌乕僪傪愝掕偱偒傑偣傫偱偟偨丅", "寈崘両", MB_ICONWARNING);
		return hr;
	}

	// 僉乕儃乕僪傊偺傾僋僙僗尃傪妉摼(擖椡惂屼奐巒)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// 僉乕儃乕僪偺廔椆張棟
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// 僉乕儃乕僪偺峏怴
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 慜夞偺僨乕僞傪曐懚
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// 僨僶僀僗偐傜僨乕僞傪庢摼
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if(g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if(g_keyStateRepeatCnt[cnt] >= 20)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// 僉乕儃乕僪傊偺傾僋僙僗尃傪庢摼
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// 僉乕儃乕僪偺僾儗僗忬懺傪庢摼
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// 僉乕儃乕僪偺僩儕僈乕忬懺傪庢摼
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// 僉乕儃乕僪偺儕僺乕僩忬懺傪庢摼
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// 僉乕儃乕僪偺儕儕亅僗忬懺傪庢摼
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// 儅僂僗娭學偺張棟
//=============================================================================
// 儅僂僗偺弶婜壔
HRESULT InitializeMouse(HINSTANCE hInst,HWND hWindow)
{
	HRESULT result;
	// 僨僶僀僗嶌惉
	result = g_pDInput->CreateDevice(GUID_SysMouse,&pMouse,NULL);
	if(FAILED(result) || pMouse==NULL)
	{
		MessageBox(hWindow,"No mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// 僨乕僞僼僅乕儅僢僩愝掕
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Can't setup mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// 懠偺傾僾儕偲嫤挷儌乕僪偵愝掕
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse mode error","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	
	// 僨僶僀僗偺愝掕
	DIPROPDWORD prop;
	
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// 儅僂僗偺堏摦抣丂憡懳抣

	result = pMouse->SetProperty(DIPROP_AXISMODE,&prop.diph);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse property error","Warning",MB_OK | MB_ICONWARNING);
		return result;	
	}
	
	// 傾僋僙僗尃傪摼傞
	pMouse->Acquire();
	return result;
}
//---------------------------------------------------------
void UninitMouse()
{
	if(pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT UpdateMouse()
{
	HRESULT result;
	// 慜夞偺抣曐懚
	DIMOUSESTATE2 lastMouseState = mouseState;
	// 僨乕僞庢摼
	result = pMouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// 儅僂僗偺儃僞儞忬懺
		for(int i=0;i<8;i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// 庢摼幐攕
	{
		// 傾僋僙僗尃傪摼偰傒傞
		result = pMouse->Acquire();
	}
	return result;
	
}

//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// 墴偝傟偨偲偒偵棫偮價僢僩傪専嵏
}
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
}
//------------------
long GetMouseX(void)
{
	return mouseState.lX;
}
long GetMouseY(void)
{
	return mouseState.lY;
}
long GetMouseZ(void)
{
	return mouseState.lZ;
}
//================================================= game pad
//---------------------------------------- 僐乕儖僶僢僋娭悢
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID )
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// 師偺僨僶僀僗傪楍嫇

}
//---------------------------------------- 弶婜壔
HRESULT InitializePad(void)			// 僷僢僪弶婜壔
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// 僕儑僀僷僢僪傪扵偡
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// 僙僢僩偟偨僐乕儖僶僢僋娭悢偑丄僷僢僪傪敪尒偟偨悢偩偗屇偽傟傞丅

	for ( i=0 ; i<padCount ; i++ ) {
		// 僕儑僀僗僥傿僢僋梡偺僨乕僞丒僼僅乕儅僢僩傪愝掕
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
			return false; // 僨乕僞僼僅乕儅僢僩偺愝掕偵幐攕

		// 儌乕僪傪愝掕乮僼僅傾僌儔僂儞僪仌旕攔懠儌乕僪乯
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // 儌乕僪偺愝掕偵幐攕

		// 幉偺抣偺斖埻傪愝掕
		// X幉丄Y幉偺偦傟偧傟偵偮偄偰丄僆僽僕僃僋僩偑曬崘壜擻側抣偺斖埻傪僙僢僩偡傞丅
		// (max-min)偼丄嵟戝10,000(?)丅(max-min)/2偑拞墰抣偵側傞丅
		// 嵎傪戝偒偔偡傟偽丄傾僫儘僌抣偺嵶偐側摦偒傪曔傜偊傜傟傞丅(僷僢僪偺惈擻偵傛傞)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X幉偺斖埻傪愝掕
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y幉偺斖埻傪愝掕
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 奺幉偛偲偵丄柍岠偺僝乕儞抣傪愝掕偡傞丅
		// 柍岠僝乕儞偲偼丄拞墰偐傜偺旝彮側僕儑僀僗僥傿僢僋偺摦偒傪柍帇偡傞斖埻偺偙偲丅
		// 巜掕偡傞抣偼丄10000偵懳偡傞憡懳抣(2000側傜20僷乕僙儞僩)丅
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X幉偺柍岠僝乕儞傪愝掕
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y幉偺柍岠僝乕儞傪愝掕
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//僕儑僀僗僥傿僢僋擖椡惂屼奐巒
		pGamePad[i]->Acquire();
	}
		
	return true;

}
//------------------------------------------- 廔椆張棟
void UninitPad(void)
{
	for (int i=0 ; i<GAMEPADMAX ; i++) {
		if ( pGamePad[i] )
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ 峏怴
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for ( i=0 ; i<padCount ; i++ ) 
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// 弶婜壔

		result = pGamePad[i]->Poll();	// 僕儑僀僗僥傿僢僋偵億乕儖傪偐偗傞
		if ( FAILED(result) ) {
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// 僨僶僀僗忬懺傪撉傒庢傞
		if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED ) {
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}
		
		/////////十字键///////////////////
		//27000  9000  18000  0
		//左     右    下    上
		if (dijs.rgdwPOV[0] == 27000)           padState[i] |= BUTTON_CROSS_LEFT;
		if (dijs.rgdwPOV[0] == 9000)           padState[i] |= BUTTON_CROSS_RIGHT;
		/////////NS右摇杆/////////////////
		DijsIxyRotAngel = atan2(dijs.lRy, dijs.lRx) + D3DX_PI / 2;
		if (dijs.lRy < 20000)					padState[i] |= BUTTON_ROT_UP;
		//* x-axis (right)
		if (dijs.lRy > 45000)					padState[i] |= BUTTON_ROT_DOWN;
		if (dijs.lRx < 20000)					padState[i] |= BUTTON_ROT_LEFT;
		//* x-axis (right)
		if (dijs.lRx > 45000)					padState[i] |= BUTTON_ROT_RIGHT;
		/////////////////////////////////
		// 俁俀偺奺價僢僩偵堄枴傪帩偨偣丄儃僞儞墴壓偵墳偠偰價僢僩傪僆儞偵偡傞
		//* y-axis (forward)
		if ( dijs.lY < 0 )					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if ( dijs.lY > 0 )					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if ( dijs.lX < 0 )					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if ( dijs.lX > 0 )					padState[i] |= BUTTON_RIGHT;
		//* 俙儃僞儞
		if ( dijs.rgbButtons[0] & 0x80 )	padState[i] |= BUTTON_A;
		//* 俛儃僞儞
		if ( dijs.rgbButtons[1] & 0x80 )	padState[i] |= BUTTON_B;
		//* 俠儃僞儞
		if ( dijs.rgbButtons[2] & 0x80 )	padState[i] |= BUTTON_C;
		//* 倃儃僞儞
		if ( dijs.rgbButtons[3] & 0x80 )	padState[i] |= BUTTON_X;
		//* 倄儃僞儞
		if ( dijs.rgbButtons[4] & 0x80 )	padState[i] |= BUTTON_Y;
		//* 倅儃僞儞
		if ( dijs.rgbButtons[5] & 0x80 )	padState[i] |= BUTTON_Z;
		//* 俴儃僞儞
		if ( dijs.rgbButtons[6] & 0x80 )	padState[i] |= BUTTON_L;
		//* 俼儃僞儞
		if ( dijs.rgbButtons[7] & 0x80 )	padState[i] |= BUTTON_R;
		//* 俽俿俙俼俿儃僞儞
		if ( dijs.rgbButtons[8] & 0x80 )	padState[i] |= BUTTON_START;
		//* 俵儃僞儞
		if ( dijs.rgbButtons[9] & 0x80 )	padState[i] |= BUTTON_M;

		// Trigger愝掕
		padTrigger[i] = ((lastPadState ^ padState[i])	// 慜夞偲堘偭偰偄偰
						& padState[i]);					// 偟偐傕崱ON偺傗偮
				// Release設定
		padRelease[i] = ((lastPadState ^ padState[i])	// 前回と違っていて
			& ~padState[i]);
	}

}
//----------------------------------------------- 専嵏
BOOL IsButtonPressed(int padNo,DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo,DWORD button)
{
	return (button & padTrigger[padNo]);
}

BOOL IsButtonRelease(int padNo, DWORD button)
{
	return (button & padRelease[padNo]);
}



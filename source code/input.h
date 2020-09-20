//=============================================================================
//
// 入力処理 [input.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************

// 僾儘僌儔儉暘偗偡傞偲偒偵巊偆
#define	USE_KEYBOARD										// 愰尵偡傞偲僉乕儃乕僪偱憖嶌壜擻偵側傞
#define	USE_MOUSE											// 愰尵偡傞偲儅僂僗偱憖嶌壜擻偵側傞
#define	USE_PAD												// 愰尵偡傞偲僷僢僪偱憖嶌壜擻偵側傞


/* game pad忣曬 */
#define BUTTON_UP		0x00000001l	// 曽岦僉乕忋(.IY<0)
#define BUTTON_DOWN		0x00000002l	// 曽岦僉乕壓(.IY>0)
#define BUTTON_LEFT		0x00000004l	// 曽岦僉乕嵍(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 曽岦僉乕塃(.IX>0)
#define BUTTON_ROT_UP		0x00100000l	// 方向キー上(.IY<0)
#define BUTTON_ROT_DOWN		0x00200000l	// 方向キー下(.IY>0)
#define BUTTON_ROT_LEFT		0x00008000l	// 方向キー左(.IX<0)
#define BUTTON_ROT_RIGHT	0x00010000l	// 方向キー右(.IX>0)
//#define BUTTON_CROSS_UP		0x00010000l	// 十字キー上
//#define BUTTON_CROSS_DOWN	0x00010000l	// 十字キー下
#define BUTTON_CROSS_LEFT	0x00004000l	// 十字キー左
#define BUTTON_CROSS_RIGHT	0x00002000l	// 十字キー右
#define BUTTON_A		0x00000010l	// 俙儃僞儞(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// 俛儃僞儞(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// 俠儃僞儞(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// 倃儃僞儞(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// 倄儃僞儞(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// 倅儃僞儞(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// 俴儃僞儞(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// 俼儃僞儞(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// 俽俿俙俼俿儃僞儞(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// 俵儃僞儞(.rgbButtons[9]&0x80)
#define GAMEPADMAX		4			// 摨帪偵愙懕偡傞僕儑僀僷僢僪偺嵟戝悢傪僙僢僩


//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // 嵍僋儕僢僋偟偨忬懺
BOOL IsMouseLeftTriggered(void);    // 嵍僋儕僢僋偟偨弖娫
BOOL IsMouseRightPressed(void);     // 塃僋儕僢僋偟偨忬懺
BOOL IsMouseRightTriggered(void);   // 塃僋儕僢僋偟偨弖娫
BOOL IsMouseCenterPressed(void);    // 拞僋儕僢僋偟偨忬懺
BOOL IsMouseCenterTriggered(void);  // 拞僋儕僢僋偟偨弖娫
long GetMouseX(void);               // 儅僂僗偑X曽岦偵摦偄偨憡懳抣
long GetMouseY(void);               // 儅僂僗偑Y曽岦偵摦偄偨憡懳抣
long GetMouseZ(void);               // 儅僂僗儂僀乕儖偑摦偄偨憡懳抣

//---------------------------- game pad
BOOL IsButtonPressed(int padNo,DWORD button);
BOOL IsButtonTriggered(int padNo,DWORD button);
BOOL IsButtonRelease(int padNo, DWORD button);



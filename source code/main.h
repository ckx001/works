//=============================================================================
//
// MAIN [main.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

//*****************************************************************************
// 僀儞僋儖乕僪僼傽僀儖
//*****************************************************************************
#include <windows.h>
#include <d3dx9.h>
#include <windowsx.h>
#define DIRECTINPUT_VERSION (0x0800)	// 寈崘懳嶔
#include <dinput.h>

//*****************************************************************************
// 儔僀僽儔儕偺儕儞僋
//*****************************************************************************
#if 1	// [偙偙傪"0"偵偟偨応崌丄"峔惉僾儘僷僥傿" -> "儕儞僇" -> "擖椡" -> "捛壛偺埶懚僼傽僀儖"偵懳徾儔僀僽儔儕傪愝掕偡傞]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "DxErr.lib")
#endif
#include <DxErr.h>
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
// 俀俢億儕僑儞捀揰僼僅乕儅僢僩( 捀揰嵗昗[2D] / 斀幩岝 / 僥僋僗僠儍嵗昗 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_2D_DIFFUSE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
// 俁俢億儕僑儞捀揰僼僅乕儅僢僩( 捀揰嵗昗[3D] / 朄慄 / 斀幩岝 / 僥僋僗僠儍嵗昗 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_3D_DIFFUSE	( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define	FVF_VERTEX_LINE	(D3DFVF_XYZ | D3DFVF_DIFFUSE)

#define SCREEN_WIDTH	(1280)				// 僂僀儞僪僂偺暆
#define SCREEN_HEIGHT	(768)				// 僂僀儞僪僂偺崅偝
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// 僂僀儞僪僂偺拞怱倃嵗昗
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// 僂僀儞僪僂偺拞怱倄嵗昗

#define	NUM_VERTEX		(4)		// 捀揰悢
#define	NUM_POLYGON		(2)		// 億儕僑儞悢

#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }       //自定义一个SAFE_RELEASE()宏,便于指针资源的释放
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }     //自定义一个SAFE_RELEASE()宏,便于COM资源的释放
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    

enum DEVICE_STATUS
{
	SYS_DEVICERUN,
	SYS_DEVICELOST
};

// 忋婰俀俢億儕僑儞捀揰僼僅乕儅僢僩偵崌傢偣偨峔憿懱傪掕媊
typedef struct
{
	D3DXVECTOR3 vtx;		// 捀揰嵗昗
	float		rhw;		// 僥僋僗僠儍偺僷乕僗儁僋僥傿僽僐儗僋僩梡
	D3DCOLOR	diffuse;	// 斀幩岝
	D3DXVECTOR2 tex;		// 僥僋僗僠儍嵗昗
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 vtx;		// 捀揰嵗昗
	float		rhw;		// 僥僋僗僠儍偺僷乕僗儁僋僥傿僽僐儗僋僩梡
	D3DCOLOR	diffuse;	// 斀幩岝
} VERTEX_2D_DIFFUSE;

// 忋婰俁俢億儕僑儞捀揰僼僅乕儅僢僩偵崌傢偣偨峔憿懱傪掕媊
typedef struct
{
	D3DXVECTOR3 vtx;		// 捀揰嵗昗
	D3DXVECTOR3 nor;		// 朄慄儀僋僩儖
	D3DCOLOR	diffuse;	// 斀幩岝
	D3DXVECTOR2 tex;		// 僥僋僗僠儍嵗昗
} VERTEX_3D;

typedef struct
{
	D3DXVECTOR3 vtx;		// 捀揰嵗昗
	D3DCOLOR	diffuse;	// 斀幩岝
} VERTEX_3D_DIFFUSE;

typedef struct
{
	D3DXVECTOR3 vtx;		// 捀揰嵗昗
	D3DCOLOR	diffuse;	// 斀幩岝
} VERTEX_LINE;
//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
int RetCurrentTime(void);
int RetCurrentDeviceStatus(void);
void ExitGame(void);
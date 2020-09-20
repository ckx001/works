//=============================================================================
//
// デバッグメッセージ[debugproc.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "main.h"
#include "debugproc.h"

//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************

//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************

//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
LPD3DXFONT	g_pD3DXFont = NULL;			// 僼僅儞僩傊偺億僀儞僞
char		g_aStrDebug[1024] = {"\0"};	// 僨僶僢僌忣曬


//=============================================================================
// 僨僶僢僌昞帵張棟偺弶婜壔
//=============================================================================
HRESULT InitDebugProc(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// 忣曬昞帵梡僼僅儞僩傪愝掕
	hr = D3DXCreateFont(pDevice, 24, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, ("Arial"), &g_pD3DXFont);

	// 忣曬僋儕傾
	memset(g_aStrDebug, 0, sizeof g_aStrDebug);

	return hr;
}

//=============================================================================
// 僨僶僢僌昞帵張棟偺廔椆張棟
//=============================================================================
void UninitDebugProc(void)
{
	if(g_pD3DXFont != NULL)
	{// 忣曬昞帵梡僼僅儞僩偺奐曻
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
}

//=============================================================================
// 僨僶僢僌昞帵張棟偺峏怴張棟
//=============================================================================
void UpdateDebugProc(void)
{

}

//=============================================================================
// 僨僶僢僌昞帵張棟偺昤夋張棟
//=============================================================================
void DrawDebugProc(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	// 忣曬昞帵
	g_pD3DXFont->DrawText(NULL, g_aStrDebug, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// 忣曬僋儕傾
	memset(g_aStrDebug, 0, sizeof g_aStrDebug);

	pDevice->SetRenderState(D3DRS_ZENABLE, false);
}

//=============================================================================
// 僨僶僢僌昞帵偺搊榐
//=============================================================================
void PrintDebugProc(char *fmt,...)
{
#if 0
	long *pParam;
	static char aBuf[256];

	pParam = (long*)&fmt;
	sprintf(aBuf, fmt, pParam[1], pParam[2], pParam[3], pParam[4],
									pParam[5], pParam[6], pParam[7], pParam[8],
									pParam[9], pParam[10], pParam[11], pParam[12]);
#else
	va_list list;			// 壜曄堷悢傪張棟偡傞堊偵巊梡偡傞曄悢
	char *pCur;
	char aBuf[256]={"\0"};
	char aWk[32];

	// 壜曄堷悢偵傾僋僙僗偡傞慜偺弶婜張棟
	va_start(list, fmt);

	pCur = fmt;
	for( ; *pCur; ++pCur)
	{
		if(*pCur != '%')
		{
			sprintf(aWk, "%c", *pCur);
		}
		else
		{
			pCur++;

			switch(*pCur)
			{
			case 'd':
				// 壜曄堷悢偵傾僋僙僗偟偰偦偺曄悢傪庢傝弌偡張棟
				sprintf(aWk, "%d", va_arg(list, int));
				break;

			case 'f':
				// 壜曄堷悢偵傾僋僙僗偟偰偦偺曄悢傪庢傝弌偡張棟
				sprintf(aWk, "%.2f", va_arg(list, double));		// double宆偱巜掕
				break;

			case 's':
				// 壜曄堷悢偵傾僋僙僗偟偰偦偺曄悢傪庢傝弌偡張棟
				sprintf(aWk, "%s", va_arg(list, char*));
				break;

			case 'c':
				// 壜曄堷悢偵傾僋僙僗偟偰偦偺曄悢傪庢傝弌偡張棟
				sprintf(aWk, "%c", va_arg(list, char));
				break;

			default:
				sprintf(aWk, "%c", *pCur);
				break;
			}
		}
		strcat(aBuf, aWk);
	}

	// 壜曄堷悢偵傾僋僙僗偟偨屻偺廔椆張棟
	va_end(list);

	// 楢寢
	if((strlen(g_aStrDebug) + strlen(aBuf)) < sizeof g_aStrDebug - 1)
	{
		strcat(g_aStrDebug, aBuf);
	}
#endif
}


//=============================================================================
//
// ライト処理 [light.cpp]
// Author :  GP-12B-191 22 程凱旋 
//
//=============================================================================
#include "main.h"
#include "light_Ex.h"

//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	NUM_LIGHT		(3)												// 儔僀僩偺悢

//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
static D3DLIGHT9				g_aLight[NUM_LIGHT];		// 儔僀僩忣曬

//=============================================================================
// 弶婜壔張棟
//=============================================================================
void InitLight_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9峔憿懱傪0偱僋儕傾偡傞
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// 儔僀僩0偺僞僀僾偺愝掕
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// 儔僀僩0偺奼嶶岝偺愝掕
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 儔僀僩0偺嬀柺斀幩岝偺愝掕
	//	g_aLight[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 儔僀僩0偺娐嫬岝偺愝掕
	g_aLight[0].Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

	// 儔僀僩0偺曽岦偺愝掕
	//vecDir = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vecDir = D3DXVECTOR3(-0.707f, -0.707f, -0.707f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// 儔僀僩0傪儗儞僟儕儞僌僷僀僾儔僀儞偵愝掕
	pDevice->SetLight(0, &g_aLight[0]);

	// 儔僀僩0傪巊梡巊梡忬懺偵
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9峔憿懱傪0偱僋儕傾偡傞
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// 儔僀僩1偺僞僀僾偺愝掕
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// 儔僀僩1偺奼嶶岝偺愝掕
	g_aLight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// 儔僀僩0偺嬀柺斀幩岝偺愝掕
	//	g_aLight[1].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 儔僀僩1偺娐嫬岝偺愝掕
	g_aLight[1].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 儔僀僩1偺曽岦偺愝掕
	//vecDir = D3DXVECTOR3(-0.0f, -1.0f, 0.8f);
	vecDir = D3DXVECTOR3(0.707f, -0.707f, 0.707f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// 儔僀僩1傪儗儞僟儕儞僌僷僀僾儔僀儞偵愝掕
	pDevice->SetLight(1, &g_aLight[1]);

	// 儔僀僩1偺傪巊梡巊梡忬懺偵
	pDevice->LightEnable(1, TRUE);

	// 儔僀僥傿儞僌儌乕僪傪ON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

D3DLIGHT9 *RetLight()
{
	return &g_aLight[0];
}

D3DVECTOR *RetLightDirection()
{
	return &g_aLight[1].Direction;
}
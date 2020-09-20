//=============================================================================
//
// エフェクト処理[effect.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "effect.h"
#include "input.h"
#include "GlobalFunction.h"
#include "camera_Ex.h"

//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	TEXTURE_EFFECT			"data/TEXTURE/effect/effect000.jpg"	// 撉傒崬傓僥僋僗僠儍僼傽僀儖柤
#define	EFFECT_SIZE_X			(50.0f)							// 價儖儃乕僪偺暆
#define	EFFECT_SIZE_Y			(50.0f)							// 價儖儃乕僪偺崅偝
#define	VALUE_MOVE_BULLET		(2.0f)							// 堏摦懍搙

#define	MAX_EFFECT				(4096)							// 僄僼僃僋僩嵟戝悢

//*****************************************************************************
// 峔憿懱掕媊
//*****************************************************************************

//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY);
void SetColorEffect(int nIdxEffect, D3DXCOLOR col);

//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;		// 僥僋僗僠儍傊偺億僀儞僞
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;		// 捀揰僶僢僼傽僀儞僞乕僼僃乕僗傊偺億僀儞僞

D3DXMATRIX				g_mtxWorldEffect;				// 儚乕儖僪儅僩儕僢僋僗

EFFECT					g_aEffect[MAX_EFFECT];			// 抏儚乕僋

//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 捀揰忣曬偺嶌惉
	MakeVertexEffect(pDevice);

	// 僥僋僗僠儍偺撉傒崬傒
	D3DXCreateTextureFromFile(pDevice,					// 僨僶僀僗傊偺億僀儞僞
								TEXTURE_EFFECT,			// 僼傽僀儖偺柤慜
								&g_pD3DTextureEffect);	// 撉傒崬傓儊儌儕乕

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fSizeX = EFFECT_SIZE_X;
		g_aEffect[nCntEffect].fSizeY = EFFECT_SIZE_Y;
		g_aEffect[nCntEffect].nTimer = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitEffect(void)
{
	if(g_pD3DTextureEffect != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect = NULL;
	}

	if(g_pD3DVtxBuffEffect != NULL)
	{// 捀揰僶僢僼傽偺奐曻
		g_pD3DVtxBuffEffect->Release();
		g_pD3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 峏怴張棟
//=============================================================================
void UpdateEffect(void)
{
	D3DXVECTOR3 rotCamera;

	CAMERA_EX *Cam = GetCamera_Ex();
	// 僇儊儔偺夞揮傪庢摼
	rotCamera = Cam->rot;

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
			g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

			//颜色渐变
			//g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].nDecAlpha;
			//if(g_aEffect[nCntEffect].col.a <= 0.0f)
			//{
			//	g_aEffect[nCntEffect].col.a = 0.0f;
			//}
			SetColorEffect(nCntEffect,
							D3DXCOLOR(g_aEffect[nCntEffect].col.r,g_aEffect[nCntEffect].col.b,
										g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a));
			if (g_aEffect[nCntEffect].nTimer != -1)
			{
				g_aEffect[nCntEffect].nTimer--;
				if (g_aEffect[nCntEffect].nTimer <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
			}
		}
	}
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScale,mtxTranslate;
	CAMERA_EX *Cam = GetCamera_Ex();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// 兛僜乕僗僇儔乕偺巜掕
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// 兛僨僗僥傿僱乕僔儑儞僇儔乕偺巜掕
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z斾妑側偟

    //pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// 傾儖僼傽僽儗儞僨傿儞僌張棟
    //pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 嵟弶偺傾儖僼傽堷悢
    //pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// 俀斣栚偺傾儖僼傽堷悢


	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(g_aEffect[nCntEffect].bUse)
		{
			// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
			D3DXMatrixIdentity(&g_mtxWorldEffect);

			// 價儏乕儅僩儕僢僋僗傪庢摼
			mtxView = Cam->mtxView;

			g_mtxWorldEffect._11 = mtxView._11;
			g_mtxWorldEffect._12 = mtxView._21;
			g_mtxWorldEffect._13 = mtxView._31;
			g_mtxWorldEffect._21 = mtxView._12;
			g_mtxWorldEffect._22 = mtxView._22;
			g_mtxWorldEffect._23 = mtxView._32;
			g_mtxWorldEffect._31 = mtxView._13;
			g_mtxWorldEffect._32 = mtxView._23;
			g_mtxWorldEffect._33 = mtxView._33;

			// 僗働乕儖傪斀塮
			D3DXMatrixScaling(&mtxScale, g_aEffect[nCntEffect].scale.x, g_aEffect[nCntEffect].scale.y, g_aEffect[nCntEffect].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScale);

			// 堏摦傪斀塮
			D3DXMatrixTranslation(&mtxTranslate, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);

			// 儚乕儖僪儅僩儕僢僋僗偺愝掕
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 捀揰僶僢僼傽傪僨僶僀僗偺僨乕僞僗僩儕乕儉偵僶僀儞僪
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 捀揰僼僅乕儅僢僩偺愝掕
			pDevice->SetFVF(FVF_VERTEX_3D);

			// 僥僋僗僠儍偺愝掕
			pDevice->SetTexture(0, g_pD3DTextureEffect);

			// 億儕僑儞偺昤夋
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEffect * 4), NUM_POLYGON);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// 兛僜乕僗僇儔乕偺巜掕
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// 兛僨僗僥傿僱乕僔儑儞僇儔乕偺巜掕
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z斾妑偁傝

//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

//=============================================================================
// 捀揰忣曬偺嶌惉
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// 僆僽僕僃僋僩偺捀揰僶僢僼傽傪惗惉
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,	// 捀揰僨乕僞梡偵妋曐偡傞僶僢僼傽僒僀僘(僶僀僩扨埵)
												D3DUSAGE_WRITEONLY,							// 捀揰僶僢僼傽偺巊梡朄丂
												FVF_VERTEX_3D,								// 巊梡偡傞捀揰僼僅乕儅僢僩
												D3DPOOL_MANAGED,							// 儕僜乕僗偺僶僢僼傽傪曐帩偡傞儊儌儕僋儔僗傪巜掕
												&g_pD3DVtxBuffEffect,						// 捀揰僶僢僼傽僀儞僞乕僼僃乕僗傊偺億僀儞僞
												NULL)))										// NULL偵愝掕
	{
        return E_FAIL;
	}

	{//捀揰僶僢僼傽偺拞恎傪杽傔傞
		VERTEX_3D *pVtx;

		// 捀揰僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
		{
			// 捀揰嵗昗偺愝掕
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);

			// 朄慄偺愝掕
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 斀幩岝偺愝掕
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 僥僋僗僠儍嵗昗偺愝掕
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 捀揰僨乕僞傪傾儞儘僢僋偡傞
		g_pD3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 捀揰嵗昗偺愝掕
//=============================================================================
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
{
	{//捀揰僶僢僼傽偺拞恎傪杽傔傞
		VERTEX_3D *pVtx;

		// 捀揰僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 捀揰嵗昗偺愝掕
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 捀揰僨乕僞傪傾儞儘僢僋偡傞
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// 捀揰僇儔乕偺愝掕
//=============================================================================
void SetColorEffect(int nIdxEffect, D3DXCOLOR col)
{
	{//捀揰僶僢僼傽偺拞恎傪杽傔傞
		VERTEX_3D *pVtx;

		// 捀揰僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 捀揰嵗昗偺愝掕
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// 捀揰僨乕僞傪傾儞儘僢僋偡傞
		g_pD3DVtxBuffEffect->Unlock();
	}
}
//=============================================================================
// 僄僼僃僋僩偺愝掕
//=============================================================================
int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEffect[nCntEffect].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fSizeX = fSizeX;
			g_aEffect[nCntEffect].fSizeY = fSizeY;
			g_aEffect[nCntEffect].nTimer = nTimer;
			g_aEffect[nCntEffect].nDecAlpha = col.a / nTimer;
			g_aEffect[nCntEffect].bUse = true;

			// 捀揰嵗昗偺愝掕
			SetVertexEffect(nCntEffect, fSizeX, fSizeY);

			// 捀揰僇儔乕偺愝掕
			SetColorEffect(nCntEffect,
							D3DXCOLOR(g_aEffect[nCntEffect].col.r,g_aEffect[nCntEffect].col.b,
										g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a));

			nIdxEffect = nCntEffect;

			break;
		}
	}

	return nIdxEffect;
}


EFFECT *RetEffect()
{
	return &g_aEffect[0];
}
//=============================================================================
//
// SkyBox処理 [MeshSky_Ex.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera_Ex.h"
#include "MeshSky_Ex.h"

//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/sky001_Re.jpg"		// 撉傒崬傓僥僋僗僠儍僼傽僀儖柤
#define	VALUE_MOVE_SKY_EX		(5.0f)							// 堏摦懍搙
#define	VALUE_ROTATE_SKY_EX	(D3DX_PI * 0.0001f)				// 夞揮懍搙

#define	SKY_EX_HEIGHT_RATE		(2.0f)		// 嬻僪乕儉偺崅偝學悢

#define	TEX_COUNT_LOOP		(1)			// 僥僋僗僠儍偺孞傝曉偟

#define	MAX_MESH_SKY_EX		(2)			// 嬻偺憤悢

//*****************************************************************************
// 峔憿懱
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;		// 捀揰僶僢僼傽傊偺億僀儞僞
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;		// 僀儞僨僢僋僗僶僢僼傽傊偺億僀儞僞
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffTop;	// 捀揰僶僢僼傽傊偺億僀儞僞
	LPDIRECT3DINDEXBUFFER9 pIdxBuffTop;		// 僀儞僨僢僋僗僶僢僼傽傊偺億僀儞僞

	D3DXMATRIX mtxWorld;					// 儚乕儖僪儅僩儕僢僋僗
	D3DXVECTOR3 pos;						// 億儕僑儞昞帵埵抲偺拞怱嵗昗
	D3DXVECTOR3 rot;						// 億儕僑儞偺夞揮妏

	int nNumBlockH;							// 僽儘僢僋悢(悈暯)
	int nNumBlockV;							// 僽儘僢僋悢(悅捈)
	int nNumVertex;							// 憤捀揰悢	
	int nNumVertexIndex;					// 憤僀儞僨僢僋僗悢
	int nNumPolygon;						// 憤億儕僑儞悢
	float fRadius;							// 敿宎
	int nTexType;							// 僥僋僗僠儍僞僀僾
	float fRotY;							// 夞揮検

	bool bReverse;							// 斀揮ON/OFF
} MESH_SKY_EX;

//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSky_Ex = NULL;	// 僥僋僗僠儍撉傒崬傒応強
MESH_SKY_EX g_aMeshSky_Ex[2] = {};				// 嬻儚乕僋
int g_nNumMeshSky_Ex = 0;						// 嬻偺悢

//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitMeshSky_Ex(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int nNumBlockH, int nNumBlockV,
	float fRadius, float fRotY, int nTexType, bool bReverse)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESH_SKY_EX *pMesh;

	if (g_nNumMeshSky_Ex >= MAX_MESH_SKY_EX)
	{
		return E_FAIL;
	}

	pMesh = &g_aMeshSky_Ex[g_nNumMeshSky_Ex];
	g_nNumMeshSky_Ex++;

	pMesh->bReverse = bReverse;

	// 億儕僑儞昞帵埵抲偺拞怱嵗昗傪愝掕
	pMesh->pos = pos;
	pMesh->rot = rot;
	pMesh->fRadius = fRadius;

	// 僥僋僗僠儍偺撉傒崬傒
	if (nTexType == -1)
	{
		D3DXCreateTextureFromFile(pDevice,				// 僨僶僀僗傊偺億僀儞僞
			TEXTURE_FILENAME,	// 僼傽僀儖偺柤慜
			&g_pTextureSky_Ex);	// 撉傒崬傓儊儌儕乕
	}
	pMesh->nTexType = nTexType;

	// 僽儘僢僋悢偺愝掕
	pMesh->nNumBlockH = nNumBlockH;
	pMesh->nNumBlockV = nNumBlockV;

	// 捀揰悢偺愝掕
	pMesh->nNumVertex = (nNumBlockH + 1) * (nNumBlockV + 1);

	// 僀儞僨僢僋僗悢偺愝掕
	pMesh->nNumVertexIndex = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);

	// 億儕僑儞悢偺愝掕
	pMesh->nNumPolygon = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);

	// 夞揮検
	pMesh->fRotY = fRotY;

	// 僆僽僕僃僋僩偺捀揰僶僢僼傽傪惗惉
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * pMesh->nNumVertex,	// 捀揰僨乕僞梡偵妋曐偡傞僶僢僼傽僒僀僘(僶僀僩扨埵)
		D3DUSAGE_WRITEONLY,					// 捀揰僶僢僼傽偺巊梡朄丂
		FVF_VERTEX_3D,						// 巊梡偡傞捀揰僼僅乕儅僢僩
		D3DPOOL_MANAGED,					// 儕僜乕僗偺僶僢僼傽傪曐帩偡傞儊儌儕僋儔僗傪巜掕
		&pMesh->pVtxBuff,					// 捀揰僶僢僼傽僀儞僞乕僼僃乕僗傊偺億僀儞僞
		NULL)))								// NULL偵愝掕
	{
		return E_FAIL;
	}

	// 僆僽僕僃僋僩偺捀揰僶僢僼傽傪惗惉
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (pMesh->nNumBlockH + 1),	// 捀揰僨乕僞梡偵妋曐偡傞僶僢僼傽僒僀僘(僶僀僩扨埵)
		D3DUSAGE_WRITEONLY,						// 捀揰僶僢僼傽偺巊梡朄丂
		FVF_VERTEX_3D,							// 巊梡偡傞捀揰僼僅乕儅僢僩
		D3DPOOL_MANAGED,						// 儕僜乕僗偺僶僢僼傽傪曐帩偡傞儊儌儕僋儔僗傪巜掕
		&pMesh->pVtxBuffTop,					// 捀揰僶僢僼傽僀儞僞乕僼僃乕僗傊偺億僀儞僞
		NULL)))									// NULL偵愝掕
	{
		return E_FAIL;
	}

	// 僆僽僕僃僋僩偺僀儞僨僢僋僗僶僢僼傽傪惗惉
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * pMesh->nNumVertexIndex,		// 捀揰僨乕僞梡偵妋曐偡傞僶僢僼傽僒僀僘(僶僀僩扨埵)
		D3DUSAGE_WRITEONLY,					// 捀揰僶僢僼傽偺巊梡朄丂
		D3DFMT_INDEX16,						// 巊梡偡傞僀儞僨僢僋僗僼僅乕儅僢僩
		D3DPOOL_MANAGED,					// 儕僜乕僗偺僶僢僼傽傪曐帩偡傞儊儌儕僋儔僗傪巜掕
		&pMesh->pIdxBuff,					// 僀儞僨僢僋僗僶僢僼傽僀儞僞乕僼僃乕僗傊偺億僀儞僞
		NULL)))								// NULL偵愝掕
	{
		return E_FAIL;
	}

	// 僆僽僕僃僋僩偺僀儞僨僢僋僗僶僢僼傽傪惗惉
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * (pMesh->nNumBlockH + 2),	// 捀揰僨乕僞梡偵妋曐偡傞僶僢僼傽僒僀僘(僶僀僩扨埵)
		D3DUSAGE_WRITEONLY,					// 捀揰僶僢僼傽偺巊梡朄丂
		D3DFMT_INDEX16,						// 巊梡偡傞僀儞僨僢僋僗僼僅乕儅僢僩
		D3DPOOL_MANAGED,					// 儕僜乕僗偺僶僢僼傽傪曐帩偡傞儊儌儕僋儔僗傪巜掕
		&pMesh->pIdxBuffTop,				// 僀儞僨僢僋僗僶僢僼傽僀儞僞乕僼僃乕僗傊偺億僀儞僞
		NULL)))								// NULL偵愝掕
	{
		return E_FAIL;
	}

	{//捀揰僶僢僼傽偺拞恎傪杽傔傞
		VERTEX_3D *pVtx;
		const float ANGLE_H = (D3DX_PI * 2.0f) / pMesh->nNumBlockH;
		const float ANGLE_V = (D3DX_PI / 8.0f) / (pMesh->nNumBlockV + 1);
		const float WIDTH = 1.0f / pMesh->nNumBlockH;
		const float HEIGHT = 1.0f / pMesh->nNumBlockV;
		float fLengthXZ;

		// 捀揰僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
		pMesh->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < (pMesh->nNumBlockV + 1); nCntV++)
		{
			fLengthXZ = cosf(ANGLE_V * nCntV) * pMesh->fRadius;

			for (int nCntH = 0; nCntH < (pMesh->nNumBlockH + 1); nCntH++, pVtx++)
			{
				// 捀揰嵗昗偺愝掕
				pVtx->vtx.x = sinf(ANGLE_H * nCntH) * fLengthXZ;
				if (pMesh->bReverse)
				{
					pVtx->vtx.y = -sinf(ANGLE_V * nCntV) * pMesh->fRadius * SKY_EX_HEIGHT_RATE;
				}
				else
				{
					pVtx->vtx.y = sinf(ANGLE_V * nCntV) * pMesh->fRadius * SKY_EX_HEIGHT_RATE;
				}
				pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

				// 朄慄偺愝掕
				pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 捀揰僇儔乕偺愝掕
				if (pMesh->bReverse)
				{
					pVtx->diffuse = D3DXCOLOR(0.0625f, 0.0625f, 0.375f, 0.5f);
				}
				else
				{
					pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				// 僥僋僗僠儍嵗昗偺愝掕
				pVtx->tex.x = nCntH * WIDTH * TEX_COUNT_LOOP;

				if (nCntV == pMesh->nNumBlockV)
				{
					pVtx->tex.y = 0.01f;
				}
				else
				{
					pVtx->tex.y = 1.0f - nCntV * HEIGHT;
				}
			}
		}

		// 捀揰僨乕僞傪傾儞儘僢僋偡傞
		pMesh->pVtxBuff->Unlock();

		// 捀揰僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
		pMesh->pVtxBuffTop->Lock(0, 0, (void**)&pVtx, 0);

		// 捀揰嵗昗偺愝掕
		pVtx->vtx.x = 0.0f;
		if (pMesh->bReverse)
		{
			pVtx->vtx.y = -sinf(ANGLE_V * (pMesh->nNumBlockV + 1)) * pMesh->fRadius * SKY_EX_HEIGHT_RATE;
		}
		else
		{
			pVtx->vtx.y = sinf(ANGLE_V * (pMesh->nNumBlockV + 1)) * pMesh->fRadius * SKY_EX_HEIGHT_RATE;
		}
		pVtx->vtx.z = 0.0f;

		// 朄慄偺愝掕
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 捀揰僇儔乕偺愝掕
		if (pMesh->bReverse)
		{
			pVtx->diffuse = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
		}
		else
		{
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// 僥僋僗僠儍嵗昗偺愝掕
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;

		pVtx++;

		fLengthXZ = cosf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius;
		for (int nCntH = 0; nCntH < pMesh->nNumBlockH; nCntH++, pVtx++)
		{
			// 捀揰嵗昗偺愝掕
			pVtx->vtx.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
			if (pMesh->bReverse)
			{
				pVtx->vtx.y = -sinf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius * SKY_EX_HEIGHT_RATE;
			}
			else
			{
				pVtx->vtx.y = sinf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius * SKY_EX_HEIGHT_RATE;
			}
			pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

			// 朄慄偺愝掕
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 捀揰僇儔乕偺愝掕
			if (pMesh->bReverse)
			{
				pVtx->diffuse = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
			}
			else
			{
				pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// 僥僋僗僠儍嵗昗偺愝掕
			pVtx->tex.x = 0.01f;
			pVtx->tex.y = 0.01f;
		}

		// 捀揰僨乕僞傪傾儞儘僢僋偡傞
		pMesh->pVtxBuffTop->Unlock();
	}

	{//僀儞僨僢僋僗僶僢僼傽偺拞恎傪杽傔傞
		WORD *pIdx;

		// 僀儞僨僢僋僗僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
		pMesh->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntV = 0; nCntV < pMesh->nNumBlockV; nCntV++)
		{
			if (nCntV != 0)
			{
				*pIdx = nCntV * (pMesh->nNumBlockH + 1);
				pIdx++;
			}

			for (int nCntH = 0; nCntH < pMesh->nNumBlockH + 1; nCntH++)
			{
				*pIdx = nCntV * (pMesh->nNumBlockH + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (pMesh->nNumBlockH + 1) + nCntH;
				pIdx++;
			}

			if (nCntV != (pMesh->nNumBlockV - 1))
			{
				*pIdx = (nCntV + 1) * (pMesh->nNumBlockH + 1);
				pIdx++;
			}
		}

		// 僀儞僨僢僋僗僨乕僞傪傾儞儘僢僋偡傞
		pMesh->pIdxBuff->Unlock();

		// 僀儞僨僢僋僗僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
		pMesh->pIdxBuffTop->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntH = 0; nCntH < pMesh->nNumBlockH + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// 僀儞僨僢僋僗僨乕僞傪傾儞儘僢僋偡傞
		pMesh->pIdxBuffTop->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitMeshSky_Ex(void)
{
	MESH_SKY_EX *pMesh;

	for (int nCntSky_Ex = 0; nCntSky_Ex < MAX_MESH_SKY_EX; nCntSky_Ex++)
	{
		pMesh = &g_aMeshSky_Ex[nCntSky_Ex];

		if (pMesh->pVtxBuff)
		{// 捀揰僶僢僼傽偺奐曻
			pMesh->pVtxBuff->Release();
			pMesh->pVtxBuff = NULL;
		}

		if (pMesh->pIdxBuff)
		{// 僀儞僨僢僋僗僶僢僼傽偺奐曻
			pMesh->pIdxBuff->Release();
			pMesh->pIdxBuff = NULL;
		}

		if (pMesh->pVtxBuffTop)
		{// 捀揰僶僢僼傽偺奐曻
			pMesh->pVtxBuffTop->Release();
			pMesh->pVtxBuffTop = NULL;
		}

		if (pMesh->pIdxBuffTop)
		{// 僀儞僨僢僋僗僶僢僼傽偺奐曻
			pMesh->pIdxBuffTop->Release();
			pMesh->pIdxBuffTop = NULL;
		}
	}

	if (g_pTextureSky_Ex)
	{// 僥僋僗僠儍偺奐曻
		g_pTextureSky_Ex->Release();
		g_pTextureSky_Ex = NULL;
	}

	g_nNumMeshSky_Ex = 0;
}

//=============================================================================
// 峏怴張棟
//=============================================================================
void UpdateMeshSky_Ex(void)
{
	MESH_SKY_EX *pMesh;

	for (int nCntSky_Ex = 0; nCntSky_Ex < MAX_MESH_SKY_EX; nCntSky_Ex++)
	{
		pMesh = &g_aMeshSky_Ex[nCntSky_Ex];

		//pMesh->rot.y += D3DX_PI * pMesh->fRotY;
		pMesh->rot.y += VALUE_ROTATE_SKY_EX;
		if (pMesh->rot.y > D3DX_PI)
		{
			pMesh->rot.y -= VALUE_ROTATE_SKY_EX;
		}
	}
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawMeshSky_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	DWORD dwSettingLighting, dwSettingCullmode;
	MESH_SKY_EX *pMesh;

	pDevice->GetRenderState(D3DRS_LIGHTING, &dwSettingLighting);
	pDevice->GetRenderState(D3DRS_CULLMODE, &dwSettingCullmode);
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
	// 儔僀僥傿儞僌傪柍岠偵
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pMesh = &g_aMeshSky_Ex[0];
	//pMesh->rot.y += 0.01f;
	for (int nCntSky_Ex = 0; nCntSky_Ex < MAX_MESH_SKY_EX; nCntSky_Ex++, pMesh++)
	{
		// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
		D3DXMatrixIdentity(&pMesh->mtxWorld);

		// 夞揮傪斀塮

		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh->rot.y,
			pMesh->rot.x,
			pMesh->rot.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxRot);

		// 堏摦傪斀塮
		D3DXMatrixTranslation(&mtxTranslate, pMesh->pos.x,
			pMesh->pos.y,
			pMesh->pos.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxTranslate);

		if (pMesh->bReverse)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	// 昞柺傪僇儕儞僌
		}
		else
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 棤柺傪僇儕儞僌
		}

		// 儚乕儖僪儅僩儕僢僋僗偺愝掕
		pDevice->SetTransform(D3DTS_WORLD, &pMesh->mtxWorld);

		// 捀揰僶僢僼傽傪儗儞僟儕儞僌僷僀僾儔僀儞偵愝掕
		pDevice->SetStreamSource(0, pMesh->pVtxBuff, 0, sizeof(VERTEX_3D));

		// 僀儞僨僢僋僗僶僢僼傽傪儗儞僟儕儞僌僷僀僾儔僀儞偵愝掕
		pDevice->SetIndices(pMesh->pIdxBuff);

		// 捀揰僼僅乕儅僢僩偺愝掕
		pDevice->SetFVF(FVF_VERTEX_3D);

		// 僥僋僗僠儍偺愝掕
		if (pMesh->nTexType == -1)
		{
			pDevice->SetTexture(0, g_pTextureSky_Ex);
		}
		else
		{
			pDevice->SetTexture(0, 0);
		}

		// 億儕僑儞偺昤夋
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, pMesh->nNumVertex, 0, pMesh->nNumPolygon);


		// 捀揰僶僢僼傽傪儗儞僟儕儞僌僷僀僾儔僀儞偵愝掕
		pDevice->SetStreamSource(0, pMesh->pVtxBuffTop, 0, sizeof(VERTEX_3D));

		// 僀儞僨僢僋僗僶僢僼傽傪儗儞僟儕儞僌僷僀僾儔僀儞偵愝掕
		pDevice->SetIndices(g_aMeshSky_Ex[nCntSky_Ex].pIdxBuffTop);

		// 億儕僑儞偺昤夋
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (pMesh->nNumBlockH + 2), 0, pMesh->nNumBlockH);

		//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 棤柺傪僇儕儞僌

		pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
	}

	// 儔僀僥傿儞僌傪桳岠偵
	pDevice->SetRenderState(D3DRS_ZENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, dwSettingLighting);
	pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
}


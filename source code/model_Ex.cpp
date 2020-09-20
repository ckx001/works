//=============================================================================
//
// Xモデル処理 [model_Ex.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "model_Ex.h"
#include "camera_Ex.h"
#include "debugproc.h"
#include "DataBuf.h"
#include "GlobalFunction.h"
#include<string>

using namespace std;

#define	MODEL_MODEL_EX		"data/MODEL/airplane000.x"		// 撉傒崬傓儌僨儖柤

#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 夞揮検

#define VALUE_MOVE (5.0f)
//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************


//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************

static MODEL_EX				g_Model_Ex[MAX_MODEL_EX];						// 僾儗僀儎乕


//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitModel_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 埵抲丒夞揮丒僗働乕儖偺弶婜愝掕
	//g_Model_Ex[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_Model_Ex[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_Model_Ex[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//
	//g_Model_Ex[i].spd = 0.0f;		// 堏摦僗僺乕僪僋儕傾
	//
	//g_Model_Ex[i].pD3DTexture = NULL;
	//g_Model_Ex[i].pD3DXMesh = NULL;
	//g_Model_Ex[i].pD3DXBuffMat = NULL;
	//g_Model_Ex[i].nNumMat = 0;
	
	for (int i = 0; i < MAX_MODEL_EX; i++)
	{
		if (g_Model_Ex[i].use)
		{
			//载入X Model
			if (FAILED(LoadObjModelMesh_Ex(i ,g_Model_Ex[i].ModelPathName)))
			{
				MessageBox(0, "MODEL创建失败", 0, 0);
			}
			//载入纹理
			int ChgListIndex = RetTexuturePointListMapIndex(g_Model_Ex[i].TexturePathNameLen, g_Model_Ex[i].TexturePathName);
			ReLoadObjModelTexture_Ex(i, ChgListIndex);
		}
	}
    

	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitModel_Ex(void)
{
	for (int i = 0; i < MAX_MODEL_EX; i++)
	{
		if (g_Model_Ex[i].pD3DTexture != NULL)
		{// 僥僋僗僠儍偺奐曻
			g_Model_Ex[i].pD3DTexture->Release();
			g_Model_Ex[i].pD3DTexture = NULL;
		}

		if (g_Model_Ex[i].pD3DXMesh != NULL)
		{// 儊僢僔儏偺奐曻
			g_Model_Ex[i].pD3DXMesh->Release();
			g_Model_Ex[i].pD3DXMesh = NULL;
		}

		if (g_Model_Ex[i].pD3DXBuffMat != NULL)
		{// 儅僥儕傾儖偺奐曻
			g_Model_Ex[i].pD3DXBuffMat->Release();
			g_Model_Ex[i].pD3DXBuffMat = NULL;
		}
	}
}

//=============================================================================
// 峏怴張棟
//=============================================================================
void UpdateModel_Ex(void)
{
	
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawModel_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	for (int i = 0; i < MAX_MODEL_EX; i++)
	{
		if (g_Model_Ex[i].use)
		{
			// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
			D3DXMatrixIdentity(&g_Model_Ex[i].mtxWorld);

			// 僗働乕儖傪斀塮
			D3DXMatrixScaling(&mtxScl, g_Model_Ex[i].scl.x, g_Model_Ex[i].scl.y, g_Model_Ex[i].scl.z);
			D3DXMatrixMultiply(&g_Model_Ex[i].mtxWorld, &g_Model_Ex[i].mtxWorld, &mtxScl);

			// 夞揮傪斀塮
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model_Ex[i].rot.y, g_Model_Ex[i].rot.x, g_Model_Ex[i].rot.z);
			D3DXMatrixMultiply(&g_Model_Ex[i].mtxWorld, &g_Model_Ex[i].mtxWorld, &mtxRot);

			// 堏摦傪斀塮
			D3DXMatrixTranslation(&mtxTranslate, g_Model_Ex[i].pos.x, g_Model_Ex[i].pos.y, g_Model_Ex[i].pos.z);
			D3DXMatrixMultiply(&g_Model_Ex[i].mtxWorld, &g_Model_Ex[i].mtxWorld, &mtxTranslate);

			// 儚乕儖僪儅僩儕僢僋僗偺愝掕
			pDevice->SetTransform(D3DTS_WORLD, &g_Model_Ex[i].mtxWorld);

			// 尰嵼偺儅僥儕傾儖傪庢摼
			pDevice->GetMaterial(&matDef);

			// 儅僥儕傾儖忣曬偵懳偡傞億僀儞僞傪庢摼
			pD3DXMat = (D3DXMATERIAL*)g_Model_Ex[i].pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Model_Ex[i].nNumMat; nCntMat++)
			{
				// 儅僥儕傾儖偺愝掕
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// 僥僋僗僠儍偺愝掕
				if(g_Model_Ex[i].pD3DTexture != NULL)
				    pDevice->SetTexture(0, g_Model_Ex[i].pD3DTexture);

				// 昤夋
				g_Model_Ex[i].pD3DXMesh->DrawSubset(nCntMat);
			}
			pDevice->SetMaterial(&matDef);
		}
		pDevice->SetTexture(0, NULL);
	}
}


//=============================================================================
// 僾儗僀儎乕忣曬傪庢摼
//=============================================================================
MODEL_EX *GetModel_Ex(void)
{
	return &g_Model_Ex[0];
}

//重载贴图
void ReLoadObjModelTexture_Ex(int index, int ChgListIndex)
{
	if (ChgListIndex != -1)
	{
		LPDIRECT3DTEXTURE9 *TexturePointList = RetTexturePointList();
		g_Model_Ex[index].pD3DTexture = TexturePointList[ChgListIndex];
	}
	else
		g_Model_Ex[index].pD3DTexture = NULL;
}


bool LoadObjModelMesh_Ex(int index, LPCSTR FileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X僼傽僀儖偺撉傒崬傒
	if (FAILED(D3DXLoadMeshFromX(FileName,			// 撉傒崬傓儌僨儖僼傽僀儖柤(X僼傽僀儖)
		D3DXMESH_SYSTEMMEM,		// 儊僢僔儏偺嶌惉僆僾僔儑儞傪巜掕
		pDevice,				// IDirect3DDevice9僀儞僞乕僼僃僀僗傊偺億僀儞僞
		NULL,					// 椬愙惈僨乕僞傪娷傓僶僢僼傽傊偺億僀儞僞
		&g_Model_Ex[index].pD3DXBuffMat,	// 儅僥儕傾儖僨乕僞傪娷傓僶僢僼傽傊偺億僀儞僞
		NULL,					// 僄僼僃僋僩僀儞僗僞儞僗偺攝楍傪娷傓僶僢僼傽傊偺億僀儞僞
		&g_Model_Ex[index].nNumMat,		// D3DXMATERIAL峔憿懱偺悢
		&g_Model_Ex[index].pD3DXMesh)))	// ID3DXMesh僀儞僞乕僼僃僀僗傊偺億僀儞僞偺傾僪儗僗
	{
		MessageBox(0,"模型加载失败", 0, 0);
		return FALSE;
	}
	return TRUE;
}
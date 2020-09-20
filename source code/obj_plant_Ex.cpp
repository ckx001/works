//=============================================================================
//
// 平面処理 [obj_plant_Ex.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "obj_plant_Ex.h"
#include "input.h"
#include "camera_Ex.h"
#include "light_Ex.h"
#include "debugproc.h"
#include "DataBuf.h"

#define	TEXTURE_OBJ_PLANT	"data/TEXTURE/field000.jpg"						// 撉傒崬傓僥僋僗僠儍僼傽僀儖柤
#define CURRENT_TEXTURE_PATH_CHECK "data/TEXTURE"

#define	VALUE_MOVE		(5.0f)											// 堏摦検
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 夞揮検

#define	SIZE_X			(20.0f)										// 抧柺偺僒僀僘(X曽岦)
#define	SIZE_Z			(20.0f)										// 抧柺偺僒僀僘(Z曽岦)


static HRESULT MakeVertexObjPlant(LPDIRECT3DDEVICE9 pDevice);
void ReLoadObjPlant_Ex(int index, int ChgListIndex);

static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;	// 僥僋僗僠儍傊偺億僀儞僞
//static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff = NULL;	// 捀揰僶僢僼傽傊偺億僀儞僞

static OBJ_PLANT_EX g_ObjPlant[OBJ_PLANT_EX_MAX];

HRESULT InitObjPlant_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//
	//// 僥僋僗僠儍偺撉傒崬傒
	D3DXCreateTextureFromFile(pDevice,	// 僨僶僀僗傊偺億僀儞僞
		TEXTURE_OBJ_PLANT,				// 僼傽僀儖偺柤慜
		&g_pD3DTexture);				// 撉傒崬傓儊儌儕乕
	//for (int i = 0; i < OBJ_PLANT_EX_MAX; i++)
	//{
	//	g_ObjPlant[i].use = false;
	//}
	////g_ObjPlant.g_pD3DVtxBuff = NULL;
	//// 捀揰忣曬偺嶌惉
	//MessageBox(0, g_ObjPlant[0].TexturePathName, 0, 0);
	for (int i = 0; i < OBJ_PLANT_EX_MAX; i++)
	{
		if (g_ObjPlant[i].use)
		{
			int ChgListIndex = RetTexuturePointListMapIndex(g_ObjPlant[i].TexturePathNameLen, g_ObjPlant[i].TexturePathName);
			ReLoadObjPlant_Ex(i, ChgListIndex);
		}
	}
	MakeVertexObjPlant(pDevice);

	return S_OK;
}


HRESULT MakeVertexObjPlant(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < OBJ_PLANT_EX_MAX; i++)
	{
		// 僆僽僕僃僋僩偺捀揰僶僢僼傽傪惗惉
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 捀揰僨乕僞梡偵妋曐偡傞僶僢僼傽僒僀僘(僶僀僩扨埵)
			D3DUSAGE_WRITEONLY,			// 捀揰僶僢僼傽偺巊梡朄丂
			FVF_VERTEX_3D,				// 巊梡偡傞捀揰僼僅乕儅僢僩
			D3DPOOL_MANAGED,			// 儕僜乕僗偺僶僢僼傽傪曐帩偡傞儊儌儕僋儔僗傪巜掕
			&g_ObjPlant[i].g_pD3DVtxBuff,				// 捀揰僶僢僼傽僀儞僞乕僼僃乕僗傊偺億僀儞僞
			NULL)))						// NULL偵愝掕
		{
			return E_FAIL;
		}
	
		{//捀揰僶僢僼傽偺拞恎傪杽傔傞
			VERTEX_3D *pVtx;
	
			// 捀揰僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
			g_ObjPlant[i].g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
			// 捀揰嵗昗偺愝掕
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, 0.0f, SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, 0.0f, SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, 0.0f, -SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, 0.0f, -SIZE_Z);
	
			// 朄慄儀僋僩儖偺愝掕
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
			// 奼嶶岝偺愝掕
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
			// 僥僋僗僠儍嵗昗偺愝掕
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
			// 捀揰僨乕僞傪傾儞儘僢僋偡傞
			g_ObjPlant[i].g_pD3DVtxBuff->Unlock();
		}
	}
	return S_OK;
}


void DrawObjPlant_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	for (int i = 0; i < OBJ_PLANT_EX_MAX; i++)
	{
		if (g_ObjPlant[i].use)
		{
			// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
			D3DXMatrixIdentity(&g_ObjPlant[i].mtxWorld);

			// 僗働乕儖傪斀塮
			D3DXMatrixScaling(&mtxScl, g_ObjPlant[i].scl.x, g_ObjPlant[i].scl.y, g_ObjPlant[i].scl.z);
			D3DXMatrixMultiply(&g_ObjPlant[i].mtxWorld, &g_ObjPlant[i].mtxWorld, &mtxScl);

			// 夞揮傪斀塮
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ObjPlant[i].rot.y, g_ObjPlant[i].rot.x, g_ObjPlant[i].rot.z);
			D3DXMatrixMultiply(&g_ObjPlant[i].mtxWorld, &g_ObjPlant[i].mtxWorld, &mtxRot);

			// 堏摦傪斀塮
			D3DXMatrixTranslation(&mtxTranslate, g_ObjPlant[i].pos.x, g_ObjPlant[i].pos.y, g_ObjPlant[i].pos.z);
			D3DXMatrixMultiply(&g_ObjPlant[i].mtxWorld, &g_ObjPlant[i].mtxWorld, &mtxTranslate);

			// 儚乕儖僪儅僩儕僢僋僗偺愝掕
			pDevice->SetTransform(D3DTS_WORLD, &g_ObjPlant[i].mtxWorld);

			// 捀揰僶僢僼傽傪僨僶僀僗偺僨乕僞僗僩儕乕儉偵僶僀儞僪
			pDevice->SetStreamSource(0, g_ObjPlant[i].g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

			// 捀揰僼僅乕儅僢僩偺愝掕
			pDevice->SetFVF(FVF_VERTEX_3D);

			// 僥僋僗僠儍偺愝掕
			if(g_ObjPlant[i].g_pD3DTexture != NULL)
			    pDevice->SetTexture(0, g_ObjPlant[i].g_pD3DTexture);

			// 億儕僑儞偺昤夋
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
		pDevice->SetTexture(0, NULL);
	}
}

void UpdateObjPlant_Ex()
{

}

void UninitObjPlant_Ex(void)
{
	if (g_pD3DTexture != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_pD3DTexture->Release();
		g_pD3DTexture = NULL;
	}

}

OBJ_PLANT_EX *RetObjPlantEx()
{
    return &g_ObjPlant[0];
}

//重载纹理
void ReLoadObjPlant_Ex(int index, int ChgListIndex)
{
	if (ChgListIndex != -1)
	{
		LPDIRECT3DTEXTURE9 *TexturePointList = RetTexturePointList();
		g_ObjPlant[index].g_pD3DTexture = TexturePointList[ChgListIndex];
	}
	else
		g_ObjPlant[index].g_pD3DTexture = NULL;
}
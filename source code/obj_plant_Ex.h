//=============================================================================
//
// 平面処理 [obj_plant_Ex.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define OBJ_PLANT_EX_MAX 4096

struct OBJ_PLANT_EX
{
	D3DXVECTOR3		pos;		// 億儕僑儞偺埵抲
	D3DXVECTOR3		rot;		// 億儕僑儞偺岦偒(夞揮)
	D3DXVECTOR3		scl;		// 億儕僑儞偺戝偒偝(僗働乕儖)
	int index;                  //plant当前的索引
	bool use;
	D3DXMATRIX		mtxWorld;	// 儚乕儖僪儅僩儕僢僋僗
	LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff;
	LPDIRECT3DTEXTURE9		g_pD3DTexture;
	int TexturePathNameLen;   //文件名长度
	TCHAR TexturePathName[MAX_PATH + 252];
};

HRESULT InitObjPlant_Ex(void);
void DrawObjPlant_Ex(void);
void UpdateObjPlant_Ex(void);
void UninitObjPlant_Ex(void);
OBJ_PLANT_EX *RetObjPlantEx(void);
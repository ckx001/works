//=============================================================================
//
// 立方体処理[cube_Ex.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define OBJ_CUBE_EX_MAX 32768

#include <string>
using namespace std;

struct OBJ_CUBE_EX
{
	D3DXVECTOR3		pos;		// 億儕僑儞偺埵抲
	D3DXVECTOR3		rot;		// 億儕僑儞偺岦偒(夞揮)
	D3DXVECTOR3		scl;		// 億儕僑儞偺戝偒偝(僗働乕儖)
	int index;                  //plant当前的索引
	bool use;
	D3DXMATRIX		mtxWorld;	// 儚乕儖僪儅僩儕僢僋僗
	//LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff;
	IDirect3DVertexBuffer9* g_pD3DVtxBuff;
	IDirect3DVertexBuffer9* g_pD3DVtxBuffShadow;
	LPDIRECT3DTEXTURE9		g_pD3DTexture;
	TCHAR *TexturePath = NULL;
	int TexturePathNameLen;   //文件名长度
	TCHAR TexturePathName[MAX_PATH + 252];
	int ShadowIdx;
	TCHAR tag[MAX_PATH + 252];
};

//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT InitObjCube_Ex(void);
void UninitObjCube_Ex(void);
void UpdateObjCube_Ex(void);
void DrawObjCube_Ex(void);
OBJ_CUBE_EX *RetObjCube_Ex(void);
void ClearAllCube(void);
void ReLoadObjCube_Ex(int index, int ChgListIndex);
void DrawObjCubeShadow(int index);
void FindEnemyBossWolfSceneCubeIndex(void);
int FindEnemyBossWolfSceneCubeIndexSide(int index, int dir);
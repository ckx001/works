//=============================================================================
//
// Xモデル処理 [MeshSky_Ex.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once
#include<string>


using namespace std;

#define MAX_MODEL_EX		(4096)					// 僾儗僀儎乕偺悢

struct MODEL_EX
{
	D3DXVECTOR3			pos;
	D3DXVECTOR3			rot;
	D3DXVECTOR3			scl;

	bool                use;
	bool                active;
	int                 type;
	LPDIRECT3DTEXTURE9	pD3DTexture;
	LPD3DXMESH			pD3DXMesh;
	LPD3DXBUFFER		pD3DXBuffMat;
	DWORD				nNumMat;

	D3DXMATRIX			mtxWorld;

	int ModelPathNameLen;   //文件名长度
	TCHAR ModelPathName[MAX_PATH + 252];
	int ModelPathNameUiIndex; //对于ui texture列表的索引

	int TexturePathNameLen;   //文件名长度
	TCHAR TexturePathName[MAX_PATH + 252];
	int TexturePathNameUiIndex; //对于ui texture列表的索引

};

//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT InitModel_Ex(void);
void UninitModel_Ex(void);
void UpdateModel_Ex(void);
void DrawModel_Ex(void);
void ReLoadObjModelTexture_Ex(int index, int ChgListIndex);
bool LoadObjModelMesh_Ex(int index, LPCSTR FileName);
MODEL_EX *GetModel_Ex(void);


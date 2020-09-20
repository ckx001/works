//=============================================================================
//
// OBJモデル処理 [ObjObjModel_Ex.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once
#include "D3DObjMesh_Ex.h"

#define OBJ_OBJMODEL_EX_MAX 4096

//纹理分类为 纹理数组 和 纹理
enum OBJOBJ_TEXTURE_TYPE
{
	OBJOBJ_TEXTURE_TYPE_TEXTURE,
	OBJOBJ_TEXTURE_TYPE_TEXTURELIST
};

struct OBJTEXTURELIST
{
	TCHAR TextureListPathName[100];
	int TextureListPathNameLen = 0;
};

struct OBJOBJMODEL_EX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3	RelativePos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	bool use;
	LPDIRECT3DTEXTURE9	pD3DTexture;
	LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff;
	int TexturePathNameLen;   //文件名长度
	TCHAR TexturePathName[MAX_PATH + 252];
	int TexturePathNameUiIndex; //对于ui texture列表的索引
	int Group = -1; //对于ui 所属Group
	CD3DMesh_Ex* g_pD3DMesh;

	int ModelPathNameLen;   //文件名长度
	TCHAR ModelPathName[MAX_PATH + 252];
	int ModelPathNameUiIndex; //对于ui texture列表的索引

	int ObjGroupNum; //obj文件中的组数
	int TrianglesList[100];  //每个组中的三角形数量分别渲染
	int matGroups; //材质的组数
	LPDIRECT3DTEXTURE9	pD3DTextureList[100]; //纹理数组
	OBJTEXTURELIST TextureListPath[10];
	float Tf[100][3]; //透明度数组
	TCHAR tag[MAX_PATH + 252];

	int SphereColliderIdx;
};

HRESULT InitObjObjModel_Ex(void);
void UninitObjObjModel_Ex(void);
void UpdateObjObjModel_Ex(void);
void DrawObjObjModel_Ex(void);
void ReLoadObjObjModelTexture_Ex(int index, int ChgListIndex, int type, int ListIndex);
bool ReLoadObjObjModelModel_Ex(int index, LPCSTR ModelPathAndName);
BOOL LoadObjObjFile_Ex(int index, LPCTSTR sFileName);
OBJOBJMODEL_EX *RetObjObjModel_Ex(void);
void ReSetObjObjModel(int index);
void ClearAllObjObjModel(void);
int CreateObjObjModel(int ObjType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
//=============================================================================
//
// データ処理[DataBuf.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once
#include<string>
#include<atlstr.h>
#include <algorithm>

using namespace std;

struct TEXTURE_BUFF
{
	int index;
	LPSTR path;
};


void GetData(string FileName);
#ifdef ENGINE_MODE
void OpenData(string FileName);
#endif
void ReloadData(HWND hDlg, HWND hwndListObjMenu);
LPDIRECT3DTEXTURE9 * RetTexturePointList();
void UnInitDataBuf();
int RetTexuturePointListMapIndex(int len, TCHAR * FindName);
void GetFilePathList();
CString GetTextureFileName(int id);
int GetTextureFileNameLen();

///////X MODEL///////
CString GetModelFileName(int id);
void GetModelFilePathList(TCHAR *IterationFile, string Path, int type);
int GetModelFileNameLen();
LPD3DXMESH *RetXModelMeshList();
LPD3DXBUFFER *RetXModelBuffMatList();
DWORD *RetXModelNumMatList();

///////Obj Model///////
CString GetObjModelFileName(int id);
int GetObjModelFileNameLen();

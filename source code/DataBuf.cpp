//=============================================================================
//
// データ処理[DataBuf.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "DataBuf.h"

#include "GlobalFunction.h"
#include "Serialize.h"

#ifdef ENGINE_MODE
#include "obj_plant.h"
#include "cube.h"
#include "ui.h"
#include "model.h"
#include "ObjObjModel.h"
#include "resource.h"
#endif

//Ex
#include "obj_plant_Ex.h"
#include "model_Ex.h"
#include "cube_Ex.h"
#include "ObjObjModel_Ex.h"
#include <io.h>
#include<vector>
#include<atlstr.h>
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;

#define CURRENT_TEXTURE_PATH "\\data\\TEXTURE\\*.jpg"
#define CURRENT_TEXTURE_PATH_TGA "\\data\\TEXTURE\\*.tga"
#define CURRENT_TEXTURE_PATH_CHECK "data\\TEXTURE\\"
#define CURRENT_TEXTURE_OBJ_MODEL_PATH_PNG "\\data\\MODEL\\ObjModel\\*.png"
#define CURRENT_TEXTURE_OBJ_PATH_CHECK "data\\MODEL\\ObjModel\\"

#define MAX_POINT 32768

static LPSTR ProjectPathPos;
vector<string> FindAllFile(const char* dir, bool ishavedir);
int FindDocumenuFile(string Syspath, string FindPath, string Path, int StartIndex);
bool RegisterTexutrePoint(int index, LPCSTR path, LPSTR FileName, string TexturePath);
bool RegisterXModelPoint(int index, LPCSTR path, LPSTR FileName);
bool RegisterObjModelPoint(int index, LPCSTR path, LPSTR FileName);
int RetXmodelPointListMapIndex(int len, TCHAR * FindName);

/////////////Texture/////////////
LPDIRECT3DTEXTURE9 TexuturePointList[MAX_POINT]; //图片种类的指针
map< int, string > TexuturePointListMap; //索引和路径的映射
map< int, string > TexuturePathPointListMap; //索引和纹理路径的映射
/////////////////////////////////
/////////////X Model/////////////
LPD3DXMESH			XModelPointListMesh[MAX_POINT];
LPD3DXBUFFER		XModelPointListBuffMat[MAX_POINT];
DWORD				XModelPointListNumMat[MAX_POINT];
map< int, string > XModelPointListMap; //索引和路径的映射
/////////////////////////////////
////////////Obj Model////////////
map< int, string > ObjModelPointListMap; //索引和路径的映射
/////////////////////////////////
CString TextureFileName[MAX_POINT]; //记录路径下所有图片文件名
CString ModelFileName[MAX_POINT]; //记录路径下所有X模型文件名
CString ObjModelFileName[MAX_POINT]; //记录路径下所有OBJ模型文件名
int TextureFileNameLen; //此路径下的图片文件名的数量
int ModelFileNameLen; //此路径下的X模型文件名的数量
int ObjModelFileNameLen; //此路径下的OBJ模型文件名的数量
//////////

//演示模式缓存反序列化后的数据到内存
//不需要载入UI相关数据
void GetData(string FileName)
{
	SAVEDATA* SerializeData = LoadData(FileName);

	OBJ_PLANT_EX *ObjPlant_Ex = RetObjPlantEx();
	MODEL_EX *ObjModel_Ex = GetModel_Ex();
	OBJ_CUBE_EX *ObjCube_Ex = RetObjCube_Ex();
	OBJOBJMODEL_EX *ObjObjModel_Ex = RetObjObjModel_Ex();

	//重写plant
	for (int i = 0; i < OBJ_PLANT_EX_MAX; i++)
	{
		ObjPlant_Ex[i].g_pD3DVtxBuff = NULL;
		ObjPlant_Ex[i].g_pD3DTexture = NULL;
		ObjPlant_Ex[i].use = false;
	}
	for (int i = 0; i < OBJ_PLANT_EX_MAX; i++)
	{
		if (SerializeData->ObjPlant[i].use == true)
		{
			ObjPlant_Ex[i].pos = SerializeData->ObjPlant[i].pos;
			ObjPlant_Ex[i].rot = SerializeData->ObjPlant[i].rot;
			ObjPlant_Ex[i].scl = SerializeData->ObjPlant[i].scl;
			ObjPlant_Ex[i].TexturePathNameLen = SerializeData->ObjPlant[i].TexturePathNameLen;
			strcpy(ObjPlant_Ex[i].TexturePathName, SerializeData->ObjPlant[i].TexturePathName);
			ObjPlant_Ex[i].use = true;
		}
	}
	//重写model
	for (int i = 0; i < MAX_MODEL_EX; i++)
	{
		ObjModel_Ex[i].pD3DTexture = NULL;
		ObjModel_Ex[i].pD3DXMesh = NULL;
		ObjModel_Ex[i].pD3DXBuffMat = NULL;
		ObjModel_Ex[i].nNumMat = 0;
		ObjModel_Ex[i].use = false;
	}
	for (int i = 0; i < MAX_MODEL_EX; i++)
	{
		if (SerializeData->ObjModel[i].use == true)
		{
			ObjModel_Ex[i].pos = SerializeData->ObjModel[i].pos;
			ObjModel_Ex[i].rot = SerializeData->ObjModel[i].rot;
			ObjModel_Ex[i].scl = SerializeData->ObjModel[i].scl;
			ObjModel_Ex[i].ModelPathNameLen = SerializeData->ObjModel[i].ModelPathNameLen;
			strcpy(ObjModel_Ex[i].ModelPathName, SerializeData->ObjModel[i].ModelPathName);
			ObjModel_Ex[i].TexturePathNameLen = SerializeData->ObjModel[i].TexturePathNameLen;
			strcpy(ObjModel_Ex[i].TexturePathName, SerializeData->ObjModel[i].TexturePathName);
			ObjModel_Ex[i].use = true;
		}
	}
	//重写cube
	for (int i = 0; i < OBJ_CUBE_EX_MAX; i++)
	{
		ObjCube_Ex[i].g_pD3DVtxBuff = NULL;
		ObjCube_Ex[i].g_pD3DTexture = NULL;
		ObjCube_Ex[i].use = false;
	}
	//for (int i = 0; i < OBJ_CUBE_EX_MAX; i++)
	//{
	//	if (SerializeData->ObjCube[i].use == true)
	//	{
	//		ObjCube_Ex[i].pos = SerializeData->ObjCube[i].pos;
	//		ObjCube_Ex[i].rot = SerializeData->ObjCube[i].rot;
	//		ObjCube_Ex[i].scl = SerializeData->ObjCube[i].scl;
	//		ObjCube_Ex[i].TexturePathNameLen = SerializeData->ObjCube[i].TexturePathNameLen;
	//		strcpy(ObjCube_Ex[i].TexturePathName, SerializeData->ObjCube[i].TexturePathName);
	//		ObjCube_Ex[i].use = true;
	//	}
	//}

	//重写Obj Model
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		ObjObjModel_Ex[i].g_pD3DVtxBuff = NULL;
		ObjObjModel_Ex[i].pD3DTexture = NULL;
		ObjObjModel_Ex[i].use = false;
	}
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (SerializeData->ObjObjModel[i].use == true)
		{
			ObjObjModel_Ex[i].pos = SerializeData->ObjObjModel[i].pos;
			ObjObjModel_Ex[i].rot = SerializeData->ObjObjModel[i].rot;
			ObjObjModel_Ex[i].scl = SerializeData->ObjObjModel[i].scl;
			ObjObjModel_Ex[i].ModelPathNameLen = SerializeData->ObjObjModel[i].ModelPathNameLen;
			strcpy(ObjObjModel_Ex[i].ModelPathName, SerializeData->ObjObjModel[i].ModelPathName);
			ObjObjModel_Ex[i].TexturePathNameLen = SerializeData->ObjObjModel[i].TexturePathNameLen;
			strcpy(ObjObjModel_Ex[i].TexturePathName, SerializeData->ObjObjModel[i].TexturePathName);
			ObjObjModel_Ex[i].ObjGroupNum = SerializeData->ObjObjModel[i].ObjGroupNum;
			ObjObjModel_Ex[i].matGroups = SerializeData->ObjObjModel[i].matGroups;
			for (int c = 0; c < 100; c++)
			{
				ObjObjModel_Ex[i].TrianglesList[c] = SerializeData->ObjObjModel[i].TrianglesList[c];
				for (int d = 0; d < 3; d++)
					ObjObjModel_Ex[i].Tf[c][d] = SerializeData->ObjObjModel[i].Tf[c][d];

			}
			for (int group = 0; group < ObjObjModel_Ex[i].matGroups; group++)
			{
				strcpy(ObjObjModel_Ex[i].TextureListPath[group].TextureListPathName, SerializeData->ObjObjModel[i].TextureListPath[group].TextureListPathName);
				ObjObjModel_Ex[i].TextureListPath[group].TextureListPathNameLen = SerializeData->ObjObjModel[i].TextureListPath[group].TextureListPathNameLen;
			}
			ObjObjModel_Ex[i].use = true;
		}
	}
	return;
}


#ifdef ENGINE_MODE
//引擎模式打开二进制文件重载数据
//需要载入UI数据
void OpenData(string FileName)
{
	SAVEDATA* SerializeData = LoadData(FileName);

	OBJ_PLANT *plant = RetObjPlant();
	MODEL *ObjModel = RetObjModel();
	OBJ_CUBE *ObjCube = RetObjCube();
	OBJOBJMODEL *ObjObjModel = RetObjObjModel();
	UI *Ui = RetUi();

	//int *SelectedTypeIdIndex = RetSelectedTypeIdIndex();
	//重写plant
	for (int i = 0; i < OBJ_PLANT_MAX; i++)
	{
		//plant[i].g_pD3DVtxBuff = NULL;
		plant[i].g_pD3DTexture = NULL;
		plant[i].use = false;
	}
	for (int i = 0; i < OBJ_PLANT_MAX; i++)
	{
		if (SerializeData->ObjPlant[i].use == true)
		{
			plant[i].pos = SerializeData->ObjPlant[i].pos;
			plant[i].rot = SerializeData->ObjPlant[i].rot;
			plant[i].scl = SerializeData->ObjPlant[i].scl;
			plant[i].TexturePathNameLen = SerializeData->ObjPlant[i].TexturePathNameLen;
			strcpy(plant[i].TexturePathName, SerializeData->ObjPlant[i].TexturePathName);
			plant[i].use = true;
		}
	}
	//重写model
	for (int i = 0; i < MAX_MODEL_NUM; i++)
	{
		ObjModel[i].pD3DTexture = NULL;
		//ObjModel[i].pD3DXMesh = NULL;
		//ObjModel[i].pD3DXBuffMat = NULL;
		//ObjModel[i].nNumMat = 0;
		ObjModel[i].use = false;
	}
	for (int i = 0; i < MAX_MODEL_NUM; i++)
	{
		if (SerializeData->ObjModel[i].use == true)
		{
			ObjModel[i].pos = SerializeData->ObjModel[i].pos;
			ObjModel[i].rot = SerializeData->ObjModel[i].rot;
			ObjModel[i].scl = SerializeData->ObjModel[i].scl;
			ObjModel[i].ModelPathNameLen = SerializeData->ObjModel[i].ModelPathNameLen;
			strcpy(ObjModel[i].ModelPathName, SerializeData->ObjModel[i].ModelPathName);
			ObjModel[i].TexturePathNameLen = SerializeData->ObjModel[i].TexturePathNameLen;
			strcpy(ObjModel[i].TexturePathName, SerializeData->ObjModel[i].TexturePathName);
			
			///////////
			ObjModel[i].nNumMat = SerializeData->ObjModel[i].nNumMat;
			ObjModel[i].pD3DXBuffMat = SerializeData->ObjModel[i].pD3DXBuffMat;
			ObjModel[i].pD3DXMesh = SerializeData->ObjModel[i].pD3DXMesh;
			////////////
			ObjModel[i].use = true;
		}
	}
	//重写cube
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		//ObjCube[i].g_pD3DVtxBuff = NULL;
		ObjCube[i].g_pD3DTexture = NULL;
		ObjCube[i].use = false;
	}
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		if (SerializeData->ObjCube[i].use == true)
		{
			ObjCube[i].pos = SerializeData->ObjCube[i].pos;
			ObjCube[i].rot = SerializeData->ObjCube[i].rot;
			ObjCube[i].scl = SerializeData->ObjCube[i].scl;
			ObjCube[i].TexturePathNameLen = SerializeData->ObjCube[i].TexturePathNameLen;
			strcpy(ObjCube[i].TexturePathName, SerializeData->ObjCube[i].TexturePathName);
			ObjCube[i].use = true;
		}
	}
	for (int i = 0; i < OBJ_OBJMODEL_MAX; i++)
	{
		ObjObjModel[i].use = false;
		strcpy(ObjObjModel[i].TexturePathName, "");
		ObjObjModel[i].TexturePathNameLen = 0;
		strcpy(ObjObjModel[i].ModelPathName, "");
		ObjObjModel[i].TexturePathNameLen = 0;
	}
	for (int i = 0; i < OBJ_OBJMODEL_MAX; i++)
	{
		if (SerializeData->ObjObjModel[i].use == true)
		{
			ObjObjModel[i].pos = SerializeData->ObjObjModel[i].pos;
			ObjObjModel[i].rot = SerializeData->ObjObjModel[i].rot;
			ObjObjModel[i].scl = SerializeData->ObjObjModel[i].scl;
			ObjObjModel[i].ModelPathNameLen = SerializeData->ObjObjModel[i].ModelPathNameLen;
			strcpy(ObjObjModel[i].ModelPathName, SerializeData->ObjObjModel[i].ModelPathName);
			ObjObjModel[i].TexturePathNameLen = SerializeData->ObjObjModel[i].TexturePathNameLen;
			strcpy(ObjObjModel[i].TexturePathName, SerializeData->ObjObjModel[i].TexturePathName);
			ObjObjModel[i].use = true;
		}
	}
	//引擎加载需要重写ui
	for (int i = 0; i < MAX_UI_OBJ_NUM; i++)
	{
		Ui[i].use = false;
		Ui[i].SelectedType = -1;
		Ui[i].SelectedTypeIdIndex = -1;
	}
	for (int i = 0; i < MAX_UI_OBJ_NUM; i++)
	{
		if (SerializeData->Ui[i].use == true)
		{
			Ui[i].SelectedType = SerializeData->Ui[i].SelectedType;
			Ui[i].SelectedTypeIdIndex = SerializeData->Ui[i].SelectedTypeIdIndex;
			Ui[i].use = true;
		}
	}
	SetListObjMenuIndexId(SerializeData->ListObjMenuIndexId);

	for (int i = 0; i < OBJ_INDEX_TYPE_NUM; i++)
	{
		SetSelectedTypeIdIndex(i, SerializeData->SelectedTypeIdIndex[i]);
	}
	return;
}
#endif

//查找dir路径下所有文件(dir包括配置符，如c:\*.txt)
//ishavedir表示返回的文件名是否包含全路径，true为返回全路径的文件名，false只返回文件名
vector<string> FindAllFile(const char* dir, bool ishavedir)
{
	_finddata_t file;
	vector<string> file_list;
	long lf;
	if ((lf = _findfirst(dir, &file)) == -1l) { //_findfirst返回的是long型; long __cdecl _findfirst(const char *, struct _finddata_t *)
		return file_list;
	}
	else {
		do {
			if (ishavedir) {
				string tmppath = dir;
				int index = tmppath.find_last_of("*.") - 1;
				tmppath = tmppath.substr(0, index) + file.name;
				file_list.push_back(tmppath);
			}
			else {
				file_list.push_back(file.name);
			}
		} while (_findnext(lf, &file) == 0);//int _findnext(long, struct _finddata_t *);如果找到下个文件的名字成功的话就返回0,否则返回-1
		_findclose(lf);
		return file_list;
	}
}

//缓存纹理指针
bool RegisterTexutrePoint(int index, LPCSTR path, LPSTR FileName, string TexturePath)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (FAILED(D3DXCreateTextureFromFile(pDevice,	// 僨僶僀僗傊偺億僀儞僞
		path,				// 僼傽僀儖偺柤慜
		&TexuturePointList[index])))
	{
		MessageBox(0, "纹理缓存失败", 0, 0);
		return FALSE;
	}

	TexuturePointListMap[index] = FileName;
	TexuturePathPointListMap[index] = TexturePath;
	return TRUE;
}


//缓存x模型指针
bool RegisterXModelPoint(int index, LPCSTR path, LPSTR FileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (FAILED(D3DXLoadMeshFromX(path,
		D3DXMESH_SYSTEMMEM,		
		pDevice,				
		NULL,					
		&XModelPointListBuffMat[index],
		NULL,				
		&XModelPointListNumMat[index],
		&XModelPointListMesh[index])))
	{
		MessageBox(0, "模型缓存失败", 0, 0);
		return FALSE;
	}

	XModelPointListMap[index] = path;
	return TRUE;
}

//缓存Obj模型指针
bool RegisterObjModelPoint(int index, LPCSTR path, LPSTR FileName)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//if (FAILED(LoadObjObjFile(index, path)))
	//	return FALSE;

	return TRUE;
}

//LPD3DXMESH			XModelPointListMesh[MAX_POINT];
//LPD3DXBUFFER		XModelPointListBuffMat[MAX_POINT];
//DWORD				XModelPointListNumMat[MAX_POINT];
LPD3DXMESH *RetXModelMeshList()
{
	return XModelPointListMesh;
}

LPD3DXBUFFER *RetXModelBuffMatList()
{
	return XModelPointListBuffMat;
}

DWORD *RetXModelNumMatList()
{
	return XModelPointListNumMat;
}
//////////////////////////


LPDIRECT3DTEXTURE9 * RetTexturePointList()
{
	return TexuturePointList;
}

//匹配纹理 返回索引
int RetTexuturePointListMapIndex(int len,TCHAR * FindName)
{
	//MessageBox(0, FindName, 0, 0);
	string FindStr;
	if (len != -1)
	{
		FindStr.append(FindName, len);

		std::map<int, string>::iterator it = TexuturePointListMap.end();
		map<int, string>::iterator iter;
		for (iter = TexuturePointListMap.begin(); iter != TexuturePointListMap.end(); iter++)
		{
			if (FindStr == iter->second.c_str())
			{
				return iter->first;
			}
			//MessageBox(0, iter->second, 0, 0);
		}
	}
	//else
	//	MessageBox(0, "载入Texture指针为空", 0, 0);
	return -1;
}

//匹配X model文件名 返回索引
int RetXmodelPointListMapIndex(int len, TCHAR * FindName)
{
	//MessageBox(0, FindName, 0, 0);
	string FindStr;
	FindStr.append(FindName, len);

	std::map<int, string>::iterator it = XModelPointListMap.end();
	map<int, string>::iterator iter;
	for (iter = XModelPointListMap.begin(); iter != XModelPointListMap.end(); iter++)
	{
		if (FindStr == iter->second.c_str())
		{
			return iter->first;
		}
	}
	return -1;
}


void UnInitDataBuf()
{
	for(int i = 0;i< MAX_POINT;i++)
		SAFE_RELEASE(TexuturePointList[i]);
	for (int i = 0; i < MAX_POINT; i++)
		SAFE_RELEASE(XModelPointListMesh[i]);
	for (int i = 0; i < MAX_POINT; i++)
		SAFE_RELEASE(XModelPointListBuffMat[i]);
}


void GetFilePathList()
{
	TCHAR syspath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, syspath);

	//指针初始化
	for (int i = 0; i < MAX_POINT; i++)
	{
		TexuturePointList[i] = NULL;
	}

	string TextureJpgName = syspath;
	TextureJpgName.append(CURRENT_TEXTURE_PATH);
	vector<char*> cfile;
	vector<string> FileList = FindAllFile(TextureJpgName.c_str(), false);
	int JpgLen = 0; //jpg文件的数量
	int i = 0; //搜索的全部文件的数量
	for (vector<string>::iterator iter1 = FileList.begin(); iter1 != FileList.end(); ++iter1)
	{
		cfile.push_back(const_cast<char *>((*iter1).c_str()));
		TextureFileName[i] = cfile[i];
		/////////////
		string result = CURRENT_TEXTURE_PATH_CHECK;
		result.append(cfile[i]);
		if (!RegisterTexutrePoint(i, result.c_str(), cfile[i], CURRENT_TEXTURE_PATH_CHECK))
		{
			MessageBox(0, "纹理数组初始化出错", 0, 0);
		}
		/////////////
		i++;
		JpgLen++;
	}

	string TextureTgaName = syspath;
	TextureTgaName.append(CURRENT_TEXTURE_PATH_TGA);
	vector<char*> cfile1;
	vector<string> FileList1 = FindAllFile(TextureTgaName.c_str(), false);
	for (vector<string>::iterator iter2 = FileList1.begin(); iter2 != FileList1.end(); ++iter2)
	{
		cfile1.push_back(const_cast<char *>((*iter2).c_str()));
		TextureFileName[i] = cfile1[i - JpgLen];
		/////////////////
		string result1 = CURRENT_TEXTURE_PATH_CHECK;
		result1.append(cfile1[i - JpgLen]);
		if (!RegisterTexutrePoint(i, result1.c_str(), cfile1[i - JpgLen], CURRENT_TEXTURE_PATH_CHECK))
		{
			MessageBox(0, "纹理数组初始化出错", 0, 0);
		}
		//////////////////
		i++;
	}

	TextureFileNameLen = i;

	int num = FindDocumenuFile(syspath, CURRENT_TEXTURE_OBJ_MODEL_PATH_PNG, CURRENT_TEXTURE_OBJ_PATH_CHECK, TextureFileNameLen);
}


//IterationFile 检索条件 \\data\\MODEL\\*.obj
//Path 模型目录位置 CURRENT_OBJMODEL_PATH_CHECK
//ModelList记录结果的数组
void GetModelFilePathList(TCHAR *IterationFile, string Path, int type)
{
	TCHAR syspath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, syspath);
	TCHAR* ModelCurrentName = IterationFile;
	int len = 0;
	int len1 = 0;
	TCHAR ModelPathName[MAX_PATH];
	//遍历文件
	for (int i = 0; syspath[i] != NULL; i++)
	{
		ModelPathName[i] = syspath[i];
		len++;
	}
	for (int i = 0; ModelCurrentName[i] != NULL; i++)
	{
		ModelPathName[i + len] = ModelCurrentName[i];
		len1++;
	}
	CString ModelName = ModelPathName;
	std::string str1 = "";
	int len_re = len + len1;

	str1.append(ModelName, len_re);

	vector<string> FileList = FindAllFile(str1.c_str(), false);
	int i = 0;
	vector<char*> cfile;
	for (vector<string>::iterator iter1 = FileList.begin(); iter1 != FileList.end(); ++iter1)
	{
		cfile.push_back(const_cast<char *>((*iter1).c_str()));
		//ModelList[i] = cfile[i];
		if(type == OBJ_MODEL_TYPE)
		    ModelFileName[i] = cfile[i];
		else if(type == OBJ_OBJMODEL_TYPE)
			ObjModelFileName[i] = cfile[i];
		/////////////
		string result = Path; 
		result.append(cfile[i]);
		if (type == OBJ_MODEL_TYPE)
		{
			if (!RegisterXModelPoint(i, result.c_str(), cfile[i]))
			{
				MessageBox(0, "X模型数组初始化出错", 0, 0);
			}
		}
		else if (type == OBJ_OBJMODEL_TYPE)
		{
			//if (!RegisterObjModelPoint(i, result.c_str(), cfile[i]))
			//{
			//	MessageBox(0, "OBJ模型数组初始化出错", 0, 0);
			//}
		}
		/////////////
		i++;
	}
	if (type == OBJ_MODEL_TYPE)
	    ModelFileNameLen = i;
	else if (type == OBJ_OBJMODEL_TYPE)
		ObjModelFileNameLen = i;
	//MessageBox(0, FileName[1], 0, 0);
}

//获取纹理图片文件名通过编号
CString GetTextureFileName(int id)
{
	return TextureFileName[id];
}

//获取纹理图片的文件的数量
int GetTextureFileNameLen()
{
	return TextureFileNameLen;
}

//获取X模型的文件名通过编号
CString GetModelFileName(int id)
{
	return ModelFileName[id];
}

//获取X模型的文件的数量
int GetModelFileNameLen()
{
	return ModelFileNameLen;
}

//获取Obj模型的文件名通过编号
CString GetObjModelFileName(int id)
{
	return ObjModelFileName[id];
}

//获取Obj模型的文件的数量
int GetObjModelFileNameLen()
{
	return ObjModelFileNameLen;
}

#ifdef ENGINE_MODE
//用于重载数据(引擎模式下打开二进制文件的重载)
void ReloadData(HWND hDlg, HWND hwndListObjMenu)
{
	OBJ_PLANT *ObjPlant = RetObjPlant();
	MODEL *ObjModel = RetObjModel();
	OBJ_CUBE *ObjCube = RetObjCube();
	OBJOBJMODEL *ObjObjModel = RetObjObjModel();
	UI *Ui = RetUi();

	for (int i = 0; i < OBJ_PLANT_MAX; i++)
	{
		if (ObjPlant[i].use)
		{
			int ChgListIndex = RetTexuturePointListMapIndex(ObjPlant[i].TexturePathNameLen, ObjPlant[i].TexturePathName);
			ReLoadObjPlant(i, ChgListIndex);
		}
	}
	for (int i = 0; i < MAX_MODEL_NUM; i++)
	{
		if (ObjModel[i].use)
		{
			int ChgListIndex = RetTexuturePointListMapIndex(ObjModel[i].TexturePathNameLen, ObjModel[i].TexturePathName);
			ReLoadObjModelTexture(i, ChgListIndex);
			//重载X Model////
			int XModelChgListIndex = RetXmodelPointListMapIndex(ObjModel[i].ModelPathNameLen,ObjModel[i].ModelPathName);
			ReLoadObjMODEL_X(i, XModelChgListIndex);
		}
	}
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		if (ObjCube[i].use)
		{
			int ChgListIndex = RetTexuturePointListMapIndex(ObjCube[i].TexturePathNameLen, ObjCube[i].TexturePathName);
			ReLoadObjCube(i, ChgListIndex);
		}
	}
	for (int i = 0; i < OBJ_OBJMODEL_MAX; i++)
	{
		if (ObjObjModel[i].use)
		{
			int ChgListIndex = RetTexuturePointListMapIndex(ObjObjModel[i].TexturePathNameLen, ObjObjModel[i].TexturePathName);
			ReLoadObjObjModelTexture(i, ChgListIndex);
			//重载Texture数组
            //如果自定义纹理没有设置
			for (int group = 0; group < ObjObjModel[i].matGroups; group++)
			{
				int ChgTextureListIndex = RetTexuturePointListMapIndex(ObjObjModel[i].TextureListPath[group].TextureListPathNameLen, ObjObjModel[i].TextureListPath[group].TextureListPathName);
				ReLoadObjObjModelTexture(i, ChgTextureListIndex, OBJOBJ_TEXTURE_TYPE_TEXTURELIST, group);
			}
			//重载Obj Model////
			//int XModelChgListIndex = RetXmodelPointListMapIndex(ObjObjModel[i].ModelPathNameLen, ObjObjModel[i].ModelPathName);
			ReLoadObjObjModelModel(i, ObjObjModel[i].ModelPathName);
		}
	}
	//清空菜单列表栏
	SendMessage(hwndListObjMenu, LB_RESETCONTENT, 0, 0);

	//重载UI数据
    for (int i = 0; i < MAX_UI_OBJ_NUM; i++)
    {
		if (Ui[i].use)
    	{
			AddListBoxValue(hwndListObjMenu, Ui[i].SelectedType, i);
    	}
    }
}
#endif // ENGINE_MODE

// i : 文件索引的起始位置
int FindDocumenuFile(string Syspath, string FindPath, string Path, int StartIndex)
{
	string TextureName = Syspath;
	TextureName.append(FindPath);
	vector<char*> cfile;
	vector<string> FileList = FindAllFile(TextureName.c_str(), false);

	int i = 0;
	for (vector<string>::iterator iter1 = FileList.begin(); iter1 != FileList.end(); ++iter1)
	{
		cfile.push_back(const_cast<char *>((*iter1).c_str()));
		//TextureFileName[i] = cfile[i];
		/////////////
		string result = Path;
		result.append(cfile[i]);
		if (!RegisterTexutrePoint(StartIndex, result.c_str(), cfile[i], Path))
		{
			MessageBox(0, "Obj纹理数组初始化出错", 0, 0);
		}
		/////////////
		StartIndex++;
		i++;
	}
	return StartIndex;
}

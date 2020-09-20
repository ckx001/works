//=============================================================================
//
// グローバル関数 [GlobalFunction.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "GlobalFunction.h"
#include "GameMath.h"
#include "Scene.h"
#include<string>
#include<stdio.h>
#include<atlstr.h>

#ifdef ENGINE_MODE
#include "camera.h"
#else
#include "camera_Ex.h"
#endif

using namespace std;


int ObjTypeNumList[OBJ_INDEX_TYPE_NUM]; //记录每一个模型类型分别有多少个obj
TCHAR ProjectPath[MAX_PATH];

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 material;
	material.Ambient = a;
	material.Diffuse = d;
	material.Specular = s;
	material.Emissive = e;
	material.Power = p;
	return material;
}

//数组记录每一个模型类型分别有多少个obj
//list[obj类型] = 数量
//为模型类型添加数量 每次加1
//参数1 数据类型
void AddObjTypeNumList(int type)
{
	ObjTypeNumList[type]++;
}

int* RetObjTypeNumList()
{
	return &ObjTypeNumList[0];
}

//清空列表类型索引
void DelObjTypeNumList()
{
	for (int i = 0; i < OBJ_INDEX_TYPE_NUM; i++)
	{
		ObjTypeNumList[i] = 0;
	}
}

//读取ObjTypeNumList列表
void ReloadObjTypeNumList()
{
	for (int i = 0; i < OBJ_INDEX_TYPE_NUM; i++)
	{
		ObjTypeNumList[i] = 0;
	}
}


//获取项目的绝对路径
void GetSysPath()
{
	GetCurrentDirectory(MAX_PATH, ProjectPath);
}

TCHAR* GetProjectPath()
{
	return &ProjectPath[0];
}

void string2tchar(std::string &src, TCHAR* buf)
{
#ifdef UNICODE  
	_stprintf_s(buf, MAX_PATH, _T("%S"), src.c_str());//%S宽字符  
#else  
	_stprintf_s(buf, MAX_PATH, _T("%s"), src.c_str());//%s单字符
#endif
}


//Camera 只显示视锥体中的Cube
bool CubeInFrustum(D3DXVECTOR3 pos)
{
	float lfAlfaX = pos.x + GRID_SIZE * 2;
	float lfDeltaX = pos.x - GRID_SIZE * 2;
	float lfAlfaY = pos.y + GRID_SIZE * 2;
	float lfDeltaY = pos.y - GRID_SIZE * 2;
	float lfAlfaZ = pos.z + GRID_SIZE * 2;
	float lfDeltaZ = pos.z - GRID_SIZE * 2;

	//float lfAlfaX = pos.x + GRID_SIZE ;
	//float lfDeltaX = pos.x - GRID_SIZE;
	//float lfAlfaY = pos.y + GRID_SIZE;
	//float lfDeltaY = pos.y - GRID_SIZE;
	//float lfAlfaZ = pos.z + GRID_SIZE;
	//float lfDeltaZ = pos.z - GRID_SIZE;

	for (int i = 0; i < 6; i++)
	{
		int j = 8;
		BOOL lbIsInAllPlanes = TRUE;
#ifdef ENGINE_MODE
		D3DXPLANE &loPlane = GetCamPlane()[i];
#else
		D3DXPLANE &loPlane = GetCamPlane_Ex()[i];
#endif

		if (loPlane.a * lfDeltaX + loPlane.b * lfDeltaY + loPlane.c * lfDeltaZ + loPlane.d < 0.0f)
			j--;
		if (loPlane.a * lfAlfaX + loPlane.b * lfDeltaY + loPlane.c * lfDeltaZ + loPlane.d < 0.0f)
			j--;
		if (loPlane.a * lfDeltaX + loPlane.b * lfAlfaY + loPlane.c * lfDeltaZ + loPlane.d < 0.0f)
			j--;
		if (loPlane.a * lfAlfaX + loPlane.b * lfAlfaY + loPlane.c * lfDeltaZ + loPlane.d < 0.0f)
			j--;
		if (loPlane.a * lfDeltaX + loPlane.b * lfDeltaY + loPlane.c * lfAlfaZ + loPlane.d < 0.0f)
			j--;
		if (loPlane.a * lfAlfaX + loPlane.b * lfDeltaY + loPlane.c * lfAlfaZ + loPlane.d < 0.0f)
			j--;
		if (loPlane.a * lfDeltaX + loPlane.b * lfDeltaY + loPlane.c * lfAlfaZ + loPlane.d < 0.0f)
			j--;
		if (loPlane.a * lfAlfaX + loPlane.b * lfAlfaY + loPlane.c * lfAlfaZ + loPlane.d < 0.0f)
			j--;

		if (0 == j)
			return FALSE;
	}
	return TRUE;
}


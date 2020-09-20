//=============================================================================
//
// データロード処理 [Serialize.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "Serialize.h"
#include "GlobalFunction.h"
#include<stdio.h>
#include<iostream>
#include <fstream>
using namespace std;

SAVEDATA	save;

#ifdef ENGINE_MODE
#include "obj_plant.h"
#include "model.h"
#include "ui.h"
#include "cube.h"
#include "ObjObjModel.h"
#include "md5file.h"


bool SaveData(string SaveFileName)
{
	OBJ_PLANT *ObjPlant = RetObjPlant();
	MODEL *ObjModel = RetObjModel();
	OBJ_CUBE *ObjCube = RetObjCube();
	OBJOBJMODEL *ObjObjModel = RetObjObjModel();
	UI *Ui = RetUi();

	int ListObjMenuIndexId = RetListObjMenuIndexId();
	int *SelectedTypeIdIndex = RetSelectedTypeIdIndex();

	TCHAR* ProjectPath = GetProjectPath(); //获取项目绝对路径
	string SaveFileName1 = ProjectPath; 
	SaveFileName1.append("\\");
	SaveFileName1.append(SaveFileName);
	// プレイヤーの人数分セーブする
	for (int i = 0; i < OBJ_PLANT_MAX; i++)
	{
		if (ObjPlant[i].use == true)
		{
			//save.ObjPlant[i] = ObjPlant[i];
			save.ObjPlant[i].pos = ObjPlant[i].pos;
			save.ObjPlant[i].rot = ObjPlant[i].rot;
			save.ObjPlant[i].scl = ObjPlant[i].scl;
			strcpy(save.ObjPlant[i].TexturePathName, ObjPlant[i].TexturePathName);
			save.ObjPlant[i].TexturePathNameLen = ObjPlant[i].TexturePathNameLen;
			save.ObjPlant[i].use = ObjPlant[i].use;
			/////////////////////////
		}
	}

	for (int i = 0; i < MAX_MODEL_NUM; i++)
	{
		if (ObjModel[i].use == true)
		{
	        //save.ObjModel[i] = ObjModel[i];
			save.ObjModel[i].pos = ObjModel[i].pos;
			save.ObjModel[i].rot = ObjModel[i].rot;
			save.ObjModel[i].scl = ObjModel[i].scl;
			strcpy(save.ObjModel[i].TexturePathName, ObjModel[i].TexturePathName);
			save.ObjModel[i].TexturePathNameLen = ObjModel[i].TexturePathNameLen;
			strcpy(save.ObjModel[i].ModelPathName, ObjModel[i].ModelPathName);
			save.ObjModel[i].ModelPathNameLen = ObjModel[i].ModelPathNameLen;
			save.ObjModel[i].pD3DXBuffMat = ObjModel[i].pD3DXBuffMat;
			save.ObjModel[i].pD3DXMesh = ObjModel[i].pD3DXMesh;
			save.ObjModel[i].nNumMat = ObjModel[i].nNumMat;
			save.ObjModel[i].use = ObjModel[i].use;
		}
	}
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		if (ObjCube[i].use == true)
		{
			//save.ObjCube[i] = ObjCube[i];
			save.ObjCube[i].pos = ObjCube[i].pos;
			save.ObjCube[i].rot = ObjCube[i].rot;
			save.ObjCube[i].scl = ObjCube[i].scl;
			strcpy(save.ObjCube[i].TexturePathName, ObjCube[i].TexturePathName);
			save.ObjCube[i].TexturePathNameLen = ObjCube[i].TexturePathNameLen;
			save.ObjCube[i].use = ObjCube[i].use;
		}
	}
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (ObjObjModel[i].use)
		{
			save.ObjObjModel[i].pos = ObjObjModel[i].pos;
			save.ObjObjModel[i].rot = ObjObjModel[i].rot;
			save.ObjObjModel[i].scl = ObjObjModel[i].scl;
			strcpy(save.ObjObjModel[i].TexturePathName, ObjObjModel[i].TexturePathName);
			save.ObjObjModel[i].TexturePathNameLen = ObjObjModel[i].TexturePathNameLen;
			strcpy(save.ObjObjModel[i].ModelPathName, ObjObjModel[i].ModelPathName);
			save.ObjObjModel[i].ModelPathNameLen = ObjObjModel[i].ModelPathNameLen;
			save.ObjObjModel[i].use = ObjObjModel[i].use;
		}
	}
	///存储UI数据/////
	for (int i = 0; i < MAX_UI_OBJ_NUM; i++)
	{
		if (Ui[i].use == true)
		{
			//save.Ui[i] = Ui[i];
			save.Ui[i].SelectedType = Ui[i].SelectedType;
			save.Ui[i].SelectedTypeIdIndex = Ui[i].SelectedTypeIdIndex;
			save.Ui[i].use = Ui[i].use;
		}
	}
	save.ListObjMenuIndexId = ListObjMenuIndexId;

	for (int i = 0; i < OBJ_INDEX_TYPE_NUM; i++)
	{
		save.SelectedTypeIdIndex[i] = SelectedTypeIdIndex[i];
	}

	ofstream os(SaveFileName1, ios_base::out | ios_base::binary);
	os.write(reinterpret_cast<char *>(&save), sizeof(SAVEDATA));
	os.close();

	return TRUE;
}
#endif // ENGINE_MODE

SAVEDATA* LoadData(string LoadFileName)
{
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		save.ObjObjModel[i].use = false;
	}
	ifstream is(LoadFileName, ios_base::in | ios_base::binary);
	if (is) {
		is.read(reinterpret_cast<char *>(&save), sizeof(save));
	}
	else
	{
		MessageBox(0,"项目载入失败",0,0);
		return false;
	}

	return &save;
}

//=============================================================================
//
// データロード処理 [Serialize.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#include "GlobalFunction.h"
#include "obj_plant_Ex.h"
#include "model_Ex.h"
#include "cube_Ex.h"
#include "ObjObjModel_Ex.h"

//#include "obj_plant.h"
//#include "model.h"
//#include "cube.h"
//#include "ui.h"


struct SAVEDATA
{
	GLOBAL_OBJ_PLANT ObjPlant[OBJ_PLANT_EX_MAX];
	GLOBAL_OBJ_MODEL ObjModel[MAX_MODEL_EX];
	GLOBAL_OBJ_CUBE ObjCube[OBJ_CUBE_EX_MAX];
	GLOBAL_OBJ_OBJMODEL ObjObjModel[OBJ_OBJMODEL_EX_MAX];
	GLOBAL_UI Ui[MAX_UI_OBJ_EX_NUM];
	int ListObjMenuIndexId;
	int SelectedTypeIdIndex[OBJ_INDEX_TYPE_NUM]; //对于ui 每个类型的索引
};


SAVEDATA* LoadData(string LoadFileName);
bool SaveData(string SaveFileName);

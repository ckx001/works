//=============================================================================
//
// シーン処理 [Scene.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once
#include "GlobalFunction.h"
#include<string>
using namespace std;

enum SCENE_ENUM
{
	SCENE_SCENE_GRASS,
	SCENE_SCENE_DESERT,
	SCENE_SCENE_HOME,
	SCENE_SCENE_MAX_NUM = 3
};

enum SMALL_MAP_TYPE
{
	SMALL_MAP_TYPE_BLANK,
	SMALL_MAP_TYPE_CUBE
};


enum CUBE_SPEC_TYPE
{
	CUBE_TYPE_TERRAIN,
	CUBE_TYPE_WALL,
	CUBE_TYPE_BRIDGE
};


struct SMALL_MAP
{
	D3DXVECTOR3 RealPos;
	int Type;
	int CubeIndex; //立方体索引
	bool open;
};

#define MAX_SCENE_NUM (5)  //总共的场景数量


void ImportSceneData(string FileName, int SceneId);
void InitSceneData();
int *RetSceneColliderIndexGrassToDesert(void);
int *RetSceneColliderIndexHomeToGrass(void);
void LoadScene(int SceneId);
int *RetSceneBoundHomeList(void);
int RetSceneBoundHomeIndex(void);
int *RetSceneBoundGrassList(void);
int RetSceneBoundGrassIndex(void);
D3DXVECTOR3 *RetBarrierList(int Scene);
int RetSceneBarrierListIndex(int Scene);
SMALL_MAP *RetSmallMap(int scene, int PosZ_Index);
void CreateCurrentSmallMap(int SceneId);
int *RetSceneColliderIndexGrassDoorToBoss(void);
GLOBAL_OBJ_OBJMODEL *RetGlobalObjObjModel(int scene);
GLOBAL_OBJ_CUBE *RetGlobalObjCube(int scene);
int RetSceneCubeNum(int scene);
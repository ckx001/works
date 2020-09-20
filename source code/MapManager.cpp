//=============================================================================
//
// 地形変更処理 [MapManager.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "MapManager.h"
#include "GlobalFunction.h"
#include "Scene.h"


#define SCENE_GRASS_START_X (-2160.0f)  //定义区域的起点坐标x
#define SCENE_GRASS_START_Z (0.0f)  //定义区域的起点坐标z

struct MAPCUBE
{
	D3DXVECTOR3 pos;
	int type;
	int CubeType; 
	int CubeIndex; //对于的真实cube索引
};

MAPCUBE MapCube[MAX_SCENE_NUM][MAX_MAP_CUBE_Z][MAX_MAP_CUBE_X];

//重新给cube按顺序编号 并标记关键信息
void OrderMapData()
{
	GLOBAL_OBJ_CUBE *GlobalGrassCube = RetGlobalObjCube(SCENE_SCENE_GRASS);

	int GrassCubeNum = RetSceneCubeNum(SCENE_SCENE_GRASS);

	for (int z = 0; z < MAX_MAP_CUBE_Z; z++)
	{
		for (int x = 0; x < MAX_MAP_CUBE_X; x++)
		{
			bool bCubeJudge = false;
			for (int num = 0; num < GrassCubeNum; num++)
			{
				GlobalGrassCube;
				if (GlobalGrassCube[num].pos.x == SCENE_GRASS_START_X + GRID_SIZE * x
					&& GlobalGrassCube[num].pos.y == 0.0f
					&& GlobalGrassCube[num].pos.z == SCENE_GRASS_START_Z + GRID_SIZE * z)
				{
					MapCube[SCENE_SCENE_GRASS][z][x].CubeIndex = num;
					MapCube[SCENE_SCENE_GRASS][z][x].type = SMALL_MAP_TYPE_CUBE;
					///////////////
					if (!strncmp(GlobalGrassCube[num].TexturePathName, "brige.jpg", 9))
					{
						MapCube[SCENE_SCENE_GRASS][z][x].CubeType = CUBE_TYPE_BRIDGE;
					}
					else
						MapCube[SCENE_SCENE_GRASS][z][x].CubeType = CUBE_TYPE_TERRAIN;
				    ///////////////
					bCubeJudge = true;
					break;
				}
			}
			if (bCubeJudge == false)
			{
				MapCube[SCENE_SCENE_GRASS][z][x].type = SMALL_MAP_TYPE_BLANK;
				MapCube[SCENE_SCENE_GRASS][z][x].CubeIndex = -1;
			}
		}
	}
}


//发现对应方块类型的索引 存在返回cube索引 不存在返回-1
int FindTypeCubeIndex(int z, int x, int sceneId, int CubeType)
{
	if(MapCube[sceneId][z][x].CubeType == CUBE_TYPE_TERRAIN)
	{
		return MapCube[sceneId][z][x].CubeIndex;
	}
	return -1;
}
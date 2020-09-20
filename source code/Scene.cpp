//=============================================================================
//
// シーン処理 [Scene.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "Scene.h"
#include "GlobalFunction.h"
#include "Serialize.h"
#include "DataBuf.h"
#include "cube_Ex.h"
#include "BoxCollider.h"
#include "GameAi.h"
#include "shadow.h"
#include "ObjObjModel_Ex.h"
#include "sphereCollider.h"
#include "GameEvent.h"
#include "UiSmallMap.h"
#include "UiPlayerHp.h"
#include "MapManager.h"
#include <string.h>
#include <time.h> 

static int SceneColliderIndexGrassToDesert; //Grass->Desert
static int SceneColliderIndexHomeToGrass; //Home->Grass

static int SceneColliderIndexGrassDoorToBoss; //Grass Door Boss
////////////BoundList//////////////////
static int SceneBoundHomeList[OBJ_CUBE_MAX];
static int SceneBoundHomeIndex = 0;
static int SceneBoundGrassList[OBJ_CUBE_MAX];
static int SceneBoundGrassIndex = 0;
static int TempBoundList[OBJ_CUBE_MAX]; //临时数组
static int TempBoundListIndex = 0;
///////////////////////////////////////
///////////BarrierList///////////
static D3DXVECTOR3 BarrierList[MAX_SCENE_NUM][OBJ_CUBE_MAX];
static int BarrierListIndex[MAX_SCENE_NUM];

static void FindSceneBound();

GLOBAL_OBJ_CUBE g_GlobalObjCube[MAX_SCENE_NUM][OBJ_CUBE_MAX];
GLOBAL_OBJ_OBJMODEL g_GlobalObjObjModel[MAX_SCENE_NUM][OBJ_OBJMODEL_EX_MAX];

static int SceneCubeNum[MAX_SCENE_NUM]; //各个区方块的总数量


void InitSceneData()
{
	//初始化Global Cube
	for (int scene = 0; scene < MAX_SCENE_NUM; scene++)
	{
		for (int i = 0; i < OBJ_CUBE_MAX; i++)
		{
			g_GlobalObjCube[scene][i].g_pD3DVtxBuff = NULL;
			g_GlobalObjCube[scene][i].g_pD3DTexture = NULL;
			g_GlobalObjCube[scene][i].use = false;
		}
		SceneCubeNum[scene] = 0;
	}

	//初始化Obj Model
	for (int scene = 0; scene < MAX_SCENE_NUM; scene++)
	{
		for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
		{
			g_GlobalObjObjModel[scene][i].use = false;
		}
	}

	//GetData("data//MAPDATA//DesertScene");
	//载入场景
	ImportSceneData("data//MAPDATA//GrassScene", SCENE_SCENE_GRASS);
	//ImportSceneData("data//MAPDATA//DesertScene", SCENE_SCENE_DESERT);
	ImportSceneData("data//MAPDATA//HomeScene", SCENE_SCENE_HOME);

	//添加小地图数组
	CreateCurrentSmallMap(SCENE_SCENE_HOME);
	//添加边界检测数组 //特别耗费性能
	//FindSceneBound();
	//重新排列场景
	OrderMapData();
	//game开始载入第一个场景
	LoadScene(SCENE_SCENE_HOME);
	//载入障碍物数组
	LoadSceneBarrierList(SCENE_SCENE_HOME);
	//载入敌人
	LoadEnemy(SCENE_SCENE_HOME);
	LoadItem();
	//创建区域触发器
	SceneColliderIndexHomeToGrass = CreateBoxCollider(SCENE_PORTAL_COLLIDER, D3DXVECTOR3{ 486.0f,20.0f,1099.0f }, D3DXVECTOR3{ 0.0f,0.0f,0.0f }, D3DXVECTOR3{ 3.5f,2.0f,2.0f }, TRUE);
	SceneColliderIndexGrassToDesert = CreateBoxCollider( SCENE_PORTAL_COLLIDER, D3DXVECTOR3{ 0.0f,20.0f,80.0f }, D3DXVECTOR3{ 0.0f,0.0f,0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f }, FALSE);
	//Grass Scene boss门前事件触发器
	SceneColliderIndexGrassDoorToBoss = CreateBoxCollider(SCENE_EVENT_COLLIDER, D3DXVECTOR3{ -66.0f, 20.0f, 4901.0f }, D3DXVECTOR3{ 0.0f,0.0f,0.0f }, D3DXVECTOR3{ 2.0f,2.0f,2.0f }, FALSE);

}


void AddBarrierList(int Scene, int num, D3DXVECTOR3 value)
{
	BarrierList[Scene][num] = value;
}


////////SmallMap////////////
SMALL_MAP SmallMap[SCENE_SCENE_MAX_NUM][MAX_UI_SMALL_MAP_Y][MAX_UI_SMALL_MAP_X];
void CreateCurrentSmallMap(int SceneId)
{

	float StartPos_x = -(MAX_UI_SMALL_MAP_X / 2) * GRID_SIZE;
	float StartPos_z = -(MAX_UI_SMALL_MAP_Y / 8) * GRID_SIZE;
	//start_pos.x = -2580.0f;
	for (int z = 0; z < MAX_UI_SMALL_MAP_Y; z++)
	{
		for (int x = 0; x < MAX_UI_SMALL_MAP_X; x++)
		{
			SmallMap[SceneId][z][x].RealPos = D3DXVECTOR3{ StartPos_x + x * GRID_SIZE, 0.0f, StartPos_z + z * GRID_SIZE };
			bool blank = true;
			for (int i = 0; g_GlobalObjCube[SceneId][i].use == true; i++)
			{
				if (g_GlobalObjCube[SceneId][i].pos.x == SmallMap[SceneId][z][x].RealPos.x
					&& g_GlobalObjCube[SceneId][i].pos.z == SmallMap[SceneId][z][x].RealPos.z)
				{
					SmallMap[SceneId][z][x].Type = SMALL_MAP_TYPE_CUBE;
					SmallMap[SceneId][z][x].CubeIndex = i;
					blank = false;
					break;
				}
			}
			if (blank)
			{
				SmallMap[SceneId][z][x].Type = SMALL_MAP_TYPE_BLANK;
				SmallMap[SceneId][z][x].CubeIndex = -1;
			}
		}
	}
}
/////////////

bool FindSceneScreen(int SceneId, float x, float z, bool bIndex)
{
	bool judge = false;

	int TempIndex = 0;
	int TempList[OBJ_CUBE_MAX];
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		if (g_GlobalObjCube[SceneId][i].use == true)
		{
			if (g_GlobalObjCube[SceneId][i].pos.x == x &&
				g_GlobalObjCube[SceneId][i].pos.z == z)
			{
				judge = true;
				//可能会有不同高度的立方体 此处用数组
				TempList[TempIndex] = i;
				TempIndex++;
			}
		}
	}
	if (bIndex == true && judge == true)
	{
		if (TempIndex > 0)
		{
			for (int i = 0; i < TempBoundListIndex; i++)
				TempBoundList[i] = -1;
			TempBoundListIndex = 0;

			for (int i = 0; i < TempIndex; i++)
			{
				TempBoundList[i] = TempList[i];
				TempBoundListIndex++;
			}
		}
	}

	//当前judge
	if(judge)
		return true;

	//当前pos为false 上一个格为true 记录上一格
	if (judge == false && bIndex == true)
	{
		//Home
		if (SceneId == 2)
		{
			for (int i = 0; i < TempBoundListIndex; i++)
			{
				SceneBoundHomeList[SceneBoundHomeIndex] = TempBoundList[i];
				SceneBoundHomeIndex++;
			}
		}
		//Grass
		else if (SceneId == 0)
		{
			for (int i = 0; i < TempBoundListIndex; i++)
			{
				SceneBoundGrassList[SceneBoundGrassIndex] = TempBoundList[i];
				SceneBoundGrassIndex++;
			}
		}
		else if (SceneId == 1)
		{
		}
	}

	return false;
}

//检索各个区域边界并把边界加入数组(去掉边界的影子)
//目前只检测右上边界
void FindSceneBound()
{
	for (int SceneId = 0; SceneId < 3; SceneId++)
	{
		//落脚点边界立方体的pos 
		//索引起点:左下角
		bool bBound_x = true;
		bool bBound_z = true;
		D3DXVECTOR3 start_pos = g_GlobalObjCube[SceneId][0].pos;
		if (SceneId == 0 || SceneId == 1)
		{
			//start_pos.x = -2580.0f;
			for (int z = 0; z < 160; z++)
			{
				for (int x = 0; x < 160; x++)
				{
					bBound_x = FindSceneScreen( SceneId, start_pos.x + x * GRID_SIZE, start_pos.z + z * GRID_SIZE, bBound_x );
				}
			}
			for (int x = 0; x < 160; x++)
			{
				for (int z = 0; z < 160; z++)
				{
					//从z坐标开始竖向遍历
					bBound_z = FindSceneScreen(SceneId, start_pos.x + x * GRID_SIZE, start_pos.z + z * GRID_SIZE, bBound_z);
				}
			}
		}
		else if (SceneId == 2)
		{
			for (int z = 0; z < 34; z++)
			{
				for (int x = 0; x < 34; x++)
				{
					bBound_x = FindSceneScreen(SceneId, start_pos.x + x * GRID_SIZE, start_pos.z + z * GRID_SIZE, bBound_x);
				}
			}
			for (int x = 0; x < 34; x++)
			{
				for (int z = 0; z < 34; z++)
				{
					//从z坐标开始竖向遍历
					bBound_z = FindSceneScreen(SceneId, start_pos.x + x * GRID_SIZE, start_pos.z + z * GRID_SIZE, bBound_z);
				}
			}
		}
	}
}


void ImportSceneData(string FileName, int SceneId )
{
	SAVEDATA* SerializeData = LoadData(FileName);
	//重写cube
	int num = 0;
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		if (SerializeData->ObjCube[i].use == true)
		{
			SceneCubeNum[SceneId]++;
			g_GlobalObjCube[SceneId][i].pos = SerializeData->ObjCube[i].pos;
			g_GlobalObjCube[SceneId][i].rot = SerializeData->ObjCube[i].rot;
			g_GlobalObjCube[SceneId][i].scl = SerializeData->ObjCube[i].scl;
			g_GlobalObjCube[SceneId][i].TexturePathNameLen = SerializeData->ObjCube[i].TexturePathNameLen;
			strcpy(g_GlobalObjCube[SceneId][i].TexturePathName, SerializeData->ObjCube[i].TexturePathName);
			strcpy(g_GlobalObjCube[SceneId][i].tag, SerializeData->ObjCube[i].tag);
			g_GlobalObjCube[SceneId][i].use = true;

			if (g_GlobalObjCube[SceneId][i].pos.y > 10.0f)
			{
				AddBarrierList(SceneId, num, g_GlobalObjCube[SceneId][i].pos);
				num++;
			}
		}
	}
	//重写ObjObjModel
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (SerializeData->ObjObjModel[i].use == true)
		{
			g_GlobalObjObjModel[SceneId][i].pos = SerializeData->ObjObjModel[i].pos;
			g_GlobalObjObjModel[SceneId][i].rot = SerializeData->ObjObjModel[i].rot;
			g_GlobalObjObjModel[SceneId][i].scl = SerializeData->ObjObjModel[i].scl;
			g_GlobalObjObjModel[SceneId][i].TexturePathNameLen = SerializeData->ObjObjModel[i].TexturePathNameLen;
			strcpy(g_GlobalObjObjModel[SceneId][i].TexturePathName, SerializeData->ObjObjModel[i].TexturePathName);
			g_GlobalObjObjModel[SceneId][i].ModelPathNameLen = SerializeData->ObjObjModel[i].ModelPathNameLen;
			strcpy(g_GlobalObjObjModel[SceneId][i].ModelPathName, SerializeData->ObjObjModel[i].ModelPathName);
			g_GlobalObjObjModel[SceneId][i].ObjGroupNum = SerializeData->ObjObjModel[i].ObjGroupNum;
			g_GlobalObjObjModel[SceneId][i].matGroups = SerializeData->ObjObjModel[i].matGroups;
			strcpy(g_GlobalObjObjModel[SceneId][i].tag, SerializeData->ObjObjModel[i].tag);
			for (int c = 0; c < 100; c++)
			{
				g_GlobalObjObjModel[SceneId][i].TrianglesList[c] = SerializeData->ObjObjModel[i].TrianglesList[c];
				for (int d = 0; d < 3; d++)
					g_GlobalObjObjModel[SceneId][i].Tf[c][d] = SerializeData->ObjObjModel[i].Tf[c][d];

			}
			for (int group = 0; group < g_GlobalObjObjModel[SceneId][i].matGroups; group++)
			{
				strcpy(g_GlobalObjObjModel[SceneId][i].TextureListPath[group].TextureListPathName, SerializeData->ObjObjModel[i].TextureListPath[group].TextureListPathName);
				g_GlobalObjObjModel[SceneId][i].TextureListPath[group].TextureListPathNameLen = SerializeData->ObjObjModel[i].TextureListPath[group].TextureListPathNameLen;
			}
			g_GlobalObjObjModel[SceneId][i].use = true;
		}
	}

	//设置障碍物数组索引数量
	BarrierListIndex[SceneId] = num;

}


void LoadScene(int SceneId)
{
	OBJ_CUBE_EX *ObjCube_Ex = RetObjCube_Ex();
	OBJOBJMODEL_EX *ObjObjModel_Ex = RetObjObjModel_Ex();

	float Dir0 = 0.0f;
	float Dir1 = D3DX_PI / 2;
	float Dir2 = D3DX_PI;
	float Dir3 = D3DX_PI / 2 * 3;
	srand((unsigned)time(NULL));

	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		if (g_GlobalObjCube[SceneId][i].use == true)
		{
			ObjCube_Ex[i].pos = g_GlobalObjCube[SceneId][i].pos;
			ObjCube_Ex[i].rot = g_GlobalObjCube[SceneId][i].rot;
			ObjCube_Ex[i].scl = g_GlobalObjCube[SceneId][i].scl;
			ObjCube_Ex[i].TexturePathNameLen = g_GlobalObjCube[SceneId][i].TexturePathNameLen;
			strcpy(ObjCube_Ex[i].TexturePathName, g_GlobalObjCube[SceneId][i].TexturePathName);
			strcpy(ObjCube_Ex[i].tag, g_GlobalObjCube[SceneId][i].tag);
			ObjCube_Ex[i].use = true;

			//添加阴影触发器
			ObjCube_Ex[i].ShadowIdx = CreateShadow(ObjCube_Ex[i].pos, ObjCube_Ex[i].rot, ObjCube_Ex[i].scl, true);
	
			int ChgListIndex = RetTexuturePointListMapIndex(ObjCube_Ex[i].TexturePathNameLen, ObjCube_Ex[i].TexturePathName);
			ReLoadObjCube_Ex(i, ChgListIndex);

			//rot四个方向随机化
			int value = rand() % 4;

			//if (strncmp(ObjCube_Ex[i].TexturePathName, "brige.jpg", 9))
			//{
			//if (value == 0)
			//	ObjCube_Ex[i].rot.y = Dir0;
			//else if (value == 1)
			//	ObjCube_Ex[i].rot.y = Dir1;
			//else if (value == 2)
			//	ObjCube_Ex[i].rot.y = Dir2;
			//else if (value == 3)
			//	ObjCube_Ex[i].rot.y = Dir3;
			//}
		}
	}

	//加载obj Model
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (g_GlobalObjObjModel[SceneId][i].use == true)
		{
			ObjObjModel_Ex[i].pos = g_GlobalObjObjModel[SceneId][i].pos;
			ObjObjModel_Ex[i].rot = g_GlobalObjObjModel[SceneId][i].rot;
			ObjObjModel_Ex[i].scl = g_GlobalObjObjModel[SceneId][i].scl;
			ObjObjModel_Ex[i].TexturePathNameLen = g_GlobalObjObjModel[SceneId][i].TexturePathNameLen;
			strcpy(ObjObjModel_Ex[i].TexturePathName, g_GlobalObjObjModel[SceneId][i].TexturePathName);
			ObjObjModel_Ex[i].ModelPathNameLen = g_GlobalObjObjModel[SceneId][i].ModelPathNameLen;
			strcpy(ObjObjModel_Ex[i].ModelPathName, g_GlobalObjObjModel[SceneId][i].ModelPathName);
			ObjObjModel_Ex[i].ObjGroupNum = g_GlobalObjObjModel[SceneId][i].ObjGroupNum;
			ObjObjModel_Ex[i].matGroups = g_GlobalObjObjModel[SceneId][i].matGroups;
			strcpy(ObjObjModel_Ex[i].tag, g_GlobalObjObjModel[SceneId][i].tag);
			for (int c = 0; c < 100; c++)
			{
				ObjObjModel_Ex[i].TrianglesList[c] = g_GlobalObjObjModel[SceneId][i].TrianglesList[c];
				for (int d = 0; d < 3; d++)
					ObjObjModel_Ex[i].Tf[c][d] = g_GlobalObjObjModel[SceneId][i].Tf[c][d];

			}
			for (int group = 0; group < ObjObjModel_Ex[i].matGroups; group++)
			{
				strcpy(ObjObjModel_Ex[i].TextureListPath[group].TextureListPathName, g_GlobalObjObjModel[SceneId][i].TextureListPath[group].TextureListPathName);
				ObjObjModel_Ex[i].TextureListPath[group].TextureListPathNameLen = g_GlobalObjObjModel[SceneId][i].TextureListPath[group].TextureListPathNameLen;
			}
			
			ObjObjModel_Ex[i].use = true;

			//添加阴影触发器
			//ObjCube_Ex[i].ShadowIdx = CreateShadow(ObjCube_Ex[i].pos, ObjCube_Ex[i].rot, ObjCube_Ex[i].scl, true);

			int ChgListIndex = RetTexuturePointListMapIndex(ObjObjModel_Ex[i].TexturePathNameLen, ObjObjModel_Ex[i].TexturePathName);
			ReLoadObjObjModelTexture_Ex(i, ChgListIndex, OBJOBJ_TEXTURE_TYPE_TEXTURE, 0);

			//重载Texture数组
            //如果自定义纹理没有设置
			for (int group = 0; group < ObjObjModel_Ex[i].matGroups; group++)
			{
				int ChgTextureListIndex = RetTexuturePointListMapIndex(ObjObjModel_Ex[i].TextureListPath[group].TextureListPathNameLen, ObjObjModel_Ex[i].TextureListPath[group].TextureListPathName);
				ReLoadObjObjModelTexture_Ex(i, ChgTextureListIndex, OBJOBJ_TEXTURE_TYPE_TEXTURELIST, group);
			}
			ReLoadObjObjModelModel_Ex(i, ObjObjModel_Ex[i].ModelPathName);

			//加载碰撞触发器
			//if(strncmp(ObjObjModel_Ex[i].tag, "red", strlen(ObjObjModel_Ex[i].tag)) == NULL)
			//    ObjObjModel_Ex[i].SphereColliderIdx = CreateSphereCollider(ObjObjModel_Ex[i].pos, ObjObjModel_Ex[i].rot, ObjObjModel_Ex[i].scl);
		}
	}
}


int *RetSceneColliderIndexGrassToDesert()
{
	return &SceneColliderIndexGrassToDesert;
}

int *RetSceneColliderIndexHomeToGrass()
{
	return &SceneColliderIndexHomeToGrass;
}

int *RetSceneColliderIndexGrassDoorToBoss()
{
	return &SceneColliderIndexGrassDoorToBoss;
}


int *RetSceneBoundHomeList()
{
	return &SceneBoundHomeList[0];
}

int RetSceneBoundHomeIndex()
{
	return SceneBoundHomeIndex;
}

int *RetSceneBoundGrassList()
{
	return &SceneBoundGrassList[0];
}

int RetSceneBoundGrassIndex()
{
	return SceneBoundGrassIndex;
}

D3DXVECTOR3 *RetBarrierList(int Scene)
{
	return &BarrierList[Scene][0];
}

int RetSceneBarrierListIndex(int Scene)
{
	return BarrierListIndex[Scene];
}

SMALL_MAP *RetSmallMap(int scene, int PosZ_Index )
{
	return &SmallMap[scene][PosZ_Index][0];
}


GLOBAL_OBJ_OBJMODEL *RetGlobalObjObjModel(int scene)
{
	return &g_GlobalObjObjModel[scene][0];
}


GLOBAL_OBJ_CUBE *RetGlobalObjCube(int scene)
{
	return &g_GlobalObjCube[scene][0];
}


int RetSceneCubeNum(int scene)
{
	return SceneCubeNum[scene];
}
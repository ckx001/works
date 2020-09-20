//=============================================================================
//
// Ai処理 [GameAi.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "GameAi.h"
#include <iostream>
#include <queue>
#include "cube_Ex.h"
#include "player1.h"
#include "Enemy_wolf.h"
#include "GlobalFunction.h"
#include "GameMath.h"
#include <vector>
#include <math.h>
#include "input.h"
#include "GameEvent.h"
#include "Time_c.h"
#include "debugproc.h"

using namespace std;

#define AI_CHECK_RANGE ( 15.0f )  //ai检测格子的距离
#define ENEMY_WOLF_MOVE_LERP_TIME (0.2f)

struct CLASS_POINT
{
	D3DXVECTOR2 pos;
	float cost;
	D3DXVECTOR2 parent;
	int parentIndex;
	bool use;
};


enum DIR
{
	DIR_LEFT,
	DIR_TOP,
	DIR_RIGHT,
	DIR_DOWN
};



static D3DXVECTOR2 RoutePath[ENEMY_WOLF_MAX_NUM][500];
static int RoutePathNum[ENEMY_WOLF_MAX_NUM];

static bool EnemyMoveAi(int index, float Point_x, float Point_z);
static int blackListIndex;
static D3DXVECTOR2 CurrentPos;

static bool bEnemyMove[ENEMY_WOLF_MAX_NUM];
static int EnemyMoveRouteListIndex = -1;
static bool bEnemyAstart[ENEMY_WOLF_MAX_NUM];

static bool bEnemyNextRouteAllow[ENEMY_WOLF_MAX_NUM]; //走到下一个节点后才能进行重新规划路线
////////障碍物///////////////////
////////Home Scene//////////
static D3DXVECTOR3 HomeCubeBarrierList[OBJ_CUBE_MAX];  //立方体地形障碍物位置数组
static int HomeCubeBarrierListIndex = 0;

static D3DXVECTOR3 CurrentCubeBarrierList[OBJ_CUBE_MAX];  //立方体地形障碍物位置数组
static int CurrentCubeBarrierListIndex = 0;
/////////////////////////////
////////实际移动全局变量////////
static float gEnemyWolfMoveTime[ENEMY_WOLF_MAX_NUM];
static int RoutePathCurrent[ENEMY_WOLF_MAX_NUM];
static D3DXVECTOR2 StartPos[ENEMY_WOLF_MAX_NUM];
static float StartRot;
static bool MoveJudge[ENEMY_WOLF_MAX_NUM];
static int RoutePathNumOld[ENEMY_WOLF_MAX_NUM];
////////////////////////////////

static CLASS_POINT Point[10000];
static D3DXVECTOR3 old_pos[ENEMY_WOLF_MAX_NUM];

CLASS_POINT OpenSetList[10000];
int OpenSetListIndex = 0;
CLASS_POINT CloseSetList[10000];
int CloseSetListIndex = 0;


void InitGameAi()
{
	OBJ_CUBE_EX *CubeObj = RetObjCube_Ex();
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	PLAYER1 *Player = GetPlayer1();

	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		old_pos[num] = Player->pos;
		bEnemyMove[num] = true;
		bEnemyAstart[num] = false;
		RoutePathCurrent[num] = 0;
		RoutePathNum[num] = 0;
		MoveJudge[num] = false;
		gEnemyWolfMoveTime[num] = 0.0f;
		bEnemyNextRouteAllow[num] = true;
	}
}

static float startRot;

void UpdateGameAi()
{
	//DWORD *timing = RetTiming();
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	PLAYER1 *Player = GetPlayer1();
	int DeviceStatus = RetCurrentDeviceStatus();


	//if (Player->pos != old_pos)
	//{
	//    for(int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	//    {
	//    	//if (EnemyWolf[num].use)
	//    	{
	//    		if (MoveJudge[num])
	//    		{
	//    			for(int i = 0; i < 500; i++)
	//    			{
	//    				RoutePath[num][i].x = NULL;
	//    				RoutePath[num][i].y = NULL;
	//    			}
	//    
	//    			RoutePathNum[num] = 0;
	//    			RoutePathNumOld[num] = 0;
	//    			bEnemyMove[num] = true;
	//    			MoveJudge[num] = false;
	//    		}
	//    	}
	//    }
	//	old_pos = Player->pos;
	//}

	//PrintDebugProc("time = %d\n", (int)timing[TIMING_ENEMY_WOLF_WALK]);
	if (DeviceStatus == SYS_DEVICERUN)
	{
		for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
		{
			if (EnemyWolf[num].use)
			{
				if (bEnemyAstart[num])
					//&& EnemyWolf[num].ColliderHit == false
				{
					//if (bEnemyMove[num])
					if (bEnemyNextRouteAllow[num])
					{
						if (Player->pos != old_pos[num])
						{
							//PrintDebugProc("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
							bool AiValue = EnemyMoveAi(num, EnemyWolf[num].pos.x, EnemyWolf[num].pos.z);
							if (AiValue == true)
							{
								//开始点为当前位置
								RoutePathCurrent[num] = RoutePathNum[num] - 1;
								//开始点为当前位置下一步
								//RoutePathCurrent[num] = RoutePathNum[num] - 2;
								StartPos[num] = RoutePath[num][RoutePathCurrent[num]];
								bEnemyMove[num] = false;
								gEnemyWolfMoveTime[num] = 0.0f;
								//RoutePathNumOld[num] = RoutePathCurrent[num];

								startRot = EnemyWolf[num].rot.y;
							}
							old_pos[num] = Player->pos;
						}
					}

					if (RoutePathCurrent[num] != -1)
					{
						gEnemyWolfMoveTime[num] = gEnemyWolfMoveTime[num] + ENEMY_WOLF_MOVE_LERP_TIME;
						//D3DXVECTOR2 resultPos = RoutePath[num][RoutePathCurrent[num]];
						D3DXVECTOR2 resultPos = D2_Lerp(StartPos[num], RoutePath[num][RoutePathCurrent[num] - 1] , gEnemyWolfMoveTime[num]);
						int aaaaa = RoutePathCurrent[num] - 1;
						EnemyWolf[num].pos.x = resultPos.x;
						EnemyWolf[num].pos.z = resultPos.y;

						float MoveRot = RetRotValue(StartPos[num], RoutePath[num][RoutePathCurrent[num] - 1]);

						//float LerpRot = F_Lerp(startRot, MoveRot, gEnemyWolfMoveTime[num]);
						//float MoveRot = atan2((Player->pos.z - EnemyWolf[num].pos.z), (Player->pos.x - EnemyWolf[num].pos.x));
						EnemyWolf[num].rot.y = 3 * D3DX_PI / 2 - MoveRot;
						
						if (resultPos.x == RoutePath[num][RoutePathCurrent[num] - 1].x && resultPos.y == RoutePath[num][RoutePathCurrent[num] - 1].y)
						{
							bEnemyNextRouteAllow[num] = true;
							RoutePathCurrent[num]--;
							StartPos[num] = RoutePath[num][RoutePathCurrent[num]];
							gEnemyWolfMoveTime[num] = 0.0f;

							startRot = EnemyWolf[num].rot.y;
						}
						else
							bEnemyNextRouteAllow[num] = false;
					}
				}
			}
		}
	}

}


float BaseCost(int index, D3DXVECTOR2 p)
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();

	float x_dis = fabs(p.x - EnemyWolf[index].pos.x);
	float y_dis = fabs(p.y - EnemyWolf[index].pos.z);
	//与开始点的距离
	return (x_dis + y_dis) + (sqrt(2) - 2.0f) * min(x_dis, y_dis);
	//欧几里得距离
	//return sqrt(x_dis * x_dis + y_dis * y_dis);
}

float HeuristicCost(D3DXVECTOR2 p)
{
	PLAYER1 *Player = GetPlayer1();

	float x_dis = fabs(Player->pos.x - p.x);
	float y_dis = fabs(Player->pos.z - p.y);
    //与结束点的距离
	//float cost = (x_dis + y_dis) + (sqrt(2) - 2.0f) * min(x_dis, y_dis);
	return (x_dis + y_dis) + (sqrt(2) - 2.0f) * min(x_dis, y_dis);
	//欧几里得距离
	//return sqrt(x_dis * x_dis + y_dis * y_dis);
}

float TotalCost(D3DXVECTOR2 p)
{
	//return BaseCost(p) + HeuristicCost(p);
	return HeuristicCost(p);
}

int SelectPointInOpenList()
{
	//int index = 0;
	int selected_index = -1;
	int min_cost = 2147483647;
	for (int i= 0; OpenSetList[i].use == true; i++)
	{
		//计算开销
		float cost = TotalCost(OpenSetList[i].pos);
		if (cost < min_cost)
		{
			min_cost = cost;
			selected_index = i;
		}
	}
	return selected_index;
}


bool IsInOpenList(D3DXVECTOR2 p)
{
	for (int i = 0; OpenSetList[i].use == true; i++)
	{
		if (OpenSetList[i].pos.x == p.x && OpenSetList[i].pos.y == p.y)
		{
			return true;
		}
	}
	return false;
}


bool IsInCloseList(D3DXVECTOR2 p)
{
	for (int i = 0; CloseSetList[i].use == true; i++)
	{
		if (CloseSetList[i].pos.x == p.x && CloseSetList[i].pos.y == p.y)
		{
			return true;
		}
	}
	return false;
}


static bool IsValidJudge(int index, D3DXVECTOR2 p)
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	//判断坐标是否在障碍物中
	for (int i = 0; i < CurrentCubeBarrierListIndex; i++)
	{
		if (CurrentCubeBarrierList[i].x + GRID_SIZE / 2 >= p.x && CurrentCubeBarrierList[i].x - GRID_SIZE / 2 <= p.x
			&& CurrentCubeBarrierList[i].z + GRID_SIZE / 2 >= p.y && CurrentCubeBarrierList[i].z - GRID_SIZE / 2 <= p.y)
			return true;
	}
	//判断绕过其它Wolf 目前作用不大
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		if (num != index)
		{
			if (EnemyWolf[num].use)
			{
				D3DXVECTOR3 vDistance = EnemyWolf[num].pos - EnemyWolf[index].pos;
				//vDistanceのベクトル長を得る
				//length = D3DXVec3Length(&vDistance);
				float length = D3DXVec3Length(&vDistance);
				if (length <= 60.0f)
				{
					return true;
				}
			}
		}
	}

	return false;
}


void ProcessPoint(int index, float x, float y)
{
	//////判断是否有效 是否撞到碰撞体////
	if (IsValidJudge(index, D3DXVECTOR2{ x, y }))
	    return;

	CLASS_POINT p;
	p.pos = D3DXVECTOR2{ x, y };
	//Point[index].pos = D3DXVECTOR2{ x, y };
	if (IsInCloseList(p.pos))
	{
		return;
	}
	if (IsInOpenList(p.pos) == false)
	{
		p.parent = Point[index].pos;
		p.parentIndex = index;
		p.cost = TotalCost(p.pos);
		OpenSetList[OpenSetListIndex].pos = p.pos;
		OpenSetList[OpenSetListIndex].use = true;
		OpenSetList[OpenSetListIndex].cost = p.cost;
		OpenSetList[OpenSetListIndex].parent = p.parent;
		OpenSetList[OpenSetListIndex].parentIndex = p.parentIndex;
		OpenSetListIndex++;
		//Point[index].parentIndex;

		return;
	}
}

void DelOpenSetList(int Index)
{

	for (int i = Index + 1; OpenSetList[i].use == true; i++)
	{
		OpenSetList[i - 1] = OpenSetList[i];
	}
	OpenSetList[OpenSetListIndex].pos.x = NULL;
	OpenSetList[OpenSetListIndex].pos.y = NULL;
	OpenSetList[OpenSetListIndex].use = false;
	OpenSetListIndex--;
}


bool IsStartPoint( int index, D3DXVECTOR2 p )
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();

	if (p.x == EnemyWolf[index].pos.x && p.y == EnemyWolf[index].pos.z)
	{
		return true;
	}
	return false;
}


static void BuildPath(int EnemyIndex, int index)
{
	RoutePathNum[EnemyIndex] = 0;
	int ParentIndex;
	while (true)
	{
		if (RoutePathNum[EnemyIndex] == 500)
			return;
		if (IsStartPoint(EnemyIndex, Point[index].pos))
			return;
		else
		{
			ParentIndex = Point[index].parentIndex;
			RoutePath[EnemyIndex][RoutePathNum[EnemyIndex]] = Point[ParentIndex].pos;
			Point[index] = Point[ParentIndex];
		}
		RoutePathNum[EnemyIndex]++;
	}
}


bool EnemyMoveAi(int Enemyindex, float Point_x, float Point_z)
{
	//ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	PLAYER1 *Player = GetPlayer1();

	OpenSetListIndex = 0;
	CloseSetListIndex = 0;
	int PointIndex = 0;
	//初始化Open列表
	for (int i = 0; i < 10000; i++)
	{
		OpenSetList[i].use = false;
	}
	//初始化Close列表
	for (int i = 0; i < 10000; i++)
	{
		CloseSetList[i].use = false;
	}

	Point[PointIndex].pos = D3DXVECTOR2{ Point_x, Point_z };
	Point[PointIndex].cost = 0;
	Point[PointIndex].parentIndex = -1;

	OpenSetList[OpenSetListIndex].pos = Point[PointIndex].pos;
	OpenSetList[OpenSetListIndex].cost = Point[PointIndex].cost;
	OpenSetList[OpenSetListIndex].use = true;
	OpenSetListIndex++;

	while (true)
	{
		if (PointIndex == 10000)
			return false;

		int index = SelectPointInOpenList();
		
		if (index < 0)
			return false;

		Point[PointIndex].pos = OpenSetList[index].pos;
		Point[PointIndex].cost = OpenSetList[index].cost;
		Point[PointIndex].parent = OpenSetList[index].parent;
		Point[PointIndex].parentIndex = OpenSetList[index].parentIndex;

		DelOpenSetList(index);

		CloseSetList[CloseSetListIndex].pos = Point[PointIndex].pos;
		CloseSetList[CloseSetListIndex].cost = Point[PointIndex].cost;
		CloseSetList[CloseSetListIndex].use = true;
		CloseSetListIndex++;

		float x = Point[PointIndex].pos.x;
		float y = Point[PointIndex].pos.y;

		ProcessPoint( PointIndex, x - AI_CHECK_RANGE, y + AI_CHECK_RANGE);
		ProcessPoint( PointIndex, x - AI_CHECK_RANGE, y);
		ProcessPoint( PointIndex, x - AI_CHECK_RANGE, y - AI_CHECK_RANGE);
		ProcessPoint( PointIndex, x, y - AI_CHECK_RANGE);
		ProcessPoint( PointIndex, x, y + AI_CHECK_RANGE);
		ProcessPoint( PointIndex, x + AI_CHECK_RANGE, y - AI_CHECK_RANGE);
		ProcessPoint( PointIndex, x + AI_CHECK_RANGE, y);
		ProcessPoint( PointIndex, x + AI_CHECK_RANGE, y + AI_CHECK_RANGE);

		if (Point[PointIndex].pos.x + GRID_SIZE / 2 >= Player->pos.x && Point[PointIndex].pos.x - GRID_SIZE / 2 <= Player->pos.x
			&& Point[PointIndex].pos.y + GRID_SIZE / 2 >= Player->pos.z && Point[PointIndex].pos.y - GRID_SIZE / 2 <= Player->pos.z)
		{
			BuildPath(Enemyindex, PointIndex);
			return true;
		}
		PointIndex++;
	}
}


//计算两个二维坐标的夹角弧度
//float MoveRot = RetRotValue(StartPos, RoutePath[RoutePathCurrent]);
//EnemyWolf->rot.y = 2 * D3DX_PI - MoveRot;
//p1 起点 p2 终点
float RetRotValue(D3DXVECTOR2 p1 , D3DXVECTOR2 p2 )
{
	return atan2((p2.y - p1.y), (p2.x - p1.x));
}


D3DXVECTOR3 *RetCurrentCubeBarrierList()
{
	return &CurrentCubeBarrierList[0];
}

void SetCurrentCubeBarrierListIndex(int index)
{
	CurrentCubeBarrierListIndex = index;
}

void SetbEnemyAstart(int index, bool judge)
{
	bEnemyAstart[index] = judge;
}


void ResetEnemyAi(int num)
{
	PLAYER1 *Player = GetPlayer1();

	old_pos[num] = Player->pos;
	bEnemyMove[num] = true;
	bEnemyAstart[num] = false;
	RoutePathCurrent[num] = 0;
	RoutePathNum[num] = 0;
	MoveJudge[num] = false;
	gEnemyWolfMoveTime[num] = 0.0f;
	bEnemyNextRouteAllow[num] = true;
}
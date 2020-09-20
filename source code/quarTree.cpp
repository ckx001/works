//=============================================================================
//
// quad tree処理 [quarTree.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "quarTree.h"
#include "cube_Ex.h"
#include "GlobalFunction.h"
//#include "player.h"
#include "player1.h"
#include "debugproc.h"
#include "hit_event.h"
#include "sphereCollider.h"
#include "Enemy_wolf.h"
#include "EnemyCannibal.h"
#include "EnemyBossWolf.h"
#include "cylinderCollider.h"
#include "bullet.h"
#include "effect.h"

#define BOUNDARY_VALUE ( GRID_SIZE )
#define MAX_NUM_Z (16)
#define MAX_NUM_X (16)

void ColliderEvent(int type, int TypeIndex, int scene_x, int scene_z);


static int HitCubeIndex = 0;
HITCUBE HitCube[100];
QUARTREE g_quarTree[MAX_NUM_X][MAX_NUM_Z];

static int g_PlayerNowCubeIndex = -1; //玩家踩着的cube


void InitHitCube()
{
	for (int i = 0; i < 100; i++)
	{
		HitCube[i].use = false;
	}
}


//pos:左下角世界坐标x,z
//size:地图尺寸 分辨率 128*128 高度图每一个格子为1
void InitQuarTree(float pos_x, float pos_z, int size_x, int size_z )
{
	OBJ_CUBE_EX *ObjCube = RetObjCube_Ex();
	//从左下角开始遍历
	for (int z = 0; z < MAX_NUM_Z; z++)
	{
		for (int x = 0; x < MAX_NUM_X; x++)
		{
			for( int c = 0; c < 5000; c++)
			    g_quarTree[x][z].CheckIndexList[c] = -1;
			g_quarTree[x][z].MinValue = D3DXVECTOR2{ pos_x + x * size_x * GRID_SIZE / 8 , pos_z + z * size_z * GRID_SIZE / 8 };
			g_quarTree[x][z].MaxValue = D3DXVECTOR2{ pos_x + (x + 1) * (size_x * GRID_SIZE / 8), pos_z + ( z + 1) * size_z * GRID_SIZE / 8  };
		    
			int num = 0;
		    /////////将需要检测的Cube的索引插入到quarTree中//////
			for (int i = 0; i < OBJ_CUBE_EX_MAX; i++)
			{
				if (ObjCube[i].use)
				{
					{
						if (ObjCube[i].pos.x >= g_quarTree[x][z].MinValue.x &&
							ObjCube[i].pos.z >= g_quarTree[x][z].MinValue.y &&
							ObjCube[i].pos.x < g_quarTree[x][z].MaxValue.x &&
							ObjCube[i].pos.z < g_quarTree[x][z].MaxValue.y)
						{
							g_quarTree[x][z].CheckIndexList[num] = i;
							num++;
						}
					}
				}
			}
			g_quarTree[x][z].CheckIndexListLen = num;
		}
	}
	
}


void UpdateQuarTree()
{
	PLAYER1 *Player = GetPlayer1();

	for (int z = 0; z < MAX_NUM_Z; z++)
	{
		for (int x = 0; x < MAX_NUM_X; x++)
		{
			//Player Event
			if (Player->use)
			{
				if (Player->pos.x >= g_quarTree[x][z].MinValue.x &&
					Player->pos.z >= g_quarTree[x][z].MinValue.y &&
					Player->pos.x < g_quarTree[x][z].MaxValue.x &&
					Player->pos.z < g_quarTree[x][z].MaxValue.y)
				{
					//检测Player是否到达区域边界 如果到达边界 碰撞需要遍历相邻的区间
					//左
					if (Player->pos.x < g_quarTree[x][z].MinValue.x + BOUNDARY_VALUE)
					{
						if (x != 0)
							ColliderEvent(OBJ_EN_PLAYER_TYPE, 0, x - 1, z);
					}
					//上
					if (Player->pos.z >= g_quarTree[x][z].MaxValue.y - BOUNDARY_VALUE)
					{
						if (z != MAX_NUM_Z - 1)
							ColliderEvent(OBJ_EN_PLAYER_TYPE, 0, x, z + 1);
					}
					//右
					if (Player->pos.x >= g_quarTree[x][z].MaxValue.x - BOUNDARY_VALUE)
					{
						if (x != MAX_NUM_X + 1)
							ColliderEvent(OBJ_EN_PLAYER_TYPE, 0, x + 1, z);
					}
					//下
					if (Player->pos.z < g_quarTree[x][z].MinValue.y + BOUNDARY_VALUE)
					{
						if (z != 0)
							ColliderEvent(OBJ_EN_PLAYER_TYPE, 0, x, z - 1);
					}
					//PrintDebugProc("Player Scene = %d,%d", x, z);
					ColliderEvent(OBJ_EN_PLAYER_TYPE, 0, x, z);
					break;
				}
			}
		}
	}

	//Enemy Event
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		if (EnemyWolf[num].use)
		{
			for (int z = 0; z < MAX_NUM_Z; z++)
			{
				for (int x = 0; x < MAX_NUM_X; x++)
				{
					if (EnemyWolf[num].pos.x >= g_quarTree[x][z].MinValue.x &&
						EnemyWolf[num].pos.z >= g_quarTree[x][z].MinValue.y &&
						EnemyWolf[num].pos.x < g_quarTree[x][z].MaxValue.x &&
						EnemyWolf[num].pos.z < g_quarTree[x][z].MaxValue.y)
					{
						//检测Player是否到达区域边界 如果到达边界 碰撞需要遍历相邻的区间
						//左
						if (EnemyWolf[num].pos.x < g_quarTree[x][z].MinValue.x + BOUNDARY_VALUE)
						{
							if (x != 0)
								ColliderEvent(OBJ_EN_ENEMY_WOLF_TYPE, num, x - 1, z);
						}
						//上
						if (EnemyWolf[num].pos.z >= g_quarTree[x][z].MaxValue.y - BOUNDARY_VALUE)
						{
							if (z != MAX_NUM_Z - 1)
								ColliderEvent(OBJ_EN_ENEMY_WOLF_TYPE, num, x, z + 1);
						}
						//右
						if (EnemyWolf[num].pos.x >= g_quarTree[x][z].MaxValue.x - BOUNDARY_VALUE)
						{
							if (x != MAX_NUM_X + 1)
								ColliderEvent(OBJ_EN_ENEMY_WOLF_TYPE, num, x + 1, z);
						}
						//下
						if (EnemyWolf[num].pos.z < g_quarTree[x][z].MinValue.y + BOUNDARY_VALUE)
						{
							if (z != 0)
								ColliderEvent(OBJ_EN_ENEMY_WOLF_TYPE, num, x, z - 1);
						}
						//PrintDebugProc("Player Scene = %d,%d", x, z);
						ColliderEvent(OBJ_EN_ENEMY_WOLF_TYPE, num, x, z);
						break;
					}
				}
			}
		}
	}
	//Enemy Cannibal
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	//for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	//{
	//	if (EnemyCannibal[num].use)
	//	{
	//		for (int z = 0; z < MAX_NUM_Z; z++)
	//		{
	//			for (int x = 0; x < MAX_NUM_X; x++)
	//			{
	//				if (EnemyCannibal[num].pos.x >= g_quarTree[x][z].MinValue.x &&
	//					EnemyCannibal[num].pos.z >= g_quarTree[x][z].MinValue.y &&
	//					EnemyCannibal[num].pos.x < g_quarTree[x][z].MaxValue.x &&
	//					EnemyCannibal[num].pos.z < g_quarTree[x][z].MaxValue.y)
	//				{
	//					//检测Player是否到达区域边界 如果到达边界 碰撞需要遍历相邻的区间
	//					//左
	//					if (EnemyCannibal[num].pos.x < g_quarTree[x][z].MinValue.x + BOUNDARY_VALUE)
	//					{
	//						if (x != 0)
	//							ColliderEvent(OBJ_EN_ENEMY_CANNIBAL_TYPE, num, x - 1, z);
	//					}
	//					//上
	//					if (EnemyCannibal[num].pos.z >= g_quarTree[x][z].MaxValue.y - BOUNDARY_VALUE)
	//					{
	//						if (z != MAX_NUM_Z - 1)
	//							ColliderEvent(OBJ_EN_ENEMY_CANNIBAL_TYPE, num, x, z + 1);
	//					}
	//					//右
	//					if (EnemyCannibal[num].pos.x >= g_quarTree[x][z].MaxValue.x - BOUNDARY_VALUE)
	//					{
	//						if (x != MAX_NUM_X + 1)
	//							ColliderEvent(OBJ_EN_ENEMY_CANNIBAL_TYPE, num, x + 1, z);
	//					}
	//					//下
	//					if (EnemyCannibal[num].pos.z < g_quarTree[x][z].MinValue.y + BOUNDARY_VALUE)
	//					{
	//						if (z != 0)
	//							ColliderEvent(OBJ_EN_ENEMY_CANNIBAL_TYPE, num, x, z - 1);
	//					}
	//					//PrintDebugProc("Player Scene = %d,%d", x, z);
	//					ColliderEvent(OBJ_EN_ENEMY_CANNIBAL_TYPE, num, x, z);
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
	//Enemy Bullet
	BULLET *Bullet = RetBullet();
	for (int num = 0; num < MAX_BULLET_NUM; num++)
	{
		if (Bullet[num].use)
		{
			for (int z = 0; z < MAX_NUM_Z; z++)
			{
				for (int x = 0; x < MAX_NUM_X; x++)
				{
					if (Bullet[num].pos.x >= g_quarTree[x][z].MinValue.x &&
						Bullet[num].pos.z >= g_quarTree[x][z].MinValue.y &&
						Bullet[num].pos.x < g_quarTree[x][z].MaxValue.x &&
						Bullet[num].pos.z < g_quarTree[x][z].MaxValue.y)
					{
						if (num == 1)
						{
							Bullet[num].pos.x;
							Bullet[num].pos.z;
							g_quarTree[x][z].MinValue.x;
							g_quarTree[x][z].MinValue.y;
						}
						//检测Player是否到达区域边界 如果到达边界 碰撞需要遍历相邻的区间
						//左
						if (Bullet[num].pos.x < g_quarTree[x][z].MinValue.x + BOUNDARY_VALUE)
						{
							if (x != 0)
								ColliderEvent(OBJ_EN_ENEMY_BULLET_TYPE, num, x - 1, z);
						}
						//上
						if (Bullet[num].pos.z >= g_quarTree[x][z].MaxValue.y - BOUNDARY_VALUE)
						{
							if (z != MAX_NUM_Z - 1)
								ColliderEvent(OBJ_EN_ENEMY_BULLET_TYPE, num, x, z + 1);
						}
						//右
						if (Bullet[num].pos.x >= g_quarTree[x][z].MaxValue.x - BOUNDARY_VALUE)
						{
							if (x != MAX_NUM_X + 1)
								ColliderEvent(OBJ_EN_ENEMY_BULLET_TYPE, num, x + 1, z);
						}
						//下
						if (Bullet[num].pos.z < g_quarTree[x][z].MinValue.y + BOUNDARY_VALUE)
						{
							if (z != 0)
								ColliderEvent(OBJ_EN_ENEMY_BULLET_TYPE, num, x, z - 1);
						}
						//PrintDebugProc("Player Scene = %d,%d", x, z);
						ColliderEvent(OBJ_EN_ENEMY_BULLET_TYPE, num, x, z);
						break;
					}
				}
			}
		}
	}
	////////////Enemy Boss Wolf//////////
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		if (EnemyBossWolf[num].use)
		{
			for (int z = 0; z < MAX_NUM_Z; z++)
			{
				for (int x = 0; x < MAX_NUM_X; x++)
				{
					if (EnemyBossWolf[num].pos.x >= g_quarTree[x][z].MinValue.x &&
						EnemyBossWolf[num].pos.z >= g_quarTree[x][z].MinValue.y &&
						EnemyBossWolf[num].pos.x < g_quarTree[x][z].MaxValue.x &&
						EnemyBossWolf[num].pos.z < g_quarTree[x][z].MaxValue.y)
					{
						if (num == 1)
						{
							EnemyBossWolf[num].pos.x;
							EnemyBossWolf[num].pos.z;
							g_quarTree[x][z].MinValue.x;
							g_quarTree[x][z].MinValue.y;
						}
						//检测Player是否到达区域边界 如果到达边界 碰撞需要遍历相邻的区间
						//左
						if (EnemyBossWolf[num].pos.x < g_quarTree[x][z].MinValue.x + BOUNDARY_VALUE)
						{
							if (x != 0)
								ColliderEvent(OBJ_EN_ENEMY_BOSSWOLF_TYPE, num, x - 1, z);
						}
						//上
						if (EnemyBossWolf[num].pos.z >= g_quarTree[x][z].MaxValue.y - BOUNDARY_VALUE)
						{
							if (z != MAX_NUM_Z - 1)
								ColliderEvent(OBJ_EN_ENEMY_BOSSWOLF_TYPE, num, x, z + 1);
						}
						//右
						if (EnemyBossWolf[num].pos.x >= g_quarTree[x][z].MaxValue.x - BOUNDARY_VALUE)
						{
							if (x != MAX_NUM_X + 1)
								ColliderEvent(OBJ_EN_ENEMY_BOSSWOLF_TYPE, num, x + 1, z);
						}
						//下
						if (EnemyBossWolf[num].pos.z < g_quarTree[x][z].MinValue.y + BOUNDARY_VALUE)
						{
							if (z != 0)
								ColliderEvent(OBJ_EN_ENEMY_BOSSWOLF_TYPE, num, x, z - 1);
						}
						ColliderEvent(OBJ_EN_ENEMY_BOSSWOLF_TYPE, num, x, z);
						break;
					}
				}
			}
		}
	}
}


void ColliderEvent( int type, int TypeIndex, int scene_x, int scene_z )
{
	PLAYER1 *Player = GetPlayer1();
	OBJ_CUBE_EX *ObjCube = RetObjCube_Ex();
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	BULLET *Bullet = RetBullet();
	EFFECT *Effect = RetEffect();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	CYLINDERCOLLIDER *CylinderCollider = GetCylinderCollider();
	SPHERECOLLIDER *SphereCollider = GetSphereCollider();

	if ( g_quarTree[scene_x][scene_z].CheckIndexListLen == -1 )
		return;
	if (type == OBJ_EN_PLAYER_TYPE)
	{
		for (int i = 0; i < g_quarTree[scene_x][scene_z].CheckIndexListLen; i++)
		{
			if (ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].use)
			{
				int result = CheckHitRectCylinder(ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].pos,
					CylinderCollider[Player->CylinderColliderIdx].pos,
					ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].scl * 0.9f,
					D3DXVECTOR3(CylinderCollider[Player->CylinderColliderIdx].scl));
				//if (result != 0)
				//	PrintDebugProc("HHHHHHHHHHHHHHHHHHit = %d\n", result);
				if (result == 1)  //除开重力碰撞外其它碰撞
				{
					Player->ColliderHit = true;
					//break;
				}
				else if (result == 2)  //上方向向量碰撞 (人物落地)
				{
					//g_quarTree[scene_x][scene_z].CheckIndexList[i];
					Player->down_dir = true;
					SetbHitEventJ();
					g_PlayerNowCubeIndex = g_quarTree[scene_x][scene_z].CheckIndexList[i];
					Player->NowCubeIndex = g_PlayerNowCubeIndex;
					//PrintDebugProc("Now Cube Index = %d\n", g_quarTree[scene_x][scene_z].CheckIndexList[i]);
					//break;
				}
			}
		}
	}
	if (type == OBJ_EN_ENEMY_WOLF_TYPE)
	{
		for (int i = 0; i < g_quarTree[scene_x][scene_z].CheckIndexListLen; i++)
		{
			bool result = CheckHitRC(ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].pos,
				SphereCollider[EnemyWolf[TypeIndex].SphereColliderIdx].pos,
				ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].scl,
				D3DXVECTOR3(SphereCollider[EnemyWolf[TypeIndex].SphereColliderIdx].scl));
			if (TypeIndex == 1)
			{
				//PrintDebugProc("Hit pos = %f, %f", SphereCollider[EnemyWolf[TypeIndex].SphereColliderIdx].pos.x, SphereCollider[EnemyWolf[TypeIndex].SphereColliderIdx].pos.z);
				//int aaaa = 0;
			}
			if (result)
			{
				//g_quarTree[scene_x][scene_z].CheckIndexList[i];
				EnemyWolf[TypeIndex].down_dir = true;
			}
		}
	}
	//if (type == OBJ_EN_ENEMY_CANNIBAL_TYPE)
	//{
	//	for (int i = 0; i < g_quarTree[scene_x][scene_z].CheckIndexListLen; i++)
	//	{
	//		bool result = CheckHitRC(ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].pos,
	//			SphereCollider[EnemyCannibal[TypeIndex].SphereColliderIdx].pos,
	//			ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].scl,
	//			D3DXVECTOR3(SphereCollider[EnemyCannibal[TypeIndex].SphereColliderIdx].scl));
	//		if (TypeIndex == 1)
	//		{
	//			//PrintDebugProc("Hit pos = %f, %f", SphereCollider[EnemyCannibal[TypeIndex].SphereColliderIdx].pos.x, SphereCollider[EnemyWolf[TypeIndex].SphereColliderIdx].pos.z);
	//			//int aaaa = 0;
	//		}
	//		if (result)
	//		{
	//			EnemyCannibal[TypeIndex].down_dir = true;
	//		}
	//	}
	//}
	if(type == OBJ_EN_ENEMY_BULLET_TYPE)
	{
		for (int i = 0; i < g_quarTree[scene_x][scene_z].CheckIndexListLen; i++)
		{
			bool result = CheckHitRC(ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].pos,
				SphereCollider[Bullet[TypeIndex].SphereColliderIdx].pos,
				ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].scl,
				D3DXVECTOR3(SphereCollider[Bullet[TypeIndex].SphereColliderIdx].scl));
			if (TypeIndex == 1)
			{
				//PrintDebugProc("Hit pos = %f, %f", SphereCollider[EnemyCannibal[TypeIndex].SphereColliderIdx].pos.x, SphereCollider[EnemyWolf[TypeIndex].SphereColliderIdx].pos.z);
				//int aaaa = 0;
			}
			if (result)
			{
				Bullet[TypeIndex].use = false;
				DeleteSphereCollider(Bullet[TypeIndex].SphereColliderIdx);
				Effect[Bullet[TypeIndex].EffectIdx].bUse = false;
			}
		}
	}
	if (type == OBJ_EN_ENEMY_BOSSWOLF_TYPE)
	{
		for (int i = 0; i < g_quarTree[scene_x][scene_z].CheckIndexListLen; i++)
		{
			int result = CheckHitRectCylinder(ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].pos,
				CylinderCollider[EnemyBossWolf[TypeIndex].CylinderIdx].pos,
				ObjCube[g_quarTree[scene_x][scene_z].CheckIndexList[i]].scl * 0.9f,
				D3DXVECTOR3(CylinderCollider[EnemyBossWolf[TypeIndex].CylinderIdx].scl));
			//if (result != 0)
			//	PrintDebugProc("HHHHHHHHHHHHHHHHHHit = %d\n", result);
			if (result == 1)  //除开重力碰撞外其它碰撞
			{
				EnemyBossWolf[TypeIndex].ColliderHit = true;
				//break;
			}
			if (result == 2)  //上方向向量碰撞 (人物落地)
			{
				//g_quarTree[scene_x][scene_z].CheckIndexList[i];
				EnemyBossWolf[TypeIndex].down_dir = true;
				//g_PlayerNowCubeIndex = g_quarTree[scene_x][scene_z].CheckIndexList[i];
				//Player->NowCubeIndex = g_PlayerNowCubeIndex;
				//PrintDebugProc("Enemy BossWolf Now Cube Index = %d\n", g_quarTree[scene_x][scene_z].CheckIndexList[i]);
				if (SphereCollider[EnemyBossWolf[TypeIndex].SphereColliderJumpAckIdx].pos.y <= 110.0f
					&&
					(EnemyBossWolf[TypeIndex].animationID == ENEMY_BOSSWOLF_JUMPACK
					|| EnemyBossWolf[TypeIndex].animationID == ENEMY_BOSSWOLF_STAMP)
					&& EnemyBossWolf[TypeIndex].JumpAckEffective)
				{
					for (int a = 0; a < 100; a++)
					{
						if (HitCube[a].use)
						{
							//if (g_quarTree[scene_x][scene_z].CheckIndexList[i] == HitCube[a].Cubeindex)
								continue;
						}
						else
						{
							HitCube[a].Cubeindex = g_quarTree[scene_x][scene_z].CheckIndexList[i];
							HitCube[a].col = D3DXVECTOR3{ 2.0f, 0.0f, 0.0f };
							HitCube[a].colStart = D3DXVECTOR3{ 2.0f, 0.0f, 0.0f };
							HitCube[a].use = true;
							for (int DirIndex = 0; DirIndex < 4; DirIndex++)
							{
								int HitSideCubeIndex = FindEnemyBossWolfSceneCubeIndexSide( HitCube[a].Cubeindex, DirIndex);
								if (HitSideCubeIndex != -1)
								{
									//int aaaaaaaaa = count * 3 + a + DirIndex + 1;
									int aaaa = a + DirIndex + 1;
									HitCube[a + DirIndex + 1].Cubeindex = HitSideCubeIndex;
									HitCube[a + DirIndex + 1].col = D3DXVECTOR3{ 2.0f, 0.0f, 0.0f };
									HitCube[a + DirIndex + 1].colStart = D3DXVECTOR3{ 2.0f, 0.0f, 0.0f };
									HitCube[a + DirIndex + 1].use = true;
									for (int count = 0; count < 4; count++)
									{
										int HitSideCubeIndexEx = FindEnemyBossWolfSceneCubeIndexSide(HitCube[a + DirIndex + 1].Cubeindex, count);
										if (HitSideCubeIndexEx == -1)
											int a = -1;
										if (HitSideCubeIndexEx != -1)
										{
											HitCube[DirIndex * count + a + DirIndex + 1].Cubeindex = HitSideCubeIndexEx;
											HitCube[DirIndex * count + a + DirIndex + 1].col = D3DXVECTOR3{ 2.0f, 0.0f, 0.0f };
											HitCube[DirIndex * count + a + DirIndex + 1].colStart = D3DXVECTOR3{ 2.0f, 0.0f, 0.0f };
											HitCube[DirIndex * count + a + DirIndex + 1].use = true;
										}
									}
								}
							}
							break;
						}
					}
				}
				//break;
			}
		}
	}
}


void ClearAllQuarTree()
{
	for (int z = 0; z < MAX_NUM_Z; z++)
	{
		for (int x = 0; x < MAX_NUM_X; x++)
		{
			for (int c = 0; c < 5000; c++)
			    g_quarTree[x][z].CheckIndexList[c] = -1;
			g_quarTree[x][z].CheckIndexListLen = -1;
			g_quarTree[x][z].MaxValue = D3DXVECTOR2{ 0 ,0 };
			g_quarTree[x][z].MinValue = D3DXVECTOR2{ 0 ,0 };
		}
	}
}


int RetPlayerNowCubeIndex()
{
	return g_PlayerNowCubeIndex;
}


HITCUBE *RetHitCube()
{
	return &HitCube[0];
}
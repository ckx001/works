//=============================================================================
//
// 事件処理 [GameEvent.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "GameEvent.h"
#include "GlobalFunction.h"
#include "BoxCollider.h"
#include "hit_event.h"
#include "player1.h"
#include "sphereCollider.h"
#include "Scene.h"
#include "cube_Ex.h"
#include "fade.h"
#include "quarTree.h"
#include "ObjObjModel_Ex.h"
#include "cylinderCollider.h"
#include "Time_c.h"
#include "UiPlayerHp.h"
#include "shadow.h"
#include "Enemy_wolf.h"
#include "EnemyCannibal.h"
#include "EnemyBossWolf.h"
#include "SoulEffect.h"
#include "debugproc.h"
#include "effect.h"
#include "UiEnemyHp.h"
#include "UiGoods.h"
#include "bullet.h"
#include "GameAi.h"
#include "sound.h"
#include "UiSmallMap.h"
#include "MapManager.h"
#include <random>

static void HitEvent(void);
static void FallEvent(void);
static void GameEvent(void);
static void ClearAllEnemy(bool BossMove);
static void SavePointEvent(void);
static void CreateGroupObjTree(void);

static int CurrentSceneIndex = SCENE_SCENE_HOME; //当前所在区域索引

//Save Point
static int SavePoint = SAVE_POINT_HOME;
//事件触发全局变量
static bool BossWolfEventActive = true;
static bool BossWolfEventAiJudge = false; //触发走入boss门的ai

//Enemy激活点
static bool bEnemyCreateActive[10];
///////////////////////////
////游戏计时器函数/////////

DWORD timing[TIMING_MAX_NUM][MAX_TIME_INDEX];
int TimeIndex[TIMING_MAX_NUM][MAX_TIME_INDEX];


void InitEnemyEvent()
{
	for (int i = 0; i < 10; i++)
	{
		bEnemyCreateActive[i] = true;
	}
}


void InitTimeEvent()
{
	//启用计时器
	TimeIndex[TIMING_PLAYER_RUN][0] = CreateTimeCollider();
	TimeIndex[TIMING_PLAYER_ACKED][0] = CreateTimeCollider();
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		TimeIndex[TIMING_ENEMY_WAIT][num] = CreateTimeCollider();
	}
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		TimeIndex[TIMING_ENEMY_CANNIBAL_WAIT][num] = CreateTimeCollider();
	}
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		TimeIndex[TIMING_ENEMY_BOSSWOLF_WAIT][num] = CreateTimeCollider();
		TimeIndex[TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT][num] = CreateTimeCollider();
	}

	Init_Time_Set();
}


void UpdateGameEvent()
{
	//游戏触发事件
	GameEvent();
	//碰撞事件
	HitEvent();
	//坠落事件
	FallEvent();
	//存档点事件
	SavePointEvent();
	//计时器事件
	timing[TIMING_PLAYER_RUN][0] = Time_Count(TimeIndex[TIMING_PLAYER_RUN][0]);
	timing[TIMING_PLAYER_ACKED][0] = Time_Count(TimeIndex[TIMING_PLAYER_ACKED][0]);
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		timing[TIMING_ENEMY_WAIT][num] = Time_Count(TimeIndex[TIMING_ENEMY_WAIT][num]);
	}
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		timing[TIMING_ENEMY_CANNIBAL_WAIT][num] = Time_Count(TimeIndex[TIMING_ENEMY_CANNIBAL_WAIT][num]);
	}
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		timing[TIMING_ENEMY_BOSSWOLF_WAIT][num] = Time_Count(TimeIndex[TIMING_ENEMY_BOSSWOLF_WAIT][num]);
		timing[TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT][num] = Time_Count(TimeIndex[TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT][num]);
	}
}


void PlayerEnemyHitEvent()
{
	PLAYER1 *Player = GetPlayer1();
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	SPHERECOLLIDER *SphereCollider = GetSphereCollider();
	CYLINDERCOLLIDER *CylinderCollider = GetCylinderCollider();
	OBJOBJMODEL_EX *ObjObjModel_Ex = RetObjObjModel_Ex();
	BULLET *Bullet = RetBullet();
	EFFECT *Effect = RetEffect();
	HITCUBE *HitCube = RetHitCube();

	/////Player ---- Enemy Wolf//////////
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		if (EnemyWolf[num].use)
	    {
			/////Player Ack ->Enemy Wolf//////////
	    	if (EnemyWolf[num].AckJudge)
	    	{
	    		if (CheckHitCC(SphereCollider[Player->SphereColliderAckIdx].pos, SphereCollider[EnemyWolf[num].SphereColliderIdx].pos,
	    			SphereCollider[Player->SphereColliderAckIdx].scl, SphereCollider[EnemyWolf[num].SphereColliderIdx].scl)
	    			&& Player->animationID == PLAYER_ATTACK1)
	    		{
	    			SetDisPlayEnemyHp(TRUE);
					ReSetEnemyHpRectFromHp( num, ENEMY_TYPE_WOLF );
	    			EnemyHpReduce( ENEMY_TYPE_WOLF, num, 10 );
					EnemyWolf[num].AckJudge = false;
					EnemyWolf[num].HurtJudge = true;
					PlaySound(SOUND_LABEL_SE_ACKHIT);
	    		}
	    	}
	        //////Player 撞到Enemy Wolf ////////

			if (CheckHitCircleCylinder(CylinderCollider[Player->CylinderColliderIdx].pos,
				SphereCollider[EnemyWolf[num].SphereColliderIdx].pos,
				CylinderCollider[Player->CylinderColliderIdx].scl,
				SphereCollider[EnemyWolf[num].SphereColliderIdx].scl))
			{
				Player->ColliderHit = true;
			}
		    //////Enemy Wolf 撞到Enemy Wolf ////////
			for (int c = 0; c < ENEMY_WOLF_MAX_NUM; c++)
			{
				if (c != num && EnemyWolf[c].use)
				{
					if (CheckHitCC(SphereCollider[EnemyWolf[c].SphereColliderIdx].pos,
						SphereCollider[EnemyWolf[num].SphereColliderIdx].pos,
						SphereCollider[EnemyWolf[c].SphereColliderIdx].scl,
						SphereCollider[EnemyWolf[num].SphereColliderIdx].scl))
					{
						//Player->ColliderHit = true;
						EnemyWolf[num].pos = EnemyWolf[num].last_frame_pos;
						EnemyWolf[c].pos = EnemyWolf[c].last_frame_pos;
	
					}
				}
			}
			////////// Enemy Wolf Ack --> Player///////////
			if (Player->AckedJudge)
			{
				if (CheckHitCircleCylinder(SphereCollider[EnemyWolf[num].SphereColliderAckIdx].pos, 
					CylinderCollider[Player->CylinderColliderIdx].pos,
					SphereCollider[EnemyWolf[num].SphereColliderAckIdx].scl,
					CylinderCollider[Player->CylinderColliderIdx].scl
					)
					&& EnemyWolf[num].animationID == ENEMY_WOLF_ACK 
					&& EnemyWolf[num].AckPlayerAckTimeJudge)
				{
					PlayerHpReduce(20);
					Player->AckedJudge = false;
				}
			}
		}
	}
	///////////Player -------- Cannibal ///////////
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		if (EnemyCannibal[num].use)
		{
			//////Player 撞到Enemy Cannibal ////////
			if (CheckHitCircleCylinder(CylinderCollider[Player->CylinderColliderIdx].pos,
				SphereCollider[EnemyCannibal[num].SphereColliderIdx].pos,
				CylinderCollider[Player->CylinderColliderIdx].scl,
				SphereCollider[EnemyCannibal[num].SphereColliderIdx].scl))
			{
				Player->ColliderHit = true;
			}
			/////Player Ack ->Enemy Cannibal//////////
			if (EnemyCannibal[num].AckJudge)
			{
				if (CheckHitCC(SphereCollider[Player->SphereColliderAckIdx].pos, SphereCollider[EnemyCannibal[num].SphereColliderIdx].pos,
					SphereCollider[Player->SphereColliderAckIdx].scl, SphereCollider[EnemyCannibal[num].SphereColliderIdx].scl)
					&& Player->animationID == PLAYER_ATTACK1)
				{
					SetDisPlayEnemyHp(TRUE);
					ReSetEnemyHpRectFromHp(num, ENEMY_TYPE_CANNIBAL);
					EnemyHpReduce(ENEMY_TYPE_CANNIBAL, num, 15);
					EnemyCannibal[num].AckJudge = false;
					PlaySound(SOUND_LABEL_SE_ACKHIT);
				}
			}
		}
	}
	///////////Player -------- ITEM////////
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (ObjObjModel_Ex[i].use)
		{
			if (Player->use)
			{
				if (strncmp(ObjObjModel_Ex[i].tag, "red", strlen(ObjObjModel_Ex[i].tag)) == NULL)
				{
					if (CheckHitCircleCylinder(SphereCollider[ObjObjModel_Ex[i].SphereColliderIdx].pos,
						CylinderCollider[Player->CylinderColliderIdx].pos,
						SphereCollider[ObjObjModel_Ex[i].SphereColliderIdx].scl,
						CylinderCollider[Player->CylinderColliderIdx].scl
					))
					{
						SetGoodsAdd(GOODS_TYPE_RED, false);
						//删除当前触发器
						DeleteSphereCollider(ObjObjModel_Ex[i].SphereColliderIdx);
						ObjObjModel_Ex[i].use = false;
					}
				}
			}
		}
	}
	/////////////Player ---------- Enemy Boss Wolf/////
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		if (EnemyBossWolf[num].use && EnemyBossWolf[num].active)
		{
			//////Player 撞到Enemy Boss ////////
			//if (EnemyBossWolf[num].animationID != ENEMY_BOSSWOLF_JUMPACK)
			//{
			//	if (CheckHitCylinderCylinder(CylinderCollider[Player->CylinderColliderIdx].pos,
			//		CylinderCollider[EnemyBossWolf[num].CylinderIdx].pos,
			//		CylinderCollider[Player->CylinderColliderIdx].scl,
			//		CylinderCollider[EnemyBossWolf[num].CylinderIdx].scl)
			//	)
			//	{
			//		Player->ColliderHit = true;
			//	}
			//}
			/////Cube Hit Ack -> Player///////////
			for (int a = 0; a < 100; a++)
			{
				if (HitCube[a].use)
				{
					if (Player->NowCubeIndex == HitCube[a].Cubeindex)
					{
						if (Player->AckedJudge)
						{
							PlayerHpReduce(10);
							Player->AckedJudge = false;
						}
					}
				}
			}
			////////////////////
			/////Player Ack ->Enemy Boss//////////
			if (EnemyBossWolf[num].AckJudge)
			{
				if (CheckHitCircleCylinder(SphereCollider[Player->SphereColliderAckIdx].pos,
					CylinderCollider[EnemyBossWolf[num].CylinderIdx].pos,
					SphereCollider[Player->SphereColliderAckIdx].scl,
					CylinderCollider[EnemyBossWolf[num].CylinderIdx].scl) 
					&& Player->animationID == PLAYER_ATTACK1)
				{
					//SetDisPlayEnemyHp(TRUE);
					//ReSetEnemyHpRectFromHp(num, ENEMY_TYPE_BOSSWOLF);
					EnemyHpReduce(ENEMY_TYPE_BOSSWOLF, num, 15);
					EnemyBossWolf[num].AckJudge = false;
					PlaySound(SOUND_LABEL_SE_ACKHIT);
				}
			}
			//// Enemy Boss --ENEMY_BOSSWOLF_JUMPACK -> Player///////
			
			///////////////////////////////////////////
			//// Player的碰撞体在Enemy Boss 内///////
			if (CheckHitCircleCylinder(SphereCollider[EnemyBossWolf[num].SphereColliderJumpAckIdx].pos,
				CylinderCollider[Player->CylinderColliderIdx].pos,
				SphereCollider[EnemyBossWolf[num].SphereColliderJumpAckIdx].scl,
				CylinderCollider[EnemyBossWolf[num].CylinderIdx].scl
				)
				&& (EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_JUMPACK
					|| (EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_STAMP && EnemyBossWolf[num].JumpAckEffective)
					))
			//if (CheckHitPointCylinder(CylinderCollider[Player->CylinderColliderIdx].pos,
			//	CylinderCollider[EnemyBossWolf[num].CylinderIdx].pos,
			//	CylinderCollider[EnemyBossWolf[num].CylinderIdx].scl)
			//	&& (EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_JUMPACK
			//	|| (EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_STAMP && EnemyBossWolf[num].JumpAckEffective)
			//		))
			{
				if (EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_JUMPACK
					&& Player->ColliderHit == false)
				{
					D3DXVECTOR3 JumpMove = D3DXVECTOR3(-sinf(EnemyBossWolf[num].rot.y) * ENEMY_BOSSWOLF_JUMPACK_MOVE, 0.0f, -cosf(EnemyBossWolf[num].rot.y) * ENEMY_BOSSWOLF_JUMPACK_MOVE);
					//加一些偏差值 防止卡住
					Player->pos.x += JumpMove.x + 0.3f;
					Player->pos.z += JumpMove.z + 0.3f;
				}
				Player->pos.y += 2.1f;
				Player_G_Use();
				if (Player->AckedJudge)
				{
					PlayerHpReduce(20);
					Player->AckedJudge = false;
				}
			}
			
			//// Enemy Boss ACK -> Player///////
			if (Player->AckedJudge)
			{
				if (CheckHitCircleCylinder(SphereCollider[EnemyBossWolf[num].SphereColliderAckIdx].pos,
					CylinderCollider[Player->CylinderColliderIdx].pos,
					SphereCollider[EnemyBossWolf[num].SphereColliderAckIdx].scl,
					CylinderCollider[Player->CylinderColliderIdx].scl)
					&& EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_ACK
					&& EnemyBossWolf[num].AckEffective)
				{
					PlayerHpReduce(20);
					Player->AckedJudge = false;
				}
			}
		}
	}
	/////////////Enemy Cannibal Bullet -> Player///////
	for (int num = 0; num < MAX_BULLET_NUM; num++)
	{
		if (Bullet[num].use)
		{
			if (CheckHitCircleCylinder(SphereCollider[Bullet[num].SphereColliderIdx].pos,
				CylinderCollider[Player->CylinderColliderIdx].pos,
				SphereCollider[Bullet[num].SphereColliderIdx].scl,
				CylinderCollider[Player->CylinderColliderIdx].scl
			))
			{
				if (Player->AckedJudge)
				{
					PlayerHpReduce(15);
					Player->AckedJudge = false;
				}
				Bullet[num].use = false;
				DeleteSphereCollider(Bullet[num].SphereColliderIdx);
				Effect[Bullet[num].EffectIdx].bUse = false;
			}
		}
	}
}


void HitEvent()
{
	PLAYER1 *Player = GetPlayer1();
	OBJ_CUBE_EX *ObjCube_Ex = RetObjCube_Ex();
	BOXCOLLIDER *BoxColliderScene = GetBoxCollider(SCENE_PORTAL_COLLIDER);
	BOXCOLLIDER *BoxColliderEvent = GetBoxCollider(SCENE_EVENT_COLLIDER);
	//SPHERECOLLIDER *SphereCollider = GetSphereCollider();
	CYLINDERCOLLIDER *CylinderCollider = GetCylinderCollider();
	int *ColliderIndexGrassToDesert = RetSceneColliderIndexGrassToDesert();
	int *ColliderIndexHomeToGrass = RetSceneColliderIndexHomeToGrass();
	int *ColliderIndexGrassDoorToBoss = RetSceneColliderIndexGrassDoorToBoss();

	switch (CurrentSceneIndex)
	{
	    case SCENE_SCENE_GRASS:
		{
			//区域切换事件 Grass->Desert
			if (CheckHitRectCylinder(BoxColliderScene[*ColliderIndexGrassToDesert].pos,
				CylinderCollider[Player->CylinderColliderIdx].pos,
				BoxColliderScene[*ColliderIndexGrassToDesert].scl,
				D3DXVECTOR3(CylinderCollider[Player->CylinderColliderIdx].scl)) == 1)
			{
				//清除当前场景
				ClearAllCube();
				ClearAllObjObjModel();
				//清除shadow
				InitShadow();
                //插入过场渐变
				SetFade(FADE_OUT);
				//载入沙漠场景
				LoadScene(SCENE_SCENE_DESERT);
				//载入障碍物数组
				LoadSceneBarrierList(SCENE_SCENE_DESERT);
				//清除当前四叉树
				ClearAllQuarTree();
				//载入新的四叉树
				InitQuarTree(-2580.0f, 0.0f, 128, 128);
				//关闭触发器
				BoxColliderScene[*ColliderIndexGrassToDesert].active = false;
				CurrentSceneIndex = SCENE_SCENE_DESERT;
			}
			break;
		}
		case SCENE_SCENE_HOME:
		{
			//区域切换事件 Home->Grass
			if (CheckHitRectCylinder(BoxColliderScene[*ColliderIndexHomeToGrass].pos,
				CylinderCollider[Player->CylinderColliderIdx].pos,
				BoxColliderScene[*ColliderIndexHomeToGrass].scl,
				D3DXVECTOR3(CylinderCollider[Player->CylinderColliderIdx].scl)) != 0)
			{
				//清除当前场景
				ClearAllCube();
				ClearAllObjObjModel();
				//清除shadow
				InitShadow();
				//插入过场渐变
				SetFade(FADE_OUT);
				//载入Grass场景
				LoadScene(SCENE_SCENE_GRASS);
				//载入小地图数据
				CreateCurrentSmallMap(SCENE_SCENE_GRASS);
				//重写小地图
				InitUiSmallMap(0, SCENE_SCENE_GRASS);
				//载入障碍物数组
				LoadSceneBarrierList(SCENE_SCENE_GRASS);
				//清除并载入新Monster
				LoadEnemy(SCENE_SCENE_GRASS);
				//载入道具
				LoadItem();
				//清除当前四叉树
				ClearAllQuarTree();
				//初始化AI
				InitGameAi();
				//载入新的四叉树
				InitQuarTree(-2580.0f, 0.0f, 128, 144);
				//载入boss区域索引
				FindEnemyBossWolfSceneCubeIndex();
				//关闭传送触发器
				BoxColliderScene[*ColliderIndexHomeToGrass].active = false;
				CurrentSceneIndex = SCENE_SCENE_GRASS;
				//打开传送触发器
				BoxColliderScene[*ColliderIndexGrassToDesert].active = true;
				//打开事件触发器
				BoxColliderEvent[*ColliderIndexGrassDoorToBoss].active = true;
				//开放boss门前的墙
				ObjCube_Ex[5833].use = false;
				ObjCube_Ex[5834].use = false;
				ObjCube_Ex[5835].use = false;
				ObjCube_Ex[5836].use = false;
				//清除道具
				ClearAllGoods();
				SavePoint = SAVE_POINT_GRASS_START;
				Player->pos = D3DXVECTOR3{ -1702.0f,20.0f, 300.0f };
				PlayerHpAdd(50);
				//Player->pos = D3DXVECTOR3{ -28.0f,20.0f, 4380.0f };
				PlaySound(SOUND_LABEL_BGM_GAME);
			}
			break;
		}
	}

	PlayerEnemyHitEvent();
}


DWORD *RetTiming(int type)
{
	return &(timing[type][0]);
}

int *RetTimeIndex(int type)
{
	return &TimeIndex[type][0];
}

int RetCurrentSceneIndex()
{
	return CurrentSceneIndex;
}


void LoadSceneBarrierList(int Scene)
{
	D3DXVECTOR3 *CurrentCubeBarrierList = RetCurrentCubeBarrierList();
	D3DXVECTOR3 *BarrierList = RetBarrierList(Scene);
	int SceneBarrierListIndex = RetSceneBarrierListIndex(Scene);
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		CurrentCubeBarrierList[i] = D3DXVECTOR3{ NULL,NULL,NULL };
	}
	for (int i = 0; i < SceneBarrierListIndex; i++)
	{
		CurrentCubeBarrierList[i] = BarrierList[i];
	}
	SetCurrentCubeBarrierListIndex(SceneBarrierListIndex);
}


void LoadEnemy(int scene)
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();

	for (int i = 0; i < ENEMY_WOLF_MAX_NUM; i++)
	{
		if (EnemyWolf[i].use)
		{
			EnemyWolf[i].use = FALSE;
			DeleteSphereCollider(EnemyWolf[i].SphereColliderAckIdx);
			DeleteSphereCollider(EnemyWolf[i].SphereColliderIdx);
		}
	}
	for (int i = 0; i < ENEMY_CANNIBAL_MAX_NUM; i++)
	{
		if (EnemyCannibal[i].use)
		{
			EnemyCannibal[i].use = FALSE;
			DeleteSphereCollider(EnemyCannibal[i].SphereColliderAckIdx);
			DeleteSphereCollider(EnemyCannibal[i].SphereColliderIdx);
		}
	}
	
	for (int i = 0; i < ENEMY_BOSSWOLF_MAX_NUM; i++)
	{
		if (EnemyBossWolf[i].use)
		{
			EnemyBossWolf[i].use = FALSE;
			DeleteSphereCollider(EnemyBossWolf[i].SphereColliderAckIdx);
			DeleteSphereCollider(EnemyBossWolf[i].SphereColliderJumpAckIdx);
			DeleteCylinderCollider(EnemyBossWolf[i].CylinderIdx);
		}
	}

	if (scene == SCENE_SCENE_HOME)
	{
		CreateEnemyWolf(D3DXVECTOR3{ 328.0f, 20.0f, 1100.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f,0.5f,0.5f }, false);
	}
	else if (scene == SCENE_SCENE_GRASS)
	{
		CreateEnemyWolf(D3DXVECTOR3{ -834.0f, 20.0f, 842.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ -994.0f, 20.0f, 842.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		//CreateEnemyWolf(D3DXVECTOR3{ -1373.0f, 20.0f, 673.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f });

		CreateEnemyWolf(D3DXVECTOR3{ -1408.0f, 20.0f, 2811.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ -1223.0f, 20.0f, 2811.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		
		CreateEnemyWolf(D3DXVECTOR3{ -721.0f, 20.0f, 2609.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);

		CreateEnemyWolf(D3DXVECTOR3{ -1340.0f, 20.0f, 3871.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ -1338.0f, 20.0f, 4247.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ -1524.0f, 20.0f, 4106.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		
		CreateEnemyWolf(D3DXVECTOR3{ -1463.0f, 20.0f, 1578.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ -164.0f, 20.0f, 3576.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ -771.0f, 20.0f, 4353.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ -2022.0f, 20.0f, 4220.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ -1883.0f, 20.0f, 3885.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);

		CreateEnemyWolf(D3DXVECTOR3{ 390.0f, 20.0f, 2007.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 172.0f, 20.0f, 1756.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 16.0f, 20.0f, 1871.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 830.0f, 20.0f, 869.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);

		CreateEnemyWolf(D3DXVECTOR3{ 2193.0f, 20.0f, 1774.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 2088.0f, 20.0f, 1774.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 1993.0f, 20.0f, 1774.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		
		CreateEnemyWolf(D3DXVECTOR3{ 1055.0f, 20.0f, 2931.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 1055.0f, 20.0f, 2936.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		
		CreateEnemyWolf(D3DXVECTOR3{ 1055.0f, 20.0f, 2931.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 1055.0f, 20.0f, 2936.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 1376.0f, 20.0f, 4232.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		
		CreateEnemyWolf(D3DXVECTOR3{ 1071.0f, 20.0f, 3071.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);
		CreateEnemyWolf(D3DXVECTOR3{ 1423.0f, 20.0f, 3047.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, false);

		CreateEnemyCannibal(D3DXVECTOR3{ -1573.0f, 20.0f, 1988.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ -1332.0f, 20.0f, 1988.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });

		CreateEnemyCannibal(D3DXVECTOR3{ -1604.0f, 20.0f, 4360.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 964.0f, 20.0f, 4715.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });

		CreateEnemyCannibal(D3DXVECTOR3{ -1404, 20, 4317 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 83, 20, 3636 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ -473, 20, 4171 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ -580, 20, 3818 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });

		CreateEnemyCannibal(D3DXVECTOR3{ 131, 20, 1001 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 257, 20, 768 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 411, 20, 700 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 705, 20, 1114 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 558, 20, 1125 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		
		CreateEnemyCannibal(D3DXVECTOR3{ 1986, 20, 2762 }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		
		CreateEnemyCannibal(D3DXVECTOR3{ 1909.0f, 20.0f, 3178.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 1081.0f, 20.0f, 1834.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 1130.0f, 20.0f, 1837.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{ 1909.0f, 20.0f, 3178.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
		CreateEnemyCannibal(D3DXVECTOR3{  909.0f, 20.0f,  4328.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });

		CreateObjObjModel(GOODS_TYPE_RED, D3DXVECTOR3{-834.0f, 20.0f, 862.0f}, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 2.0f, 2.0f,2.0f });
		CreateObjObjModel(GOODS_TYPE_RED, D3DXVECTOR3{ -2115.0f, 20.0f, 3759.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 2.0f, 2.0f,2.0f });
		CreateObjObjModel(GOODS_TYPE_RED, D3DXVECTOR3{ 1152.0f, 20.0f, 1246.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 2.0f, 2.0f,2.0f });
		CreateObjObjModel(GOODS_TYPE_RED, D3DXVECTOR3{ 2123.0f, 20.0f, 1499.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 2.0f, 2.0f,2.0f });

		CreateGroupObjTree();
		//CreateObjObjModel(GOODS_TYPE_TREE, D3DXVECTOR3{ -834.0f, 20.0f, 842.0f  }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 2.0f, 2.0f,2.0f });
		
		CreateEnemyBossWolf(D3DXVECTOR3{ -64.0f, 20.0f, 5486.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 1.0f,1.0f,1.0f });
	}
}


void LoadItem()
{
	SPHERECOLLIDER *SphereCollider = GetSphereCollider();
	OBJOBJMODEL_EX *ObjObjModel_Ex = RetObjObjModel_Ex();
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (ObjObjModel_Ex[i].use)
		{
			if (strncmp(ObjObjModel_Ex[i].tag, "red", strlen(ObjObjModel_Ex[i].tag)) == NULL)
				ObjObjModel_Ex[i].SphereColliderIdx = CreateSphereCollider(ObjObjModel_Ex[i].pos, ObjObjModel_Ex[i].rot, ObjObjModel_Ex[i].scl);
		}
	}
}


void FallEvent()
{
	PLAYER1 *Player = GetPlayer1();
	OBJ_CUBE_EX *ObjCube_Ex = RetObjCube_Ex();
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();

	if (Player->pos.y < -160.0f)
	{
		Player->pos.x = ObjCube_Ex[Player->NowCubeIndex].pos.x;
		Player->pos.y = 20.0f;
		Player->pos.z = ObjCube_Ex[Player->NowCubeIndex].pos.z;
		PlayerHpReduce(20);
	}

	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		if (EnemyWolf[num].pos.y < -160.0f)
		{
			EnemyWolf[num].use = false;
			//关闭寻路
			SetbEnemyAstart(num, false);
			DeleteSphereCollider(EnemyWolf[num].SphereColliderAckIdx);
			DeleteSphereCollider(EnemyWolf[num].SphereColliderIdx);
		}
	}
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		if (EnemyCannibal[num].pos.y < -160.0f)
		{
			EnemyCannibal[num].use = false;
			DeleteSphereCollider(EnemyCannibal[num].SphereColliderAckIdx);
			DeleteSphereCollider(EnemyCannibal[num].SphereColliderIdx);
		}
	}
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		if (EnemyBossWolf[num].use)
		{
			//掉落复位
			if (EnemyBossWolf[num].pos.y < -160.0f)
			{
				EnemyBossWolf[num].pos = EnemyBossWolf[num].start_pos;
			}
		}
	}
}





void GameEvent()
{
	PLAYER1 *Player = GetPlayer1();
	OBJ_CUBE_EX *ObjCube_Ex = RetObjCube_Ex();
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	CYLINDERCOLLIDER *CylinderCollider = GetCylinderCollider();
	BOXCOLLIDER *BoxColliderEvent = GetBoxCollider(SCENE_EVENT_COLLIDER);
	int *ColliderIndexGrassDoorToBoss = RetSceneColliderIndexGrassDoorToBoss();

	if (CurrentSceneIndex == SCENE_SCENE_GRASS)
	{
		if (bEnemyCreateActive[0])
		{
			if (Player->pos.x > -1376.0f
				&& Player->pos.x < -1306.0f
				&& Player->pos.z > 3400.0f
				&& Player->pos.z < 3500.0f)
			{
				CreateEnemyWolf(D3DXVECTOR3{ -1340.0f, 20.0f, 3100.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, true);
				bEnemyCreateActive[0] = false;
			}
		}
		if (bEnemyCreateActive[1])
		{
			if (Player->pos.x > 943.0f
				&& Player->pos.x < 1349.0f
				&& Player->pos.z > 2716.0f
				&& Player->pos.z < 2750.0f)
			{
				CreateEnemyWolf(D3DXVECTOR3{ 1200.0f, 20.0f, 2464.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, true);
				CreateEnemyWolf(D3DXVECTOR3{ 1251.0f, 20.0f, 2465.0f }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 0.5f, 0.5f,0.5f }, true);
				bEnemyCreateActive[1] = false;
			}
		}

		//触发boss战
		if (BossWolfEventActive)
		{
			if (BossWolfEventAiJudge == false)
			{
				if (CheckHitRectCylinder(BoxColliderEvent[*ColliderIndexGrassDoorToBoss].pos,
					CylinderCollider[Player->CylinderColliderIdx].pos,
					BoxColliderEvent[*ColliderIndexGrassDoorToBoss].scl,
					D3DXVECTOR3(CylinderCollider[Player->CylinderColliderIdx].scl)) != 0)
				{
					BossWolfEventAiJudge = true;
					StopSound(SOUND_LABEL_BGM_GAME);
					PlaySound(SOUND_LABEL_BGM_BOSS);
				}
			}
			if (BossWolfEventAiJudge)
			{
				if (PlayerAiMoveEvent(Player->pos, D3DXVECTOR3(-58.0f, Player->pos.y, Player->pos.z)))
				{
					if (PlayerAiMoveEvent(Player->pos, D3DXVECTOR3(Player->pos.x, Player->pos.y, 5220.0f)))
					{
						BossWolfEventActive = false;
						EnemyBossWolf[0].active = true;
						SetDisPlayEnemyHp(TRUE);
						ReSetEnemyHpRectFromHp(0, ENEMY_TYPE_BOSSWOLF);
						//移除地图上除boss外所有敌人
						ClearAllEnemy(false);
						//关闭boss门前的墙
						ObjCube_Ex[5833].use = true;
						ObjCube_Ex[5834].use = true;
						ObjCube_Ex[5835].use = true;
						ObjCube_Ex[5836].use = true;
					}
				}
			}
		}
		////临时解决bug 人物die复活不传送到目标位置/////////
		/////////////强制传送//////////////////////////////
		if (BossWolfEventAiJudge == false)
		{
			if (Player->pos.z < 5670.0f
				&& Player->pos.z > 5020.0f
				&& Player->pos.x > -370.0f
				&& Player->pos.x < 303.0f)
				Player->pos = D3DXVECTOR3{ -28.0f,20.0f, 4380.0f };
		}
		///////////////////////////////////////////////////
	}
}


//用于重载场景(Die -> 复活)
void ReSetScene(int scene)
{
	PLAYER1 *Player = GetPlayer1();
	OBJ_CUBE_EX *ObjCube_Ex = RetObjCube_Ex();
	BOXCOLLIDER *BoxColliderScene = GetBoxCollider(SCENE_PORTAL_COLLIDER);
	BOXCOLLIDER *BoxColliderEvent = GetBoxCollider(SCENE_EVENT_COLLIDER);
	//SPHERECOLLIDER *SphereCollider = GetSphereCollider();
	CYLINDERCOLLIDER *CylinderCollider = GetCylinderCollider();
	int *ColliderIndexGrassToDesert = RetSceneColliderIndexGrassToDesert();
	int *ColliderIndexHomeToGrass = RetSceneColliderIndexHomeToGrass();
	int *ColliderIndexGrassDoorToBoss = RetSceneColliderIndexGrassDoorToBoss();
	switch (scene)
	{
	case SCENE_SCENE_GRASS:
		//清除当前场景
		ClearAllCube();
		ClearAllObjObjModel();
		//清除shadow
		InitShadow();
		//插入过场渐变
		SetFade(FADE_OUT);
		//载入Grass场景
		LoadScene(SCENE_SCENE_GRASS);
		//载入小地图数据
		CreateCurrentSmallMap(SCENE_SCENE_GRASS);
		//重写小地图
		InitUiSmallMap(0, SCENE_SCENE_GRASS);
		//载入障碍物数组
		LoadSceneBarrierList(SCENE_SCENE_GRASS);
		ResetEnemyBossWolf(0);
		//清除并载入新Monster
		LoadEnemy(SCENE_SCENE_GRASS);
		LoadItem();
		//清除当前四叉树
		ClearAllQuarTree();
		//载入新的四叉树
		InitQuarTree(-2580.0f, 0.0f, 128, 144);
		//关闭传送触发器
		BoxColliderScene[*ColliderIndexHomeToGrass].active = false;
		CurrentSceneIndex = SCENE_SCENE_GRASS;
		//打开传送触发器
		BoxColliderScene[*ColliderIndexGrassToDesert].active = true;
		//打开事件触发器
		BoxColliderEvent[*ColliderIndexGrassDoorToBoss].active = true;
		//开放boss门前的墙
		ObjCube_Ex[5833].use = false;
		ObjCube_Ex[5834].use = false;
		ObjCube_Ex[5835].use = false;
		ObjCube_Ex[5836].use = false;
		BossWolfEventActive = true;
		BossWolfEventAiJudge = false;
		SetDisPlayEnemyHp(FALSE);
		//New Player Pos
		//Player->pos = D3DXVECTOR3{ -1702.0f,20.0f, 300.0f };
		//清除携带道具
		//ClearAllGoods();
		//给一瓶药
		SetGoodsAdd(GOODS_TYPE_RED, true);
		ResetPlayer1();
		PlayerHpAdd(100);
		StopSound(SOUND_LABEL_BGM_GAME);
		PlaySound(SOUND_LABEL_BGM_GAME);
		switch (SavePoint)
		{
		case SAVE_POINT_GRASS_START:
			Player->pos = D3DXVECTOR3{ -1702.0f,20.0f, 300.0f };
			break;
		case SAVE_POINT_GRASS_BOSS:
			Player->pos = D3DXVECTOR3{ -28.0f,20.0f, 4504.0f };
			break;
		}
		break;
	case SCENE_SCENE_HOME:
		CurrentSceneIndex = SCENE_SCENE_HOME;
		SavePoint = SAVE_POINT_HOME;
		InitBullet();
		ReInitCannibalAnimation();
		//清除当前场景
		ClearAllCube();
		ClearAllObjObjModel();
		//清除shadow
		InitShadow();
		//插入过场渐变
		SetFade(FADE_OUT);
		//载入Home场景
		LoadScene(SCENE_SCENE_HOME);
		//载入小地图数据
		CreateCurrentSmallMap(SCENE_SCENE_HOME);
		//重写小地图
		InitUiSmallMap(0, SCENE_SCENE_HOME);
		//载入障碍物数组
		LoadSceneBarrierList(SCENE_SCENE_HOME);
		ResetEnemyBossWolf(0);
		//清除并载入新Monster
		LoadEnemy(SCENE_SCENE_HOME);
		//载入道具
		LoadItem();
		//清除当前四叉树
		ClearAllQuarTree();
		//载入新的四叉树
		InitQuarTree(-660.0f, 0.0f, 64, 64);
		BossWolfEventActive = true;
		BossWolfEventAiJudge = false;
		SetDisPlayEnemyHp(FALSE);
		//清除携带道具
		ClearAllGoods();
		ResetPlayer1();
		PlayerHpAdd(100);
		Player->pos = D3DXVECTOR3(720.11f, 300.0f, 85.0f);
		ReInitEnemyHp();
		break;
	}
	InitEnemyEvent();
}


void SavePointEvent()
{
	PLAYER1 *Player = GetPlayer1();

	if (Player->pos.x > -250.0f
		&& Player->pos.x < 180.0f
		&& Player->pos.z > 4504.0f
		&& Player->pos.z < 4624.0f
		)
	    SavePoint = SAVE_POINT_GRASS_BOSS;
}


int *RetSavePoint()
{
	return &SavePoint;
}


//移除除boss外 全部Enemy
//BossMove false 不移除boss
//         true  移除boss
void ClearAllEnemy(bool BossMove)
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();

	for (int i = 0; i < ENEMY_WOLF_MAX_NUM; i++)
	{
		if (EnemyWolf[i].use)
		{
			EnemyWolf[i].use = FALSE;
			DeleteSphereCollider(EnemyWolf[i].SphereColliderAckIdx);
			DeleteSphereCollider(EnemyWolf[i].SphereColliderIdx);
		}
	}
	for (int i = 0; i < ENEMY_CANNIBAL_MAX_NUM; i++)
	{
		if (EnemyCannibal[i].use)
		{
			EnemyCannibal[i].use = FALSE;
			DeleteSphereCollider(EnemyCannibal[i].SphereColliderAckIdx);
			DeleteSphereCollider(EnemyCannibal[i].SphereColliderIdx);
		}
	}
	if (BossMove)
	{
		for (int i = 0; i < ENEMY_BOSSWOLF_MAX_NUM; i++)
		{
			if (EnemyBossWolf[i].use)
			{
				EnemyBossWolf[i].use = FALSE;
				DeleteSphereCollider(EnemyBossWolf[i].SphereColliderAckIdx);
				DeleteSphereCollider(EnemyBossWolf[i].SphereColliderJumpAckIdx);
				DeleteCylinderCollider(EnemyBossWolf[i].CylinderIdx);
			}
		}
	}
}


//创建固定种子分布的tree obj模型
void CreateGroupObjTree()
{
	OBJ_CUBE_EX *Cube_Ex = RetObjCube_Ex();
	int GrassCubeNum = RetSceneCubeNum(SCENE_SCENE_GRASS);
	srand(44568);
	///考虑正态分布//////

	/////////////////////
	for (int i = 0; i < 100; i++)
	{
		int z = rand() % MAX_MAP_CUBE_Z / 2 - 20;
		int x = rand() % MAX_MAP_CUBE_X / 2;
		int resultIndex = FindTypeCubeIndex(z, x, SCENE_SCENE_GRASS, CUBE_TYPE_TERRAIN);
		if (resultIndex != -1)
		{
			CreateObjObjModel(GOODS_TYPE_TREE, D3DXVECTOR3{ Cube_Ex[resultIndex].pos.x, 20.0f, Cube_Ex[resultIndex].pos.z }, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, D3DXVECTOR3{ 2.0f, 2.0f,2.0f });
		}
	}

}
//=============================================================================
//
// エネミーWolf処理[Enemy_wolf.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "Enemy_wolf.h"
#include "camera_Ex.h"
#include "input.h"
#include "AllocateHierarchyClass.h"
#include "cylinderCollider.h"
#include "debugproc.h"
#include "GlobalFunction.h"
#include "GameMath.h"
#include "GameAi.h"
#include "player1.h"
#include "sphereCollider.h"
#include "UiEnemyHp.h"
#include "Time_c.h"
#include "shadow.h"
#include "GameEvent.h"
#include "SoulEffect.h"
#include "Scene.h"
#include "sound.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
//#define	MODEL_ENEMY_WOLF		"data/MODEL/WolfIdle.X"	// 撉傒崬傓儌僨儖柤
#define	MODEL_ENEMY_WOLF            "data/MODEL/EnemyWolfIdle.X"
#define	MODEL_ENEMY_WOLF_RUN		"data/MODEL/EnemyWolfRun.X"	// 撉傒崬傓儌僨儖柤
#define	MODEL_ENEMY_WOLF_DIE		"data/MODEL/EnemyWolfDie.X"	// 撉傒崬傓儌僨儖柤
#define	MODEL_ENEMY_WOLF_ACK        "data/MODEL/EnemyWolfAck.X"
#define	MODEL_ENEMY_WOLF_HURT        "data/MODEL/EnemyWolfHurt.X"

#define	TEXTURE_ENEMY_WOLF_FILENAME	"data/MODEL/ninja.png"

#define	ENEMY_WOLF_RADIUS		(10.0f)						// 敿宎
//#define	VALUE_MOVE_ENEMY_WOLF	(0.155f)					// 堏摦懍搙
#define	VALUE_MOVE_ENEMY_WOLF	(5.0f)					// 堏摦懍搙
#define	RATE_MOVE_ENEMY_WOLF	(0.025f)					// 堏摦姷惈學悢
#define	VALUE_ROTATE_ENEMY_WOLF	(D3DX_PI * 0.025f)			// 夞揮懍搙
#define	RATE_ROTATE_ENEMY_WOLF	(0.10f)						// 夞揮姷惈學悢
#define	VALUE_MOVE_BULLET	(1.5f)						// 抏偺堏摦懍搙


#define  ENEMY_WOLF_WAIT_TIME (100) //每个状态的等待时间

#define  ENEMY_WOLF_RUN_CHG_WAIT_TIME (700) //玩家从攻击范围脱离enemy 追击的等待时间
#define  ENEMY_WOLF_ACK_RANGE (350) //非警戒状态下触发追击的范围
#define  ENEMY_WOLF_ALERT_ACK_RANGE (1300) //警戒状态下触发追击的范围

#define ENEMY_WOLF1_VALUE_MOVE (5.0f)
#define ACK_PLAYER_ROT_TIME_MOVE (0.05f)  //转向速度
//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
static void ChangeAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller, int animationID);
static void SetAnimationTime(int index, int name, float *animationTime);
static void ResetAnimationTdPosition(int index, LPD3DXANIMATIONCONTROLLER   *Controller);
static bool EnemyWolfWaitWithSecend(int index, int NowStatus, int LastStatus);
static void ReInitOneAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller);
static void InitController(LPD3DXANIMATIONCONTROLLER *Controller, int EnemyIndex, bool bEnable);
//bool frame_change(void);

static bool bHitEvent = false;
static bool bHitEventG[ENEMY_WOLF_MAX_NUM]; //辅助重力

//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureEnemyWolf;		// 僥僋僗僠儍撉傒崬傒応強
LPD3DXMESH			g_pMeshEnemyWolf;				// ID3DXMesh僀儞僞乕僼僃僀僗傊偺億僀儞僞
LPD3DXBUFFER		g_pD3DXMatBuffEnemyWolf;		// 儊僢僔儏偺儅僥儕傾儖忣曬傪奿擺
DWORD				g_nNumMatEnemyWolf;			// 懏惈忣曬偺憤悢

D3DXMATRIX			g_mtxWorldEnemyWolf;			// 儚乕儖僪儅僩儕僢僋僗
static ENEMY_WOLF				g_EnemyWolf[ENEMY_WOLF_MAX_NUM];					// 僾儗僀儎乕儚乕僋
static ENEMY_WOLF               g_EnemyWolf_bak[ENEMY_WOLF_MAX_NUM];
//static ID3DXAnimationController*   g_pAnimController = NULL;
static LPD3DXFRAME                 g_pFrameRoot[ENEMY_WOLF_MAX_NUM];;
static D3DXMATRIX*                 g_pBoneMatrices = NULL;
static CAllocateHierarchy*         g_pAllocateHier[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimController[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSet[ENEMY_WOLF_MAX_NUM];
//////////////////////
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerTotal[ENEMY_WOLF_MAX_NUM];
//////animation/////////
static LPD3DXFRAME                 g_pFrameRootRun[ENEMY_WOLF_MAX_NUM];;
static D3DXMATRIX*                 g_pBoneMatricesRun = NULL;
static CAllocateHierarchy*         g_pAllocateHierRun[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerRun[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetRun[ENEMY_WOLF_MAX_NUM];
////////////////////////
//////DIE/////////
static LPD3DXFRAME                 g_pFrameRootDie[ENEMY_WOLF_MAX_NUM];;
static D3DXMATRIX*                 g_pBoneMatricesDie = NULL;
static CAllocateHierarchy*         g_pAllocateHierDie[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerDie[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetDie[ENEMY_WOLF_MAX_NUM];
////////////////////////
//////ACK///////////////
static LPD3DXFRAME                 g_pFrameRootAck[ENEMY_WOLF_MAX_NUM];;
static D3DXMATRIX*                 g_pBoneMatricesAck = NULL;
static CAllocateHierarchy*         g_pAllocateHierAck[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerAck[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetAck[ENEMY_WOLF_MAX_NUM];
/////////////////////////
//////HURT///////////////
static LPD3DXFRAME                 g_pFrameRootHurt[ENEMY_WOLF_MAX_NUM];;
static D3DXMATRIX*                 g_pBoneMatricesHurt = NULL;
static CAllocateHierarchy*         g_pAllocateHierHurt[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerHurt[ENEMY_WOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetHurt[ENEMY_WOLF_MAX_NUM];
/////////////////////////


static char *StateName[ENEMY_WOLF_ANIMATION_MAX] =
{
	"IDLE",
	"RUN",
	"DIE",
	"ACK",
	"HURT"
};
////////////////像素着色器/////////////////////////////////////
//static D3DXHANDLE ViewProjMatrixHandle = NULL;
//static D3DXHANDLE PixTextureHandle = NULL;
//
//static LPD3DXEFFECT g_pEffect = NULL;

static D3DXHANDLE Samp0Handle = NULL;
static D3DXCONSTANT_DESC Samp0Desc;
static IDirect3DPixelShader9* g_ToonShader = 0;
static IDirect3DPixelShader9* TexPS = 0;
static ID3DXConstantTable* TransformConstantTable = 0;
static D3DXHANDLE EnemyWolfAlphaHandle;
static D3DXHANDLE EnemyWolfColorHandle;
static D3DXHANDLE WorldViewMatrixHandle;
static bool LoadPixHLSLEnemyWolf();
////////////////////////////////////////////////////////////////////////
//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitEnemyWolf(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD *timing = RetTiming(TIMING_ENEMY_WAIT);

	g_pD3DTextureEnemyWolf = NULL;
	g_pMeshEnemyWolf = NULL;
	g_pD3DXMatBuffEnemyWolf = NULL;

	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		//g_EnemyWolf[num].pos = D3DXVECTOR3(40.0f, 20.0f, 40.0f);
		g_EnemyWolf[num].old_pos = g_EnemyWolf[num].pos;
		g_EnemyWolf[num].last_frame_pos = g_EnemyWolf[num].pos;
		g_EnemyWolf[num].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyWolf[num].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyWolf[num].scl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		g_EnemyWolf[num].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_EnemyWolf[num].use = false;
		g_EnemyWolf[num].AckJudge = true;
		g_EnemyWolf[num].AckPlayerRotJudge = true;
		g_EnemyWolf[num].AckPlayerAckTimeJudge = false;
		g_EnemyWolf[num].AckPlayerRotStartJudge = true;
		g_EnemyWolf[num].AckPlayerRotStartYValue = 0.0f;
		g_EnemyWolf[num].DieJudge = true;
		g_EnemyWolf[num].HurtJudge = false;
		g_EnemyWolf[num].HurtJudgeStart = true;
		g_EnemyWolf[num].LastAnimationID = ENEMY_WOLF_IDLE;
		g_EnemyWolf[num].status = ENEMY_WOLF_IDLE;
		g_EnemyWolf[num].hp = ENEMY_WOLF_MAX_HP;
		g_EnemyWolf[num].down_dir = false;
		g_EnemyWolf[num].fRadius = ENEMY_WOLF_RADIUS;
		g_EnemyWolf[num].AlertStatus = false;
		for(int i = 0; i < ENEMY_WOLF_ANIMATION_MAX; i++)
		    g_EnemyWolf[num].PlayerActionOverJudge[i] = false;
		//g_EnemyWolf[num].SphereColliderIdx = CreateSphereCollider(g_EnemyWolf[num].pos, g_EnemyWolf[num].rot, g_EnemyWolf[num].scl * 3);
		//g_EnemyWolf[num].ShadowIdx = CreateShadow(g_EnemyWolf[num].pos, g_EnemyWolf[num].rot, g_EnemyWolf[num].scl, true);
		Time_Stop(timing[num]);
		bHitEventG[num] = false;
	}

	// X僼傽僀儖偺撉傒崬傒
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
	    g_pAllocateHier[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 
	    
	    g_pAllocateHierRun[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 
	    
	    g_pAllocateHierDie[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 
	    
	    g_pAllocateHierAck[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 

		g_pAllocateHierHurt[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 
		// X僼傽僀儖傪撉傒崬傓
		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_WOLF,
			D3DXMESH_MANAGED,
			pDevice,
			g_pAllocateHier[num],
			NULL,
			&g_pFrameRoot[num],
			&g_pAnimController[num]
		)))
		{
			//g_pAnimController->GetEventDesc();
			return E_FAIL;
		}

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_WOLF_RUN,
			D3DXMESH_MANAGED,
			pDevice,
			g_pAllocateHierRun[num],
			NULL,
			&g_pFrameRootRun[num],
			&g_pAnimControllerRun[num]
		)))
		{
			return E_FAIL;
		}

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_WOLF_DIE,
			D3DXMESH_MANAGED,
			pDevice,
			g_pAllocateHierDie[num],
			NULL,
			&g_pFrameRootDie[num],
			&g_pAnimControllerDie[num]
		)))
		{
			return E_FAIL;
		}

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_WOLF_ACK,
			D3DXMESH_MANAGED,
			pDevice,
			g_pAllocateHierAck[num],
			NULL,
			&g_pFrameRootAck[num],
			&g_pAnimControllerAck[num]
		)))
		{
			return E_FAIL;
		}

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_WOLF_HURT,
			D3DXMESH_MANAGED,
			pDevice,
			g_pAllocateHierHurt[num],
			NULL,
			&g_pFrameRootHurt[num],
			&g_pAnimControllerHurt[num]
		)))
		{
			return E_FAIL;
		}

		SetupBoneMatrixPointers(g_pFrameRoot[num], g_pFrameRoot[num]);

		SetupBoneMatrixPointers(g_pFrameRootRun[num], g_pFrameRootRun[num]);

		SetupBoneMatrixPointers(g_pFrameRootDie[num], g_pFrameRootDie[num]);

		SetupBoneMatrixPointers(g_pFrameRootAck[num], g_pFrameRootAck[num]);

		SetupBoneMatrixPointers(g_pFrameRootHurt[num], g_pFrameRootHurt[num]);
	}
	//g_pFrameRoot->pMeshContainer;
	//崪偺埵抲傪惂掕
//#if 0
	// 僥僋僗僠儍偺撉傒崬傒
	D3DXCreateTextureFromFile(pDevice,					// 僨僶僀僗傊偺億僀儞僞
		TEXTURE_ENEMY_WOLF_FILENAME,		// 僼傽僀儖偺柤慜
		&g_pD3DTextureEnemyWolf);	// 撉傒崬傓儊儌儕乕
//#endif
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		InitController(g_pAnimController, num, true);
		InitController(g_pAnimControllerRun, num, false);
		InitController(g_pAnimControllerDie, num, false);
		InitController(g_pAnimControllerAck, num, false);
		InitController(g_pAnimControllerHurt, num, true);
		g_pAnimControllerTotal[num] = g_pAnimController[num];
		InitController(g_pAnimControllerTotal, num, true);
	}
	//g_EnemyWolf.SphereColliderIdx = CreateSphereCollider(g_EnemyWolf.pos, g_EnemyWolf.rot, g_EnemyWolf.scl);

	//HRESULT hr;
	////
	////// Load the effect from file.
	//LPD3DXBUFFER pErrors = NULL;
	//hr = D3DXCreateEffectFromFile(pDevice, TEXT("data\\SHADER\\D3DObjViewer.fx"), NULL, NULL,
	//	0, NULL, &g_pEffect, &pErrors);
	//if (FAILED(hr))
	//{
	//	if (pErrors)
	//	{
	//		//MessageBoxA(g_hWnd, (LPCSTR)pErrors->GetBufferPointer(), "Effect error", MB_OK | MB_ICONSTOP);
	//		pErrors->Release();
	//	}
	//	return hr;
	//}
	LoadPixHLSLEnemyWolf();
	/////////////像素着色器句柄///////////////
	//Samp0Handle = TransformConstantTable->GetConstantByName(0, "Samp0");
	//设置对着色器变量Samp0、Samp1的描述
	UINT count;
	//TransformConstantTable->GetConstantDesc(Samp0Handle, &Samp0Desc, &count);
	//设定各着色器变量为初始值
	//TransformConstantTable->SetDefaults(pDevice);

	//EnemyWolfAlphaHandle = TransformConstantTable->GetConstantByName(NULL, "player_alpha");
	//EnemyWolfColorHandle = TransformConstantTable->GetConstantByName(NULL, "Color");
	//WorldViewMatrixHandle = TransformConstantTable->GetConstantByName(NULL, "WorldViewMatrix");
	/////////////////////////////////////////////////////

	//g_EnemyWolf.CylinderColliderIdx = CreateCylinderCollider(g_EnemyWolf.pos, g_EnemyWolf.rot, g_EnemyWolf.scl);
	//g_EnemyWolf.CylinderColliderIdx = CreateCylinderCollider(g_EnemyWolf.pos, g_EnemyWolf.rot, g_EnemyWolf.scl);
	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitEnemyWolf(void)
{
	if (g_pD3DTextureEnemyWolf != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_pD3DTextureEnemyWolf->Release();
		g_pD3DTextureEnemyWolf = NULL;
	}

	if (g_pMeshEnemyWolf != NULL)
	{// 儊僢僔儏偺奐曻
		g_pMeshEnemyWolf->Release();
		g_pMeshEnemyWolf = NULL;
	}

	if (g_pD3DXMatBuffEnemyWolf != NULL)
	{// 儅僥儕傾儖偺奐曻
		g_pD3DXMatBuffEnemyWolf->Release();
		g_pD3DXMatBuffEnemyWolf = NULL;
	}
	//g_pEffect->Release();
}


float lerpTime = 0.0f;
void UpdateEnemyWolf(void)
{
	PLAYER1 *Player = GetPlayer1();
	int CurrentSceneIndex = RetCurrentSceneIndex();
	DWORD *timing = RetTiming(TIMING_ENEMY_WAIT);
	int *timeIndex = RetTimeIndex(TIMING_ENEMY_WAIT);

	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		if (g_EnemyWolf[num].use)
		{
			if(g_EnemyWolf[num].rot.y >= 2 * D3DX_PI)
			{
				g_EnemyWolf[num].rot.y = 0.0f;
			}
			else if(g_EnemyWolf[num].rot.y <= -(2 * D3DX_PI))
			{
				g_EnemyWolf[num].rot.y = 0.0f;
			}
			//PrintDebugProc("enemy 0 = pos.x=%f,y=%f\n", g_EnemyWolf[num].pos.x, g_EnemyWolf[num].pos.y);
			//重力///////////////
			if (g_EnemyWolf[num].down_dir)
			{
				if (bHitEventG[num] == false)
				{
					bHitEventG[num] = true;
				}
			}
			if (bHitEventG[num] == false)
				g_EnemyWolf[num].pos.y -= 3.0f;
			///////////////////////////
			if (g_EnemyWolf[num].hp <= 0)
			{
				D3DXTRACK_DESC td1;
				ChangeAnimation(num, g_pAnimControllerDie, ENEMY_WOLF_DIE);
				g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyWolf[num].currentTrack, &td1);

				if (g_EnemyWolf[num].DieJudge)
				{
					ReInitOneAnimation(num, g_pAnimControllerTotal);
					DeleteSphereCollider(g_EnemyWolf[num].SphereColliderIdx);
					DeleteSphereCollider(g_EnemyWolf[num].SphereColliderAckIdx);
					SetSoul(num , ENEMY_TYPE_WOLF, 5);
					g_EnemyWolf[num].DieJudge = false;
				}

				if (td1.Position >= 1.0f)
				{
					SetDisPlayEnemyHp(FALSE);
					D3DXTRACK_DESC td2;
					td2.Enable = false;
					g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyWolf[num].currentTrack, &td2);
					g_EnemyWolf[num].use = false;
				}
				//PrintDebugProc("enemy:%f\n", td1.Position);
			}
			else
			{
				if (g_EnemyWolf[num].HurtJudge == false)
				{
					D3DXVECTOR3 vDistance = Player->pos - g_EnemyWolf[num].pos;

					float length = D3DXVec3Length(&vDistance);
					int Range;
					//PrintDebugProc("enemy status = %d\n", g_EnemyWolf[num].status);
					//发现player
					if (g_EnemyWolf[num].AlertStatus)
					{
						Range = ENEMY_WOLF_ALERT_ACK_RANGE;
					}
					else
						Range = ENEMY_WOLF_ACK_RANGE;
					if (length > Range)
					{
						//如果在警戒状态 取消
						if (g_EnemyWolf[num].AlertStatus)
							g_EnemyWolf[num].AlertStatus = false;
						SetbEnemyAstart(num, false);
						ChangeAnimation(num, g_pAnimController, ENEMY_WOLF_IDLE);
						g_EnemyWolf[num].status = ENEMY_WOLF_STATUS_IDLE;
					}
					else
					{
						//进入警戒状态
						if (g_EnemyWolf[num].AlertStatus == false)
							g_EnemyWolf[num].AlertStatus = true;
						//任何状态切换的等待
						if (g_EnemyWolf[num].animationID != g_EnemyWolf[num].LastAnimationID)
						{

							SetbEnemyAstart(num, false);
							bool WaitJudge = EnemyWolfWaitWithSecend(num, g_EnemyWolf[num].animationID, g_EnemyWolf[num].LastAnimationID);
							if (WaitJudge == false)
							{
								Time_Start(timeIndex[num]);
								//ChangeAnimation(num, g_pAnimController, ENEMY_WOLF_IDLE);
								g_EnemyWolf[num].status = ENEMY_WOLF_STATUS_IDLE;
							}
							else
							{
								//Time_Reset(timeIndex[num]);
								Time_Stop(timeIndex[num]);
								g_EnemyWolf[num].LastAnimationID = g_EnemyWolf[num].animationID;
								g_EnemyWolf[num].status = ENEMY_WOLF_STATUS_WAITOVER;
							}
						}
						else
						{
							if (length > 80 
								&& g_EnemyWolf[num].animationID !=ENEMY_WOLF_ACK)
							{
								SetbEnemyAstart(num, true);
								//if(g_EnemyWolf[num].status == ENEMY_WOLF_STATUS_WAITOVER)
								ChangeAnimation(num, g_pAnimControllerRun, ENEMY_WOLF_RUN);
								g_EnemyWolf[num].status = ENEMY_WOLF_RUN;
							}
							else
							{
								//PrintDebugProc("Rot Y = %f, %f\n", g_EnemyWolf[num].rot.y, g_EnemyWolf[num].AckPlayerRotStartYValue);
								if (g_EnemyWolf[num].AckPlayerRotJudge)
								{
									SetbEnemyAstart(num, false);
									if (g_EnemyWolf[num].AckPlayerRotStartJudge)
									{
										float MoveRot = RetRotValue(D3DXVECTOR2{ Player->pos.x, Player->pos.z }, D3DXVECTOR2{ g_EnemyWolf[num].pos.x, g_EnemyWolf[num].pos.z });
										g_EnemyWolf[num].rot.y = D3DX_PI / 2 - MoveRot;
										//g_EnemyWolf[num].AckPlayerRotTime += ACK_PLAYER_ROT_TIME_MOVE;
										////起点
										//g_EnemyWolf[num].AckPlayerRotStartYValue = g_EnemyWolf[num].rot.y;
										////终点
										//g_EnemyWolf[num].AckPlayerRotOverYValue = D3DX_PI / 2 - RetRotValue(D3DXVECTOR2{ Player->pos.x, Player->pos.z }, D3DXVECTOR2{ g_EnemyWolf[num].pos.x, g_EnemyWolf[num].pos.z });

										//////////////////////////
										//D3DXVECTOR3 axis = g_EnemyWolf[num].pos - Player->pos;
										//D3DXVECTOR3 Result;
										//D3DXMATRIX mtxRot, mtxTranslate, mtx;
										//
										//D3DXMatrixIdentity(&mtx);
										//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EnemyWolf[num].rot.y, g_EnemyWolf[num].rot.x, g_EnemyWolf[num].rot.z);
										//D3DXMatrixTranslation(&mtxTranslate, g_EnemyWolf[num].pos.x, g_EnemyWolf[num].pos.y, g_EnemyWolf[num].pos.z);
										//
										//mtx = mtxTranslate * mtxRot;
										//D3DXVec3Cross(&Result, &D3DXVECTOR3{ 0.0f,0.0f,mtx._43 }, &axis);
										//if (Result.y > 0)
										//	g_EnemyWolf[num].AckPlayerRotDirect = 1;
										//else
										//	g_EnemyWolf[num].AckPlayerRotDirect = 0;
										//////////////////////////
										g_EnemyWolf[num].AckPlayerRotJudge = false;
										g_EnemyWolf[num].AckPlayerRotStartJudge = false;
										//lerpTime = 0.0f;
									} 
									 
									//PrintDebugProc("AAAAA = %f , %f, %f\n", g_EnemyWolf[num].AckPlayerRotStartYValue, g_EnemyWolf[num].rot.y, g_EnemyWolf[num].AckPlayerRotOverYValue);
									//PrintDebugProc("BBBBBBBBBB = %f\n", g_EnemyWolf[num].AckPlayerRotStartYValue - g_EnemyWolf[num].AckPlayerRotOverYValue);
									//if(fabs(g_EnemyWolf[num].AckPlayerRotStartYValue - g_EnemyWolf[num].AckPlayerRotOverYValue) < 0.3f)
									//{
									//	g_EnemyWolf[num].AckPlayerRotJudge = false;
                                    //}
									//else
									//{
									//	PrintDebugProc("RRRREUSLT = %f, %f\n", fabs(g_EnemyWolf[num].rot.y), fabs(g_EnemyWolf[num].AckPlayerRotOverYValue) + ACK_PLAYER_ROT_TIME_MOVE + 0.1f);
									//	float bbbb = fabs(g_EnemyWolf[num].AckPlayerRotOverYValue) + ACK_PLAYER_ROT_TIME_MOVE + 0.1f;
									//	if (fabs(g_EnemyWolf[num].rot.y) <= fabs(g_EnemyWolf[num].AckPlayerRotOverYValue) + ACK_PLAYER_ROT_TIME_MOVE + 0.1f)
									//	{
									//		float aaaa = fabs(g_EnemyWolf[num].AckPlayerRotOverYValue)- ACK_PLAYER_ROT_TIME_MOVE - 0.1f;
									//		if (fabs(g_EnemyWolf[num].rot.y) >= fabs(g_EnemyWolf[num].AckPlayerRotOverYValue) - ACK_PLAYER_ROT_TIME_MOVE - 0.1f)
									//			g_EnemyWolf[num].AckPlayerRotJudge = false;
									//	}
									//	//左
									//	if (g_EnemyWolf[num].AckPlayerRotDirect == 0)
									//	{
									//		g_EnemyWolf[num].rot.y -= ACK_PLAYER_ROT_TIME_MOVE;
									//	}
									//	//右
									//	else
									//	{
									//		g_EnemyWolf[num].rot.y += ACK_PLAYER_ROT_TIME_MOVE;
									//	}
									//}
								}
								else
								{
									SetbEnemyAstart(num, false);
									g_EnemyWolf[num].status = ENEMY_WOLF_STATUS_ACK;
									ChangeAnimation(num, g_pAnimControllerAck, ENEMY_WOLF_ACK);

									D3DXTRACK_DESC td1;
									g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyWolf[num].currentTrack, &td1);

									//PlaySound(SOUND_LABEL_SE_ENEMY_WOLF_ACK);
									if (td1.Position > 0.9f)
									{
										g_EnemyWolf[num].AckPlayerAckTimeJudge = true;
									}

									if (td1.Position > 1.5f)
									{
										ResetAnimationTdPosition(num, g_pAnimControllerTotal);
										ChangeAnimation(num, g_pAnimController, ENEMY_WOLF_IDLE);
										Time_Start(timeIndex[num]);
										g_EnemyWolf[num].AckPlayerRotJudge = true;
										g_EnemyWolf[num].AckPlayerAckTimeJudge = false;
										g_EnemyWolf[num].AckPlayerRotStartJudge = true;
									}
								}
							}
						}
					}
				}
				//受伤事件
				else
				{

					if (g_EnemyWolf[num].HurtJudgeStart)
					{
						//////////初始化攻击事件///////
						g_EnemyWolf[num].AckPlayerRotJudge = true;
						g_EnemyWolf[num].AckPlayerAckTimeJudge = false;
						g_EnemyWolf[num].AckPlayerRotStartJudge = true;
						//////////////////////////////
						D3DXTRACK_DESC td2;
						td2.Position = 0.05f;
						td2.Speed = 1.0f;
						td2.Weight = 1.0f;
						td2.Enable = true;
						g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyWolf[num].currentTrack, &td2);
						g_EnemyWolf[num].HurtJudgeStart = false;
						ChangeAnimation(num, g_pAnimControllerHurt, ENEMY_WOLF_HURT);
						g_EnemyWolf[num].status = ENEMY_WOLF_STATUS_HURT;
					}
					SetbEnemyAstart(num, false);

					D3DXTRACK_DESC td1;
					g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyWolf[num].currentTrack, &td1);

					if (td1.Position > 0.5f)
					{
						D3DXTRACK_DESC td2;
						td2.Enable = false;
						td2.Speed = 1.0f;
						td2.Weight = 1.0f;
						g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyWolf[num].currentTrack, &td2);
						//ResetAnimationTdPosition(num, g_pAnimControllerTotal);
						g_EnemyWolf[num].HurtJudge = false;
						g_EnemyWolf[num].HurtJudgeStart = true;
					}
				}
			}
			g_pAnimControllerTotal[num]->AdvanceTime(0.015f, NULL);

			//D3DXTRACK_DESC td1;
			//g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyWolf[num].currentTrack, &td1);

			SetPositionShadow(g_EnemyWolf[num].ShadowIdx, g_EnemyWolf[num].pos, g_EnemyWolf[num].rot, 
				D3DXVECTOR3{ g_EnemyWolf[num].scl.x,
				g_EnemyWolf[num].scl.y,
				g_EnemyWolf[num].scl.z});
			//PrintDebugProc("enemy: %f, %f\n", g_EnemyWolf[num].pos.x, g_EnemyWolf[num].pos.z);
	        ////////////////////////////////////////////////////////////////
			//D3DXVECTOR3 axis = g_EnemyWolf[num].pos - Player->pos;
			//D3DXVECTOR3 Result;
			//D3DXMATRIX mtxRot, mtxTranslate,mtx;
			//// 夞揮傪斀塮
			//D3DXMatrixIdentity(&mtx);
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EnemyWolf[num].rot.y, g_EnemyWolf[num].rot.x, g_EnemyWolf[num].rot.z);
			//D3DXMatrixTranslation(&mtxTranslate, g_EnemyWolf[num].pos.x, g_EnemyWolf[num].pos.y, g_EnemyWolf[num].pos.z);
			//
			//mtx = mtxTranslate * mtxRot;
			//D3DXVec3Cross(&Result, &D3DXVECTOR3{ 0.0f,0.0f,mtx._43 }, &axis);
		    //PrintDebugProc("JIIIIIIIIIIIIIa jiao = %f\n", Result.y);
			/////////////////////////////////////////////////////////////
			SetPositionSphereCollider(g_EnemyWolf[num].SphereColliderIdx,
				D3DXVECTOR3{ g_EnemyWolf[num].pos.x,g_EnemyWolf[num].pos.y + 10.0f, g_EnemyWolf[num].pos.z },
				D3DXVECTOR3{ g_EnemyWolf[num].rot.x ,g_EnemyWolf[num].rot.y,g_EnemyWolf[num].rot.z }, D3DXVECTOR3{ 1.0f ,1.0f,1.0f } *3.0f);
			SetPositionSphereCollider(g_EnemyWolf[num].SphereColliderAckIdx,
				D3DXVECTOR3{ g_EnemyWolf[num].pos.x + sinf(g_EnemyWolf[num].rot.y + D3DX_PI) * 65,
				g_EnemyWolf[num].pos.y + 20.0f,
				g_EnemyWolf[num].pos.z + cosf(g_EnemyWolf[num].rot.y + D3DX_PI) * 65 },
				D3DXVECTOR3{ g_EnemyWolf[num].rot.x, g_EnemyWolf[num].rot.y, g_EnemyWolf[num].rot.z }, D3DXVECTOR3{ 1.0f ,1.0f,1.0f } * 2.0f );

			if (g_EnemyWolf[num].pos != g_EnemyWolf[num].last_frame_pos)

			{
				g_EnemyWolf[num].down_dir = false;
				bHitEventG[num] = false;
			}
			g_EnemyWolf[num].last_frame_pos = g_EnemyWolf[num].pos;
		}
	}
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawEnemyWolf(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScl;
	D3DXMATERIAL *pD3DXMat;
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		if (g_EnemyWolf[num].use)
		{
			if (CubeInFrustum(g_EnemyWolf[num].pos))
			{
				// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
				D3DXMatrixIdentity(&g_mtxWorldEnemyWolf);

				D3DXMatrixScaling(&mtxScl, g_EnemyWolf[num].scl.x, g_EnemyWolf[num].scl.y, g_EnemyWolf[num].scl.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyWolf, &g_mtxWorldEnemyWolf, &mtxScl);

				// 夞揮傪斀塮
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EnemyWolf[num].rot.y, g_EnemyWolf[num].rot.x, g_EnemyWolf[num].rot.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyWolf, &g_mtxWorldEnemyWolf, &mtxRot);

				// 堏摦傪斀塮
				D3DXMatrixTranslation(&mtxTranslate, g_EnemyWolf[num].pos.x, g_EnemyWolf[num].pos.y, g_EnemyWolf[num].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyWolf, &g_mtxWorldEnemyWolf, &mtxTranslate);
				//if(g_pD3DTextureEnemyWolf != NULL)
				//    pDevice->SetTexture(0, g_pD3DTextureEnemyWolf);
				//// 儚乕儖僪儅僩儕僢僋僗偺愝掕

				//TransformConstantTable->SetMatrix(pDevice, WorldViewMatrixHandle, &g_mtxWorldEnemyWolf);
				////TransformConstantTable->SetFloat(pDevice, g_Color, 0);
				//TransformConstantTable->SetVector(pDevice, EnemyWolfColorHandle, &g_Color[0]);
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemyWolf);

				//pDevice->SetRenderState(D3DRS_ZENABLE, true);
				//DrawShadow(g_EnemyWolf[num].ShadowIdx, OBJ_EN_ENEMY_TYPE, num);
				//pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
				//pDevice->SetRenderState(D3DRS_ZENABLE, false);

				pDevice->SetRenderState(D3DRS_ZENABLE, true);
				if (g_EnemyWolf[num].animationID == ENEMY_WOLF_RUN)
				{
					UpdateFrameMatrices(g_pFrameRootRun[num], &g_mtxWorldEnemyWolf);
					DrawFrame(pDevice, g_pFrameRootRun[num], 0);
				}
				else if (g_EnemyWolf[num].animationID == ENEMY_WOLF_IDLE)
				{
					UpdateFrameMatrices(g_pFrameRoot[num], &g_mtxWorldEnemyWolf);
					DrawFrame(pDevice, g_pFrameRoot[num], 0);
				}
				else if (g_EnemyWolf[num].animationID == ENEMY_WOLF_DIE)
				{
					UpdateFrameMatrices(g_pFrameRootDie[num], &g_mtxWorldEnemyWolf);
					DrawFrame(pDevice, g_pFrameRootDie[num], 0);
				}
				else if (g_EnemyWolf[num].animationID == ENEMY_WOLF_ACK)
				{
					UpdateFrameMatrices(g_pFrameRootAck[num], &g_mtxWorldEnemyWolf);
					DrawFrame(pDevice, g_pFrameRootAck[num], 0);
				}
				else if (g_EnemyWolf[num].animationID == ENEMY_WOLF_HURT)
				{
					UpdateFrameMatrices(g_pFrameRootHurt[num], &g_mtxWorldEnemyWolf);
					DrawFrame(pDevice, g_pFrameRootHurt[num], 0);
				}
				pDevice->SetRenderState(D3DRS_ZENABLE, false);

				//UINT passes = 0;
				//if ( SUCCEEDED(g_pEffect->Begin(&passes, 0)))
				//{
				//	g_pEffect->BeginPass(0);
				//
				//	D3DXMATRIX mT, mR, mS;
				//	D3DXMATRIX mWVP, mWIT, mView, mProj;
				//	D3DXMATRIX mWorld;
				//
				//	D3DXMatrixIdentity(&mWorld);
				//
				//	D3DXMatrixScaling(&mS, g_EnemyWolf.scl.x, g_EnemyWolf.scl.y, g_EnemyWolf.scl.z);
				//	D3DXMatrixMultiply(&mWorld, &mWorld, &mS);
				//
				//	D3DXMatrixRotationYawPitchRoll(&mR, g_EnemyWolf.rot.y, g_EnemyWolf.rot.x, g_EnemyWolf.rot.z);
				//	D3DXMatrixMultiply(&mWorld, &mWorld, &mR);
				//
				//	D3DXMatrixTranslation(&mT, g_EnemyWolf.pos.x, g_EnemyWolf.pos.y, g_EnemyWolf.pos.z);
				//	D3DXMatrixMultiply(&mWorld, &mWorld, &mT);
				//
				//	pDevice->GetTransform(D3DTS_VIEW, &mView);
				//	pDevice->GetTransform(D3DTS_PROJECTION, &mProj);
				//
				//	mWVP = mWorld * mView * mProj;
				//
				//	//if (g_EnemyWolf.pD3DTexture != NULL)
				//	//{
				//	//	g_pEffect->SetTexture("texDiffuse", g_EnemyWolf[i].pD3DTexture);
				//	//}
				//	//else
				//	//	g_pEffect->SetTexture("texDiffuse", NULL);
				//	//g_pEffect->SetTexture("texDiffuse", pD3DTexture);
				//	g_pEffect->SetBool("useDiffuseTexture", true);
				//
				//	g_pEffect->SetMatrix("mWorld", &mWorld);
				//	g_pEffect->SetMatrix("mWVP", &mWVP);
				//	g_pEffect->SetMatrix("mWIT", &mWIT);
				//
				//	g_pEffect->CommitChanges();
				//
				//	UpdateFrameMatrices(g_pFrameRoot, &mWorld);
				//	DrawFrame(pDevice, g_pFrameRoot, 0);
				//
				//	g_pEffect->EndPass();
				//	g_pEffect->End();
				//}
			}
		}
	}
}



ENEMY_WOLF *GetEnemyWolf(void)
{
	return &g_EnemyWolf[0];
}


void SetAnimationTime(int index, int name, float *animationTime)
{
	float time;
	switch (name)
	{
	case ENEMY_WOLF_IDLE:
		time = 0.0f;
		break;
	case ENEMY_WOLF_RUN:
		time = 0.0f;
		break;
	case ENEMY_WOLF_DIE:
		time = 0.0f;
		break;
	case ENEMY_WOLF_ACK:
		time = 0.0f;
		break;
	case ENEMY_WOLF_HURT:
		time = 0.0f;
		break;
	}


	//animationTime[index] = time / 30;

	return;
}
//=============================================================================
// 傾僯儊乕僔儑儞傪愗傝懼偊
//=============================================================================

void ChangeAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller, int animationID)
{

	if (g_EnemyWolf[index].animationID != animationID)
	{
		//g_pAnimControllerTotal[index] = Controller;
		//
		//g_EnemyWolf.animationTime[g_EnemyWolf.animationID] = 0;
		//
		//SetAnimationTime(animationID, g_EnemyWolf.animationTime[animationID]);
		//
		//DWORD dwNewTrack = (g_EnemyWolf.currentTrack == 0 ? 1 : 0);
		//
		//LPD3DXANIMATIONSET g_pAnimationSet = NULL;
		//
		//Controller->GetAnimationSetByName(StateName[animationID], &g_pAnimationSet);
		//Controller->SetTrackAnimationSet(dwNewTrack, g_pAnimationSet);
		//g_pAnimationSet->Release();
		//
		//Controller->UnkeyAllTrackEvents(g_EnemyWolf.currentTrack);
		//Controller->UnkeyAllTrackEvents(dwNewTrack);
		//
		//D3DXTRACK_DESC td;
		//Controller->GetTrackDesc(g_EnemyWolf.currentTrack, &td);
		//
		//td.Position = 0.0f;
		//
		//Controller->SetTrackDesc(dwNewTrack, &td);
		//g_EnemyWolf.animationID = animationID;
		//
		//Controller->KeyTrackEnable(g_EnemyWolf.currentTrack, FALSE, g_EnemyWolf.curTime + 0.25f);
		//Controller->KeyTrackSpeed(g_EnemyWolf.currentTrack, 0.0f, g_EnemyWolf.curTime, 0.25f, D3DXTRANSITION_LINEAR);
		//Controller->KeyTrackWeight(g_EnemyWolf.currentTrack, 0.0f, g_EnemyWolf.curTime, 0.25f, D3DXTRANSITION_LINEAR);
		//
		//
		//Controller->SetTrackEnable(dwNewTrack, TRUE);
		//Controller->KeyTrackSpeed(dwNewTrack, 1.0f, g_EnemyWolf.curTime, 0.25f, D3DXTRANSITION_LINEAR);
		//Controller->KeyTrackWeight(dwNewTrack, 1.0f, g_EnemyWolf.curTime, 0.25f, D3DXTRANSITION_LINEAR);
		//
		//
		//g_EnemyWolf.currentTrack = dwNewTrack;
		//
		//Controller->SetTrackAnimationSet(g_EnemyWolf.currentTrack, g_pAnimationSet);
		//
		//g_EnemyWolf.curTime = 0.0f;

		ReInitOneAnimation(index, Controller);
		
		g_EnemyWolf[index].animationTime[g_EnemyWolf[index].animationID] = 0;
		
		SetAnimationTime(index, animationID, &g_EnemyWolf[index].animationTime[animationID]);
		
		DWORD dwNewTrack = (g_EnemyWolf[index].currentTrack == 0 ? 1 : 0);
		
		LPD3DXANIMATIONSET g_pAnimationSet = NULL;
		
		Controller[index]->GetAnimationSetByName(StateName[animationID], &g_pAnimationSet);
		Controller[index]->SetTrackAnimationSet(dwNewTrack, g_pAnimationSet);
		//g_pAnimationSet->Release();
		
		Controller[index]->UnkeyAllTrackEvents(g_EnemyWolf[index].currentTrack);
		Controller[index]->UnkeyAllTrackEvents(dwNewTrack);
		
		D3DXTRACK_DESC td;
		Controller[index]->GetTrackDesc(g_EnemyWolf[index].currentTrack, &td);

		td.Position = 0.1f;
		
		Controller[index]->SetTrackDesc(dwNewTrack, &td);
		g_EnemyWolf[index].animationID = animationID;
		
		Controller[index]->KeyTrackEnable(g_EnemyWolf[index].currentTrack, FALSE, g_EnemyWolf[index].curTime + 0.25f);
		Controller[index]->KeyTrackSpeed(g_EnemyWolf[index].currentTrack, 0.0f, g_EnemyWolf[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);
		Controller[index]->KeyTrackWeight(g_EnemyWolf[index].currentTrack, 0.0f, g_EnemyWolf[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);
		
		
		Controller[index]->SetTrackEnable(dwNewTrack, TRUE);
		Controller[index]->KeyTrackSpeed(dwNewTrack, 1.0f, g_EnemyWolf[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);
		Controller[index]->KeyTrackWeight(dwNewTrack, 1.0f, g_EnemyWolf[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);
		
		
		g_EnemyWolf[index].currentTrack = dwNewTrack;
		
		Controller[index]->SetTrackAnimationSet(g_EnemyWolf[index].currentTrack, g_pAnimationSet);
		
		g_EnemyWolf[index].curTime = 0.0f;

		g_pAnimControllerTotal[index] = Controller[index];
	}
}


/////////////////////////////////像素着色器////////////////////////////
bool LoadPixHLSLEnemyWolf()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ID3DXBuffer* shader = 0;
	ID3DXBuffer* errorBuffer = 0;
	HRESULT hr = 0;

	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

	hr = D3DXCompileShaderFromFile(
		"data/SHADER/PixHLSL_Player1.fx",
		0,
		0,
		"ps_main",
		//D3DXGetPixelShaderProfile(g_d3d_device),
		"ps_3_0",
		D3DXSHADER_DEBUG,
		&shader,
		&errorBuffer,
		&TransformConstantTable);
	if (errorBuffer)
	{
		::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		//d3d::Release<ID3DXBuffer*>(errorBuffer);
		return false;
	}

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", 0, 0);
		return false;
	}

	hr = pDevice->CreatePixelShader(
		(DWORD*)shader->GetBufferPointer(),
		&g_ToonShader);
	return true;
}


void BackUpEnemyWolf(void)
{
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		g_EnemyWolf_bak[num] = g_EnemyWolf[num];
	}
}

HRESULT RestartEnemyWolf(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		g_EnemyWolf[num] = g_EnemyWolf_bak[num];
	}
	return S_OK;
}


void ResetEnemyWolfAckJudge()
{
	for(int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		g_EnemyWolf[num].AckJudge = true;
	}
}


bool EnemyWolfWaitWithSecend(int index, int NowStatus, int LastStatus)
{
	DWORD *timing = RetTiming(TIMING_ENEMY_WAIT);
	int *timeIndex = RetTimeIndex(TIMING_ENEMY_WAIT);

	//PrintDebugProc("Enemy Wait %d = %d\n", index, (int)timing[index]);
	//ENEMY_WOLF_RUN_CHG_WAIT_TIME;

	int time;
	switch (LastStatus)
	{
	    case ENEMY_WOLF_STATUS_ACK:
			time = ENEMY_WOLF_RUN_CHG_WAIT_TIME;
			break;
		default:
			time = ENEMY_WOLF_WAIT_TIME;
			break;
	}

	//PrintDebugProc("Now Enemy Wolf Time = %f\n", time);
	if ((int)timing[index] > time)
	{
		Time_Stop(timeIndex[index]);
		return true;
	}
	return false;
}


void ResetAnimationTdPosition(int index, LPD3DXANIMATIONCONTROLLER   *Controller)
{
	D3DXTRACK_DESC td;
	Controller[index]->GetTrackDesc(g_EnemyWolf[index].currentTrack, &td);

	td.Position = 0.1f;

	Controller[index]->SetTrackDesc(g_EnemyWolf[index].currentTrack, &td);
}



void ReInitOneAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller)
{
	D3DXTRACK_DESC td0;
	td0.Enable = true;
	td0.Position = 0.1f;
	td0.Speed = 1.0f;
	td0.Weight = 1.0f;
	Controller[index]->SetTrackDesc(g_EnemyWolf[index].currentTrack, &td0);
}


int CreateEnemyWolf(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool AlertStatus)
{
	int nIdxEnemyWolf = -1;

	for (int num = 0; num < ENEMY_WOLF_MAX_NUM; num++)
	{
		if (!g_EnemyWolf[num].use)
		{
			g_EnemyWolf[num].pos = pos;
			g_EnemyWolf[num].old_pos = pos;
			g_EnemyWolf[num].rot = rot;
			g_EnemyWolf[num].scl = scl;
			g_EnemyWolf[num].hp = ENEMY_WOLF_MAX_HP;
			bHitEventG[num] = false;
			g_EnemyWolf[num].use = true;
			g_EnemyWolf[num].AlertStatus = AlertStatus;
			nIdxEnemyWolf = num;
			g_EnemyWolf[num].SphereColliderIdx = 
				CreateSphereCollider(g_EnemyWolf[num].pos, 
					g_EnemyWolf[num].rot, 
					D3DXVECTOR3{ 2.0f, 2.0f, 2.0f});
			g_EnemyWolf[num].SphereColliderAckIdx = CreateSphereCollider(g_EnemyWolf[num].pos, g_EnemyWolf[num].rot, g_EnemyWolf[num].scl);
			g_EnemyWolf[num].ShadowIdx = CreateShadow(g_EnemyWolf[num].pos, g_EnemyWolf[num].rot, g_EnemyWolf[num].scl, true);
			ResetEnemyAi(num);
			SetbEnemyAstart(num, false);
			break;
		}
	}
	return nIdxEnemyWolf;
}


void DrawEnemyWolfShadow(int EnemyWolfIndex, D3DXMATRIX mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 更新骨骼动画
	if (g_EnemyWolf[EnemyWolfIndex].animationID == ENEMY_WOLF_RUN)
	{
		UpdateFrameMatrices(g_pFrameRootRun[EnemyWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootRun[EnemyWolfIndex], 1);
	}
	else if (g_EnemyWolf[EnemyWolfIndex].animationID == ENEMY_WOLF_IDLE)
	{
		UpdateFrameMatrices(g_pFrameRoot[EnemyWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRoot[EnemyWolfIndex], 1);
	}
	else if (g_EnemyWolf[EnemyWolfIndex].animationID == ENEMY_WOLF_DIE)
	{
		UpdateFrameMatrices(g_pFrameRootDie[EnemyWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootDie[EnemyWolfIndex], 1);
	}
	else if (g_EnemyWolf[EnemyWolfIndex].animationID == ENEMY_WOLF_ACK)
	{
		UpdateFrameMatrices(g_pFrameRootAck[EnemyWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootAck[EnemyWolfIndex], 1);
	}
	else if (g_EnemyWolf[EnemyWolfIndex].animationID == ENEMY_WOLF_HURT)
	{
		UpdateFrameMatrices(g_pFrameRootHurt[EnemyWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootHurt[EnemyWolfIndex], 1);
	}

}


void InitController(LPD3DXANIMATIONCONTROLLER *Controller, int EnemyIndex, bool bEnable)
{
	Controller[EnemyIndex]->GetAnimationSetByName(StateName[g_EnemyWolf[EnemyIndex].animationID], &g_pAnimationSet[EnemyIndex]);
	Controller[EnemyIndex]->SetTrackAnimationSet(g_EnemyWolf[EnemyIndex].currentTrack, g_pAnimationSet[EnemyIndex]);
	//g_pAnimationSet->Release();
	Controller[EnemyIndex]->UnkeyAllTrackEvents(g_EnemyWolf[EnemyIndex].currentTrack);
	Controller[EnemyIndex]->SetTrackEnable(g_EnemyWolf[EnemyIndex].currentTrack, bEnable);
	Controller[EnemyIndex]->KeyTrackSpeed(g_EnemyWolf[EnemyIndex].currentTrack, 1.0f, 0.01f, 0.25f, D3DXTRANSITION_LINEAR);
	Controller[EnemyIndex]->KeyTrackWeight(g_EnemyWolf[EnemyIndex].currentTrack, 1.0f, 0.01f, 0.25f, D3DXTRANSITION_LINEAR);

	g_EnemyWolf[EnemyIndex].curTime = g_EnemyWolf[EnemyIndex].prvTime = timeGetTime();

}
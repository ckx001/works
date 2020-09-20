//=============================================================================
//
// ボス処理[EnemyBossWolf.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "EnemyBossWolf.h"
#include "camera_Ex.h"
#include "input.h"
#include "AllocateHierarchyClass.h"
#include "cylinderCollider.h"
#include "debugproc.h"
#include "GlobalFunction.h"
#include "GameMath.h"
#include "GameAi.h"
#include "player1.h"
#include "bullet.h"
#include "sphereCollider.h"
#include "UiEnemyHp.h"
#include "Time_c.h"
#include "sound.h"
#include "shadow.h"
#include "GameEvent.h"
#include "SoulEffect.h"
#include "effect.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define MODEL_ENEMY_BOSSWOLF_RANGE_MIN_X (-400.0f)  //移动限制
#define MODEL_ENEMY_BOSSWOLF_RANGE_MAX_X (273.0f)  //移动限制
#define MODEL_ENEMY_BOSSWOLF_RANGE_MIN_Z (5094.0f)//移动限制
#define MODEL_ENEMY_BOSSWOLF_RANGE_MAX_Z (5670.0f)//移动限制
//#define MODEL_ENEMY_BOSSWOLF_RANGE_MAX_X

#define	MODEL_ENEMY_BOSSWOLF		"data/MODEL/EnemyBossWolfIdle.X"	// 撉傒崬傓儌僨儖柤
#define	MODEL_ENEMY_BOSSWOLF_DIE		"data/MODEL/EnemyBossWolfDie.X"	// 撉傒崬傓儌僨儖柤
#define	MODEL_ENEMY_BOSSWOLF_ACK        "data/MODEL/EnemyBossWolfAck.X"
#define	MODEL_ENEMY_BOSSWOLF_JUMPACK        "data/MODEL/EnemyBossWolfJumpAck.X"
#define	MODEL_ENEMY_BOSSWOLF_WALK        "data/MODEL/EnemyBossWolfWalk.X"
#define	MODEL_ENEMY_BOSSWOLF_STAMP        "data/MODEL/EnemyBossJumpStamp.X"


#define	ENEMY_BOSSWOLF_RADIUS		(10.0f)						// 敿宎
//#define	VALUE_MOVE_ENEMY_BOSSWOLF	(0.155f)					// 堏摦懍搙
#define	VALUE_MOVE_ENEMY_BOSSWOLF	(1.0f)					// 堏摦懍搙
#define	RATE_MOVE_ENEMY_BOSSWOLF	(0.025f)					// 堏摦姷惈學悢
#define	VALUE_ROTATE_ENEMY_BOSSWOLF	(D3DX_PI * 0.025f)			// 夞揮懍搙
#define	RATE_ROTATE_ENEMY_BOSSWOLF	(0.10f)						// 夞揮姷惈學悢
#define	VALUE_MOVE_BULLET	(1.5f)						// 抏偺堏摦懍搙

#define  ENEMY_BOSSWOLF_WAIT_TIME (1000) 
#define  ENEMY_BOSSWOLF_ACK_CHG_TIME (500) 

#define ENEMY_BOSSWOLF_VALUE_MOVE (5.0f)
#define ENEMY_BOSSWOLF_JUMPACK_COLLIDER_POS_Y_MOVE (3.0f) //触发器上下移动量
#define ENEMY_BOSSWOLF_JUMPACK_COLLIDER_POS_Y_BOTTM_MOVE (3.6f) //触发器下移动量
//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
static void ChangeAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller, int animationID);
static void SetAnimationTime(int index, int name, float *animationTime);
static void ResetAnimationTdPosition(int index, LPD3DXANIMATIONCONTROLLER   *Controller);
static bool EnemyBossWolfWaitWithSecend(int index);
static void ReInitOneAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller);
static void InitController(LPD3DXANIMATIONCONTROLLER *Controller, int EnemyIndex, bool bEnable);
static void AiMoveToPlayer(D3DXVECTOR3 player_pos, D3DXVECTOR3 *enemy_pos, D3DXVECTOR3 *enemy_rot, float speed);
static bool EnemyBossWolfAckChgWait(int index);

static bool bHitEvent = false;
static bool bHitEventG[ENEMY_BOSSWOLF_MAX_NUM]; //辅助重力
static int TmpStatus = ENEMY_BOSSWOLF_IDLE; //临时状态记录

//ack///////
static float StartAckRot = 0.0f;
static float EndAckRot = 0.0f;
static float AckCurrentTime = 0.0f;
//////////
//jump ack///
static float JumpAckPosYMove = 0.0f; //y碰撞器辅助移动累计
static bool bJumpAckPosYMoveUp = true; //y碰撞器辅助上升下降开关
//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureEnemyBossWolf;		// 僥僋僗僠儍撉傒崬傒応強
LPD3DXMESH			g_pMeshEnemyBossWolf;				// ID3DXMesh僀儞僞乕僼僃僀僗傊偺億僀儞僞
LPD3DXBUFFER		g_pD3DXMatBuffEnemyBossWolf;		// 儊僢僔儏偺儅僥儕傾儖忣曬傪奿擺
DWORD				g_nNumMatEnemyBossWolf;			// 懏惈忣曬偺憤悢

D3DXMATRIX			g_mtxWorldEnemyBossWolf;			// 儚乕儖僪儅僩儕僢僋僗
static ENEMY_BOSSWOLF				g_EnemyBossWolf[ENEMY_BOSSWOLF_MAX_NUM];					// 僾儗僀儎乕儚乕僋
static ENEMY_BOSSWOLF               g_EnemyBossWolf_bak[ENEMY_BOSSWOLF_MAX_NUM];
//static ID3DXAnimationController*   g_pAnimController = NULL;
static LPD3DXFRAME                 g_pFrameRoot[ENEMY_BOSSWOLF_MAX_NUM];
static D3DXMATRIX*                 g_pBoneMatrices = NULL;
static CAllocateHierarchy*         g_pAllocateHier[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimController[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSet[ENEMY_BOSSWOLF_MAX_NUM];
//////////////////////
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerTotal[ENEMY_BOSSWOLF_MAX_NUM];
//////animation/////////
////////DIE/////////
static LPD3DXFRAME                 g_pFrameRootDie[ENEMY_BOSSWOLF_MAX_NUM];
static D3DXMATRIX*                 g_pBoneMatricesDie = NULL;
static CAllocateHierarchy*         g_pAllocateHierDie[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerDie[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetDie[ENEMY_BOSSWOLF_MAX_NUM];
////////////////////////
//////ACK///////////////
static LPD3DXFRAME                 g_pFrameRootAck[ENEMY_BOSSWOLF_MAX_NUM];
static D3DXMATRIX*                 g_pBoneMatricesAck = NULL;
static CAllocateHierarchy*         g_pAllocateHierAck[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerAck[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetAck[ENEMY_BOSSWOLF_MAX_NUM];
/////////////////////////
//////JUMP ACK///////////////
static LPD3DXFRAME                 g_pFrameRootJumpAck[ENEMY_BOSSWOLF_MAX_NUM];
static D3DXMATRIX*                 g_pBoneMatricesJumpAck = NULL;
static CAllocateHierarchy*         g_pAllocateHierJumpAck[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerJumpAck[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetJumpAck[ENEMY_BOSSWOLF_MAX_NUM];
/////////////////////////
//////JUMP WALK///////////////
static LPD3DXFRAME                 g_pFrameRootWalk[ENEMY_BOSSWOLF_MAX_NUM];
static D3DXMATRIX*                 g_pBoneMatricesWalk = NULL;
static CAllocateHierarchy*         g_pAllocateHierWalk[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerWalk[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetWalk[ENEMY_BOSSWOLF_MAX_NUM];
/////////////////////////
//////STAMP///////////////
static LPD3DXFRAME                 g_pFrameRootStamp[ENEMY_BOSSWOLF_MAX_NUM];
static D3DXMATRIX*                 g_pBoneMatricesStamp = NULL;
static CAllocateHierarchy*         g_pAllocateHierStamp[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerStamp[ENEMY_BOSSWOLF_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetStamp[ENEMY_BOSSWOLF_MAX_NUM];
/////////////////////////

static char *StateName[ENEMY_BOSSWOLF_ANIMATION_MAX] =
{
	"IDLE",
	"JUMPACK",
	"ACK",
	"DIE",
	"WALK",
	"STAMP"
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
static D3DXHANDLE EnemyBossWolfAlphaHandle;
static D3DXHANDLE EnemyBossWolfColorHandle;
static D3DXHANDLE WorldViewMatrixHandle;
static bool LoadPixHLSLEnemyBossWolf();
////////////////////////////////////////////////////////////////////////
//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitEnemyBossWolf(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//DWORD *timing = RetTiming(TIMING_ENEMY_WAIT);
	int *timeIndexAckChg = RetTimeIndex(TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT);

	g_pD3DTextureEnemyBossWolf = NULL;
	g_pMeshEnemyBossWolf = NULL;
	g_pD3DXMatBuffEnemyBossWolf = NULL;

	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		g_EnemyBossWolf[num].last_frame_pos = g_EnemyBossWolf[num].pos;
		g_EnemyBossWolf[num].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyBossWolf[num].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyBossWolf[num].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_EnemyBossWolf[num].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_EnemyBossWolf[num].use = false;
		g_EnemyBossWolf[num].AckJudge = true;
		g_EnemyBossWolf[num].AckPlayerRotJudge = true;
		g_EnemyBossWolf[num].AckPlayerJudge = true;
		g_EnemyBossWolf[num].AckPlayerAckTimeJudge = false;
		g_EnemyBossWolf[num].DieJudge = true;
		g_EnemyBossWolf[num].LastAnimationID = ENEMY_BOSSWOLF_IDLE;
		g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_IDLE;
		g_EnemyBossWolf[num].LastStatus = ENEMY_BOSSWOLF_IDLE;
		g_EnemyBossWolf[num].hp = ENEMY_BOSSWOLF_MAX_HP;
		g_EnemyBossWolf[num].down_dir = false;
		g_EnemyBossWolf[num].active = false;
		g_EnemyBossWolf[num].AckStartChgJudge = false;
		g_EnemyBossWolf[num].AckStartChgStartJudge = false;
		g_EnemyBossWolf[num].AckChgJudge = false;
		g_EnemyBossWolf[num].AckChgStartJudge = false;
		g_EnemyBossWolf[num].AckEffective = false;
		g_EnemyBossWolf[num].JumpAckEffective = false;
		g_EnemyBossWolf[num].fRadius = ENEMY_BOSSWOLF_RADIUS;
		//g_EnemyBossWolf[num].ShadowIdx = CreateShadow(g_EnemyBossWolf[num].pos, g_EnemyBossWolf[num].rot, g_EnemyBossWolf[num].scl, true);
		Time_Stop(timeIndexAckChg[num]);
		bHitEventG[num] = false;
		for (int a = 0; a < ENEMY_BOSSWOLF_ANIMATION_MAX; a++)
		{
			//初始化所有动作都判定为结束
			g_EnemyBossWolf[num].ActionOverJudge[a] = true;
			//初始化所有动作都判定为开始
			g_EnemyBossWolf[num].ActionStartJudge[a] = true;
		}
	}

	// X僼傽僀儖偺撉傒崬傒
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		g_pAllocateHier[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 

		g_pAllocateHierDie[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 

		g_pAllocateHierAck[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 

		g_pAllocateHierJumpAck[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔

		g_pAllocateHierWalk[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔

		g_pAllocateHierStamp[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔
		
        // X僼傽僀儖傪撉傒崬傓
		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_BOSSWOLF,
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

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_BOSSWOLF_DIE,
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
		
		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_BOSSWOLF_ACK,
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

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_BOSSWOLF_JUMPACK,
			D3DXMESH_MANAGED,
			pDevice,
			g_pAllocateHierJumpAck[num],
			NULL,
			&g_pFrameRootJumpAck[num],
			&g_pAnimControllerJumpAck[num]
		)))
		{
			return E_FAIL;
		}

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_BOSSWOLF_WALK,
			D3DXMESH_MANAGED,
			pDevice,
			g_pAllocateHierWalk[num],
			NULL,
			&g_pFrameRootWalk[num],
			&g_pAnimControllerWalk[num]
		)))
		{
			return E_FAIL;
		}

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_BOSSWOLF_STAMP,
			D3DXMESH_MANAGED,
			pDevice,
			g_pAllocateHierStamp[num],
			NULL,
			&g_pFrameRootStamp[num],
			&g_pAnimControllerStamp[num]
		)))
		{
			return E_FAIL;
		}

		SetupBoneMatrixPointers(g_pFrameRoot[num], g_pFrameRoot[num]);

		SetupBoneMatrixPointers(g_pFrameRootJumpAck[num], g_pFrameRootJumpAck[num]);

		//SetupBoneMatrixPointers(g_pFrameRootRun[num], g_pFrameRootRun[num]);

		SetupBoneMatrixPointers(g_pFrameRootDie[num], g_pFrameRootDie[num]);

		SetupBoneMatrixPointers(g_pFrameRootAck[num], g_pFrameRootAck[num]);

		SetupBoneMatrixPointers(g_pFrameRootWalk[num], g_pFrameRootWalk[num]);

		SetupBoneMatrixPointers(g_pFrameRootStamp[num], g_pFrameRootStamp[num]);
	}


	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		InitController(g_pAnimController, num, true);
		InitController(g_pAnimControllerDie, num, false);
		InitController(g_pAnimControllerAck, num, false);
		InitController(g_pAnimControllerWalk, num, false);
		InitController(g_pAnimControllerJumpAck, num, false);
		InitController(g_pAnimControllerStamp, num, false);
		g_pAnimControllerTotal[num] = g_pAnimController[num];
		InitController(g_pAnimControllerTotal, num, true);
	}

	LoadPixHLSLEnemyBossWolf();

	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitEnemyBossWolf(void)
{
	if (g_pD3DTextureEnemyBossWolf != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_pD3DTextureEnemyBossWolf->Release();
		g_pD3DTextureEnemyBossWolf = NULL;
	}

	if (g_pMeshEnemyBossWolf != NULL)
	{// 儊僢僔儏偺奐曻
		g_pMeshEnemyBossWolf->Release();
		g_pMeshEnemyBossWolf = NULL;
	}

	if (g_pD3DXMatBuffEnemyBossWolf != NULL)
	{// 儅僥儕傾儖偺奐曻
		g_pD3DXMatBuffEnemyBossWolf->Release();
		g_pD3DXMatBuffEnemyBossWolf = NULL;
	}
	//g_pEffect->Release();
}


static float Ackpos;
static bool bJumpAckStartLastFrameJudge = true;

void UpdateEnemyBossWolf(void)
{
	PLAYER1 *Player = GetPlayer1();
	int *timeIndex = RetTimeIndex(TIMING_ENEMY_BOSSWOLF_WAIT);
	DWORD *timingAckChg = RetTiming(TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT);
	int *timeIndexAckChg = RetTimeIndex(TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT);

	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		if (g_EnemyBossWolf[num].use)
		{
	         //PrintDebugProc("enemy 0 = pos.x=%f,y=%f\n", g_EnemyBossWolf[num].pos.x, g_EnemyBossWolf[num].pos.y);
	         //////重力///////////////
		    if (g_EnemyBossWolf[num].down_dir)
		    {
		    	if (bHitEventG[num] == false)
		    	{
		    		bHitEventG[num] = true;
		    	}
		    }
		    if (bHitEventG[num] == false)
		    	g_EnemyBossWolf[num].pos.y -= 2.0f;
	         /////////////////////////////
						//////墙体碰撞////////
			//if (g_EnemyBossWolf[num].face_dir == dir.x && g_EnemyBossWolf[num].ColliderHit == true)
			//{
			//	g_EnemyBossWolf[num].pos = g_EnemyBossWolf[num].last_frame_pos;
			//	g_EnemyBossWolf[num].move = D3DXVECTOR3{ 0.0f, 0.0f , 0.0f };
			//	bHitEvent = true; //触发了碰撞事件
			//}
			//if (g_EnemyBossWolf[num].face_dir != dir.x)
			//{
			//	bHitEvent = false;
			//}
			//if (bHitEvent)
			//	g_EnemyBossWolf[num].move = D3DXVECTOR3{ 0.0f, 0.0f , 0.0f };
			//
			//
			//if (g_EnemyBossWolf[num].ColliderHit == false)
			//{
			//	if (g_EnemyBossWolf[num].last_frame_pos != g_EnemyBossWolf[num].pos)
			//	{
			//		g_EnemyBossWolf[num].last_frame_pos = g_EnemyBossWolf[num].pos;
			//	}
			//}
			/////////////////////
			if (g_EnemyBossWolf[num].active)
			{
				if (g_EnemyBossWolf[num].hp <= 0)
				{
					D3DXTRACK_DESC td1;
					ChangeAnimation(num, g_pAnimControllerDie, ENEMY_BOSSWOLF_DIE);
					g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyBossWolf[num].currentTrack, &td1);
					StopSound(SOUND_LABEL_BGM_BOSS);
					if (g_EnemyBossWolf[num].DieJudge)
					{
						ReInitOneAnimation(num, g_pAnimControllerTotal);
						DeleteCylinderCollider(g_EnemyBossWolf[num].CylinderIdx);
						SetSoul(num, ENEMY_TYPE_BOSSWOLF, 80);
						g_EnemyBossWolf[num].DieJudge = false;
					}
					if (td1.Position >= 1.5f)
					{
						SetDisPlayEnemyHp(FALSE);
						D3DXTRACK_DESC td2;
						td2.Enable = false;
						g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyBossWolf[num].currentTrack, &td2);
						//g_EnemyBossWolf[num].use = false;
						Player1WinAnimation();
					}
					//PrintDebugProc("enemy:%f\n", td1.Position);
				}
				else
				{
					//限制移动区域
					//if (g_EnemyBossWolf[num].pos.z >= 5120.0f 
					//	&& g_EnemyBossWolf[num].pos.x < MODEL_ENEMY_BOSSWOLF_RANGE_MAX_X
					//	&& g_EnemyBossWolf[num].pos.x > MODEL_ENEMY_BOSSWOLF_RANGE_MIN_X)
					{
						D3DXVECTOR3 vDistance = Player->pos - g_EnemyBossWolf[num].pos;

						float length = D3DXVec3Length(&vDistance);
						//PrintDebugProc("enemy status = %d\n", g_EnemyBossWolf[num].status);
						//当距离远时使用jump ack
						//如果距离远并且不为当前动作 或者 如果在进行此动作中
						if (length > 200
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_JUMPACK]
							&& g_EnemyBossWolf[num].status != ENEMY_BOSSWOLF_STATUS_ACK
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_JUMPACK]
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_STAMP])
						{
							if (g_EnemyBossWolf[num].pos.x < MODEL_ENEMY_BOSSWOLF_RANGE_MAX_X
								&& g_EnemyBossWolf[num].pos.x > MODEL_ENEMY_BOSSWOLF_RANGE_MIN_X
								&& g_EnemyBossWolf[num].pos.z < MODEL_ENEMY_BOSSWOLF_RANGE_MAX_Z
								&& g_EnemyBossWolf[num].pos.z > MODEL_ENEMY_BOSSWOLF_RANGE_MIN_Z)
							{
								g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_JUMPACK;
								JumpAckPosYMove = 0.0f;
								bJumpAckPosYMoveUp = true;
							}
							else
								g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_WALK;

						}
						else if (
							length > 100
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_ACK]
							&& g_EnemyBossWolf[num].status != ENEMY_BOSSWOLF_STATUS_ACK
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_JUMPACK]
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_STAMP]
							)
						{
							g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_WALK;
						}
						else if (length > 55
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_JUMPACK]
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_STAMP]
							)
						{
							g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_ACK;
						}
						else if (length <= 55
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_ACK]
							&& g_EnemyBossWolf[num].status != ENEMY_BOSSWOLF_STATUS_ACK
							&& g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_JUMPACK])
						{
							g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_STAMP;
						}
						//else
						//	g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_IDLE;

						//PrintDebugProc("Boss Wolf Status = %d\n", g_EnemyBossWolf[num].status);
						if (g_EnemyBossWolf[num].status == ENEMY_BOSSWOLF_STATUS_JUMPACK)
						{
							if (g_EnemyBossWolf[num].ActionStartJudge[ENEMY_BOSSWOLF_JUMPACK])
							{
								ResetAnimationTdPosition(num, g_pAnimControllerTotal);

								float move_rot_y = atan2((Player->pos.z - g_EnemyBossWolf[num].pos.z), (Player->pos.x - g_EnemyBossWolf[num].pos.x));
								g_EnemyBossWolf[num].rot.y = (3 * D3DX_PI) / 2 - move_rot_y;

								ChangeAnimation(num, g_pAnimControllerJumpAck, ENEMY_BOSSWOLF_JUMPACK);
								//g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_JUMPACK;
								g_EnemyBossWolf[num].ActionStartJudge[ENEMY_BOSSWOLF_JUMPACK] = false;
							}

							D3DXTRACK_DESC td1;
							g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyBossWolf[num].currentTrack, &td1);

							//跳在空中
							if (td1.Position > 0.4 && td1.Position < 1.8f)
							{
								///////////////////
								//////一共140帧
								///////////////////
								D3DXVECTOR3 JumpMove = D3DXVECTOR3(-sinf(g_EnemyBossWolf[num].rot.y) * ENEMY_BOSSWOLF_JUMPACK_MOVE, 0.0f, -cosf(g_EnemyBossWolf[num].rot.y) * ENEMY_BOSSWOLF_JUMPACK_MOVE);
								if (bJumpAckStartLastFrameJudge)
								{
									if (g_EnemyBossWolf[num].pos.x < MODEL_ENEMY_BOSSWOLF_RANGE_MAX_X
										&& g_EnemyBossWolf[num].pos.x > MODEL_ENEMY_BOSSWOLF_RANGE_MIN_X
										&& g_EnemyBossWolf[num].pos.z < MODEL_ENEMY_BOSSWOLF_RANGE_MAX_Z
										&& g_EnemyBossWolf[num].pos.z > MODEL_ENEMY_BOSSWOLF_RANGE_MIN_Z)
									{
										g_EnemyBossWolf[num].pos.x += JumpMove.x;
										g_EnemyBossWolf[num].pos.z += JumpMove.z;
									}
									else
									{
										g_EnemyBossWolf[num].pos.x -= JumpMove.x;
										g_EnemyBossWolf[num].pos.z -= JumpMove.z;
										bJumpAckStartLastFrameJudge = false;
									}
								}

								if (bJumpAckPosYMoveUp)
								{
									if (JumpAckPosYMove <= 140.0f)
									{
										JumpAckPosYMove += ENEMY_BOSSWOLF_JUMPACK_COLLIDER_POS_Y_MOVE;
									}
									else
										bJumpAckPosYMoveUp = false;
								}
								else
								{
									JumpAckPosYMove -= ENEMY_BOSSWOLF_JUMPACK_COLLIDER_POS_Y_BOTTM_MOVE;
								}
							}

		
							g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_JUMPACK] = false;
							//地板震地攻击开始
							if (td1.Position > 2.0f)
							    g_EnemyBossWolf[num].JumpAckEffective = true;

							//JumpAck动作结束
							if (td1.Position > 2.8f)
							{
								g_EnemyBossWolf[num].ActionStartJudge[ENEMY_BOSSWOLF_JUMPACK] = true;
								g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_JUMPACK] = true;
								bJumpAckStartLastFrameJudge = true;
								g_EnemyBossWolf[num].JumpAckEffective = false;
								g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_IDLE;
							}		
						}
						//当距离过远切换Run状态
						else if (g_EnemyBossWolf[num].status == ENEMY_BOSSWOLF_STATUS_WALK)
						{
							//SetbEnemyAstart(num, false);
							if (g_EnemyBossWolf[num].status == ENEMY_BOSSWOLF_STATUS_ACK)
								ResetAnimationTdPosition(num, g_pAnimControllerTotal);
							AiMoveToPlayer(Player->pos,
								&g_EnemyBossWolf[num].pos,
								&g_EnemyBossWolf[num].rot,
								VALUE_MOVE_ENEMY_BOSSWOLF);
							ChangeAnimation(num, g_pAnimControllerWalk, ENEMY_BOSSWOLF_WALK);
							//g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_WALK;
						}
						else if (g_EnemyBossWolf[num].status == ENEMY_BOSSWOLF_STATUS_ACK)
						{
							//任何状态切换的等待
							//if (g_EnemyBossWolf[num].animationID != g_EnemyBossWolf[num].LastAnimationID)
							//{
							//	//ChangeAnimation(num, g_pAnimController, ENEMY_BOSSWOLF_IDLE);
							//	bool WaitJudge = EnemyBossWolfWaitWithSecend(num);
							//	if (WaitJudge == false)
							//	{
							//		Time_Start(timeIndex[num]);
							//		g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_WAIT;
							//	}
							//	else
							//	{
							//		Time_Stop(timeIndex[num]);
							//		g_EnemyBossWolf[num].LastAnimationID = g_EnemyBossWolf[num].animationID;
							//		g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_WAITOVER;
							//	}
							//}
							//else
							//{
								if (g_EnemyBossWolf[num].AckPlayerRotJudge)
								{
									float MoveRot = RetRotValue(D3DXVECTOR2{ Player->pos.x, Player->pos.z }, 
									D3DXVECTOR2{ g_EnemyBossWolf[num].pos.x, g_EnemyBossWolf[num].pos.z });
									//g_EnemyWolf[num].rot.y = D3DX_PI - MoveRot;
									g_EnemyBossWolf[num].rot.y = D3DX_PI / 2 - MoveRot;
									g_EnemyBossWolf[num].AckPlayerRotJudge = false;
									////////////
									Ackpos = g_EnemyBossWolf[num].rot.y - D3DX_PI / 2;
									StartAckRot = g_EnemyBossWolf[num].rot.y - D3DX_PI / 2;
									EndAckRot = g_EnemyBossWolf[num].rot.y - D3DX_PI;
									AckCurrentTime = 0.0f;
									////////////
								}
								else
								{
									D3DXTRACK_DESC td_ackPositon;
									g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyBossWolf[num].currentTrack, &td_ackPositon);

									if ((int)timingAckChg[num] > ENEMY_BOSSWOLF_ACK_CHG_TIME)
										g_EnemyBossWolf[num].AckChgCJudgeCylinder++;
									
									if (
										(int)timingAckChg[num] != 0 
										&& 
										g_EnemyBossWolf[num].AckChgCJudgeCylinder == 1)
										AckCurrentTime = 0.0f;
									//切换到左手
									if (Ackpos == EndAckRot)
									{
										StartAckRot = g_EnemyBossWolf[num].rot.y - D3DX_PI / 2 - D3DX_PI;
										AckCurrentTime = 0.0f;
									}

									//右手运动
									if ((int)timingAckChg[num] == 0
										&& g_EnemyBossWolf[num].AckChgCJudgeCylinder == 1)
									{
										AckCurrentTime += 0.03f;
										g_EnemyBossWolf[num].AckEffective = true;
									}
									//左手运动
									if (Ackpos != EndAckRot
										&& (int)timingAckChg[num] == 0
										&& g_EnemyBossWolf[num].AckChgCJudgeCylinder == 2)
									{
										AckCurrentTime += 0.04f;
										g_EnemyBossWolf[num].AckEffective = true;
									}

									//动作结束
									if (td_ackPositon.Position > 1.2f)
									{
										Ackpos = g_EnemyBossWolf[num].rot.y - D3DX_PI / 2;
										g_EnemyBossWolf[num].AckEffective = false;
									}
									else
										Ackpos = F_Lerp(StartAckRot, EndAckRot, AckCurrentTime);
								}

								D3DXTRACK_DESC td1;
								g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyBossWolf[num].currentTrack, &td1);

								g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_ACK;
								ChangeAnimation(num, g_pAnimControllerAck, ENEMY_BOSSWOLF_ACK);

								g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_ACK] = false;
								//PlaySound(SOUND_LABEL_SE_ENEMY_WOLF_ACK);

								//右手等待
								if (td1.Position >= 0.3f
									&& g_EnemyBossWolf[num].AckStartChgJudge == false)
								{
									bool AckChg = EnemyBossWolfAckChgWait(num);
									if (AckChg == false)
									{
										Time_Start(timeIndexAckChg[num]);
								
										if (g_EnemyBossWolf[num].AckStartChgStartJudge == false)
										{
											D3DXTRACK_DESC TdStartAckChgStop;
											TdStartAckChgStop.Position = 0.3f;
											TdStartAckChgStop.Speed = 0.1f;
											TdStartAckChgStop.Weight = 1.0f;
											TdStartAckChgStop.Enable = true;
											g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyBossWolf[num].currentTrack, &TdStartAckChgStop);
											g_EnemyBossWolf[num].AckStartChgStartJudge = true;
										}
									}
									else
									{
										D3DXTRACK_DESC TdStartAckChgStart;
										TdStartAckChgStart.Position = 0.3f;
										TdStartAckChgStart.Weight = 1.0f;
										TdStartAckChgStart.Speed = 1.0f;
										TdStartAckChgStart.Enable = true;
										g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyBossWolf[num].currentTrack, &TdStartAckChgStart);
										g_EnemyBossWolf[num].AckStartChgJudge = true;
									}
								
								}
								///////////////
								//右手左手切换等待
								if (td1.Position >= 0.82f
									&& g_EnemyBossWolf[num].AckChgJudge == false)
								{
									bool AckChg = EnemyBossWolfAckChgWait(num);
									if (AckChg == false)
									{
										Time_Start(timeIndexAckChg[num]);

										if (g_EnemyBossWolf[num].AckChgStartJudge == false)
										{
											D3DXTRACK_DESC TdAckChgStop;
											TdAckChgStop.Position = 0.82f;
											TdAckChgStop.Speed = 0.1f;
											TdAckChgStop.Weight = 1.0f;
											TdAckChgStop.Enable = true;
											g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyBossWolf[num].currentTrack, &TdAckChgStop);
											g_EnemyBossWolf[num].AckChgStartJudge = true;
										}
									}
									else
									{
										D3DXTRACK_DESC TdAckChgStart;
										TdAckChgStart.Position = 0.82f;
										TdAckChgStart.Weight = 1.0f;
										TdAckChgStart.Speed = 1.0f;
										TdAckChgStart.Enable = true;
										g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyBossWolf[num].currentTrack, &TdAckChgStart);
										g_EnemyBossWolf[num].AckChgJudge = true;
									}

								}

								if (td1.Position > 0.9f)
								{
									g_EnemyBossWolf[num].AckPlayerAckTimeJudge = true;
								}

								//攻击动作完成
								if (td1.Position > 1.5f)
								{
									ResetAnimationTdPosition(num, g_pAnimControllerTotal);
									ChangeAnimation(num, g_pAnimController, ENEMY_BOSSWOLF_IDLE);
									Time_Start(timeIndex[num]);
									g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_IDLE;
									g_EnemyBossWolf[num].AckPlayerRotJudge = true;
									g_EnemyBossWolf[num].AckPlayerAckTimeJudge = false;
									g_EnemyBossWolf[num].AckStartChgJudge = false;
									g_EnemyBossWolf[num].AckStartChgStartJudge = false;
									g_EnemyBossWolf[num].AckChgJudge = false;
									g_EnemyBossWolf[num].AckChgStartJudge = false;
									g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_ACK] = true;
									g_EnemyBossWolf[num].AckChgCJudgeCylinder = 0;
									AckCurrentTime = 0.0f;
									Ackpos = g_EnemyBossWolf[num].rot.y - D3DX_PI / 2;
								}
						}
						else if (g_EnemyBossWolf[num].status == ENEMY_BOSSWOLF_STATUS_STAMP)
						{
						    if (g_EnemyBossWolf[num].ActionStartJudge[ENEMY_BOSSWOLF_STAMP])
						    {
						    	ResetAnimationTdPosition(num, g_pAnimControllerTotal);
						    
						    	ChangeAnimation(num, g_pAnimControllerStamp, ENEMY_BOSSWOLF_STAMP);
						    	g_EnemyBossWolf[num].ActionStartJudge[ENEMY_BOSSWOLF_STAMP] = false;
						    }
						    
							g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_STAMP] = false;

						    D3DXTRACK_DESC td1;
						    g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyBossWolf[num].currentTrack, &td1);
							//地板震地攻击开始
							if (td1.Position > 2.1f)
								g_EnemyBossWolf[num].JumpAckEffective = true;
							
							//攻击动作完成
							if (td1.Position > 3.0f)
							{
								ResetAnimationTdPosition(num, g_pAnimControllerTotal);
								ChangeAnimation(num, g_pAnimController, ENEMY_BOSSWOLF_IDLE);
								g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_STATUS_IDLE;
								g_EnemyBossWolf[num].ActionStartJudge[ENEMY_BOSSWOLF_STAMP] = true;
								g_EnemyBossWolf[num].ActionOverJudge[ENEMY_BOSSWOLF_STAMP] = true;
								g_EnemyBossWolf[num].JumpAckEffective = false;
							}
                        }
					}
					//else
					//{
					//    g_EnemyBossWolf[num].pos = g_EnemyBossWolf[num].start_pos;
                    //}
				}
			}
			if(g_EnemyBossWolf[num].status == ENEMY_BOSSWOLF_STATUS_JUMPACK
				|| g_EnemyBossWolf[num].status == ENEMY_BOSSWOLF_STATUS_STAMP)
				g_pAnimControllerTotal[num]->AdvanceTime(0.016f, NULL);
			else
			    g_pAnimControllerTotal[num]->AdvanceTime(0.01f, NULL);
	

			SetPositionShadow(g_EnemyBossWolf[num].ShadowIdx, 
				g_EnemyBossWolf[num].pos, 
				g_EnemyBossWolf[num].rot,
				D3DXVECTOR3{ g_EnemyBossWolf[num].scl.x,
				g_EnemyBossWolf[num].scl.y / 2,
				g_EnemyBossWolf[num].scl.z });
			
			SetPositionSphereCollider(g_EnemyBossWolf[num].SphereColliderAckIdx,
				D3DXVECTOR3{ g_EnemyBossWolf[num].pos.x + sinf(Ackpos) * 85, g_EnemyBossWolf[num].pos.y + 40.0f, g_EnemyBossWolf[num].pos.z + cosf(Ackpos) * 85 },
				D3DXVECTOR3{ g_EnemyBossWolf[num].rot.x,g_EnemyBossWolf[num].rot.y,g_EnemyBossWolf[num].rot.z },
				D3DXVECTOR3{ 4.5f ,4.5f,4.5f });

			SetPositionCylinderCollider(g_EnemyBossWolf[num].CylinderIdx,
				D3DXVECTOR3{ g_EnemyBossWolf[num].pos.x, g_EnemyBossWolf[num].pos.y + 80.0f, g_EnemyBossWolf[num].pos.z },
				D3DXVECTOR3{ D3DX_PI / 2 ,0,0 }, 
				D3DXVECTOR3{ 5.0f ,5.0f,5.0f });
			SetPositionSphereCollider(g_EnemyBossWolf[num].SphereColliderJumpAckIdx,
				D3DXVECTOR3{ g_EnemyBossWolf[num].pos.x, g_EnemyBossWolf[num].pos.y + 80.0f + JumpAckPosYMove, g_EnemyBossWolf[num].pos.z },
				D3DXVECTOR3{ g_EnemyBossWolf[num].rot.x,g_EnemyBossWolf[num].rot.y,g_EnemyBossWolf[num].rot.z },
				D3DXVECTOR3{ 6.0f ,6.0f,6.0f });

			//PrintDebugProc("EnemyBoss Pos = %f, %f, %f\n", g_EnemyBossWolf[num].pos.x, g_EnemyBossWolf[num].pos.y, g_EnemyBossWolf[num].pos.z);
			//PrintDebugProc("JumpAck Co = %f\n", g_EnemyBossWolf[num].pos.y + 80.0f + JumpAckPosYMove);
			
			if (g_EnemyBossWolf[num].pos != g_EnemyBossWolf[num].last_frame_pos)
			{
				g_EnemyBossWolf[num].down_dir = false;
				bHitEventG[num] = false;
			}
			/////////记录上一个状态(跳过Wait状态)/////////////
			if (g_EnemyBossWolf[num].status != ENEMY_BOSSWOLF_STATUS_WAIT
				&& g_EnemyBossWolf[num].status != ENEMY_BOSSWOLF_STATUS_WAITOVER)
			{
				if (g_EnemyBossWolf[num].status != TmpStatus)
				{
					g_EnemyBossWolf[num].LastStatus = TmpStatus;
					TmpStatus = g_EnemyBossWolf[num].status;
				}
			}
			g_EnemyBossWolf[num].ColliderHit = false;
			//PrintDebugProc("Enemy Wolf Hp = %d\n", g_EnemyBossWolf[num].hp);
			//PrintDebugProc("Enemy Wolf Rot = %f\n", g_EnemyBossWolf[num].rot.y);
			//PrintDebugProc("aaaa now , Last = %d, %d\n", g_EnemyBossWolf[num].status, g_EnemyBossWolf[num].LastStatus);
			////////////////////////////////////
			g_EnemyBossWolf[num].last_frame_pos = g_EnemyBossWolf[num].pos;
		}
	}
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawEnemyBossWolf(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScl;
	D3DXMATERIAL *pD3DXMat;
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		if (g_EnemyBossWolf[num].use)
		{
			if (CubeInFrustum(g_EnemyBossWolf[num].pos))
			{
				// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
				D3DXMatrixIdentity(&g_mtxWorldEnemyBossWolf);

				D3DXMatrixScaling(&mtxScl, g_EnemyBossWolf[num].scl.x, g_EnemyBossWolf[num].scl.y, g_EnemyBossWolf[num].scl.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyBossWolf, &g_mtxWorldEnemyBossWolf, &mtxScl);

				// 夞揮傪斀塮
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EnemyBossWolf[num].rot.y, g_EnemyBossWolf[num].rot.x, g_EnemyBossWolf[num].rot.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyBossWolf, &g_mtxWorldEnemyBossWolf, &mtxRot);

				// 堏摦傪斀塮
				D3DXMatrixTranslation(&mtxTranslate, g_EnemyBossWolf[num].pos.x, g_EnemyBossWolf[num].pos.y, g_EnemyBossWolf[num].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyBossWolf, &g_mtxWorldEnemyBossWolf, &mtxTranslate);
				//if(g_pD3DTextureEnemyBossWolf != NULL)
				//    pDevice->SetTexture(0, g_pD3DTextureEnemyBossWolf);
				//// 儚乕儖僪儅僩儕僢僋僗偺愝掕

				//TransformConstantTable->SetMatrix(pDevice, WorldViewMatrixHandle, &g_mtxWorldEnemyBossWolf);
				////TransformConstantTable->SetFloat(pDevice, g_Color, 0);
				//TransformConstantTable->SetVector(pDevice, EnemyBossWolfColorHandle, &g_Color[0]);
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemyBossWolf);

				//pDevice->SetRenderState(D3DRS_ZENABLE, true);
				//DrawShadow(g_EnemyBossWolf[num].ShadowIdx, OBJ_EN_ENEMY_TYPE, num);
				//pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
				//pDevice->SetRenderState(D3DRS_ZENABLE, false);

				pDevice->SetRenderState(D3DRS_ZENABLE, true);

				if (g_EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_IDLE)
				{
					UpdateFrameMatrices(g_pFrameRoot[num], &g_mtxWorldEnemyBossWolf);
					DrawFrame(pDevice, g_pFrameRoot[num], 0);
				}
				else if (g_EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_JUMPACK)
				{
					UpdateFrameMatrices(g_pFrameRootJumpAck[num], &g_mtxWorldEnemyBossWolf);
					DrawFrame(pDevice, g_pFrameRootJumpAck[num], 0);
				}
				else if (g_EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_DIE)
				{
					UpdateFrameMatrices(g_pFrameRootDie[num], &g_mtxWorldEnemyBossWolf);
					DrawFrame(pDevice, g_pFrameRootDie[num], 0);
				}
				else if (g_EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_ACK)
				{
					UpdateFrameMatrices(g_pFrameRootAck[num], &g_mtxWorldEnemyBossWolf);
					DrawFrame(pDevice, g_pFrameRootAck[num], 0);
				}
				else if (g_EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_WALK)
				{
					UpdateFrameMatrices(g_pFrameRootWalk[num], &g_mtxWorldEnemyBossWolf);
					DrawFrame(pDevice, g_pFrameRootWalk[num], 0);
				}
				else if (g_EnemyBossWolf[num].animationID == ENEMY_BOSSWOLF_STAMP)
				{
					UpdateFrameMatrices(g_pFrameRootStamp[num], &g_mtxWorldEnemyBossWolf);
					DrawFrame(pDevice, g_pFrameRootStamp[num], 0);
				}

				pDevice->SetRenderState(D3DRS_ZENABLE, false);
			}
		}
	}
}



ENEMY_BOSSWOLF *GetEnemyBossWolf(void)
{
	return &g_EnemyBossWolf[0];
}


void  SetAnimationTime(int index, int name, float *animationTime)
{
	float time;
	switch (name)
	{
	case ENEMY_BOSSWOLF_IDLE:
		time = 0.0f;
		break;
	case ENEMY_BOSSWOLF_JUMPACK:
		time = 0.0f;
		break;
	case ENEMY_BOSSWOLF_ACK:
		time = 0.0f;
		break;
	case ENEMY_BOSSWOLF_DIE:
		time = 0.0f;
		break;
	case ENEMY_BOSSWOLF_WALK:
		time = 0.0f;
		break;
	case ENEMY_BOSSWOLF_STAMP:
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

	if (g_EnemyBossWolf[index].animationID != animationID)
	{

		ReInitOneAnimation(index, Controller);

		g_EnemyBossWolf[index].animationTime[g_EnemyBossWolf[index].animationID] = 0;

		SetAnimationTime(index, animationID, &g_EnemyBossWolf[index].animationTime[animationID]);

		DWORD dwNewTrack = (g_EnemyBossWolf[index].currentTrack == 0 ? 1 : 0);

		LPD3DXANIMATIONSET g_pAnimationSet = NULL;

		Controller[index]->GetAnimationSetByName(StateName[animationID], &g_pAnimationSet);
		Controller[index]->SetTrackAnimationSet(dwNewTrack, g_pAnimationSet);
		//g_pAnimationSet->Release();

		Controller[index]->UnkeyAllTrackEvents(g_EnemyBossWolf[index].currentTrack);
		Controller[index]->UnkeyAllTrackEvents(dwNewTrack);

		D3DXTRACK_DESC td;
		Controller[index]->GetTrackDesc(g_EnemyBossWolf[index].currentTrack, &td);

		td.Position = 0.1f;

		Controller[index]->SetTrackDesc(dwNewTrack, &td);
		g_EnemyBossWolf[index].animationID = animationID;

		Controller[index]->KeyTrackEnable(g_EnemyBossWolf[index].currentTrack, FALSE, g_EnemyBossWolf[index].curTime + 0.25f);
		Controller[index]->KeyTrackSpeed(g_EnemyBossWolf[index].currentTrack, 0.0f, g_EnemyBossWolf[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);
		Controller[index]->KeyTrackWeight(g_EnemyBossWolf[index].currentTrack, 0.0f, g_EnemyBossWolf[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);


		Controller[index]->SetTrackEnable(dwNewTrack, TRUE);
		Controller[index]->KeyTrackSpeed(dwNewTrack, 1.0f, g_EnemyBossWolf[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);
		Controller[index]->KeyTrackWeight(dwNewTrack, 1.0f, g_EnemyBossWolf[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);


		g_EnemyBossWolf[index].currentTrack = dwNewTrack;

		Controller[index]->SetTrackAnimationSet(g_EnemyBossWolf[index].currentTrack, g_pAnimationSet);

		g_EnemyBossWolf[index].curTime = 0.0f;

		g_pAnimControllerTotal[index] = Controller[index];
	}
}


/////////////////////////////////像素着色器////////////////////////////
bool LoadPixHLSLEnemyBossWolf()
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


void BackUpEnemyBossWolf(void)
{
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		g_EnemyBossWolf_bak[num] = g_EnemyBossWolf[num];
	}
}

HRESULT RestartEnemyBossWolf(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		g_EnemyBossWolf[num] = g_EnemyBossWolf_bak[num];
	}
	return S_OK;
}


void ResetEnemyBossWolfAckJudge()
{
	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		g_EnemyBossWolf[num].AckJudge = true;
	}
}


bool EnemyBossWolfWaitWithSecend(int index)
{
	DWORD *timing = RetTiming(TIMING_ENEMY_BOSSWOLF_WAIT);
	int *timeIndex = RetTimeIndex(TIMING_ENEMY_BOSSWOLF_WAIT);
	
	//PrintDebugProc("Enemy Wait %d = %d\n", index, (int)timing[index]);
	
	if ((int)timing[index] > ENEMY_BOSSWOLF_WAIT_TIME)
	{
		Time_Stop(timeIndex[index]);
		return true;
	}
	return false;
}


bool EnemyBossWolfAckChgWait(int index)
{
	DWORD *timing = RetTiming(TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT);
	int *timeIndex = RetTimeIndex(TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT);

	//PrintDebugProc("Enemy Boss Ack Chg %d = %d\n", index, (int)timing[index]);

	if ((int)timing[index] > ENEMY_BOSSWOLF_ACK_CHG_TIME)
	{
		Time_Stop(timeIndex[index]);
		return true;
	}
	return false;
}


void ResetAnimationTdPosition(int index, LPD3DXANIMATIONCONTROLLER   *Controller)
{
	D3DXTRACK_DESC td;
	Controller[index]->GetTrackDesc(g_EnemyBossWolf[index].currentTrack, &td);

	td.Position = 0.1f;

	Controller[index]->SetTrackDesc(g_EnemyBossWolf[index].currentTrack, &td);
}



void ReInitOneAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller)
{
	D3DXTRACK_DESC td0;
	td0.Enable = true;
	td0.Position = 0.1f;
	td0.Speed = 1.0f;
	td0.Weight = 1.0f;
	Controller[index]->SetTrackDesc(g_EnemyBossWolf[index].currentTrack, &td0);
}


int CreateEnemyBossWolf(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	int nIdxEnemyBossWolf = -1;

	for (int num = 0; num < ENEMY_BOSSWOLF_MAX_NUM; num++)
	{
		if (!g_EnemyBossWolf[num].use)
		{
			g_EnemyBossWolf[num].pos = pos;
			g_EnemyBossWolf[num].start_pos = pos;
			g_EnemyBossWolf[num].rot = rot;
			g_EnemyBossWolf[num].scl = scl;
			g_EnemyBossWolf[num].hp = ENEMY_BOSSWOLF_MAX_HP;
			bHitEventG[num] = false;
			g_EnemyBossWolf[num].use = true;
			nIdxEnemyBossWolf = num;
			Ackpos = g_EnemyBossWolf[num].rot.y - D3DX_PI / 2;
			g_EnemyBossWolf[num].SphereColliderJumpAckIdx = CreateSphereCollider(g_EnemyBossWolf[num].pos, g_EnemyBossWolf[num].rot, g_EnemyBossWolf[num].scl * 5);
			g_EnemyBossWolf[num].SphereColliderAckIdx = CreateSphereCollider(g_EnemyBossWolf[num].pos, g_EnemyBossWolf[num].rot, g_EnemyBossWolf[num].scl * 5);
			g_EnemyBossWolf[num].CylinderIdx = CreateCylinderCollider(g_EnemyBossWolf[num].pos, g_EnemyBossWolf[num].rot, g_EnemyBossWolf[num].scl);
			g_EnemyBossWolf[num].ShadowIdx = CreateShadow(g_EnemyBossWolf[num].pos, g_EnemyBossWolf[num].rot, g_EnemyBossWolf[num].scl * 3, true);
			break;
		}
	}
	return nIdxEnemyBossWolf;
}


void DrawEnemyBossWolfShadow(int EnemyBossWolfIndex, D3DXMATRIX mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 更新骨骼动画
	if (g_EnemyBossWolf[EnemyBossWolfIndex].animationID == ENEMY_BOSSWOLF_IDLE)
	{
		UpdateFrameMatrices(g_pFrameRoot[EnemyBossWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRoot[EnemyBossWolfIndex], 1);
	}
	else if (g_EnemyBossWolf[EnemyBossWolfIndex].animationID == ENEMY_BOSSWOLF_JUMPACK)
	{
		UpdateFrameMatrices(g_pFrameRootJumpAck[EnemyBossWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootJumpAck[EnemyBossWolfIndex], 1);
	}
	else if (g_EnemyBossWolf[EnemyBossWolfIndex].animationID == ENEMY_BOSSWOLF_DIE)
	{
		UpdateFrameMatrices(g_pFrameRootDie[EnemyBossWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootDie[EnemyBossWolfIndex], 1);
	}
	else if (g_EnemyBossWolf[EnemyBossWolfIndex].animationID == ENEMY_BOSSWOLF_ACK)
	{
		UpdateFrameMatrices(g_pFrameRootAck[EnemyBossWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootAck[EnemyBossWolfIndex], 1);
	}
	else if (g_EnemyBossWolf[EnemyBossWolfIndex].animationID == ENEMY_BOSSWOLF_WALK)
	{
		UpdateFrameMatrices(g_pFrameRootWalk[EnemyBossWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootWalk[EnemyBossWolfIndex], 1);
	}
	else if (g_EnemyBossWolf[EnemyBossWolfIndex].animationID == ENEMY_BOSSWOLF_STAMP)
	{
		UpdateFrameMatrices(g_pFrameRootStamp[EnemyBossWolfIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootStamp[EnemyBossWolfIndex], 1);
	}
}


void InitController(LPD3DXANIMATIONCONTROLLER *Controller, int EnemyIndex, bool bEnable)
{
	Controller[EnemyIndex]->GetAnimationSetByName(StateName[g_EnemyBossWolf[EnemyIndex].animationID], &g_pAnimationSet[EnemyIndex]);
	Controller[EnemyIndex]->SetTrackAnimationSet(g_EnemyBossWolf[EnemyIndex].currentTrack, g_pAnimationSet[EnemyIndex]);
	//g_pAnimationSet->Release();
	Controller[EnemyIndex]->UnkeyAllTrackEvents(g_EnemyBossWolf[EnemyIndex].currentTrack);
	Controller[EnemyIndex]->SetTrackEnable(g_EnemyBossWolf[EnemyIndex].currentTrack, bEnable);
	Controller[EnemyIndex]->KeyTrackSpeed(g_EnemyBossWolf[EnemyIndex].currentTrack, 1.0f, 0.01f, 0.25f, D3DXTRANSITION_LINEAR);
	Controller[EnemyIndex]->KeyTrackWeight(g_EnemyBossWolf[EnemyIndex].currentTrack, 1.0f, 0.01f, 0.25f, D3DXTRANSITION_LINEAR);

	g_EnemyBossWolf[EnemyIndex].curTime = g_EnemyBossWolf[EnemyIndex].prvTime = timeGetTime();

}


//朝向player移动的AI
void AiMoveToPlayer(D3DXVECTOR3 player_pos, D3DXVECTOR3 *enemy_pos, D3DXVECTOR3 *enemy_rot, float speed)
{
	float move_rot_y = atan2((player_pos.z - enemy_pos->z), (player_pos.x - enemy_pos->x));
	float move_x = sinf(enemy_rot->y) * speed;
	float move_z = cosf(enemy_rot->y) * speed;

	enemy_rot->y = (3 * D3DX_PI) / 2 - move_rot_y;
	enemy_pos->x -= move_x;
	enemy_pos->z -= move_z;
}


void ResetEnemyBossWolf(int num)
{
	g_EnemyBossWolf[num].last_frame_pos = g_EnemyBossWolf[num].pos;
	g_EnemyBossWolf[num].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_EnemyBossWolf[num].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_EnemyBossWolf[num].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_EnemyBossWolf[num].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_EnemyBossWolf[num].use = false;
	g_EnemyBossWolf[num].AckJudge = true;
	g_EnemyBossWolf[num].AckPlayerRotJudge = true;
	g_EnemyBossWolf[num].AckPlayerJudge = true;
	g_EnemyBossWolf[num].AckPlayerAckTimeJudge = false;
	g_EnemyBossWolf[num].DieJudge = true;
	g_EnemyBossWolf[num].LastAnimationID = ENEMY_BOSSWOLF_IDLE;
	g_EnemyBossWolf[num].status = ENEMY_BOSSWOLF_IDLE;
	g_EnemyBossWolf[num].LastStatus = ENEMY_BOSSWOLF_IDLE;
	g_EnemyBossWolf[num].hp = ENEMY_BOSSWOLF_MAX_HP;
	g_EnemyBossWolf[num].down_dir = false;
	g_EnemyBossWolf[num].active = false;
	g_EnemyBossWolf[num].AckStartChgJudge = false;
	g_EnemyBossWolf[num].AckStartChgStartJudge = false;
	g_EnemyBossWolf[num].AckChgJudge = false;
	g_EnemyBossWolf[num].AckChgStartJudge = false;
	g_EnemyBossWolf[num].AckEffective = false;
	g_EnemyBossWolf[num].JumpAckEffective = false;
	g_EnemyBossWolf[num].fRadius = ENEMY_BOSSWOLF_RADIUS;
	ChangeAnimation(num, g_pAnimController, ENEMY_BOSSWOLF_IDLE);
	for (int a = 0; a < ENEMY_BOSSWOLF_ANIMATION_MAX; a++)
	{
		//初始化所有动作都判定为结束
		g_EnemyBossWolf[num].ActionOverJudge[a] = true;
		//初始化所有动作都判定为开始
		g_EnemyBossWolf[num].ActionStartJudge[a] = true;
	}
}
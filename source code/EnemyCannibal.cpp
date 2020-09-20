//=============================================================================
//
// エネミーCannibal処理[EnemyCannibal.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "EnemyCannibal.h"
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
#include "shadow.h"
#include "GameEvent.h"
#include "SoulEffect.h"
#include "effect.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	MODEL_ENEMY_CANNIBAL		"data/MODEL/EnemyCannibal_Idle.X"	// 撉傒崬傓儌僨儖柤
#define	MODEL_ENEMY_CANNIBAL_DIE		"data/MODEL/EnemyCannibal_Die.X"	// 撉傒崬傓儌僨儖柤
#define	MODEL_ENEMY_CANNIBAL_ACK        "data/MODEL/EnemyCannibal_Ack.X"

#define	TEXTURE_ENEMY_CANNIBAL_FILENAME	"data/MODEL/ninja.png"

#define	ENEMY_CANNIBAL_RADIUS		(10.0f)						// 敿宎
//#define	VALUE_MOVE_ENEMY_CANNIBAL	(0.155f)					// 堏摦懍搙
#define	VALUE_MOVE_ENEMY_CANNIBAL	(5.0f)					// 堏摦懍搙
#define	RATE_MOVE_ENEMY_CANNIBAL	(0.025f)					// 堏摦姷惈學悢
#define	VALUE_ROTATE_ENEMY_CANNIBAL	(D3DX_PI * 0.025f)			// 夞揮懍搙
#define	RATE_ROTATE_ENEMY_CANNIBAL	(0.10f)						// 夞揮姷惈學悢
#define	VALUE_MOVE_BULLET	(1.5f)						// 抏偺堏摦懍搙

#define  ENEMY_CANNIBAL_WAIT_TIME (200) 

#define ENEMY_CANNIBAL_VALUE_MOVE (5.0f)
#define ENEMY_CANNIBAL_BULLET_MOVE (3.5f)
#define ENEMY_CANNIBAL_ACK_RANGE (380)  //警戒范围
//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
static void ChangeAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller, int animationID);
static void SetAnimationTime(int index, int name, float *animationTime);
static void ResetAnimationTdPosition(int index, LPD3DXANIMATIONCONTROLLER   *Controller);
static void ReInitOneAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller);
static void InitController(LPD3DXANIMATIONCONTROLLER *Controller, int EnemyIndex, bool bEnable);
static bool EnemyCannibalWaitWithSecend(int index);

static bool bHitEvent = false;
static bool bHitEventG[ENEMY_CANNIBAL_MAX_NUM]; //辅助重力
static int TmpStatus = ENEMY_CANNIBAL_IDLE; //临时状态记录
//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureEnemyCannibal;		// 僥僋僗僠儍撉傒崬傒応強
LPD3DXMESH			g_pMeshEnemyCannibal;				// ID3DXMesh僀儞僞乕僼僃僀僗傊偺億僀儞僞
LPD3DXBUFFER		g_pD3DXMatBuffEnemyCannibal;		// 儊僢僔儏偺儅僥儕傾儖忣曬傪奿擺
//DWORD				g_nNumMatEnemyCannibal;			// 懏惈忣曬偺憤悢

D3DXMATRIX			g_mtxWorldEnemyCannibal;			// 儚乕儖僪儅僩儕僢僋僗
static ENEMY_CANNIBAL				g_EnemyCannibal[ENEMY_CANNIBAL_MAX_NUM];					// 僾儗僀儎乕儚乕僋
static ENEMY_CANNIBAL               g_EnemyCannibal_bak[ENEMY_CANNIBAL_MAX_NUM];
//static ID3DXAnimationController*   g_pAnimController = NULL;
static LPD3DXFRAME                 g_pFrameRoot[ENEMY_CANNIBAL_MAX_NUM];;
static D3DXMATRIX*                 g_pBoneMatrices = NULL;
static CAllocateHierarchy*         g_pAllocateHier[ENEMY_CANNIBAL_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimController[ENEMY_CANNIBAL_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSet[ENEMY_CANNIBAL_MAX_NUM];
//////////////////////
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerTotal[ENEMY_CANNIBAL_MAX_NUM];
//////animation/////////
////////DIE/////////
static LPD3DXFRAME                 g_pFrameRootDie[ENEMY_CANNIBAL_MAX_NUM];
static D3DXMATRIX*                 g_pBoneMatricesDie = NULL;
static CAllocateHierarchy*         g_pAllocateHierDie[ENEMY_CANNIBAL_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerDie[ENEMY_CANNIBAL_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetDie[ENEMY_CANNIBAL_MAX_NUM];
////////////////////////
//////ACK///////////////
static LPD3DXFRAME                 g_pFrameRootAck[ENEMY_CANNIBAL_MAX_NUM];
static D3DXMATRIX*                 g_pBoneMatricesAck = NULL;
static CAllocateHierarchy*         g_pAllocateHierAck[ENEMY_CANNIBAL_MAX_NUM];
static LPD3DXANIMATIONCONTROLLER   g_pAnimControllerAck[ENEMY_CANNIBAL_MAX_NUM];
static LPD3DXANIMATIONSET          g_pAnimationSetAck[ENEMY_CANNIBAL_MAX_NUM];
/////////////////////////

static char *StateName[ENEMY_CANNIBAL_ANIMATION_MAX] =
{
	"IDLE",
	"DIE",
	"ACK"
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
static bool LoadPixHLSLEnemyCannibal();
////////////////////////////////////////////////////////////////////////
//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitEnemyCannibal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//DWORD *timing = RetTiming(TIMING_ENEMY_WAIT);

	g_pD3DTextureEnemyCannibal = NULL;
	g_pMeshEnemyCannibal = NULL;
	g_pD3DXMatBuffEnemyCannibal = NULL;

	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		//g_EnemyCannibal[num].pos = D3DXVECTOR3(40.0f, 20.0f, 40.0f);
		g_EnemyCannibal[num].old_pos = g_EnemyCannibal[num].pos;
		g_EnemyCannibal[num].last_frame_pos = g_EnemyCannibal[num].pos;
		g_EnemyCannibal[num].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyCannibal[num].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyCannibal[num].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_EnemyCannibal[num].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_EnemyCannibal[num].use = false;
		g_EnemyCannibal[num].AckJudge = true;
		g_EnemyCannibal[num].AckPlayerRotJudge = true;
		g_EnemyCannibal[num].AckPlayerJudge = true;
		g_EnemyCannibal[num].DieJudge = true;
		g_EnemyCannibal[num].LastAnimationID = ENEMY_CANNIBAL_IDLE;
		g_EnemyCannibal[num].status = ENEMY_CANNIBAL_IDLE;
		g_EnemyCannibal[num].LastStatus = ENEMY_CANNIBAL_IDLE;
		g_EnemyCannibal[num].hp = ENEMY_CANNIBAL_MAX_HP;
		g_EnemyCannibal[num].down_dir = false;
		g_EnemyCannibal[num].fRadius = ENEMY_CANNIBAL_RADIUS;
		//g_EnemyCannibal[num].SphereColliderIdx = CreateSphereCollider(g_EnemyCannibal[num].pos, g_EnemyCannibal[num].rot, g_EnemyCannibal[num].scl * 3);
		//g_EnemyCannibal[num].ShadowIdx = CreateShadow(g_EnemyCannibal[num].pos, g_EnemyCannibal[num].rot, g_EnemyCannibal[num].scl, true);
		//Time_Stop(timing[num]);
		bHitEventG[num] = false;
	}

	// X僼傽僀儖偺撉傒崬傒
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		g_pAllocateHier[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 

	    g_pAllocateHierDie[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 

		g_pAllocateHierAck[num] = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 
		// X僼傽僀儖傪撉傒崬傓
		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_CANNIBAL,
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

		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_CANNIBAL_DIE,
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
		
		if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_ENEMY_CANNIBAL_ACK,
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
		SetupBoneMatrixPointers(g_pFrameRoot[num], g_pFrameRoot[num]);

		//SetupBoneMatrixPointers(g_pFrameRootRun[num], g_pFrameRootRun[num]);

		SetupBoneMatrixPointers(g_pFrameRootDie[num], g_pFrameRootDie[num]);

		SetupBoneMatrixPointers(g_pFrameRootAck[num], g_pFrameRootAck[num]);
	}
	//g_pFrameRoot->pMeshContainer;
	//崪偺埵抲傪惂掕

	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		InitController(g_pAnimController, num, true);
		InitController(g_pAnimControllerDie, num, false);
		InitController(g_pAnimControllerAck, num, false);
		g_pAnimControllerTotal[num] = g_pAnimController[num];
		InitController(g_pAnimControllerTotal, num, true);
	}
	
	LoadPixHLSLEnemyCannibal();

	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitEnemyCannibal(void)
{
	if (g_pD3DTextureEnemyCannibal != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_pD3DTextureEnemyCannibal->Release();
		g_pD3DTextureEnemyCannibal = NULL;
	}

	if (g_pMeshEnemyCannibal != NULL)
	{// 儊僢僔儏偺奐曻
		g_pMeshEnemyCannibal->Release();
		g_pMeshEnemyCannibal = NULL;
	}

	if (g_pD3DXMatBuffEnemyCannibal != NULL)
	{// 儅僥儕傾儖偺奐曻
		g_pD3DXMatBuffEnemyCannibal->Release();
		g_pD3DXMatBuffEnemyCannibal = NULL;
	}
	//g_pEffect->Release();
}



void UpdateEnemyCannibal(void)
{
	PLAYER1 *Player = GetPlayer1();
	BULLET *Bullet = RetBullet();
	DWORD *timing = RetTiming(TIMING_ENEMY_CANNIBAL_WAIT);
	int *timeIndex = RetTimeIndex(TIMING_ENEMY_CANNIBAL_WAIT);

	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		if (g_EnemyCannibal[num].use)
		{
			//PrintDebugProc("enemy 0 = pos.x=%f,y=%f\n", g_EnemyCannibal[num].pos.x, g_EnemyCannibal[num].pos.y);
			//重力///////////////
			//if (g_EnemyCannibal[num].down_dir)
			//{
			//	if (bHitEventG[num] == false)
			//	{
			//		bHitEventG[num] = true;
			//	}
			//}
			//if (bHitEventG[num] == false)
			//	g_EnemyCannibal[num].pos.y -= 2.0f;
			///////////////////////////
			if (g_EnemyCannibal[num].hp <= 0)
			{
				D3DXTRACK_DESC td1;
				ChangeAnimation(num, g_pAnimControllerDie, ENEMY_CANNIBAL_DIE);
				g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyCannibal[num].currentTrack, &td1);

				if (g_EnemyCannibal[num].DieJudge)
				{
					ReInitOneAnimation(num, g_pAnimControllerTotal);
					DeleteSphereCollider(g_EnemyCannibal[num].SphereColliderIdx);
					SetSoul(num, ENEMY_TYPE_CANNIBAL, 3);
					g_EnemyCannibal[num].DieJudge = false;
				}

				if (td1.Position >= 1.0f)
				{
					SetDisPlayEnemyHp(FALSE);
					D3DXTRACK_DESC td2;
					td2.Enable = false;
					g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyCannibal[num].currentTrack, &td2);
					g_EnemyCannibal[num].use = false;
				}
				//PrintDebugProc("enemy:%f\n", td1.Position);
			}
			else
			{
				D3DXVECTOR3 vDistance = Player->pos - g_EnemyCannibal[num].pos;

				float length = D3DXVec3Length(&vDistance);
				//PrintDebugProc("enemy status = %d\n", g_EnemyCannibal[num].status);
				//当距离过远切换等待状态
				//
				if (length > ENEMY_CANNIBAL_ACK_RANGE
					&& g_EnemyCannibal[num].animationID != ENEMY_CANNIBAL_ACK)
				{
					//SetbEnemyAstart(num, false);
					if(g_EnemyCannibal[num].status == ENEMY_CANNIBAL_STATUS_ACK)
					    ResetAnimationTdPosition(num, g_pAnimControllerTotal);
					ChangeAnimation(num, g_pAnimController, ENEMY_CANNIBAL_IDLE);
					g_EnemyCannibal[num].status = ENEMY_CANNIBAL_STATUS_IDLE;

					g_EnemyCannibal[num].AckPlayerRotJudge = true;
					g_EnemyCannibal[num].AckPlayerJudge = true;
				}
				else
				{
					//任何状态切换的等待
					if (g_EnemyCannibal[num].animationID != g_EnemyCannibal[num].LastAnimationID)
					{

						//SetbEnemyAstart(num, false);
						bool WaitJudge = EnemyCannibalWaitWithSecend(num);
						if (WaitJudge == false)
						{
							Time_Start(timeIndex[num]);
							g_EnemyCannibal[num].status = ENEMY_CANNIBAL_STATUS_WAIT;
						}
						else
						{
							//Time_Reset(timeIndex[num]);
							Time_Stop(timeIndex[num]);
							g_EnemyCannibal[num].LastAnimationID = g_EnemyCannibal[num].animationID;
							g_EnemyCannibal[num].status = ENEMY_CANNIBAL_STATUS_WAITOVER;
						}
					}
					else
					{    
					    if (g_EnemyCannibal[num].AckPlayerRotJudge)
					    {
					    
					    	float MoveRot = RetRotValue(D3DXVECTOR2{ Player->pos.x, Player->pos.z }, D3DXVECTOR2{ g_EnemyCannibal[num].pos.x, g_EnemyCannibal[num].pos.z });
					    	g_EnemyCannibal[num].rot.y = D3DX_PI / 2 - MoveRot;
					    	g_EnemyCannibal[num].AckPlayerRotJudge = false;
					    }
					    g_EnemyCannibal[num].status = ENEMY_CANNIBAL_STATUS_ACK;
					    ChangeAnimation(num, g_pAnimControllerAck, ENEMY_CANNIBAL_ACK);
					    				    
					    D3DXTRACK_DESC td1;
					    g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyCannibal[num].currentTrack, &td1);
					    
					    if (td1.Position >= 1.9f)
					    {
					    	if (g_EnemyCannibal[num].AckPlayerJudge)
					    	{
								//int RotY = RetRotValue(D3DXVECTOR2{ Player->pos.x, Player->pos.z }, D3DXVECTOR2{ g_EnemyCannibal[num].pos.x, g_EnemyCannibal[num].pos.z });
					    		int BulletIndex = 
									CreateBullet(
					    			D3DXVECTOR3(g_EnemyCannibal[num].pos.x, g_EnemyCannibal[num].pos.y + 30.0f, g_EnemyCannibal[num].pos.z),
					    			g_EnemyCannibal[num].rot,
					    			D3DXVECTOR3{ 1.2f, 1.2f, 1.2f },
									//D3DXVECTOR3(-sinf(RotY) * ENEMY_CANNIBAL_BULLET_MOVE, 0.0f, -cosf(RotY) * ENEMY_CANNIBAL_BULLET_MOVE),
					    			D3DXVECTOR3(-sinf(g_EnemyCannibal[num].rot.y) * ENEMY_CANNIBAL_BULLET_MOVE, 0.0f, -cosf(g_EnemyCannibal[num].rot.y) * ENEMY_CANNIBAL_BULLET_MOVE), 
										200);
								//PrintDebugProc("%d\n", BulletIndex);
								Bullet[BulletIndex].EffectIdx =
									SetEffect(D3DXVECTOR3(g_EnemyCannibal[num].pos.x, g_EnemyCannibal[num].pos.y + 30.0f, g_EnemyCannibal[num].pos.z),
										//D3DXVECTOR3(-sinf(RotY) * ENEMY_CANNIBAL_BULLET_MOVE, 0.0f, -cosf(RotY) * ENEMY_CANNIBAL_BULLET_MOVE),
										D3DXVECTOR3(-sinf(g_EnemyCannibal[num].rot.y) * ENEMY_CANNIBAL_BULLET_MOVE, 0.0f, -cosf(g_EnemyCannibal[num].rot.y) * ENEMY_CANNIBAL_BULLET_MOVE),
										D3DXCOLOR(1.65f, 0.05f, 0.85f, 0.80f), 26.0f, 26.0f, 200);
					    		g_EnemyCannibal[num].AckPlayerJudge = false;
					    	}
					    }
						//动作完成
					    if (td1.Position >= 3.0f)
					    {
					    	ResetAnimationTdPosition(num, g_pAnimControllerTotal);
					    	ChangeAnimation(num, g_pAnimController, ENEMY_CANNIBAL_IDLE);
					    	g_EnemyCannibal[num].status = ENEMY_CANNIBAL_IDLE;
					    	Time_Start(timeIndex[num]);
					    	g_EnemyCannibal[num].AckPlayerRotJudge = true;
					    	g_EnemyCannibal[num].AckPlayerJudge = true;
					    }
					    
					}
				}
			}
			//g_pAnimController->AdvanceTime(0.02f, NULL);
			g_pAnimControllerTotal[num]->AdvanceTime(0.02f, NULL);

			//D3DXTRACK_DESC td1;
			//g_pAnimControllerTotal[num]->GetTrackDesc(g_EnemyCannibal[num].currentTrack, &td1);
			//PrintDebugProc("enemy:%d, %f, ID = %d\n", num, td1.Position, g_EnemyCannibal[num].animationID);

			SetPositionShadow(g_EnemyCannibal[num].ShadowIdx, g_EnemyCannibal[num].pos, g_EnemyCannibal[num].rot,
				D3DXVECTOR3{ g_EnemyCannibal[num].scl.x,
				g_EnemyCannibal[num].scl.y / 2,
				g_EnemyCannibal[num].scl.z });
			//PrintDebugProc("e................%f\n", g_EnemyCannibal[1].pos.z);
			SetPositionSphereCollider(g_EnemyCannibal[num].SphereColliderIdx,
				D3DXVECTOR3{ g_EnemyCannibal[num].pos.x,g_EnemyCannibal[num].pos.y + 10.0f, g_EnemyCannibal[num].pos.z },
				D3DXVECTOR3{ g_EnemyCannibal[num].rot.x ,g_EnemyCannibal[num].rot.y,g_EnemyCannibal[num].rot.z }, D3DXVECTOR3{ 1.0f ,1.0f,1.0f } *2.5);

			//if (g_EnemyCannibal[num].pos != g_EnemyCannibal[num].last_frame_pos)
			//{
			//	g_EnemyCannibal[num].down_dir = false;
			//	bHitEventG[num] = false;
			//}
			/////////记录上一个状态(跳过Wait状态)/////////////
			if (g_EnemyCannibal[num].status != ENEMY_CANNIBAL_STATUS_WAIT 
			&& g_EnemyCannibal[num].status != ENEMY_CANNIBAL_STATUS_WAITOVER)
			{
				if (g_EnemyCannibal[num].status != TmpStatus )
				{
					g_EnemyCannibal[num].LastStatus = TmpStatus;
					TmpStatus = g_EnemyCannibal[num].status;
				}
			}
			//PrintDebugProc("aaaa now , Last = %d, %d\n", g_EnemyCannibal[num].status, g_EnemyCannibal[num].LastStatus);
			////////////////////////////////////
			//g_EnemyCannibal[num].last_frame_pos = g_EnemyCannibal[num].pos;
		}
	}
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawEnemyCannibal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScl;
	D3DXMATERIAL *pD3DXMat;
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		if (g_EnemyCannibal[num].use)
		{
			if (CubeInFrustum(g_EnemyCannibal[num].pos))
			{
				// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
				D3DXMatrixIdentity(&g_mtxWorldEnemyCannibal);

				D3DXMatrixScaling(&mtxScl, g_EnemyCannibal[num].scl.x, g_EnemyCannibal[num].scl.y, g_EnemyCannibal[num].scl.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyCannibal, &g_mtxWorldEnemyCannibal, &mtxScl);

				// 夞揮傪斀塮
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EnemyCannibal[num].rot.y, g_EnemyCannibal[num].rot.x, g_EnemyCannibal[num].rot.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyCannibal, &g_mtxWorldEnemyCannibal, &mtxRot);

				// 堏摦傪斀塮
				D3DXMatrixTranslation(&mtxTranslate, g_EnemyCannibal[num].pos.x, g_EnemyCannibal[num].pos.y, g_EnemyCannibal[num].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldEnemyCannibal, &g_mtxWorldEnemyCannibal, &mtxTranslate);

				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemyCannibal);

				pDevice->SetRenderState(D3DRS_ZENABLE, true);

				if (g_EnemyCannibal[num].animationID == ENEMY_CANNIBAL_IDLE)
				{
					UpdateFrameMatrices(g_pFrameRoot[num], &g_mtxWorldEnemyCannibal);
					DrawFrame(pDevice, g_pFrameRoot[num], 0);
				}
				else if (g_EnemyCannibal[num].animationID == ENEMY_CANNIBAL_DIE)
				{
					UpdateFrameMatrices(g_pFrameRootDie[num], &g_mtxWorldEnemyCannibal);
					DrawFrame(pDevice, g_pFrameRootDie[num], 0);
				}
				else if (g_EnemyCannibal[num].animationID == ENEMY_CANNIBAL_ACK)
				{
					UpdateFrameMatrices(g_pFrameRootAck[num], &g_mtxWorldEnemyCannibal);
					DrawFrame(pDevice, g_pFrameRootAck[num], 0);
				}

				pDevice->SetRenderState(D3DRS_ZENABLE, false);
			}
		}
	}
}



ENEMY_CANNIBAL *GetEnemyCannibal(void)
{
	return &g_EnemyCannibal[0];
}


void  SetAnimationTime(int index, int name, float *animationTime)
{
	float time;
	switch (name)
	{
	case ENEMY_CANNIBAL_IDLE:
		time = 0.0f;
		break;
	case ENEMY_CANNIBAL_DIE:
		time = 0.0f;
		break;
	case ENEMY_CANNIBAL_ACK:
		time = 0.0f;
		break;
	//case ENEMY_CANNIBAL_RUN:
	//	time = 0.0f;
	//	break;
	}
	time = 0.0f;

	//animationTime[index] = time / 30;

	return;
}
//=============================================================================
// 傾僯儊乕僔儑儞傪愗傝懼偊
//=============================================================================

void ChangeAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller, int animationID)
{

	if (g_EnemyCannibal[index].animationID != animationID)
	{

		ReInitOneAnimation(index, Controller);

		g_EnemyCannibal[index].animationTime[g_EnemyCannibal[index].animationID] = 0;

		SetAnimationTime(index, animationID, &g_EnemyCannibal[index].animationTime[animationID]);

		DWORD dwNewTrack = (g_EnemyCannibal[index].currentTrack == 0 ? 1 : 0);

		LPD3DXANIMATIONSET g_pAnimationSet = NULL;

		Controller[index]->GetAnimationSetByName(StateName[animationID], &g_pAnimationSet);
		Controller[index]->SetTrackAnimationSet(dwNewTrack, g_pAnimationSet);
		//g_pAnimationSet->Release();

		Controller[index]->UnkeyAllTrackEvents(g_EnemyCannibal[index].currentTrack);
		Controller[index]->UnkeyAllTrackEvents(dwNewTrack);

		D3DXTRACK_DESC td;
		Controller[index]->GetTrackDesc(g_EnemyCannibal[index].currentTrack, &td);

		td.Position = 0.1f;

		Controller[index]->SetTrackDesc(dwNewTrack, &td);
		g_EnemyCannibal[index].animationID = animationID;

		Controller[index]->KeyTrackEnable(g_EnemyCannibal[index].currentTrack, FALSE, g_EnemyCannibal[index].curTime + 0.25f);
		Controller[index]->KeyTrackSpeed(g_EnemyCannibal[index].currentTrack, 0.0f, g_EnemyCannibal[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);
		Controller[index]->KeyTrackWeight(g_EnemyCannibal[index].currentTrack, 0.0f, g_EnemyCannibal[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);


		Controller[index]->SetTrackEnable(dwNewTrack, TRUE);
		Controller[index]->KeyTrackSpeed(dwNewTrack, 1.0f, g_EnemyCannibal[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);
		Controller[index]->KeyTrackWeight(dwNewTrack, 1.0f, g_EnemyCannibal[index].curTime, 0.25f, D3DXTRANSITION_LINEAR);


		g_EnemyCannibal[index].currentTrack = dwNewTrack;

		Controller[index]->SetTrackAnimationSet(g_EnemyCannibal[index].currentTrack, g_pAnimationSet);

		g_EnemyCannibal[index].curTime = 0.0f;

		g_pAnimControllerTotal[index] = Controller[index];
	}
}


/////////////////////////////////像素着色器////////////////////////////
bool LoadPixHLSLEnemyCannibal()
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


void BackUpEnemyCannibal(void)
{
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		g_EnemyCannibal_bak[num] = g_EnemyCannibal[num];
	}
}

HRESULT RestartEnemyCannibal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		g_EnemyCannibal[num] = g_EnemyCannibal_bak[num];
	}
	return S_OK;
}


void ResetEnemyCannibalAckJudge()
{
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		g_EnemyCannibal[num].AckJudge = true;
	}
}


bool EnemyCannibalWaitWithSecend(int index)
{
	DWORD *timing = RetTiming(TIMING_ENEMY_CANNIBAL_WAIT);
	int *timeIndex = RetTimeIndex(TIMING_ENEMY_CANNIBAL_WAIT);

	//PrintDebugProc("Enemy Wait %d = %d\n", index, (int)timing[index]);

	if ((int)timing[index] > ENEMY_CANNIBAL_WAIT_TIME)
	{
		Time_Stop(timeIndex[index]);
		return true;
	}
	return false;
}


void ResetAnimationTdPosition(int index, LPD3DXANIMATIONCONTROLLER   *Controller)
{
	D3DXTRACK_DESC td;
	Controller[index]->GetTrackDesc(g_EnemyCannibal[index].currentTrack, &td);

	td.Position = 0.1f;

	Controller[index]->SetTrackDesc(g_EnemyCannibal[index].currentTrack, &td);
}



void ReInitOneAnimation(int index, LPD3DXANIMATIONCONTROLLER *Controller)
{
	D3DXTRACK_DESC td0;
	td0.Enable = true;
	td0.Position = 0.1f;
	td0.Speed = 1.0f;
	td0.Weight = 1.0f;
	Controller[index]->SetTrackDesc(g_EnemyCannibal[index].currentTrack, &td0);
}


int CreateEnemyCannibal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	int nIdxEnemyCannibal = -1;

	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		if (!g_EnemyCannibal[num].use)
		{
			g_EnemyCannibal[num].pos = pos;
			g_EnemyCannibal[num].rot = rot;
			g_EnemyCannibal[num].scl = scl;
			g_EnemyCannibal[num].hp = ENEMY_CANNIBAL_MAX_HP;
			bHitEventG[num] = false;
			g_EnemyCannibal[num].use = true;
			nIdxEnemyCannibal = num;
			g_EnemyCannibal[num].SphereColliderIdx = CreateSphereCollider(g_EnemyCannibal[num].pos, g_EnemyCannibal[num].rot, g_EnemyCannibal[num].scl * 3);
			g_EnemyCannibal[num].ShadowIdx = CreateShadow(g_EnemyCannibal[num].pos, g_EnemyCannibal[num].rot, g_EnemyCannibal[num].scl, true);
			g_EnemyCannibal[num].AckJudge = true;
			g_EnemyCannibal[num].AckPlayerRotJudge = true;
			g_EnemyCannibal[num].AckPlayerJudge = true;
			g_EnemyCannibal[num].DieJudge = true;
			g_EnemyCannibal[num].LastAnimationID = ENEMY_CANNIBAL_IDLE;
			g_EnemyCannibal[num].status = ENEMY_CANNIBAL_IDLE;
			g_EnemyCannibal[num].LastStatus = ENEMY_CANNIBAL_IDLE;
			g_EnemyCannibal[num].hp = ENEMY_CANNIBAL_MAX_HP;
			g_EnemyCannibal[num].down_dir = false;
			g_EnemyCannibal[num].fRadius = ENEMY_CANNIBAL_RADIUS;
			break;
		}
	}
	return nIdxEnemyCannibal;
}


void DrawEnemyCannibalShadow(int EnemyCannibalIndex, D3DXMATRIX mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 更新骨骼动画
	if (g_EnemyCannibal[EnemyCannibalIndex].animationID == ENEMY_CANNIBAL_IDLE)
	{
		UpdateFrameMatrices(g_pFrameRoot[EnemyCannibalIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRoot[EnemyCannibalIndex], 1);
	}
	else if (g_EnemyCannibal[EnemyCannibalIndex].animationID == ENEMY_CANNIBAL_DIE)
	{
		UpdateFrameMatrices(g_pFrameRootDie[EnemyCannibalIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootDie[EnemyCannibalIndex], 1);
	}
	else if (g_EnemyCannibal[EnemyCannibalIndex].animationID == ENEMY_CANNIBAL_ACK)
	{
		UpdateFrameMatrices(g_pFrameRootAck[EnemyCannibalIndex], &mtxWorld);
		DrawFrame(pDevice, g_pFrameRootAck[EnemyCannibalIndex], 1);
	}

}


void InitController(LPD3DXANIMATIONCONTROLLER *Controller, int EnemyIndex, bool bEnable)
{
	Controller[EnemyIndex]->GetAnimationSetByName(StateName[g_EnemyCannibal[EnemyIndex].animationID], &g_pAnimationSet[EnemyIndex]);
	Controller[EnemyIndex]->SetTrackAnimationSet(g_EnemyCannibal[EnemyIndex].currentTrack, g_pAnimationSet[EnemyIndex]);
	//g_pAnimationSet->Release();
	Controller[EnemyIndex]->UnkeyAllTrackEvents(g_EnemyCannibal[EnemyIndex].currentTrack);
	Controller[EnemyIndex]->SetTrackEnable(g_EnemyCannibal[EnemyIndex].currentTrack, bEnable);
	Controller[EnemyIndex]->KeyTrackSpeed(g_EnemyCannibal[EnemyIndex].currentTrack, 1.0f, 0.01f, 0.25f, D3DXTRANSITION_LINEAR);
	Controller[EnemyIndex]->KeyTrackWeight(g_EnemyCannibal[EnemyIndex].currentTrack, 1.0f, 0.01f, 0.25f, D3DXTRANSITION_LINEAR);

	g_EnemyCannibal[EnemyIndex].curTime = g_EnemyCannibal[EnemyIndex].prvTime = timeGetTime();

}


void ReInitCannibalAnimation()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int num = 0; num < ENEMY_CANNIBAL_MAX_NUM; num++)
	{
		D3DXTRACK_DESC td1;
		td1.Enable = false;
		td1.Position = 0.1f;
		td1.Speed = 1.0f;
		td1.Weight = 1.0f;
		g_pAnimControllerTotal[num]->SetTrackDesc(g_EnemyCannibal[num].currentTrack, &td1);
	
	
	}
}
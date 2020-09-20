//=============================================================================
//
// プレイヤー処理 [player1.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "player1.h"
#include "camera_Ex.h"
#include "input.h"
#include "AllocateHierarchyClass.h"
#include "cylinderCollider.h"
#include "debugproc.h"
#include "GlobalFunction.h"
#include "GameMath.h"
#include "GameAi.h"
#include "Time_c.h"
#include "GameEvent.h"
#include "sphereCollider.h"
#include "Enemy_wolf.h"
#include "EnemyCannibal.h"
#include "EnemyBossWolf.h"
#include "shadow.h"
#include "sound.h"
#include "Scene.h"
#include "UiTitleMenu.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
//#define	MODEL_PLAYER		"data/MODEL/w31.x"	// 撉傒崬傓儌僨儖柤
#define	MODEL_PLAYER		"data/MODEL/UNITY.X"

#define	PLAYER_RADIUS		(10.0f)						// 敿宎
//#define	VALUE_MOVE_PLAYER	(0.155f)					// 堏摦懍搙
#define	VALUE_MOVE_PLAYER	(5.0f)					// 堏摦懍搙
#define	RATE_MOVE_PLAYER	(0.025f)					// 堏摦姷惈學悢
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// 夞揮懍搙
#define	RATE_ROTATE_PLAYER	(0.10f)						// 夞揮姷惈學悢
#define	VALUE_MOVE_BULLET	(1.5f)						// 抏偺堏摦懍搙
#define PLAYER1_ATTACK_SPEED_TIME (0.031f)  //人物攻击碰撞器攻击速度
#define PLAYER1_ACKED_TIME (950) //被攻击恢复时间
#define PLAYER1_VALUE_MOVE (5.0f)
//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
static void ChangeAnimation(LPD3DXANIMATIONCONTROLLER Controller, int animationID);

//static void ChangeAnimation(LPD3DXANIMATIONCONTROLLER Controller, int animationID);
static void SetAnimationTime(int name, float &animationTime);
//bool frame_change(void);

static bool bHitEvent = false;
static bool bHitEventG = false; //重力判定辅助变量
static bool bHitEventJ = false; //跳跃判定辅助变量
static bool bCameraPosChg = true;  //视点切换辅助变量
static D3DXVECTOR3 dir;
//Jump////
static float JumpMove = 0.0;
//animation//////
//ack///////
static float StartAckRot = 0.0f;
static float EndAckRot = 0.0f;
static float AckCurrentTime = 0.0f;
static float fCurWeightTime = 0.0f;

//////////
//////////////////////
//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer1;		// 僥僋僗僠儍撉傒崬傒応強
LPD3DXMESH			g_pMeshPlayer1;				// ID3DXMesh僀儞僞乕僼僃僀僗傊偺億僀儞僞
LPD3DXBUFFER		g_pD3DXMatBuffPlayer1;		// 儊僢僔儏偺儅僥儕傾儖忣曬傪奿擺
DWORD				g_nNumMatPlayer1;			// 懏惈忣曬偺憤悢

D3DXMATRIX			g_mtxWorldPlayer1;			// 儚乕儖僪儅僩儕僢僋僗
static PLAYER1				g_player;					// 僾儗僀儎乕儚乕僋
static PLAYER1               g_PlayerBak;

static LPD3DXFRAME                 g_pFrameRoot = NULL;
static D3DXMATRIX*                 g_pBoneMatrices = NULL;
static CAllocateHierarchy*         g_pAllocateHier = NULL;
static LPD3DXANIMATIONCONTROLLER   g_pAnimController = NULL;
static LPD3DXANIMATIONSET          g_pAnimationSet = NULL;

static int HurtEventIndexTime = 0;


static char *StateName[ANIMATION_MAX] =
{
	"IDLE",
	"RUN",
	"SLASH1",
	"SLASH2",
	"MAGIC",
	"KICK",
	"JUMP",
	"ROLL",
	"DEATH1",
	"DEATH2",
	"IMPACT",
	"SIT",
	"WIN"
};
////////////////像素着色器/////////////////////////////////////
static D3DXHANDLE Samp0Handle = NULL;
static D3DXCONSTANT_DESC Samp0Desc;
static IDirect3DPixelShader9* g_ToonShader = 0;
static IDirect3DPixelShader9* TexPS = 0;
static ID3DXConstantTable* TransformConstantTable = 0;
static D3DXHANDLE Player1AlphaHandle;
static D3DXHANDLE Player1ColorHandle;

bool LoadPixHLSLPlayer1();
////////////////////////////////////////////////////////////////////////
//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitPlayer1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_pD3DTexturePlayer1 = NULL;
	g_pMeshPlayer1 = NULL;
	g_pD3DXMatBuffPlayer1 = NULL;

	g_player.pos = D3DXVECTOR3(720.11f, 300.0f, 85.0f);
	g_player.old_pos = g_player.pos;
	for (int i = 0; i < 5; i++)
		g_player.last_frame_pos[i] = g_player.pos;
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.use = true;
	g_player.fRadius = PLAYER_RADIUS;
	g_player.animationID = PLAYER_IDLE;
	g_player.AckJudge = true;
	g_player.AckedJudge = true;
	g_player.HurtJudge = false;
	g_player.DieJudge = false;
	g_player.AiMode = false;
	g_player.DieSe = true;
	g_player.SoulNum = 0;
	g_player.hp = 100;
	g_player.WalkSe = 0;
	// X僼傽僀儖偺撉傒崬傒
	g_pAllocateHier = new CAllocateHierarchy(); //僗僉儞儊僢僔儏傾僯儊乕僔儑儞弶婜壔 

	 // X僼傽僀儖傪撉傒崬傓
	if (FAILED(D3DXLoadMeshHierarchyFromX(MODEL_PLAYER,
		D3DXMESH_MANAGED,
		pDevice,
		g_pAllocateHier,
		NULL,
		&g_pFrameRoot,
		&g_pAnimController
	)))
	{
		return E_FAIL;
	}

	//崪偺埵抲傪惂掕
	SetupBoneMatrixPointers(g_pFrameRoot, g_pFrameRoot);


#if 0
	// 僥僋僗僠儍偺撉傒崬傒
	D3DXCreateTextureFromFile(pDevice,					// 僨僶僀僗傊偺億僀儞僞
		TEXTURE_FILENAME,		// 僼傽僀儖偺柤慜
		&g_pD3DTexturePlayer1);	// 撉傒崬傓儊儌儕乕
#endif

	//傾僯儊乕僔儑儞僙僢僩弶婜壔
	g_pAnimController->GetAnimationSetByName(StateName[g_player.animationID], &g_pAnimationSet);
	g_pAnimController->SetTrackAnimationSet(g_player.currentTrack, g_pAnimationSet);
	//g_pAnimationSet->Release();
	g_pAnimController->UnkeyAllTrackEvents(g_player.currentTrack);
	g_pAnimController->SetTrackEnable(g_player.currentTrack, TRUE);
	g_pAnimController->KeyTrackSpeed(g_player.currentTrack, 1.0f, 0.01f, 0.25f, D3DXTRANSITION_LINEAR);
	g_pAnimController->KeyTrackWeight(g_player.currentTrack, 1.0f, 0.01f, 0.25f, D3DXTRANSITION_LINEAR);
	g_player.curTime = g_player.prvTime = timeGetTime();

	//g_player.SphereColliderIdx = CreateSphereCollider(g_player.pos, g_player.rot, g_player.scl);

	if (!LoadPixHLSLPlayer1())
	{
		MessageBox(0, "Failed to Load Pix HLSL!", 0, 0);
		return E_FAIL;
	}
	/////////////像素着色器句柄///////////////
	Samp0Handle = TransformConstantTable->GetConstantByName(0, "Samp0");
	//设置对着色器变量Samp0、Samp1的描述
	UINT count;
	TransformConstantTable->GetConstantDesc(Samp0Handle, &Samp0Desc, &count);
	//设定各着色器变量为初始值
	TransformConstantTable->SetDefaults(pDevice);

	Player1AlphaHandle = TransformConstantTable->GetConstantByName(NULL, "player_alpha");
	Player1ColorHandle = TransformConstantTable->GetConstantByName(NULL, "player_color");
	/////////////////////////////////////////////////////

	g_player.CylinderColliderIdx = CreateCylinderCollider(g_player.pos, g_player.rot, g_player.scl);
	g_player.SphereColliderAckIdx = CreateSphereCollider(g_player.pos, g_player.rot, g_player.scl);
	g_player.ShadowIdx = CreateShadow(g_player.pos, g_player.rot, g_player.scl, true);
	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitPlayer1(void)
{
	if (g_pD3DTexturePlayer1 != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_pD3DTexturePlayer1->Release();
		g_pD3DTexturePlayer1 = NULL;
	}

	if (g_pMeshPlayer1 != NULL)
	{// 儊僢僔儏偺奐曻
		g_pMeshPlayer1->Release();
		g_pMeshPlayer1 = NULL;
	}

	if (g_pD3DXMatBuffPlayer1 != NULL)
	{// 儅僥儕傾儖偺奐曻
		g_pD3DXMatBuffPlayer1->Release();
		g_pD3DXMatBuffPlayer1 = NULL;
	}
	//SAFE_RELEASE(g_ToonShader);
	//SAFE_RELEASE(TexPS);
	//SAFE_RELEASE(TransformConstantTable);
}


void UpdatePlayer1(void)
{
	DWORD *timing = RetTiming(TIMING_PLAYER_RUN);
	DWORD *TimingPlayerAcked = RetTiming(TIMING_PLAYER_ACKED);
	int *timeIndexPlayerAcked = RetTimeIndex(TIMING_PLAYER_ACKED);
	int *NowStruct = RetNowStruct();
	int NowScene = RetCurrentSceneIndex();

	if (g_player.use)
	{
#ifdef PLAYER_DIE_SWITCH
		if (NowScene != SCENE_SCENE_HOME)
		{
			if (g_player.hp <= 0)
			{
				g_player.DieJudge = true;
				ChangeAnimation(g_pAnimController, PLAYER_DEATH2);
				D3DXTRACK_DESC td_die;
				g_pAnimController->GetTrackDesc(g_player.currentTrack, &td_die);
				if (g_player.DieSe)
				{
					StopSound(SOUND_LABEL_BGM_BOSS);
					StopSound(SOUND_LABEL_BGM_GAME);
					PlaySound(SOUND_LABEL_SE_DIE);
					g_player.DieSe = false;
				}
				//PlaySound(SOUND_LABEL_SE_ENEMY_WOLF_ACK);
				if (td_die.Position >= 1.0f)
				{
					D3DXTRACK_DESC td;
					td.Enable = false;
					g_pAnimController->SetTrackDesc(g_player.currentTrack, &td);
				}
			}
		}
#endif // PLAYER_DIE_SWITCH

		g_player.animationTime[g_player.animationID] -= 0.02f;


		CAMERA_EX *cam = GetCamera_Ex();

		dir.x = -1;
		dir.y = 1;
		///////dir////////////
		/////////////////////
		//     1  2  3
		//     0-----4
		//	   7  6  5
		if (g_player.animationID != PLAYER_ATTACK1 && g_player.animationID != PLAYER_ROLL
			&& g_player.down_dir
			&& 	g_player.AckedJudge 
			&& g_player.animationTime[PLAYER_IDLE] <= 0
			&& g_player.DieJudge == false
			&& g_player.AiMode == false
			&& *NowStruct != STRUCT_TYPE_MENU
			)
		{
			if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
			{
				if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
				{
					dir.x = 1;
					g_player.move.x -= sinf(cam->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
					g_player.move.z -= cosf(cam->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

					g_player.rotDest.y = cam->rot.y + D3DX_PI * 0.75f;
				}
				else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
				{
					dir.x = 7;
					g_player.move.x -= sinf(cam->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
					g_player.move.z -= cosf(cam->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

					g_player.rotDest.y = cam->rot.y + D3DX_PI * 0.25f;

				}
				else
				{
					dir.x = 0;
					g_player.move.x -= sinf(cam->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
					g_player.move.z -= cosf(cam->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

					g_player.rotDest.y = cam->rot.y + D3DX_PI * 0.50f;
				}
				if (g_player.animationID != PLAYER_ATTACK1)
				{
					ChangeAnimation(g_pAnimController, PLAYER_RUN);
				}
			}
			else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
			{
				if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
				{
					dir.x = 3;
					g_player.move.x -= sinf(cam->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
					g_player.move.z -= cosf(cam->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

					g_player.rotDest.y = cam->rot.y - D3DX_PI * 0.75f;
				}
				else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
				{
					dir.x = 5;
					g_player.move.x -= sinf(cam->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
					g_player.move.z -= cosf(cam->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

					g_player.rotDest.y = cam->rot.y - D3DX_PI * 0.25f;
				}
				else
				{
					dir.x = 4;
					g_player.move.x -= sinf(cam->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
					g_player.move.z -= cosf(cam->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

					g_player.rotDest.y = cam->rot.y - D3DX_PI * 0.50f;
				}
				if (g_player.animationID != PLAYER_ATTACK1)
					ChangeAnimation(g_pAnimController, PLAYER_RUN);
			}
			else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
			{
				dir.x = 2;
				g_player.move.x -= sinf(D3DX_PI + cam->rot.y) * VALUE_MOVE_PLAYER;
				g_player.move.z -= cosf(D3DX_PI + cam->rot.y) * VALUE_MOVE_PLAYER;

				g_player.rotDest.y = D3DX_PI + cam->rot.y;

				if (g_player.animationID != PLAYER_ATTACK1)
					ChangeAnimation(g_pAnimController, PLAYER_RUN);
			}
			else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
			{
				dir.x = 6;
				g_player.move.x -= sinf(cam->rot.y) * VALUE_MOVE_PLAYER;
				g_player.move.z -= cosf(cam->rot.y) * VALUE_MOVE_PLAYER;

				g_player.rotDest.y = cam->rot.y;
				if (g_player.animationID != PLAYER_ATTACK1)
					ChangeAnimation(g_pAnimController, PLAYER_RUN);
			}

			if (GetKeyboardRelease(DIK_W) || GetKeyboardRelease(DIK_S) || GetKeyboardRelease(DIK_A) || GetKeyboardRelease(DIK_D)
				|| IsButtonRelease(0, BUTTON_UP) || IsButtonRelease(0, BUTTON_DOWN) || IsButtonRelease(0, BUTTON_LEFT) || IsButtonRelease(0, BUTTON_RIGHT)
				)
			{
				if (g_player.animationID != PLAYER_ATTACK1)
					ChangeAnimation(g_pAnimController, PLAYER_RUN);
			}
		}

		if (g_player.animationID == PLAYER_RUN)
		{
			if (g_player.WalkSe <= 0)
			{
				g_player.WalkSe = 30;
				PlaySound(SOUND_LABEL_SE_WALK);
			}
			else
			    g_player.WalkSe--;
		}

		//受伤事件
		if (g_player.AckedJudge == false && g_player.HurtJudge == false
			&& g_player.DieJudge == false)
		{
			g_player.HurtJudge = true;
			PlaySound(SOUND_LABEL_SE_HURT);
			ChangeAnimation(g_pAnimController, PLAYER_IMPACT);
		}

		//普通攻击
		//对应键位 BUTTON_B: Switch Control A
		if (g_player.animationID != PLAYER_ATTACK1
			&& g_player.animationID != PLAYER_IMPACT
			&& g_player.animationID != PLAYER_ROLL
			&& g_player.DieJudge == false
			&& g_player.AiMode == false
			&& g_player.AckedJudge
			&& *NowStruct != STRUCT_TYPE_MENU)
		{
			if (GetKeyboardTrigger(DIK_J) || IsButtonTriggered(0, BUTTON_B))
			{
				ChangeAnimation(g_pAnimController, PLAYER_ATTACK1);
				PlaySound(SOUND_LABEL_SE_ACK);
			}
		}
		//if (GetKeyboardPress(DIK_I))
		//{
		//	ChangeAnimation(g_pAnimController, PLAYER_JUMP);
		//}

		//if (GetKeyboardPress(DIK_O))
		//{
		//	ChangeAnimation(g_pAnimController, PLAYER_PUSH);
		//}

		//if (GetKeyboardPress(DIK_H))
		//{
		//	ChangeAnimation(g_pAnimController, PLAYER_MAGIC);
		//}

		//if (GetKeyboardPress(DIK_L))
		//{
		//	ChangeAnimation(g_pAnimController, PLAYER_DEATH1);
		//}

		//翻滚：对应键位 BUTTON_A: Switch Control B
		if (g_player.animationID != PLAYER_ROLL
		    && g_player.animationID != PLAYER_ATTACK1
			&& g_player.DieJudge == false
			&& g_player.down_dir
			&& g_player.AiMode == false
			&& *NowStruct != STRUCT_TYPE_MENU)
		{
			if (g_player.AckedJudge)
			{
				if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_A))
				{
					PlaySound(SOUND_LABEL_SE_ROLL);
					ChangeAnimation(g_pAnimController, PLAYER_ROLL);
				}
			}
		}

		if (g_player.animationID == PLAYER_ROLL)
		{
			g_player.move.x += sinf(D3DX_PI + g_player.rotDest.y) * VALUE_MOVE_PLAYER * 1.0f;
			g_player.move.z += cosf(D3DX_PI + g_player.rotDest.y) * VALUE_MOVE_PLAYER * 1.0f;
		}

		//if (GetKeyboardPress(DIK_SPACE))
		//{
		//	g_player.pos.z = g_player.pos.x = g_player.pos.y = 60.0f;
		//}

		//if (GetKeyboardPress(DIK_J))
		//{
		//	ChangeAnimation(g_pAnimController, PLAYER_WIN);
		//}

		//Test Jump 
		//if (bHitEventJ == false && GetKeyboardPress(DIK_X))
		//{
		//	bHitEventJ = true;
		//	JumpMove = 30.0f;
		//	//ChangeAnimation(g_pAnimController, PLAYER_JUMP);
		//}

		//jump
		if (bHitEventJ)
		{
			if (JumpMove > 0)
			{
				g_player.pos.y += 5.0f;
				JumpMove -= 2.0f;
			}
			else
			{
				g_player.down_dir = false;
				bHitEventG = false;
			}
		}

		//重力////
		if (g_player.down_dir)
		{
			if (bHitEventG == false)
			{
				bHitEventG = true;
			}
		}
		if (bHitEventG == false)
		{
			if(g_player.animationID != PLAYER_ROLL)
			    g_player.pos.y -= 2.0f;
			else
				g_player.pos.y -= 0.9f;
		}

		if (g_player.face_dir == dir.x && g_player.ColliderHit == true)
		{
			g_player.pos = g_player.old_pos;
			g_player.move = D3DXVECTOR3{ 0.0f, 0.0f , 0.0f };
			bHitEvent = true; //触发了碰撞事件
		}

		if (g_player.face_dir != dir.x)
		{
			bHitEvent = false;
		}
		if (bHitEvent)
			g_player.move = D3DXVECTOR3{ 0.0f, 0.0f , 0.0f };

		///////////Title Cam Event///////////
		int *Nowstruct = RetNowStruct();
		if ( *Nowstruct == STRUCT_TYPE_MENU)
		{
			cam->at.x = g_player.pos.x;
			cam->at.y = g_player.pos.y + 50.0f;
			cam->at.z = g_player.pos.z;
			cam->pos.x = g_player.pos.x - 50.0f;
			//cam->pos.x = 760.0f;
			cam->pos.y = g_player.pos.y;
			cam->pos.z = g_player.pos.z - 50.0f;
			//cam->pos.z = 336.0f;
			ChangeAnimation(g_pAnimController, PLAYER_SIT);
		}
		else
		{
			if (bCameraPosChg)
			{
				cam->at.x = g_player.pos.x;
				cam->at.y = 0.0f;
				cam->at.z = g_player.pos.z;
				cam->pos.x = cam->at.x - sinf(cam->rot.y) * cam->len;
				cam->pos.y = 430.0f;
				cam->pos.z = cam->at.z - cosf(cam->rot.y) * cam->len;
				bCameraPosChg = false;
			}
		}
		////////////////////

		if (g_player.ColliderHit == false)
		{
			if (g_player.old_pos != g_player.pos)
			{
	            g_player.old_pos = g_player.pos;
			}
		}


		float fDiffRotY = g_player.rotDest.y - g_player.rot.y;
		if (fDiffRotY > D3DX_PI)
		{
			fDiffRotY -= D3DX_PI * 2.0f;
		}
		if (fDiffRotY < -D3DX_PI)
		{
			fDiffRotY += D3DX_PI * 2.0f;
		}

		// 栚揑偺妏搙傑偱姷惈傪偐偗傞
		g_player.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
		if (g_player.rot.y > D3DX_PI)
		{
			g_player.rot.y -= D3DX_PI * 2.0f;
		}
		if (g_player.rot.y < -D3DX_PI)
		{
			g_player.rot.y += D3DX_PI * 2.0f;
		}

		/// 埵抲堏摦
		g_player.pos.x += g_player.move.x;
		g_player.pos.y += g_player.move.y;
		g_player.pos.z += g_player.move.z;

		// 堏摦検偵姷惈傪偐偗傞
		//g_player.move.x += (0.0f - g_player.move.x) * RATE_MOVE_PLAYER;
		//g_player.move.y += (0.0f - g_player.move.y) * RATE_MOVE_PLAYER;
		//g_player.move.z += (0.0f - g_player.move.z) * RATE_MOVE_PLAYER;
		g_player.move.x = 0.0f;
		g_player.move.y = 0.0f;
		g_player.move.z = 0.0f;

		{
			//ChangeAnimation(g_pAnimController, PLAYER_RUN);
			//(PLAYER_ATTACK)

			//g_player.rot.y = roty;
			//g_player.pos.x -= sinf(g_player.rot.y) * g_player.spd;
			//g_player.pos.z -= cosf(g_player.rot.y) * g_player.spd;
			//
			//// 僇儊儔偺拲帇揰偲帇揰傪庡恖岞偵捛廬偝偣傞
			cam->at.x = g_player.pos.x;
			cam->at.z = g_player.pos.z;
			cam->pos.x = cam->at.x - sinf(cam->rot.y) * cam->len;
			cam->pos.z = cam->at.z - cosf(cam->rot.y) * cam->len;

		}

		//g_player.spd *= 0.0f;

		// 塭傕僾儗僀儎乕偺埵抲偵崌傢偣傞
		//PrintDebugProc("player pos x:%f, y: %f, z: %f\n", g_player.pos.x, g_player.pos.y, g_player.pos.z);
		//PrintDebugProc("player cam x:%f, y: %f, z: %f\n", cam->pos.x, g_player.pos.y, cam->pos.z);
		g_player.face_dir = dir.x;
		if (g_player.pos != g_player.old_pos)
		{
			g_player.down_dir = false;
			bHitEventG = false;
		}

		g_player.ColliderHit = false;

		SetPositionCylinderCollider(g_player.CylinderColliderIdx,
			D3DXVECTOR3{ g_player.pos.x,g_player.pos.y + 20, g_player.pos.z },
			D3DXVECTOR3{ D3DX_PI / 2 ,0,0 }, D3DXVECTOR3{ 1.0f ,1.0f,1.0f });
		///////////////////////acked ////////////////////////
		//被攻击恢复事件
		if (g_player.AckedJudge == false)
		{
			Time_Start(timeIndexPlayerAcked[0]);
			if (TimingPlayerAcked[0] > PLAYER1_ACKED_TIME)
			{
				Time_Reset(timeIndexPlayerAcked[0]);
				g_player.AckedJudge = true;
				//ChangeAnimation(g_pAnimController, PLAYER_IDLE);
				g_player.HurtJudge = false;
			}
		}
		else
			Time_Stop(timeIndexPlayerAcked[0]);
		///////////////////////ack animation////////////////
		if (g_player.animationID != PLAYER_ATTACK1)
		{
			SetPositionSphereCollider(g_player.SphereColliderAckIdx,
				D3DXVECTOR3{ g_player.pos.x + sinf(g_player.rot.y - D3DX_PI / 2) * 45, g_player.pos.y + 20.0f, g_player.pos.z + cosf(g_player.rot.y - D3DX_PI / 2) * 45 },
				D3DXVECTOR3{ g_player.rot.x,g_player.rot.y,g_player.rot.z }, D3DXVECTOR3{ 2.5f ,2.5f,2.5f });
			StartAckRot = g_player.rot.y - D3DX_PI / 2;
			EndAckRot = g_player.rot.y - D3DX_PI - D3DX_PI /3;
			AckCurrentTime = 0.0f;
			//重置攻击触发器
			ResetEnemyWolfAckJudge();
			ResetEnemyCannibalAckJudge();
			ResetEnemyBossWolfAckJudge();
		}
		else
		{
			//0.03f
			AckCurrentTime += PLAYER1_ATTACK_SPEED_TIME;
			float Ackpos = F_Lerp(StartAckRot, EndAckRot, AckCurrentTime);
			SetPositionSphereCollider(g_player.SphereColliderAckIdx,
				D3DXVECTOR3{ g_player.pos.x + sinf(Ackpos) * 45, g_player.pos.y + 20.0f, g_player.pos.z + cosf(Ackpos) * 45 },
				D3DXVECTOR3{ g_player.rot.x,g_player.rot.y,g_player.rot.z }, D3DXVECTOR3{ 2.5f ,2.5f,2.5f });
		}
		////////////////////////////////////////////////////
			// 合成中か否かを判定
		fCurWeightTime += 0.02f;
		DWORD dwNewTrack = (g_player.currentTrack == 0 ? 1 : 0);
		float aWeight;

		if (g_player.animationID == PLAYER_ROLL)
		{
			aWeight = 0.5f;
		}
		else if (g_player.animationID == PLAYER_RUN)
		{
			aWeight = 0.5f;
		}
		else
		{
			aWeight = 0.5f;
		}

		if (fCurWeightTime <= aWeight)
		{
			// 合成中。ウェイトを算出
			FLOAT Weight = fCurWeightTime / aWeight;
			// ウェイトを登録
			g_pAnimController->SetTrackWeight(g_player.currentTrack, Weight);       // 現在のアニメーション
			g_pAnimController->SetTrackWeight(dwNewTrack, 1 - Weight);   // 前のアニメーション
		}
		else
		{
			// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
			g_pAnimController->SetTrackWeight(g_player.currentTrack, 1.0f);       // 現在のアニメーション
			g_pAnimController->SetTrackEnable(dwNewTrack, false);      // 前のアニメーションを無効にする
		}

		// 時間を更新

		g_pAnimController->AdvanceTime(0.025f, NULL);



		/////////////除去Idle 状态只维持一次/////////////
		D3DXTRACK_DESC td1;
		g_pAnimController->GetTrackDesc(g_player.currentTrack, &td1);

		if (td1.Position > 1.0f)
		{
			//ChangeAnimation(g_pAnimController, PLAYER_IDLE);
		}

		if (g_player.animationTime[g_player.animationID] <= 0)
		{
			ChangeAnimation(g_pAnimController, PLAYER_IDLE);
		}

		///////////////////////
		SetPositionShadow(g_player.ShadowIdx, g_player.pos, g_player.rot,
			D3DXVECTOR3{ g_player.scl.x,
			g_player.scl.y / 3,
			g_player.scl.z });

		//PrintDebugProc("player hp = %d, soul = %d\n", g_player.hp, g_player.SoulNum);

	}
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawPlayer1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScl;
	D3DXMATERIAL *pD3DXMat;
	if (g_player.use)
	{
		// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
		D3DXMatrixIdentity(&g_mtxWorldPlayer1);

		D3DXMatrixScaling(&mtxScl, g_player.scl.x, g_player.scl.y, g_player.scl.z);
		D3DXMatrixMultiply(&g_mtxWorldPlayer1, &g_mtxWorldPlayer1, &mtxScl);

		// 夞揮傪斀塮
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_mtxWorldPlayer1, &g_mtxWorldPlayer1, &mtxRot);

		// 堏摦傪斀塮
		D3DXMatrixTranslation(&mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldPlayer1, &g_mtxWorldPlayer1, &mtxTranslate);


		// 儚乕儖僪儅僩儕僢僋僗偺愝掕
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer1);

		UpdateFrameMatrices(g_pFrameRoot, &g_mtxWorldPlayer1);
		////////////////////////////////
		//设置玩家透明度
		TransformConstantTable->SetFloat(pDevice, Player1AlphaHandle, 1.0f);

		pDevice->SetRenderState(D3DRS_ZENABLE, true);
		DrawFrame(pDevice, g_pFrameRoot, 0);
		pDevice->SetRenderState(D3DRS_ZENABLE, false);

		//pDevice->SetPixelShader(NULL);
		//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

//=============================================================================
// 僾儗僀儎乕傪庢摼
//=============================================================================
PLAYER1 *GetPlayer1(void)
{
	return &g_player;
}

//=============================================================================
// 埵抲庢摼
//=============================================================================
D3DXVECTOR3 GetPositionPlayer1(void)
{
	return g_player.pos;
}

//=============================================================================
// 岦偒庢摼
//=============================================================================
D3DXVECTOR3 GetRotationPlayer1(void)
{
	return g_player.rot;
}

//=============================================================================
// 栚揑偺岦偒庢摼
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer1(void)
{
	return g_player.rotDest;
}

//=============================================================================
// 堏摦検庢摼
//=============================================================================
D3DXVECTOR3 GetMovePlayer1(void)
{
	return g_player.move;
}
//=============================================================================
// 傾僯儊乕僔儑儞帪娫傪惂掕
//=============================================================================

void  SetAnimationTime(int name, float &animationTime)
{
	float time;
	switch (name)
	{

	case PLAYER_IDLE:
		time = 6;//60
		break;
	case PLAYER_RUN:
		time = 6;//18
		break;
	case PLAYER_ATTACK1:
		//time = 36;
		time = 28;
		break;
	case PLAYER_ATTACK2:
		time = 89;
		break;
	case PLAYER_MAGIC:
		time = 31;
		break;
	case PLAYER_ROLL:
		time = 19;
		break;
	case PLAYER_DEATH1:
		time = 117;
		break;
	case PLAYER_DEATH2:
		time = 66;
		break;
	case PLAYER_KICK:
		time = 36;//18
		break;
	case PLAYER_IMPACT:
		time = 10;
		break;
	case PLAYER_SIT:
		time = 326;
		break;
	case PLAYER_WIN:
		time = 135;
		break;
	}

	animationTime = time / 30;

	return;
}
//=============================================================================
// 傾僯儊乕僔儑儞傪愗傝懼偊
//=============================================================================

void ChangeAnimation(LPD3DXANIMATIONCONTROLLER Controller, int animationID)
{

	SetAnimationTime(animationID, g_player.animationTime[animationID]);

	if (g_player.animationID != animationID)
	{
		g_player.animationTime[g_player.animationID] = 0;


		DWORD dwNewTrack = (g_player.currentTrack == 0 ? 1 : 0);

		LPD3DXANIMATIONSET g_pAnimationSet = NULL;

		Controller->GetAnimationSetByName(StateName[animationID], &g_pAnimationSet);
		Controller->SetTrackAnimationSet(dwNewTrack, g_pAnimationSet);
		g_pAnimationSet->Release();

		Controller->UnkeyAllTrackEvents(g_player.currentTrack);
		Controller->UnkeyAllTrackEvents(dwNewTrack);

		D3DXTRACK_DESC td;
		Controller->GetTrackDesc(g_player.currentTrack, &td);

		td.Position = 0.0f;

		Controller->SetTrackDesc(dwNewTrack, &td);
		g_player.animationID = animationID;

		//Controller->KeyTrackEnable(g_player.currentTrack, FALSE, g_player.curTime + 0.25f);
		//Controller->KeyTrackSpeed(g_player.currentTrack, 0.0f, g_player.curTime, 0.25f, D3DXTRANSITION_LINEAR);
		//Controller->KeyTrackWeight(g_player.currentTrack, 0.0f, g_player.curTime, 0.25f, D3DXTRANSITION_LINEAR);

		Controller->SetTrackEnable(dwNewTrack, TRUE);

		Controller->SetTrackEnable(g_player.currentTrack, TRUE);
		//Controller->KeyTrackSpeed(dwNewTrack, 1.0f, g_player.curTime, 0.25f, D3DXTRANSITION_LINEAR);
		//Controller->KeyTrackWeight(dwNewTrack, 1.0f, g_player.curTime, 0.25f, D3DXTRANSITION_LINEAR);


		g_player.currentTrack = dwNewTrack;

		Controller->SetTrackAnimationSet(g_player.currentTrack, g_pAnimationSet);

		g_player.curTime = 0.0f;

		fCurWeightTime = 0.0f;

	}
}

//void ChangeAnimation(LPD3DXANIMATIONCONTROLLER Controller, int animationID)
//{
//
//	SetAnimationTime(animationID, g_player.animationTime[animationID]);
//
//	if (g_player.animationID != animationID)
//	{
//		//g_player.animationTime[g_player.animationID] = 0;
//
//
//		// 現在のアニメーションセットの設定値を取得
//		D3DXTRACK_DESC TD;   // トラックの能力
//		g_pAnimController->GetTrackDesc(0, &TD);
//		//TD.Position = 0.1f;
//		// 今のアニメーションをトラック1に移行し
//		// トラックの設定値も移行
//
//		LPD3DXANIMATIONSET g_pAnimationSet = NULL;
//		// 新しいアニメーションセットをトラック0に設定
//		Controller->GetAnimationSetByName(StateName[animationID], &g_pAnimationSet);
//		g_pAnimController->SetTrackAnimationSet(0, g_pAnimationSet);
//		//FLOAT DefTime = g_pAnimationSet->GetPeriod();
//		// トラック0のスfピードの設定
//		g_pAnimController->SetTrackSpeed(0, 1.0f);
//
//		g_pAnimationSet->Release();
//		Controller->GetAnimationSetByName(StateName[g_player.animationID], &g_pAnimationSet);
//		g_pAnimController->SetTrackAnimationSet(1, g_pAnimationSet);
//		g_pAnimController->SetTrackDesc(1, &TD);
//
//
//		// トラックの合成を許可
//		g_pAnimController->SetTrackEnable(0, true);
//		g_pAnimController->SetTrackEnable(1, true);
//
//		fCurWeightTime = 0.0f;
//		g_player.animationID = animationID;
//	}
//	// ウェイト時間を初期化
//	//m_Anim[animID].fCurWeightTime = 1.0f;
//
//	//if (g_player.animationID != animationID)
//	//{
//
//	//	//DWORD dwNewTrack = (g_player.currentTrack == 0 ? 1 : 0);
//
//	//	LPD3DXANIMATIONSET g_pAnimationSet = NULL;
//
//	//	Controller->GetAnimationSetByName(StateName[animationID], &g_pAnimationSet);
//	//	Controller->SetTrackAnimationSet(g_player.track[animationID], g_pAnimationSet);
//	//	g_pAnimationSet->Release();
//
//	//	Controller->UnkeyAllTrackEvents(g_player.track[animationID]);
//	//	//Controller->UnkeyAllTrackEvents(g_player.currentTrack);
//
//	//	D3DXTRACK_DESC td;
//	//	//Controller->GetTrackDesc(g_player.currentTrack, &td);
//	//	
//	//	td.Position = 0.0f;
//	//	td.Speed = 1.0f;
//	//	td.Weight = 1.0f;
//
//	//	Controller->SetTrackDesc(g_player.track[animationID], &td);
//
//	//	//Controller->KeyTrackEnable(g_player.currentTrack, TRUE, g_player.curTime + g_player.animationTime[g_player.animationID]);
//	//	//Controller->KeyTrackSpeed(g_player.currentTrack, 1.0f, g_player.curTime, g_player.animationTime[g_player.animationID], D3DXTRANSITION_LINEAR);
//	//	//Controller->KeyTrackWeight(g_player.currentTrack, 1.0f, g_player.curTime, g_player.animationTime[g_player.animationID], D3DXTRANSITION_LINEAR);
//
//
//	//	Controller->SetTrackEnable(g_player.track[animationID], enable);
//	//	Controller->KeyTrackSpeed(g_player.track[animationID], speed, g_player.curTime, g_player.animationTime[animationID], D3DXTRANSITION_LINEAR);
//	//	Controller->KeyTrackWeight(g_player.track[animationID], weight, g_player.curTime, g_player.animationTime[animationID], D3DXTRANSITION_LINEAR);
//
//
//	//	g_player.currentTrack = g_player.track[animationID];
//
//	//	Controller->SetTrackAnimationSet(g_player.track[animationID], g_pAnimationSet);
//
//	//	//g_player.curTime = 0.0f;
//
//	//	Controller->ResetTime();
//	//}
//}

/////////////////////////////////像素着色器////////////////////////////
bool LoadPixHLSLPlayer1()
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



void SetbHitEventJ()
{
	if (bHitEventJ)
	{
		bHitEventJ = false;
		g_player.pos = g_player.old_pos;
	}
}

void BackUpPlayer1(void)
{
	g_PlayerBak = g_player;
}


//重新加载Player1
HRESULT RestartPlayer1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_player = g_PlayerBak;

	g_player.CylinderColliderIdx = CreateCylinderCollider(g_player.pos, g_player.rot, g_player.scl);
	return S_OK;
}



void Player1AnimationIdle()
{
	ChangeAnimation(g_pAnimController, PLAYER_IDLE);
}


void DrawPlayer1Shadow(D3DXMATRIX mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_player.use)
	{
		UpdateFrameMatrices(g_pFrameRoot, &mtxWorld);   //更新框架中的变换矩阵
		DrawFrame(pDevice, g_pFrameRoot, 1);
	}
}


//Player激活重力
void Player_G_Use()
{
	g_player.down_dir = false;
	bHitEventG = false;
}


static float PlayerAiMoveTime = 0.0f;
static bool bPlayerAiMoveFirst = true; //是否首次调用
D3DXVECTOR3 PlayerAiStart;
D3DXVECTOR3 PlayerAiEnd;
bool PlayerAiMoveEvent( D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	//VALUE_MOVE_PLAYER;
	if (bPlayerAiMoveFirst)
	{
		PlayerAiStart = StartPos;
		PlayerAiEnd = EndPos;
		bPlayerAiMoveFirst = false;
	}
	float distance = D3DXVec3Length(&(PlayerAiEnd - PlayerAiStart));
	float t = 1.0f / ((distance) / VALUE_MOVE_PLAYER);
	PlayerAiMoveTime += t;
	D3DXVECTOR3 Move = Lerp(PlayerAiStart, PlayerAiEnd, PlayerAiMoveTime);
	ChangeAnimation(g_pAnimController, PLAYER_RUN);
	g_player.pos = Move;
	float MoveRot = RetRotValue(D3DXVECTOR2{ PlayerAiStart.x, PlayerAiStart.z }, D3DXVECTOR2{ PlayerAiEnd.x, PlayerAiEnd.z });
	g_player.rot.y = D3DX_PI + D3DX_PI / 2 - MoveRot;
	if (g_player.pos == PlayerAiEnd)
	{
		PlayerAiMoveTime = 0.0f;
		bPlayerAiMoveFirst = true;
		return true;
	}
	return false;
}


void ResetPlayer1()
{
	static int HurtEventIndexTime = 0;
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.use = true;
	g_player.fRadius = PLAYER_RADIUS;
	ChangeAnimation(g_pAnimController, PLAYER_IDLE);
	g_player.AckJudge = true;
	g_player.AckedJudge = true;
	g_player.HurtJudge = false;
	g_player.DieJudge = false;
	g_player.AiMode = false;
	g_player.WalkSe = 0;
	g_player.DieSe = true;
	D3DXTRACK_DESC td;
	td.Position = 0.0f;
	td.Speed = 1.0f;
	td.Weight = 1.0f;
	td.Enable = true;
	g_pAnimController->SetTrackDesc(g_player.currentTrack, &td);
}


void Player1WinAnimation()
{
	ChangeAnimation(g_pAnimController, PLAYER_WIN);
}
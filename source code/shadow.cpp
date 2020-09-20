//=============================================================================
//
// 影処理[shadow.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "shadow.h"
#include "player1.h"
#include "cube_Ex.h"
#include "light_Ex.h"
#include "GlobalFunction.h"
#include "GameEvent.h"
#include "Scene.h"
#include "Enemy_wolf.h"
#include "EnemyCannibal.h"
#include "EnemyBossWolf.h"
#include "debugproc.h"

static SHADOW				g_Shadow[MAX_SHADOW];

static D3DXVECTOR3 HomeNoShadowList[MAX_SHADOW];  //需要剔除影子的数组
static int HomeNoShadowListIndex = 0;

HRESULT InitShadow()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].use = false;
		g_Shadow[nCntShadow].active = false;
		//g_Shadow[nCntShadow].pos = D3DXVECTOR3{0,0,0};
	}

	return S_OK;
}

//bool bStatic 是否静态 静态不需要放入Update中
int CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 scl,bool bStatic)
{
	int nIdxShadow = -1;

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_Shadow[nCntShadow].use)
		{
			g_Shadow[nCntShadow].pos = pos;
			g_Shadow[nCntShadow].rot = rot;
			g_Shadow[nCntShadow].scl = scl;
			g_Shadow[nCntShadow].use = true;
			if(bStatic)
				g_Shadow[nCntShadow].active = true;
			else
			    g_Shadow[nCntShadow].active = false;

			nIdxShadow = nCntShadow;
			break;
		}
	}
	return nIdxShadow;
}

void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 scl)
{
	if (nIdxShadow < 0) return;	

    g_Shadow[nIdxShadow].active = true;
    g_Shadow[nIdxShadow].pos = pos;
    g_Shadow[nIdxShadow].rot = rot;
    g_Shadow[nIdxShadow].scl = scl;
}


void DrawShadow(int ShadowIndex, int type, int objIndex)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX t, mtxScl;
	OBJ_CUBE_EX *ObjCube = RetObjCube_Ex();
	D3DVECTOR *Light = RetLightDirection();
	int *SceneBoundHomeList = RetSceneBoundHomeList();
	int SceneBoundHomeIndex = RetSceneBoundHomeIndex();
	int *SceneBoundGrassList = RetSceneBoundGrassList();
	int SceneBoundGrassIndex = RetSceneBoundGrassIndex();
	int CurrentScene = RetCurrentSceneIndex();
    
	/////////////剔除周边没有地面的阴影 临时解决///////////////
	if (type == OBJ_EN_TERRAIN_TYPE)
	{
		if (CurrentScene == SCENE_SCENE_HOME)
		{
			if (g_Shadow[ShadowIndex].pos.x < -654.0f || g_Shadow[ShadowIndex].pos.z < 0.0f)
				return;
			//for (int i = 0; i < SceneBoundHomeIndex; i++)
			//{
			//	SceneBoundHomeList[i];
			//	if (objIndex == SceneBoundHomeList[i])
			//		return;
			//}
		}
		else if (CurrentScene == SCENE_SCENE_GRASS)
		{
			for (int i = 0; i < SceneBoundGrassIndex; i++)
			{
				if (objIndex == SceneBoundGrassList[i])
					return;
			}
		}
	}
	///////////////////////
	// position shadow
	//D3DXVECTOR4 lightDirection(0.707f, -0.707f, 0.707f, 0.0f);
	D3DXVECTOR4 lightDirection(-0.707f, -0.707f, -0.707f, 0.0f);
	//D3DXVECTOR4 lightDirection(Light->x, Light->y, Light->z, 0.0f );
	D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 21.0f);

	D3DXMATRIX W, S;
	D3DXMatrixShadow(
		&S,
		&lightDirection,
		&groundPlane);

	D3DXMATRIX T;
	D3DXMatrixTranslation(
		&T,
		g_Shadow[ShadowIndex].pos.x,
		g_Shadow[ShadowIndex].pos.y,
		g_Shadow[ShadowIndex].pos.z);
	
	D3DXMATRIX R;
	D3DXMatrixRotationYawPitchRoll(&R,
		g_Shadow[ShadowIndex].rot.y,
		g_Shadow[ShadowIndex].rot.x,
		g_Shadow[ShadowIndex].rot.z);

	D3DXMATRIX Y;
	D3DXMatrixScaling(&Y,
		g_Shadow[ShadowIndex].scl.x,
		g_Shadow[ShadowIndex].scl.y,
		g_Shadow[ShadowIndex].scl.z);


	W = Y * R * T * S;

	pDevice->SetTransform(D3DTS_WORLD, &W);

	pDevice->SetTexture(0, 0);
	if (type == OBJ_CUBE_TYPE)
		DrawObjCubeShadow(objIndex);
	else if (type == OBJ_EN_PLAYER_TYPE)
	{
		DrawPlayer1Shadow(W);
	}
	else if (type == OBJ_EN_ENEMY_WOLF_TYPE)
	{
		//PrintDebugProc("Enemy Index = %d, %f,%f", objIndex, W._41,W._43);
		DrawEnemyWolfShadow(objIndex, W);
	}
	else if (type == OBJ_EN_ENEMY_CANNIBAL_TYPE)
	{
		//PrintDebugProc("Enemy Index = %d, %f,%f", objIndex, W._41,W._43);
		DrawEnemyCannibalShadow(objIndex, W);
	}
	else if (type == OBJ_EN_ENEMY_BOSS_WOLF_TYPE)
	{
		//PrintDebugProc("Enemy Index = %d, %f,%f", objIndex, W._41,W._43);
		DrawEnemyBossWolfShadow(objIndex, W);
	}
}

SHADOW *RetShadow()
{
	return &g_Shadow[0];
}



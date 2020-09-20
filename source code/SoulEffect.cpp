//=============================================================================
//
// 魂処理 [SoulEffect.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "SoulEffect.h"
#include "GlobalFunction.h"
#include "EnemyCannibal.h"
#include "Enemy_wolf.h"
#include "EnemyBossWolf.h"
#include "effect.h"
#include "GameMath.h"
#include "player1.h"
#include <time.h>

#define SOULTIME_DELAYTIME (0.01f)
#define ABSORB_TIME_DELAYTIME (0.02f)
#define SOUL_MAX_HEIGHT (100.0f)
#define SOUL_MAX_NUM (4096)
#define SOUL_RANGE (100.0f)
#define SOUL_PLAYER_NUM (5)              //每个魂给与玩家时候的数量 

static float ParabolaIndicator(int EffectIndex, D3DXVECTOR3 pos, D3DXVECTOR3 move, float SoulTime);

SOUL g_Soul[SOUL_MAX_NUM];


static int g_SoulIndex = 0; //全局Soul索引
void InitSoul()
{
	for (int i = 0; i < SOUL_MAX_NUM; i++)
	{
		g_Soul[i].soul_time = 0.0f;
		g_Soul[i].absorb_time = 0.0f;
		g_Soul[i].FallJudge = false;
		g_Soul[i].AbsorbJudge = false;
		g_Soul[i].AbsorbIngJudge = false;
	}
}


void SetSoul( int index, int ENEMY_TYPE, int SoulNum )
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	srand((unsigned)time(NULL));
	for (int i = 0; i < SoulNum; i++)
	{
		//X坐标十个方向
		int value_rot_x = rand() % 10;
		int value_d_x = rand() % 2;
		float f_value_rot_x = (float)value_rot_x / 10.0f;
		if (value_d_x == 1)
			f_value_rot_x = -f_value_rot_x;

		//Z坐标十个方向
		int value_rot_z = rand() % 10;
		int value_d_z = rand() % 2;
		float f_value_rot_z = (float)value_rot_z / 10.0f;
		if (value_d_z == 1)
			f_value_rot_z = -f_value_rot_z;

		int EffectIndex = -1;
		switch (ENEMY_TYPE)
		{
		case ENEMY_TYPE_WOLF:
			g_Soul[g_SoulIndex].start_pos = EnemyWolf[index].pos;
			//g_Soul[g_SoulIndex].move = D3DXVECTOR3(-sinf(EnemyWolf[index].rot.y), 0.0f, -cosf(EnemyWolf[index].rot.y));
			g_Soul[g_SoulIndex].move = D3DXVECTOR3(f_value_rot_x, 0.0f, f_value_rot_z);
			EffectIndex = SetEffect(
				EnemyWolf[index].pos,
				D3DXVECTOR3(
					0.0f,
					0.0f,
					0.0f),
				D3DXCOLOR(255, 0, 0, 0.8),
				16, 16, -1);
			break;
		case ENEMY_TYPE_CANNIBAL:
			g_Soul[g_SoulIndex].start_pos = EnemyCannibal[index].pos;
			//g_Soul[g_SoulIndex].move = D3DXVECTOR3(-sinf(EnemyCannibal[index].rot.y), 0.0f, -cosf(EnemyCannibal[index].rot.y));
			g_Soul[g_SoulIndex].move = D3DXVECTOR3(f_value_rot_x, 0.0f, f_value_rot_z);
			EffectIndex = SetEffect(
				EnemyCannibal[index].pos,
				D3DXVECTOR3(
					0.0f,
					0.0f,
					0.0f),
				D3DXCOLOR(255, 0, 0, 0.8),
				16, 16, 1000);
			break;
		case ENEMY_TYPE_BOSSWOLF:
			g_Soul[g_SoulIndex].start_pos = EnemyBossWolf[index].pos;
			//g_Soul[g_SoulIndex].move = D3DXVECTOR3(-sinf(EnemyCannibal[index].rot.y), 0.0f, -cosf(EnemyCannibal[index].rot.y));
			g_Soul[g_SoulIndex].move = D3DXVECTOR3(f_value_rot_x, 0.0f, f_value_rot_z);
			EffectIndex = SetEffect(
				EnemyBossWolf[index].pos,
				D3DXVECTOR3(
					0.0f,
					0.0f,
					0.0f),
				D3DXCOLOR(255, 0, 0, 0.8),
				16, 16, 1000);
			break;
	    }
		g_Soul[g_SoulIndex].EffectIndex = EffectIndex;
		g_Soul[g_SoulIndex].FallJudge = true;
		g_SoulIndex++;
	}
}


void UpdateSoul()
{
	PLAYER1 *Player = GetPlayer1();
	EFFECT *Effect = RetEffect();

	for (int i = 0; i < SOUL_MAX_NUM; i++)
	{
		if (g_Soul[i].FallJudge)
		{
			g_Soul[i].soul_time = g_Soul[i].soul_time + SOULTIME_DELAYTIME;
			float time = ParabolaIndicator(g_Soul[i].EffectIndex, g_Soul[i].start_pos, g_Soul[i].move, g_Soul[i].soul_time);
			if (time >= 1.0f)
			{
				g_Soul[i].pos = Effect[g_Soul[i].EffectIndex].pos;
				g_Soul[i].FallJudge = false;
				g_Soul[i].AbsorbJudge = true;
			}
		}
		if (g_Soul[i].AbsorbJudge)
		{

			D3DXVECTOR3 vDistance = g_Soul[i].pos - Player->pos;
			float length = D3DXVec3Length(&vDistance);
			if (length < 200)
			{
				g_Soul[i].AbsorbJudge = false;
				g_Soul[i].AbsorbIngJudge = true;
			}
		}
		if (g_Soul[i].AbsorbIngJudge)
		{
			g_Soul[i].absorb_time += ABSORB_TIME_DELAYTIME;
			D3DXVECTOR3 value = Lerp(g_Soul[i].pos, Player->pos, g_Soul[i].absorb_time);
			g_Soul[i].AbsorbJudge = false;
			Effect[g_Soul[i].EffectIndex].pos = value;
			if (g_Soul[i].absorb_time >= 1.0f)
			{
				g_Soul[i].absorb_time = 0.0f;
				Effect[g_Soul[i].EffectIndex].bUse = false;
				Player->SoulNum += SOUL_PLAYER_NUM;
				g_Soul[i].AbsorbIngJudge = false;
			}
		}
	}
}

//抛物线
float ParabolaIndicator(int EffectIndex, D3DXVECTOR3 pos, D3DXVECTOR3 move, float SoulTime)
{
	EFFECT *Effect = RetEffect();

	D3DXVECTOR3 throw_start_pos = pos;
	D3DXVECTOR3 throw_to_pos =
		D3DXVECTOR3(throw_start_pos.x + move.x * SOUL_RANGE,
			throw_start_pos.y,
			throw_start_pos.z + move.z * SOUL_MAX_HEIGHT);

	D3DXVECTOR3 value = Lerp(throw_start_pos, throw_to_pos, SoulTime); // x z 轴上的线性位移
	value.y = F_Lerp(throw_start_pos.y, SOUL_MAX_HEIGHT, sinf(SoulTime * D3DX_PI));
	Effect[EffectIndex].pos = value;
	return SoulTime;
}
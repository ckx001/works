//=============================================================================
//
// 魂処理 [SoulEffect.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

struct SOUL
{
	D3DXVECTOR3 pos; //落地时的位置
	D3DXVECTOR3 move;
	D3DXVECTOR3 start_pos; //开始位置
	int EffectIndex;
	float soul_time;
	bool FallJudge;
	bool AbsorbJudge;
	bool AbsorbIngJudge;
	float absorb_time;
};


void InitSoul(void);
void UpdateSoul(void);
void SetSoul(int index, int ENEMY_TYPE, int SoulNum);
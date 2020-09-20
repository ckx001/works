//=============================================================================
//
// 事件処理 [GameEvent.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

//计时器索引
enum TIME_INDEX
{
	TIMING_PLAYER_RUN,
	TIMING_PLAYER_ACKED, //被攻击恢复时间
	TIMING_ENEMY_WAIT,
	TIMING_ENEMY_CANNIBAL_WAIT,
	TIMING_ENEMY_BOSSWOLF_WAIT,
	TIMING_ENEMY_BOSSWOLF_ACK_CHG_WAIT, //攻击 两拳中间的等待时间
	TIMING_MAX_NUM = 6
};


void InitEnemyEvent(void);
void InitTimeEvent(void);
void UpdateGameEvent(void);
DWORD *RetTiming(int type);
int RetCurrentSceneIndex(void);
void LoadSceneBarrierList(int Scene);
int *RetTimeIndex(int type);
void LoadEnemy(int scene);
void LoadItem(void);
void ReSetScene(int scene);
int *RetSavePoint(void);
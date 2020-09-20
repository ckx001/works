//=============================================================================
//
// エネミーWolf処理[Enemy_wolf.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define TEXTURE_ENEMY_WOLF ("data/MeshTexture/body_01.tga")

#define ENEMY_WOLF_MAX_HP 30
#define ENEMY_WOLF_MAX_NUM 35
//*****************************************************************************
// 峔憿懱掕媊
//*****************************************************************************

enum ENEMY_WOLF_ENUM
{
	ENEMY_WOLF_IDLE,
	ENEMY_WOLF_RUN,
	ENEMY_WOLF_DIE,
	ENEMY_WOLF_ACK,
	ENEMY_WOLF_HURT,
	ENEMY_WOLF_ANIMATION_MAX
};

enum ENEMY_WOLF_STATUS_ENUM
{
	ENEMY_WOLF_STATUS_IDLE,
	ENEMY_WOLF_STATUS_RUN,
	ENEMY_WOLF_STATUS_DIE,
	ENEMY_WOLF_STATUS_ACK,
	ENEMY_WOLF_STATUS_HURT,
	ENEMY_WOLF_STATUS_WAITOVER
};

typedef struct
{
	D3DXVECTOR3 pos;		
	D3DXVECTOR3 old_pos;
	D3DXVECTOR3 last_frame_pos; //记录玩家上一次位置
	D3DXVECTOR3 move;		
	D3DXVECTOR3 rot;		
	D3DXVECTOR3 scl;		
	D3DXVECTOR3 rotDest;	
	float fRadius;			
	int nIdxShadow;			// 塭ID
	UINT               currentTrack;       
	float               fTimeDelta;        
	float               prvTime;           
	float               curTime;           
	bool use;
	int TimeIndex[2];
	bool pos_colider_status[256];
	int               animationID;                 
	int               LastAnimationID;
	float             animationTime[ENEMY_WOLF_ANIMATION_MAX]; 
	float             animationSpeed;   

	int ShadowIdx; //阴影索引
	int SphereColliderIdx;  //体积碰撞器
	int SphereColliderAckIdx; //攻击触发器
	int hp;
	float				spd;
	bool                ColliderHit;
	int                 face_dir;
	bool                down_dir;
	bool                AckJudge; //只触发一次攻击事件 Player->Enemy
	bool                AckPlayerRotJudge; //只触发一次攻击转向事件 Enemy->Player
	bool                DieJudge; //只触发一次死亡事件
	bool                HurtJudge; //进入受伤事件开关
	bool                HurtJudgeStart; //进入受伤事件开始开关
	bool                AckPlayerAckTimeJudge; //只触发一次攻击时机有效（在攻击的什么时机触发攻击)
	int                 status;
	bool                AlertStatus; //是否进入警戒状态
	bool                AckPlayerRotStartJudge; //转向开始
	float               AckPlayerRotStartYValue; //转向开始的绕Y轴的值
	float               AckPlayerRotOverYValue; //转向终点的绕Y轴的值
	int                 AckPlayerRotDirect; //朝什么方向转向终点 0 左 1 右
	bool                PlayerActionOverJudge[ENEMY_WOLF_ANIMATION_MAX]; //动作结束事件
} ENEMY_WOLF;


//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT InitEnemyWolf(void);
void UninitEnemyWolf(void);
void UpdateEnemyWolf(void);
void DrawEnemyWolf(void);
ENEMY_WOLF *GetEnemyWolf(void);
void ResetEnemyWolfAckJudge(void);
void SetbHitEventJ(void);
void BackUpEnemyWolf(void);
HRESULT RestartEnemyWolf(void);
int CreateEnemyWolf(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool AlertStatus);
void DrawEnemyWolfShadow(int EnemyWolfIndex, D3DXMATRIX mtxWorld);

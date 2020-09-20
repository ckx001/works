//=============================================================================
//
// ボス処理[EnemyBossWolf.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define TEXTURE_ENEMY_BOSSWOLF ("data/MeshTexture/body_01.tga")

#define ENEMY_BOSSWOLF_MAX_HP 250
#define ENEMY_BOSSWOLF_MAX_NUM 1

#define ENEMY_BOSSWOLF_JUMPACK_MOVE (4.0f) //跳跃攻击移动量
//*****************************************************************************
// 峔憿懱掕媊
//*****************************************************************************

enum ENEMY_BOSSWOLF_ENUM
{
	ENEMY_BOSSWOLF_IDLE,
	ENEMY_BOSSWOLF_JUMPACK,
	ENEMY_BOSSWOLF_ACK,
	ENEMY_BOSSWOLF_DIE,
	ENEMY_BOSSWOLF_WALK,
	ENEMY_BOSSWOLF_STAMP,
	ENEMY_BOSSWOLF_ANIMATION_MAX
};

enum ENEMY_BOSSWOLF_STATUS_ENUM
{
	ENEMY_BOSSWOLF_STATUS_IDLE,
	ENEMY_BOSSWOLF_STATUS_JUMPACK,
	ENEMY_BOSSWOLF_STATUS_WAIT,
	ENEMY_BOSSWOLF_STATUS_ACK,
	ENEMY_BOSSWOLF_STATUS_DIE,
	ENEMY_BOSSWOLF_STATUS_WALK,
	ENEMY_BOSSWOLF_STATUS_STAMP,
	ENEMY_BOSSWOLF_STATUS_WAITOVER
};

typedef struct
{
	D3DXVECTOR3 pos;		// 尰嵼偺埵抲
	D3DXVECTOR3 start_pos;
	D3DXVECTOR3 last_frame_pos; //记录玩家上一次位置
	D3DXVECTOR3 move;		// 堏摦検
	D3DXVECTOR3 rot;		// 尰嵼偺岦偒
	D3DXVECTOR3 scl;		// 尰嵼偺岦偒
	D3DXVECTOR3 rotDest;	// 栚揑偺岦偒
	float fRadius;			// 敿宎
	int nIdxShadow;			// 塭ID
	UINT               currentTrack;       //尰嵼偺僩儔僢僋
	float               fTimeDelta;        //帪娫僨儖僞
	float               prvTime;            //尰嵼偺帪娫
	float               curTime;            //慜偺帪娫
	bool use;
	bool active;
	int TimeIndex[2];
	bool pos_colider_status[256];
	int               animationID;                  //傾僯儊乕僔儑儞ID
	int               LastAnimationID;
	float             animationTime[ENEMY_BOSSWOLF_ANIMATION_MAX]; //傾僯儊乕僔儑儞僾儗僀帪娫
	float             animationSpeed;               // 傾僯儊乕僔儑儞僗僺乕僪

	int ShadowIdx; //阴影索引
	int CylinderIdx;  //体积碰撞器
	int SphereColliderAckIdx; //攻击触发器
	int SphereColliderJumpAckIdx; //跳跃攻击触发器
	int hp;
	float				spd;
	bool                ColliderHit;
	int                 face_dir;
	bool                down_dir;
	bool                AckJudge; //只触发一次攻击事件 Player->Enemy
	bool                AckPlayerRotJudge; //只触发一次攻击转向事件 Enemy->Player
	bool                AckPlayerAckTimeJudge;
	bool                AckPlayerJudge; //只触发一次攻击事件 Enemy->Player
	bool                DieJudge; //只触发一次死亡事件
	int                 status;
	int                 LastStatus; //上一个状态
	bool                ActionOverJudge[ENEMY_BOSSWOLF_ANIMATION_MAX]; //动作结束判断(一次)
	bool                ActionStartJudge[ENEMY_BOSSWOLF_ANIMATION_MAX]; //动作刚开始判断(一次)
	bool                AckStartChgJudge; //攻击动作开始动作等待的判断
	bool                AckStartChgStartJudge; //攻击动作开始动作等待的开始判断
	bool                AckChgJudge; //攻击动作中间动作等待的判断
	bool                AckChgStartJudge; //攻击动作中间动作等待的开始判断
	int                 AckChgCJudgeCylinder = 0; //攻击动作等待的碰撞器判断
	bool                AckEffective; //ACK动作 对player是否有效(掉血)
	bool                JumpAckEffective; //Jump Ack动作 对player是否有效(掉血)
} ENEMY_BOSSWOLF;


//***************************************************************CANNIBAL**************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT InitEnemyBossWolf(void);
void UninitEnemyBossWolf(void);
void UpdateEnemyBossWolf(void);
void DrawEnemyBossWolf(void);
ENEMY_BOSSWOLF *GetEnemyBossWolf(void);
void ResetEnemyBossWolfAckJudge(void);
void SetbHitEventJ(void);
void BackUpEnemyBossWolf(void);
HRESULT RestartEnemyBossWolf(void);
int CreateEnemyBossWolf(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
void DrawEnemyBossWolfShadow(int EnemyBossWolfIndex, D3DXMATRIX mtxWorld);
void ResetEnemyBossWolf(int num);

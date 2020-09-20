//=============================================================================
//
// エネミーCannibal処理[EnemyCannibal.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define TEXTURE_ENEMY_CANNIBAL ("data/MeshTexture/body_01.tga")

#define ENEMY_CANNIBAL_MAX_HP 30
#define ENEMY_CANNIBAL_MAX_NUM 30
//*****************************************************************************
// 峔憿懱掕媊
//*****************************************************************************

enum ENEMY_CANNIBAL_ENUM
{
	ENEMY_CANNIBAL_IDLE,
	ENEMY_CANNIBAL_DIE,
	ENEMY_CANNIBAL_ACK,
	ENEMY_CANNIBAL_ANIMATION_MAX
};

enum ENEMY_CANNIBAL_STATUS_ENUM
{
	ENEMY_CANNIBAL_STATUS_IDLE,
	ENEMY_CANNIBAL_STATUS_WAIT,
	ENEMY_CANNIBAL_STATUS_DIE,
	ENEMY_CANNIBAL_STATUS_ACK,
	ENEMY_CANNIBAL_STATUS_WAITOVER
};

typedef struct
{
	D3DXVECTOR3 pos;		// 尰嵼偺埵抲
	D3DXVECTOR3 old_pos;
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
	int TimeIndex[2];
	bool pos_colider_status[256];
	int               animationID;                  //傾僯儊乕僔儑儞ID
	int               LastAnimationID;
	float             animationTime[ENEMY_CANNIBAL_ANIMATION_MAX]; //傾僯儊乕僔儑儞僾儗僀帪娫
	float             animationSpeed;               // 傾僯儊乕僔儑儞僗僺乕僪

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
	bool                AckPlayerJudge; //只触发一次攻击事件 Enemy->Player
	bool                DieJudge; //只触发一次死亡事件
	int                 status;
	int                 LastStatus; //上一个状态
} ENEMY_CANNIBAL;


//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT InitEnemyCannibal(void);
void UninitEnemyCannibal(void);
void UpdateEnemyCannibal(void);
void DrawEnemyCannibal(void);
ENEMY_CANNIBAL *GetEnemyCannibal(void);
void ResetEnemyCannibalAckJudge(void);
void SetbHitEventJ(void);
void BackUpEnemyCannibal(void);
HRESULT RestartEnemyCannibal(void);
int CreateEnemyCannibal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
void DrawEnemyCannibalShadow(int EnemyCannibalIndex, D3DXMATRIX mtxWorld);
void ReInitCannibalAnimation(void);

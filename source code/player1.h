//=============================================================================
//
// プレイヤー処理 [player1.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once


//*****************************************************************************
// 峔憿懱掕媊
//*****************************************************************************

enum
{
	PLAYER_IDLE,
	PLAYER_RUN,
	PLAYER_ATTACK1,
	PLAYER_ATTACK2,
	PLAYER_MAGIC,
	PLAYER_KICK,
	PLAYER_JUMP,
	PLAYER_ROLL,
	PLAYER_DEATH1,
	PLAYER_DEATH2,
	PLAYER_IMPACT,
	PLAYER_SIT,
	PLAYER_WIN,
	ANIMATION_MAX
};

typedef struct
{
	D3DXVECTOR3 pos;		// 尰嵼偺埵抲
	D3DXVECTOR3 old_pos;
	D3DXVECTOR3 last_frame_pos[5]; //记录玩家当前前5帧的位置
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
	float             animationTime[ANIMATION_MAX]; //傾僯儊乕僔儑儞僾儗僀帪娫
	float             animationSpeed;               // 傾僯儊乕僔儑儞僗僺乕僪

	int CylinderColliderIdx; //人物目前碰撞索引
	int SphereColliderAckIdx; //人物攻击索引
	int ShadowIdx; //阴影索引
	int status; //人物当前状态
	float				spd;
	bool                ColliderHit;
	int                 face_dir;
	bool                 down_dir;
	bool AckJudge; //一次攻击只能触发一次攻击触发器
	int NowCubeIndex = -1;
	bool AckedJudge; //被攻击触发器
	int hp;
	int SoulNum; //魂的数量
	bool HurtJudge;
	bool DieJudge;
	bool AiMode; //Ai模式 自己移动
	int WalkSe;
	bool DieSe;
} PLAYER1;


//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT InitPlayer1(void);
void UninitPlayer1(void);
void UpdatePlayer1(void);
void DrawPlayer1(void);

PLAYER1 *GetPlayer1(void);

D3DXVECTOR3 GetPositionPlayer1(void);
D3DXVECTOR3 GetRotationPlayer1(void);
D3DXVECTOR3 GetRotationDestPlayer1(void);
D3DXVECTOR3 GetMovePlayer1(void);
HRESULT RestartPlayer1(void);
void BackUpPlayer1(void);
void SetbHitEventJ(void);
void DrawPlayer1Shadow(D3DXMATRIX mtxWorld);
void Player_G_Use(void);
bool PlayerAiMoveEvent(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);
void Player1AnimationIdle(void);
void ResetPlayer1(void);
void Player1WinAnimation(void);
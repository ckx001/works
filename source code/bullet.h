//=============================================================================
//
// 弾丸処理[bullet.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define MAX_BULLET_NUM (3000)


struct BULLET
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	int SphereColliderIdx; //对应的球体触发器索引
	int EffectIdx; //对应的Effect索引
	int Timer;  //子弹消失计时
	bool use;
};


void InitBullet(void);
void UpdateBullet(void);
int CreateBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, D3DXVECTOR3 move, int time);
BULLET *RetBullet(void);

//=============================================================================
//
// 弾丸処理[bullet.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "bullet.h"
#include "sphereCollider.h"


static BULLET g_Bullet[MAX_BULLET_NUM];


void InitBullet()
{
	for (int num = 0; num < MAX_BULLET_NUM; num++)
	{
		g_Bullet[num].use = false;
		g_Bullet[num].SphereColliderIdx = -1;
		g_Bullet[num].EffectIdx = -1;
	}
}


void UpdateBullet()
{
	int num_count = 0;
	for (int num = 0; num < MAX_BULLET_NUM; num++)
	{
		if (g_Bullet[num].use)
		{
			if (g_Bullet[num].SphereColliderIdx != -1)
			{
				g_Bullet[num].pos.x += g_Bullet[num].move.x;
				g_Bullet[num].pos.z += g_Bullet[num].move.z;
				SetPositionSphereCollider(g_Bullet[num].SphereColliderIdx, g_Bullet[num].pos, g_Bullet[num].rot, g_Bullet[num].scl);
				if (g_Bullet[num].Timer != -1)
				{
					g_Bullet[num].Timer--;
					if (g_Bullet[num].Timer <= 0)
					{
						DeleteSphereCollider(g_Bullet[num].SphereColliderIdx);
						g_Bullet[num].use = false;
					}
				}
			}
			num_count++;
		}
	}
	if (num_count >= MAX_BULLET_NUM - 1)
	{
		InitBullet();
	}
}


//move:移动量
//time:距离消失的时间
int CreateBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, D3DXVECTOR3 move, int time)
{
    int BulletIndex = -1;

    for (int num = 0; num < MAX_BULLET_NUM; num++)
    {
    	if (!g_Bullet[num].use)
    	{
			g_Bullet[num].pos = pos;
			g_Bullet[num].rot = rot;
			g_Bullet[num].scl = scl;
			g_Bullet[num].move = move;
			g_Bullet[num].Timer = time;
			g_Bullet[num].use = true;
			g_Bullet[num].SphereColliderIdx = CreateSphereCollider(pos, rot, scl);
			BulletIndex = num;
    		break;
    	}
    }
    return BulletIndex;
}


BULLET *RetBullet()
{
	return &g_Bullet[0];
}
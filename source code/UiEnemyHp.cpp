//=============================================================================
//
// UIエネミーHp処理[UiEnemyHp.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "UiEnemyHp.h"
#include "debugproc.h"
#include "Enemy_wolf.h"
#include "EnemyCannibal.h"
#include "EnemyBossWolf.h"
#include "GlobalFunction.h"

#define MAX_HP (350)

static UI_ENEMY_HP UiEnemyHpWk[MAX_HP];
static UI_ENEMY_HP_BG UiEnemyHpBgWk[MAX_HP];

//static int now_hp = ENEMY_WOLF_MAX_HP;

HRESULT MakeVertexUiEnemyHp(int i);
static void SetVertexUiEnemyHp(int i);
static void SetVertexUiEnemyHpBg(int i);
static void SetDiffuseUiEnemyHp(int i);
static void SetDiffuseUiEnemyHpBg(int i);

static bool g_bDisplay = false;   //是否显示血条
static int MaxHp = -1;
static int AckedObjType = -1;
static int LastAckedObjType = -1;

//VERTEX_RECT* vertices;

HRESULT InitUiEnemyUp(int type)
{
	LPDIRECT3DDEVICE9 g_d3d_device = GetDevice();
	if (type == 0)
	{
		for (int i = 0; i < MAX_HP; i++)
		{
			UiEnemyHpWk[i].pos = D3DXVECTOR3{ (float)SCREEN_WIDTH / 5 * 2 + i * UI_ENEMY_HP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT / 12, 0.0f };
			UiEnemyHpWk[i].use = true;

			MakeVertexUiEnemyHp(i);
		}
		for (int i = 0; i < MAX_HP; i++)
		{
			UiEnemyHpBgWk[i].pos = D3DXVECTOR3{ (float)SCREEN_WIDTH / 5 * 2 + i * UI_ENEMY_HP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT / 12, 0.0f };
			SetVertexUiEnemyHpBg(i);
			SetDiffuseUiEnemyHpBg(i);
		}
	}
	return S_OK;
}


void UpdateUiEnemyUp()
{
	if (LastAckedObjType != AckedObjType)
	{
		switch (AckedObjType)
		{
		case ENEMY_TYPE_WOLF:
			MaxHp = ENEMY_WOLF_MAX_HP;
			break;
		case ENEMY_TYPE_CANNIBAL:
			MaxHp = ENEMY_CANNIBAL_MAX_HP;
			break;
		case ENEMY_TYPE_BOSSWOLF:
			MaxHp = ENEMY_BOSSWOLF_MAX_HP;
			break;
		}
		LastAckedObjType = AckedObjType;
	}
	//PrintDebugProc("MAX HP = %d\n", MaxHp);
	//PrintDebugProc("\n");
	//PrintDebugProc("\n");
	//PrintDebugProc("\n");
	for (int i = 0; i < MAX_HP; i++)
	{
		UiEnemyHpWk[i].pos = D3DXVECTOR3{ (float)SCREEN_WIDTH / 5 * 2 + i * UI_ENEMY_HP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT / 12, 0.0f };
	}
	for (int i = 0; i < MAX_HP; i++)
	{
		UiEnemyHpBgWk[i].pos = D3DXVECTOR3{ (float)SCREEN_WIDTH / 5 * 2 + i * UI_ENEMY_HP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT / 12, 0.0f };
	}
    for (int i = 0; i < MAX_HP; i++)
    {
    	MakeVertexUiEnemyHp(i);
    	SetVertexUiEnemyHpBg(i);
    	SetDiffuseUiEnemyHpBg(i);
    }
}

void DrawUiEnemyUp()
{
	LPDIRECT3DDEVICE9 g_d3d_device = GetDevice();
	g_d3d_device->SetRenderState(D3DRS_ZENABLE, true);
	g_d3d_device->SetFVF(FVF_VERTEX_2D_DIFFUSE);
	/////////////
	//for (int i = 0; i < MAX_HP; i++)
	//{
	//	for (int a = 0; a < 4; a++)
	//	{
	//
	//		if (UiEnemyHpWk[i].vertexWk[a].vtx.z < 0 || UiEnemyHpWk[i].pos.z < 0)
	//		{
	//			UiEnemyHpWk[i].vertexWk[a].vtx.z = 0.0f;
	//			UiEnemyHpWk[i].pos.z = 0.0f;
	//		}
	//		if (UiEnemyHpWk[i].vertexWk[a].vtx.z > 10000.0f || UiEnemyHpWk[i].pos.z > 10000.0f)
	//		{
	//			UiEnemyHpWk[i].vertexWk[a].vtx.z = 0.0f;
	//			UiEnemyHpWk[i].pos.z = 0.0f;
	//		}
	//		if (UiEnemyHpWk[i].vertexWk[a].vtx.x < 0 || UiEnemyHpWk[i].pos.x < 0)
	//		{
	//			UiEnemyHpWk[i].vertexWk[a].vtx.x = 0.0f;
	//			UiEnemyHpWk[i].pos.x = 0.0f;
	//		}
	//		if (UiEnemyHpWk[i].vertexWk[a].vtx.x > 10000.0f || UiEnemyHpWk[i].pos.x > 10000.0f)
	//		{
	//			UiEnemyHpWk[i].vertexWk[a].vtx.x = 0.0f;
	//			UiEnemyHpWk[i].pos.x = 0.0f;
	//		}
	//	}
	//}
	/////////////
	if (g_bDisplay)
	{
		if (MaxHp >= 0 && MaxHp < MAX_HP)
		{
			for (int i = 0; i <= MaxHp; i++)
			{
				g_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, UiEnemyHpBgWk[i].vertexWk, sizeof(VERTEX_2D_DIFFUSE));
				if (UiEnemyHpWk[i].use)
				{
					g_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, UiEnemyHpWk[i].vertexWk, sizeof(VERTEX_2D_DIFFUSE));
				}
			}
		}
	}
	g_d3d_device->SetRenderState(D3DRS_ZENABLE, false);
}

void UninitUiEnemyUp()
{

}

HRESULT MakeVertexUiEnemyHp(int i)
{
	// 頂点座標の設定
	SetVertexUiEnemyHp(i);

	// 反射光の設定
	UiEnemyHpWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiEnemyHpWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiEnemyHpWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiEnemyHpWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);

	return S_OK;
}

void SetDiffuseUiEnemyHp(int i)
{
	UiEnemyHpWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiEnemyHpWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiEnemyHpWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiEnemyHpWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
}

void SetDiffuseUiEnemyHpBg(int i)
{
	UiEnemyHpBgWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	UiEnemyHpBgWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	UiEnemyHpBgWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	UiEnemyHpBgWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexUiEnemyHp(int i)
{
	// rhwの設定
	UiEnemyHpWk[i].vertexWk[0].rhw =
		UiEnemyHpWk[i].vertexWk[1].rhw =
		UiEnemyHpWk[i].vertexWk[2].rhw =
		UiEnemyHpWk[i].vertexWk[3].rhw = 1.0f;

	UiEnemyHpWk[i].vertexWk[0].vtx.x = UiEnemyHpWk[i].pos.x - UI_ENEMY_HP_TEXTURE_SIZE_X;
	UiEnemyHpWk[i].vertexWk[0].vtx.y = UiEnemyHpWk[i].pos.y - UI_ENEMY_HP_TEXTURE_SIZE_Y;
	UiEnemyHpWk[i].vertexWk[0].vtx.z = 0.0f;

	UiEnemyHpWk[i].vertexWk[1].vtx.x = UiEnemyHpWk[i].pos.x + UI_ENEMY_HP_TEXTURE_SIZE_X;
	UiEnemyHpWk[i].vertexWk[1].vtx.y = UiEnemyHpWk[i].pos.y - UI_ENEMY_HP_TEXTURE_SIZE_Y;
	UiEnemyHpWk[i].vertexWk[1].vtx.z = 0.0f;

	UiEnemyHpWk[i].vertexWk[2].vtx.x = UiEnemyHpWk[i].pos.x - UI_ENEMY_HP_TEXTURE_SIZE_X;
	UiEnemyHpWk[i].vertexWk[2].vtx.y = UiEnemyHpWk[i].pos.y + UI_ENEMY_HP_TEXTURE_SIZE_Y;
	UiEnemyHpWk[i].vertexWk[2].vtx.z = 0.0f;

	UiEnemyHpWk[i].vertexWk[3].vtx.x = UiEnemyHpWk[i].pos.x + UI_ENEMY_HP_TEXTURE_SIZE_X;
	UiEnemyHpWk[i].vertexWk[3].vtx.y = UiEnemyHpWk[i].pos.y + UI_ENEMY_HP_TEXTURE_SIZE_Y;
	UiEnemyHpWk[i].vertexWk[3].vtx.z = 0.0f;

}

void SetVertexUiEnemyHpBg(int i)
{
	UiEnemyHpBgWk[i].vertexWk[0].rhw =
		UiEnemyHpBgWk[i].vertexWk[1].rhw =
		UiEnemyHpBgWk[i].vertexWk[2].rhw =
		UiEnemyHpBgWk[i].vertexWk[3].rhw = 1.0f;

	UiEnemyHpBgWk[i].vertexWk[0].vtx.x = UiEnemyHpBgWk[i].pos.x - UI_ENEMY_HP_TEXTURE_SIZE_X;
	UiEnemyHpBgWk[i].vertexWk[0].vtx.y = UiEnemyHpBgWk[i].pos.y - UI_ENEMY_HP_TEXTURE_SIZE_Y;
	UiEnemyHpBgWk[i].vertexWk[0].vtx.z = 0.0f;

	UiEnemyHpBgWk[i].vertexWk[1].vtx.x = UiEnemyHpBgWk[i].pos.x + UI_ENEMY_HP_TEXTURE_SIZE_X;
	UiEnemyHpBgWk[i].vertexWk[1].vtx.y = UiEnemyHpBgWk[i].pos.y - UI_ENEMY_HP_TEXTURE_SIZE_Y;
	UiEnemyHpBgWk[i].vertexWk[1].vtx.z = 0.0f;

	UiEnemyHpBgWk[i].vertexWk[2].vtx.x = UiEnemyHpBgWk[i].pos.x - UI_ENEMY_HP_TEXTURE_SIZE_X;
	UiEnemyHpBgWk[i].vertexWk[2].vtx.y = UiEnemyHpBgWk[i].pos.y + UI_ENEMY_HP_TEXTURE_SIZE_Y;
	UiEnemyHpBgWk[i].vertexWk[2].vtx.z = 0.0f;

	UiEnemyHpBgWk[i].vertexWk[3].vtx.x = UiEnemyHpBgWk[i].pos.x + UI_ENEMY_HP_TEXTURE_SIZE_X;
	UiEnemyHpBgWk[i].vertexWk[3].vtx.y = UiEnemyHpBgWk[i].pos.y + UI_ENEMY_HP_TEXTURE_SIZE_Y;
	UiEnemyHpBgWk[i].vertexWk[3].vtx.z = 0.0f;

}


void EnemyHpReduce(int ENEMY_TYPE,int index, int hp)
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	int now_hp = -1;
	switch (ENEMY_TYPE)
	{
	    case ENEMY_TYPE_WOLF:
			now_hp = EnemyWolf[index].hp;
			EnemyWolf[index].hp = now_hp - hp;
	    	break;
		case ENEMY_TYPE_CANNIBAL:
			now_hp = EnemyCannibal[index].hp;
			EnemyCannibal[index].hp = now_hp - hp;
			break;
		case ENEMY_TYPE_BOSSWOLF:
			now_hp = EnemyBossWolf[index].hp;
			EnemyBossWolf[index].hp = now_hp - hp;
			break;
	}

	AckedObjType = ENEMY_TYPE;
	if (LastAckedObjType != AckedObjType)
	{
		for (int i = 0; i < MAX_HP; i++)
		{

			UiEnemyHpWk[i].use = true;
		}
	}
	for (int i = now_hp; i >= now_hp - hp; i--)
	{

		UiEnemyHpWk[i].use = false;
	}

	if ( now_hp >= 0 )
	{
		for (int i = now_hp; i >= now_hp - hp; i--)
		{

			UiEnemyHpWk[i].use = false;
		}
	}
}

void EnemyHpAdd(int hp)
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	//player[0].hp += hp;
	//if (player[0].hp >= UI_ENEMY_HP_MAX)
	//	player[0].hp = UI_ENEMY_HP_MAX;
	//now_hp = player[0].hp;
	int now_hp = 0;
	for (int i = 0; i < now_hp; i++)
	{
		UiEnemyHpWk[i].use = true;
	}
}


void ReInitEnemyHpRect()
{
	int now_hp;
	now_hp = ENEMY_WOLF_MAX_HP;
	for (int i = 0; i < ENEMY_WOLF_MAX_HP; i++)
		UiEnemyHpWk[i].use = true;
}

void ReSetEnemyHpRectFromHp(int index, int ENEMY_TYPE)
{
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	switch (ENEMY_TYPE)
	{
	case ENEMY_TYPE_WOLF:
		for (int i = 0; i < EnemyWolf[index].hp; i++)
			UiEnemyHpWk[i].use = true;
		break;
	case ENEMY_TYPE_CANNIBAL:
		for (int i = 0; i < EnemyCannibal[index].hp; i++)
			UiEnemyHpWk[i].use = true;
		break;
	case ENEMY_TYPE_BOSSWOLF:
		MaxHp = EnemyBossWolf[index].hp;
		for (int i = 0; i < EnemyBossWolf[index].hp; i++)
			UiEnemyHpWk[i].use = true;
		break;
	}
}

void SetDisPlayEnemyHp(bool b)
{
	g_bDisplay = b;
}


void ReInitEnemyHp()
{
	g_bDisplay = false;   //是否显示血条
	MaxHp = -1;
	AckedObjType = -1;
	LastAckedObjType = -1;
}
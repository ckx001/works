//=============================================================================
//
// UI プレイヤーHp処理[UiPlayerHp.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "UiPlayerHp.h"
#include "debugproc.h"
#include "player1.h"
#include "UiTitleMenu.h"
#include "GlobalFunction.h"

static void SetDiffuseBgUiPlayerHp();
static void SetVertexBgUiPlayerHp(int MaxHp);

static UI_PLAYER_HP	UiPlayerHpWk[UI_PLAYER_HP_MAX];
static UI_PLAYER_HP	UiPlayerHpWkBg;
static int const MaxHp = 100;

HRESULT MakeVertexUiPlayerHp(int i);
static void SetVertexUiPlayerHp(int i);
static void SetDiffuseUiPlayerHp(int i);

//VERTEX_RECT* vertices;

HRESULT InitUiPlayerUp(int type)
{
	//BYTE* vertex_ptr;
	LPDIRECT3DDEVICE9 g_d3d_device = GetDevice();
	PLAYER1 *Player = GetPlayer1();
	if (type == 0)
	{

		//MaxHp = Player->hp;
		for (int i = 0; i < Player->hp; i++)
		{
			//UiPlayerHpWk[i].pos_trans = 50.0f + i * 2;
			UiPlayerHpWk[i].pos = { 50.0f + i * UI_PLAYER_HP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT / 12, 0.0f };
			UiPlayerHpWk[i].use = true;
		
			MakeVertexUiPlayerHp(i);
		}

		UiPlayerHpWkBg.pos = { 50.0f, (float)SCREEN_HEIGHT / 12, 0.0f };
		UiPlayerHpWkBg.use = true;

		SetVertexBgUiPlayerHp(MaxHp);
		SetDiffuseBgUiPlayerHp();

	}
	return S_OK;
}

void UpdateUiPlayerUp()
{
	PLAYER1 *Player = GetPlayer1();
	for (int i = 0; i < Player->hp; i++)
	{
		MakeVertexUiPlayerHp(i);
	}

	SetVertexBgUiPlayerHp(MaxHp);
	SetDiffuseBgUiPlayerHp();
}

void DrawUiPlayerUp()
{
	LPDIRECT3DDEVICE9 g_d3d_device = GetDevice();
	PLAYER1 *Player = GetPlayer1();
	int *NowStruct = RetNowStruct();

	if (*NowStruct != STRUCT_TYPE_MENU)
	{
	    g_d3d_device->SetRenderState(D3DRS_ZENABLE, true);
	    g_d3d_device->SetFVF(FVF_VERTEX_2D_DIFFUSE);

		if (UiPlayerHpWkBg.use == true)
		{
			g_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, UiPlayerHpWkBg.vertexWk, sizeof(VERTEX_2D_DIFFUSE));
		}
		for (int i = 0; i < Player->hp; i++)
		{
			if (UiPlayerHpWk[i].use == true)
			{
				g_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, UiPlayerHpWk[i].vertexWk, sizeof(VERTEX_2D_DIFFUSE));
			}
		}
		g_d3d_device->SetRenderState(D3DRS_ZENABLE, false);
	}
}


void UninitUiPlayerUp()
{
	
}

HRESULT MakeVertexUiPlayerHp(int i)
{
	// 頂点座標の設定
	SetVertexUiPlayerHp(i);

	// rhwの設定
	UiPlayerHpWk[i].vertexWk[0].rhw =
	UiPlayerHpWk[i].vertexWk[1].rhw =
	UiPlayerHpWk[i].vertexWk[2].rhw =
	UiPlayerHpWk[i].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	UiPlayerHpWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiPlayerHpWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiPlayerHpWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiPlayerHpWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);


	return S_OK;
}

void SetDiffuseUiPlayerHp(int i)
{
	UiPlayerHpWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiPlayerHpWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiPlayerHpWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	UiPlayerHpWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
}


void SetDiffuseBgUiPlayerHp()
{
	UiPlayerHpWkBg.vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	UiPlayerHpWkBg.vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	UiPlayerHpWkBg.vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	UiPlayerHpWkBg.vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexUiPlayerHp(int i)
{
	UiPlayerHpWk[i].vertexWk[0].vtx.x = UiPlayerHpWk[i].pos.x - UI_PLAYER_HP_TEXTURE_SIZE_X;
	UiPlayerHpWk[i].vertexWk[0].vtx.y = UiPlayerHpWk[i].pos.y - UI_PLAYER_HP_TEXTURE_SIZE_Y;
	UiPlayerHpWk[i].vertexWk[0].vtx.z = 0.0f;

	UiPlayerHpWk[i].vertexWk[1].vtx.x = UiPlayerHpWk[i].pos.x + UI_PLAYER_HP_TEXTURE_SIZE_X;
	UiPlayerHpWk[i].vertexWk[1].vtx.y = UiPlayerHpWk[i].pos.y - UI_PLAYER_HP_TEXTURE_SIZE_Y;
	UiPlayerHpWk[i].vertexWk[1].vtx.z = 0.0f;

	UiPlayerHpWk[i].vertexWk[2].vtx.x = UiPlayerHpWk[i].pos.x - UI_PLAYER_HP_TEXTURE_SIZE_X;
	UiPlayerHpWk[i].vertexWk[2].vtx.y = UiPlayerHpWk[i].pos.y + UI_PLAYER_HP_TEXTURE_SIZE_Y;
	UiPlayerHpWk[i].vertexWk[2].vtx.z = 0.0f;

	UiPlayerHpWk[i].vertexWk[3].vtx.x = UiPlayerHpWk[i].pos.x + UI_PLAYER_HP_TEXTURE_SIZE_X;
	UiPlayerHpWk[i].vertexWk[3].vtx.y = UiPlayerHpWk[i].pos.y + UI_PLAYER_HP_TEXTURE_SIZE_Y;
	UiPlayerHpWk[i].vertexWk[3].vtx.z = 0.0f;

}


void SetVertexBgUiPlayerHp(int MaxHp)
{
	// rhwの設定
	UiPlayerHpWkBg.vertexWk[0].rhw =
		UiPlayerHpWkBg.vertexWk[1].rhw =
		UiPlayerHpWkBg.vertexWk[2].rhw =
		UiPlayerHpWkBg.vertexWk[3].rhw = 1.0f;

	UiPlayerHpWkBg.vertexWk[0].vtx.x = UiPlayerHpWkBg.pos.x;
	UiPlayerHpWkBg.vertexWk[0].vtx.y = UiPlayerHpWkBg.pos.y - UI_PLAYER_HP_TEXTURE_SIZE_Y;
	UiPlayerHpWkBg.vertexWk[0].vtx.z = 0.0f;

	UiPlayerHpWkBg.vertexWk[1].vtx.x = UiPlayerHpWkBg.pos.x + MaxHp * UI_PLAYER_HP_TEXTURE_SIZE_X;
	UiPlayerHpWkBg.vertexWk[1].vtx.y = UiPlayerHpWkBg.pos.y - UI_PLAYER_HP_TEXTURE_SIZE_Y;
	UiPlayerHpWkBg.vertexWk[1].vtx.z = 0.0f;

	UiPlayerHpWkBg.vertexWk[2].vtx.x = UiPlayerHpWkBg.pos.x;
	UiPlayerHpWkBg.vertexWk[2].vtx.y = UiPlayerHpWkBg.pos.y + UI_PLAYER_HP_TEXTURE_SIZE_Y;
	UiPlayerHpWkBg.vertexWk[2].vtx.z = 0.0f;

	UiPlayerHpWkBg.vertexWk[3].vtx.x = UiPlayerHpWkBg.pos.x + MaxHp * UI_PLAYER_HP_TEXTURE_SIZE_X;
	UiPlayerHpWkBg.vertexWk[3].vtx.y = UiPlayerHpWkBg.pos.y + UI_PLAYER_HP_TEXTURE_SIZE_Y;
	UiPlayerHpWkBg.vertexWk[3].vtx.z = 0.0f;

}


void PlayerHpReduce(int hp)
{
	PLAYER1 *Player = GetPlayer1();
	int LastHp = Player->hp;
	Player->hp -= hp;
	if (Player->hp < 0)
		Player->hp = 0;
	for (int i = LastHp; i >= Player->hp; i--)
	{
		UiPlayerHpWk[i].use = false;
	}
}

void PlayerHpAdd(int hp)
{
	PLAYER1 *Player = GetPlayer1();
	int LastHp = Player->hp;
	Player->hp += hp;
	if (Player->hp > MaxHp)
		Player->hp = MaxHp;
	for (int i = LastHp; i <= Player->hp; i++)
	{
		UiPlayerHpWk[i].use = true;
	}
}


void Reinit_hp_rect()
{

}
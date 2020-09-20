//=============================================================================
//
// UI文字入力処理[UiFontPrint.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "UiFontPrint.h"
#include "player1.h"
#include "GameEvent.h"
#include "scene.h"
#include "input.h"
#include "EnemyBossWolf.h"
#include "UiGoods.h"
#include "UiTitleMenu.h"
#include <stdio.h>

LPD3DXFONT	            g_pD3DXFont_Die_Menu = NULL;
LPD3DXFONT	            g_pD3DXFont_Hint = NULL;
LPD3DXFONT	            g_pD3DXFont_Tag = NULL;


enum NowHintFont
{
	FONT_BLANK,
	FONT_HELP_MOVE,
	FONT_HELP_ROT,
	FONT_HELP_ROLL,
	FONT_HELP_ACK,
	FONT_HELP_USE,
	FONT_SAVE,
	FONT_WIN,
	FONT_PLAYER_DIE
};


static int NowFontIndex = FONT_BLANK;  //当前文字索引
static int FontSelectIndex = 0; //当前文字列选项索引(用于切换)
static int SavePointBak = SAVE_POINT_HOME;
static int SavePointFontActive = 0;
static bool bSavePointFontActive = false;


void InitFontPrint()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXCreateFont(pDevice, 48, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, ("Arial"), &g_pD3DXFont_Die_Menu);
	D3DXCreateFont(pDevice, 48, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, ("Arial"), &g_pD3DXFont_Hint);
	D3DXCreateFont(pDevice, 34, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, ("Arial"), &g_pD3DXFont_Tag);
}


void UpdateFontPrint()
{
	int CurrentSceneIndex = RetCurrentSceneIndex();
	PLAYER1 *Player = GetPlayer1();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	int *SavePoint = RetSavePoint();
	int *NowStruct = RetNowStruct();

	if (CurrentSceneIndex == SCENE_SCENE_GRASS)
	{
		if (EnemyBossWolf->hp <= 0)
		{
			Player->AiMode = true;
			NowFontIndex = FONT_WIN;
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
			{
				ExitGame();
			}
		}
		else if (Player->DieJudge)
		{
			if (IsButtonTriggered(0, BUTTON_CROSS_RIGHT)
				|| GetKeyboardTrigger(DIK_S))
			{
				if (FontSelectIndex < 1)
					FontSelectIndex++;
			}
			if (IsButtonTriggered(0, BUTTON_CROSS_LEFT)
				|| GetKeyboardTrigger(DIK_W))
			{
				if (FontSelectIndex > 0)
					FontSelectIndex--;
			}
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
			{
				switch (FontSelectIndex)
				{
					//continue;
				case 0:
					ReSetScene(SCENE_SCENE_GRASS);
					NowFontIndex = FONT_BLANK;
					break;
					//restart;
				//case 1:
				//	*NowStruct = STRUCT_TYPE_MENU;
				//	ReSetScene(SCENE_SCENE_HOME);
				//	NowFontIndex = FONT_BLANK;
				//	break;
					//exit
				case 1:
					ExitGame();
					break;
				}
				FontSelectIndex = 0;
			}
			NowFontIndex = FONT_PLAYER_DIE;
		}
		else
			NowFontIndex = FONT_BLANK;
	}
	else if (CurrentSceneIndex == SCENE_SCENE_HOME)
	{
		if (Player->pos.z > 20.0f
			&& Player->pos.z < 264.0f
			&& Player->pos.x > -619.0f
			&& Player->pos.x < 140.0f)
		{
			NowFontIndex = FONT_HELP_MOVE;
		}
		else if (Player->pos.z > 900.0f
			&& Player->pos.z < 1300.0f
			&& Player->pos.x > -609.0f
			&& Player->pos.x < -344.0f)
	    {
			NowFontIndex = FONT_HELP_ROT;
		}
		else if (Player->pos.z > 900.0f
			&& Player->pos.z < 1300.0f
			&& Player->pos.x > -268.0f
			&& Player->pos.x < -50.0f)
		{
			NowFontIndex = FONT_HELP_ROLL;
		}
		else if (Player->pos.z > 500.0f
			&& Player->pos.z < 750.0f
			&& Player->pos.x > -620.0f
			&& Player->pos.x < -380.0f)
		{
			NowFontIndex = FONT_HELP_USE;
		}
		else if (Player->pos.z > 900.0f
			&& Player->pos.z < 1300.0f
			&& Player->pos.x > -20.0f
			&& Player->pos.x < 400.0f)
		{
			NowFontIndex = FONT_HELP_ACK;
		}
		else
			NowFontIndex = FONT_BLANK;
	}
	else
		NowFontIndex = FONT_BLANK;


    //Save Point chg
	if (*SavePoint != SavePointBak)
	{
		SavePointFontActive = 100;
		SavePointBak = *SavePoint;
	}
	if (SavePointFontActive > 0)
	{
		NowFontIndex = FONT_SAVE;
		SavePointFontActive--;
		bSavePointFontActive = true;
	}
	else if (bSavePointFontActive)
	{
		NowFontIndex = FONT_BLANK;
		bSavePointFontActive = false;
	}
}


void DrawFontPrint()
{
	int *NowStruct = RetNowStruct();
	char str_1[256];
	RECT rect;
	D3DXCOLOR Col = BLACK_COLOR;
	switch (NowFontIndex)
	{
	case FONT_HELP_MOVE:
		rect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("移動 :　ASDW　　Switch NS Joy-con:　左スティック"));
		g_pD3DXFont_Hint->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x10, 0x10, 0xff));
		break;
	case FONT_HELP_ROLL:
		rect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("回避 :　K　　Switch NS Joy-con:　Bボタン"));
		g_pD3DXFont_Hint->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x10, 0x10, 0xff));
		break;
	case FONT_HELP_ACK:
		rect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("攻撃 :　J　　Switch NS Joy-con:　Aボタン"));
		g_pD3DXFont_Hint->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x10, 0x10, 0xff));
		break;
	case FONT_HELP_ROT:
		rect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("カメラ操作 : Z C　　Switch NS Joy-con:　右スティック"));
		g_pD3DXFont_Hint->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x10, 0x10, 0xff));
		break;
	case FONT_HELP_USE:
		rect = { SCREEN_WIDTH / 6 , SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("アイテム使用: 数字キー　　\nSwitch NS Joy-con: 選択 左・右方向十字キー \n使用 Xボタン"));
		g_pD3DXFont_Hint->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x10, 0x10, 0xff));
		break;
	case FONT_PLAYER_DIE:
		rect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3 - 40, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("Continue"));
		if (FontSelectIndex == 0)
			Col = WHITE_COLOR;
		else
			Col = BLACK_COLOR;
		g_pD3DXFont_Die_Menu->DrawText(NULL, str_1, -1, &rect, DT_LEFT, Col);
		//rect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3 + 40, SCREEN_WIDTH, SCREEN_HEIGHT };
		//sprintf(str_1, ("Restart"));
		//if (FontSelectIndex == 1)
		//	Col = WHITE_COLOR;
		//else
		//	Col = BLACK_COLOR;
		//g_pD3DXFont_Die_Menu->DrawText(NULL, str_1, -1, &rect, DT_LEFT, Col);
		rect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3 + 40, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("Exit"));
		if (FontSelectIndex == 1)
			Col = WHITE_COLOR;
		else
			Col = BLACK_COLOR;
		g_pD3DXFont_Die_Menu->DrawText(NULL, str_1, -1, &rect, DT_LEFT, Col);
		break;
	case FONT_WIN:
		rect = { SCREEN_WIDTH / 5 * 4 , 50, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("Enterキー押す\nAボタン"));
		g_pD3DXFont_Tag->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
		rect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("ゲームクリアおめでとう"));
		g_pD3DXFont_Hint->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x10, 0x10, 0xff));
		break;
	case FONT_SAVE:
		rect = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("SAVE"));
		g_pD3DXFont_Hint->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x10, 0x10, 0xff));
		break;
	}

	if (*NowStruct != STRUCT_TYPE_MENU)
	{
		for (int a = 0; a < 5; a++)
		{
			rect = { SCREEN_WIDTH / 5 + a * UIGOODS_POS_SPACE - 20, SCREEN_HEIGHT / 5 * 4 + 30, SCREEN_WIDTH, SCREEN_HEIGHT };
			sprintf(str_1, ("%d"), a + 1);
			g_pD3DXFont_Tag->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
		}
	}
	else
	{
		rect = { SCREEN_WIDTH /5 * 4 , 50, SCREEN_WIDTH, SCREEN_HEIGHT };
		sprintf(str_1, ("Enterキー押す\nAボタン"));
		g_pD3DXFont_Tag->DrawText(NULL, str_1, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
	}
}


void UninitFontPrint()
{
	SAFE_RELEASE(g_pD3DXFont_Die_Menu);
	SAFE_RELEASE(g_pD3DXFont_Hint);
	SAFE_RELEASE(g_pD3DXFont_Tag);
}
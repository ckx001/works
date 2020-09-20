//=============================================================================
//
// UIミニマップ処理[UiSmallMap.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "UiSmallMap.h"
#include "Scene.h"
#include "player1.h"
#include "quarTree.h"
#include "GameEvent.h"
#include "UiTitleMenu.h"

UI_SMALL_MAP g_UiSmallMap[MAX_UI_SMALL_MAP_Y][MAX_UI_SMALL_MAP_X];
UI_SMALL_MAP_SHADOW g_UiSmallMapShadow[MAX_UI_SMALL_MAP_Y][MAX_UI_SMALL_MAP_X];
static int OldPlayerNowCubeIndex = -1;
static int LastPlayerNowCubeIndex_X = -1;
static int LastPlayerNowCubeIndex_Z = -1;

HRESULT MakeVertexUiSmallMap(int SceneId, int x, int y);
void SetDiffuseUiSmallMap(int x, int y);
void SetVertexUiEnemyHp(int x, int y);
void SetVertexUiEnemyHpShadow(int x, int y);

HRESULT InitUiSmallMap(int type, int SceneId)
{
	PLAYER1 *Player = GetPlayer1();
	LPDIRECT3DDEVICE9 g_d3d_device = GetDevice();

	if (type == 0)
	{
		for (int y = 0; y < MAX_UI_SMALL_MAP_Y; y++)
		{
			for (int x = 0; x < MAX_UI_SMALL_MAP_X; x++)
			{
				g_UiSmallMap[y][x].pos = D3DXVECTOR3{ (float)SCREEN_WIDTH - MAX_UI_SMALL_MAP_X * UI_SMALL_MAP_TEXTURE_SIZE_X + x * UI_SMALL_MAP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT - y * UI_SMALL_MAP_TEXTURE_SIZE_Y, 0.0f };
				g_UiSmallMap[y][x].use = true;

				g_UiSmallMapShadow[y][x].pos = D3DXVECTOR3{ (float)SCREEN_WIDTH - MAX_UI_SMALL_MAP_X * UI_SMALL_MAP_TEXTURE_SIZE_X + x * UI_SMALL_MAP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT - MAX_UI_SMALL_MAP_Y * UI_SMALL_MAP_TEXTURE_SIZE_Y + y * UI_SMALL_MAP_TEXTURE_SIZE_Y, 0.0f };
				g_UiSmallMapShadow[y][x].use = true;

				MakeVertexUiSmallMap(SceneId, x, y);
			}
		}
	}

	return S_OK;
}


void UpdateUiSmallMap()
{
	PLAYER1 *Player = GetPlayer1();
	int CurrentSceneIndex = RetCurrentSceneIndex();

	//实时载入玩家位置
	int PlayerNowCubeIndex = RetPlayerNowCubeIndex();
	int *NowStruct = RetNowStruct();

	if (*NowStruct != STRUCT_TYPE_MENU)
	{
		for (int y = 0; y < MAX_UI_SMALL_MAP_Y; y++)
		{
			SMALL_MAP *SmallMap = RetSmallMap(CurrentSceneIndex, y);
			for (int x = 0; x < MAX_UI_SMALL_MAP_X; x++)
			{
				if (SmallMap[x].CubeIndex == PlayerNowCubeIndex)
				{
					//走过的Index替换回原来的颜色
					if (LastPlayerNowCubeIndex_X != -1 && LastPlayerNowCubeIndex_Z != -1)
					{
						g_UiSmallMap[LastPlayerNowCubeIndex_Z][LastPlayerNowCubeIndex_X].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 0, 60);
						g_UiSmallMap[LastPlayerNowCubeIndex_Z][LastPlayerNowCubeIndex_X].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 0, 60);
						g_UiSmallMap[LastPlayerNowCubeIndex_Z][LastPlayerNowCubeIndex_X].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 0, 60);
						g_UiSmallMap[LastPlayerNowCubeIndex_Z][LastPlayerNowCubeIndex_X].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 0, 60);
					}

					if (OldPlayerNowCubeIndex != -1)
					{
						g_UiSmallMap[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 60);
						g_UiSmallMap[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 60);
						g_UiSmallMap[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 60);
						g_UiSmallMap[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 60);
					}
					
					LastPlayerNowCubeIndex_Z = y;
					LastPlayerNowCubeIndex_X = x;

		            g_UiSmallMap[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 80);
		            g_UiSmallMap[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 80);
		            g_UiSmallMap[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 80);
		            g_UiSmallMap[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 80);
					break;
				}
				g_UiSmallMap[y][x].pos = D3DXVECTOR3{ (float)SCREEN_WIDTH - MAX_UI_SMALL_MAP_X * UI_SMALL_MAP_TEXTURE_SIZE_X + x * UI_SMALL_MAP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT - y * UI_SMALL_MAP_TEXTURE_SIZE_Y, 0.0f };
				g_UiSmallMapShadow[y][x].pos = D3DXVECTOR3{ (float)SCREEN_WIDTH - MAX_UI_SMALL_MAP_X * UI_SMALL_MAP_TEXTURE_SIZE_X + x * UI_SMALL_MAP_TEXTURE_SIZE_X , (float)SCREEN_HEIGHT - MAX_UI_SMALL_MAP_Y * UI_SMALL_MAP_TEXTURE_SIZE_Y + y * UI_SMALL_MAP_TEXTURE_SIZE_Y, 0.0f };
				SetVertexUiEnemyHp(x, y);
				SetVertexUiEnemyHpShadow(x, y);
			}
		}
		OldPlayerNowCubeIndex = PlayerNowCubeIndex;
	}
}

void DrawUiSmallMap()
{
	int *NowStruct = RetNowStruct();

	if (*NowStruct != STRUCT_TYPE_MENU)
	{
		LPDIRECT3DDEVICE9 g_d3d_device = GetDevice();
		g_d3d_device->SetRenderState(D3DRS_ZENABLE, true);
		g_d3d_device->SetFVF(FVF_VERTEX_2D_DIFFUSE);
		//g_d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		//反向绘制
		for (int y = 0; y < MAX_UI_SMALL_MAP_Y; y++)
		{
			for (int x = 0; x < MAX_UI_SMALL_MAP_X; x++)
			{
				if (g_UiSmallMap[y][x].use)
					g_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_UiSmallMap[y][x].vertexWk, sizeof(VERTEX_2D_DIFFUSE));
				//if (g_UiSmallMapShadow[y][x].use)
				//	g_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_UiSmallMapShadow[y][x].vertexWk, sizeof(VERTEX_2D_DIFFUSE));
			}
		}

		g_d3d_device->SetRenderState(D3DRS_ZENABLE, false);
		//g_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//g_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	}
}

void UninitUiSmallMap()
{

}


HRESULT MakeVertexUiSmallMap(int SceneId, int x, int y)
{
	// 頂点座標の設定
	SetVertexUiEnemyHp(x, y);

	// 反射光の設定
	g_UiSmallMapShadow[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 60);
	g_UiSmallMapShadow[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 60);
	g_UiSmallMapShadow[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 60);
	g_UiSmallMapShadow[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 60);

	SMALL_MAP *SmallMapList = RetSmallMap(SceneId, y);
	
	if (SmallMapList[x].Type == SMALL_MAP_TYPE_BLANK)
	{
		g_UiSmallMap[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 60);
		g_UiSmallMap[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 60);
		g_UiSmallMap[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 60);
		g_UiSmallMap[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 60);
	}	
	else if (SmallMapList[x].Type == SMALL_MAP_TYPE_CUBE)
	{
		g_UiSmallMap[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 0, 60);
		g_UiSmallMap[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 0, 60);
		g_UiSmallMap[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 0, 60);
		g_UiSmallMap[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 0, 60);
	}
	return S_OK;
}

void SetDiffuseUiSmallMap(int x, int y)
{
	g_UiSmallMapShadow[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
	g_UiSmallMapShadow[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
	g_UiSmallMapShadow[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
	g_UiSmallMapShadow[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);

	g_UiSmallMap[y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	g_UiSmallMap[y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	g_UiSmallMap[y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
	g_UiSmallMap[y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 255);
}


void SetVertexUiEnemyHp(int x, int y)
{
	g_UiSmallMap[y][x].vertexWk[0].rhw =
		g_UiSmallMap[y][x].vertexWk[1].rhw =
		g_UiSmallMap[y][x].vertexWk[2].rhw =
		g_UiSmallMap[y][x].vertexWk[3].rhw = 1.0f;

	g_UiSmallMap[y][x].vertexWk[0].vtx.x = g_UiSmallMap[y][x].pos.x - UI_SMALL_MAP_TEXTURE_SIZE_X;
	g_UiSmallMap[y][x].vertexWk[0].vtx.y = g_UiSmallMap[y][x].pos.y - UI_SMALL_MAP_TEXTURE_SIZE_Y;
	g_UiSmallMap[y][x].vertexWk[0].vtx.z = 0.0f;

	g_UiSmallMap[y][x].vertexWk[1].vtx.x = g_UiSmallMap[y][x].pos.x + UI_SMALL_MAP_TEXTURE_SIZE_X;
	g_UiSmallMap[y][x].vertexWk[1].vtx.y = g_UiSmallMap[y][x].pos.y - UI_SMALL_MAP_TEXTURE_SIZE_Y;
	g_UiSmallMap[y][x].vertexWk[1].vtx.z = 0.0f;

	g_UiSmallMap[y][x].vertexWk[2].vtx.x = g_UiSmallMap[y][x].pos.x - UI_SMALL_MAP_TEXTURE_SIZE_X;
	g_UiSmallMap[y][x].vertexWk[2].vtx.y = g_UiSmallMap[y][x].pos.y + UI_SMALL_MAP_TEXTURE_SIZE_Y;
	g_UiSmallMap[y][x].vertexWk[2].vtx.z = 0.0f;

	g_UiSmallMap[y][x].vertexWk[3].vtx.x = g_UiSmallMap[y][x].pos.x + UI_SMALL_MAP_TEXTURE_SIZE_X;
	g_UiSmallMap[y][x].vertexWk[3].vtx.y = g_UiSmallMap[y][x].pos.y + UI_SMALL_MAP_TEXTURE_SIZE_Y;
	g_UiSmallMap[y][x].vertexWk[3].vtx.z = 0.0f;
}

void SetVertexUiEnemyHpShadow(int x, int y)
{
	g_UiSmallMapShadow[y][x].vertexWk[0].rhw =
		g_UiSmallMapShadow[y][x].vertexWk[1].rhw =
		g_UiSmallMapShadow[y][x].vertexWk[2].rhw =
		g_UiSmallMapShadow[y][x].vertexWk[3].rhw = 1.0f;

	g_UiSmallMapShadow[y][x].vertexWk[0].vtx.x = g_UiSmallMapShadow[y][x].pos.x - UI_SMALL_MAP_TEXTURE_SIZE_X;
	g_UiSmallMapShadow[y][x].vertexWk[0].vtx.y = g_UiSmallMapShadow[y][x].pos.y - UI_SMALL_MAP_TEXTURE_SIZE_Y;
	g_UiSmallMapShadow[y][x].vertexWk[0].vtx.z = 0.0f;

	g_UiSmallMapShadow[y][x].vertexWk[1].vtx.x = g_UiSmallMapShadow[y][x].pos.x + UI_SMALL_MAP_TEXTURE_SIZE_X;
	g_UiSmallMapShadow[y][x].vertexWk[1].vtx.y = g_UiSmallMapShadow[y][x].pos.y - UI_SMALL_MAP_TEXTURE_SIZE_Y;
	g_UiSmallMapShadow[y][x].vertexWk[1].vtx.z = 0.0f;

	g_UiSmallMapShadow[y][x].vertexWk[2].vtx.x = g_UiSmallMapShadow[y][x].pos.x - UI_SMALL_MAP_TEXTURE_SIZE_X;
	g_UiSmallMapShadow[y][x].vertexWk[2].vtx.y = g_UiSmallMapShadow[y][x].pos.y + UI_SMALL_MAP_TEXTURE_SIZE_Y;
	g_UiSmallMapShadow[y][x].vertexWk[2].vtx.z = 0.0f;

	g_UiSmallMapShadow[y][x].vertexWk[3].vtx.x = g_UiSmallMapShadow[y][x].pos.x + UI_SMALL_MAP_TEXTURE_SIZE_X;
	g_UiSmallMapShadow[y][x].vertexWk[3].vtx.y = g_UiSmallMapShadow[y][x].pos.y + UI_SMALL_MAP_TEXTURE_SIZE_Y;
	g_UiSmallMapShadow[y][x].vertexWk[3].vtx.z = 0.0f;

}
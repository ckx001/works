//=============================================================================
//
// UIタイトル処理[UiTitleMenu.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "UiTitleMenu.h"
#include "player1.h"
#include "GlobalFunction.h"
#include "input.h"
#include "fade.h"
#include "UiPlayerHp.h"
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);
HRESULT MakeVertexTitleTitle(void);
void SetVertexLogo(int i);
void SetTextureTitleLogo(int i, int cntPattern, int index);
void SetVertexTitle(int i);
void SetTextureTitleTitle(int i, int cntPattern, int index);
//void SetVertexLogoColor(void);


#define	LOGO_HIGHT			(80)							// ロゴの高さ
#define LOGO_W				(480)							// 幅

//初始化当前游戏的结构
static int NowStruct = STRUCT_TYPE_MENU; 
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureTitleStart = NULL;	// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		g_pD3DTextureTitleTitle = NULL;	// テクスチャへのポインタ

static VERTEX_2D				g_vertexWkTitle[NUM_VERTEX];					// 頂点情報格納ワーク

static IDirect3DTexture9*      g_texture = NULL;
static BYTE* vertex_ptr;

static TITLELOGO					TitleLogoWk[TITLELOGO_MAX];				// エネミー構造体
static TITLELOGO					TitleTitleWk[TITLELOGO_MAX];				// エネミー構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		//D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		//	TEXTURE_TITLE,				// ファイルの名前
		//	&g_pD3DTextureTitle);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_UI_TTILE_START,			// ファイルの名前
			&g_pD3DTextureTitleStart);	// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_UI_TTILE_TITLE,			// ファイルの名前
			&g_pD3DTextureTitleTitle);	// 読み込むメモリー
	}

	//g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	TitleLogoWk[0].pos = D3DXVECTOR3{ SCREEN_WIDTH / 6, SCREEN_HEIGHT / 5 * 3 + 50.0f, 0.0f };
	TitleLogoWk[0].png_index = 0;
	TitleLogoWk[0].PatternAnim = 1;
	TitleLogoWk[0].CountAnim = 0;
	for (int i = 0; i < TITLELOGO_MAX; i++)
	{
		TitleLogoWk[i].use = true;
		TitleLogoWk[i].Texture = g_pD3DTextureTitleStart;
	}
	// 頂点情報の作成
	MakeVertexTitle();
	
	TitleTitleWk[0].pos = D3DXVECTOR3{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 2 - 50, 0.0f };
	TitleTitleWk[0].png_index = 0;
	TitleTitleWk[0].PatternAnim = 1;
	TitleTitleWk[0].CountAnim = 0;
	for (int i = 0; i < TITLELOGO_MAX; i++)
	{
		TitleTitleWk[i].use = true;
		TitleTitleWk[i].Texture = g_pD3DTextureTitleStart;
	}
	MakeVertexTitleTitle();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{

	if (g_pD3DTextureTitleStart != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitleStart->Release();
		g_pD3DTextureTitleStart = NULL;
	}
}

static bool G_change_use = true;
static bool bTitleBgm = true;
//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	PLAYER1 *Player = GetPlayer1();
#ifdef MENU_SWITCH
	if (bTitleBgm)
	{
		PlaySound(SOUND_LABEL_BGM_TITLE);
		bTitleBgm = false;
	}
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
	{
		//插入过场渐变
		SetFade(FADE_OUT);
		//Player->use = true;
		Player->pos = D3DXVECTOR3(0.11f, 22.0f, 143.0f);
		PlayerHpReduce(50);
		Player1AnimationIdle();
		NowStruct = STRUCT_TYPE_GAME;
		StopSound(SOUND_LABEL_BGM_TITLE);
	}
	//else
		//Player->rot.y = D3DX_PI;
	for (int i = 0; i < TITLELOGO_MAX; i++)
	{
		SetTextureTitleTitle(i, TitleTitleWk[i].PatternAnim, TitleTitleWk[i].png_index);
		SetVertexTitle(i);
	}
	for (int i = 0; i < TITLELOGO_MAX; i++)
	{
		SetTextureTitleLogo(i, TitleLogoWk[i].PatternAnim, TitleLogoWk[i].png_index);
		SetVertexLogo(i);
	}
#else
	//Player->use = true;
	NowStruct = STRUCT_TYPE_GAME;
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
#ifdef MENU_SWITCH
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleTitle);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, TitleTitleWk[0].vertexWk, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleStart);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, TitleLogoWk[0].vertexWk, sizeof(VERTEX_2D));
#endif 
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(void)
{

	for (int i = 0; i < TITLELOGO_MAX; i++)
	{
		SetTextureTitleLogo(i, TitleLogoWk[i].PatternAnim, TitleLogoWk[i].png_index);
		// 頂点座標の設定
		TitleLogoWk[i].vertexWk[0].vtx = D3DXVECTOR3(TitleLogoWk[i].pos.x, TitleLogoWk[i].pos.y, 0.0f);
		TitleLogoWk[i].vertexWk[1].vtx = D3DXVECTOR3(TitleLogoWk[i].pos.x + TITLELOGO_SIZE_X, TitleLogoWk[i].pos.y, 0.0f);
		TitleLogoWk[i].vertexWk[2].vtx = D3DXVECTOR3(TitleLogoWk[i].pos.x, TitleLogoWk[i].pos.y + TITLELOGO_SIZE_Y, 0.0f);
		TitleLogoWk[i].vertexWk[3].vtx = D3DXVECTOR3(TitleLogoWk[i].pos.x + TITLELOGO_SIZE_X, TitleLogoWk[i].pos.y + TITLELOGO_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		TitleLogoWk[i].vertexWk[0].rhw =
			TitleLogoWk[i].vertexWk[1].rhw =
			TitleLogoWk[i].vertexWk[2].rhw =
			TitleLogoWk[i].vertexWk[3].rhw = 1.0f;

		// 反射光の設定
		TitleLogoWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		TitleLogoWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		TitleLogoWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		TitleLogoWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		TitleLogoWk[i].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		TitleLogoWk[i].vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		TitleLogoWk[i].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		TitleLogoWk[i].vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}


HRESULT MakeVertexTitleTitle(void)
{

	for (int i = 0; i < TITLELOGO_MAX; i++)
	{
		SetTextureTitleLogo(i, TitleLogoWk[i].PatternAnim, TitleLogoWk[i].png_index);
		// 頂点座標の設定
		TitleTitleWk[i].vertexWk[0].vtx = D3DXVECTOR3(TitleLogoWk[i].pos.x, TitleLogoWk[i].pos.y, 0.0f);
		TitleTitleWk[i].vertexWk[1].vtx = D3DXVECTOR3(TitleLogoWk[i].pos.x + TITLELOGO_SIZE_X, TitleLogoWk[i].pos.y, 0.0f);
		TitleTitleWk[i].vertexWk[2].vtx = D3DXVECTOR3(TitleLogoWk[i].pos.x, TitleLogoWk[i].pos.y + TITLELOGO_SIZE_Y, 0.0f);
		TitleTitleWk[i].vertexWk[3].vtx = D3DXVECTOR3(TitleLogoWk[i].pos.x + TITLELOGO_SIZE_X, TitleLogoWk[i].pos.y + TITLELOGO_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		TitleTitleWk[i].vertexWk[0].rhw =
		TitleTitleWk[i].vertexWk[1].rhw =
		TitleTitleWk[i].vertexWk[2].rhw =
		TitleTitleWk[i].vertexWk[3].rhw = 1.0f;

		// 反射光の設定
		TitleTitleWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		TitleTitleWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		TitleTitleWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		TitleTitleWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		TitleTitleWk[i].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		TitleTitleWk[i].vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		TitleTitleWk[i].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		TitleTitleWk[i].vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}
//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexLogo(int i)
{
	// 頂点座標の設定
	TitleLogoWk[i].vertexWk[0].vtx.x = TitleLogoWk[i].pos.x - TITLELOGO_SIZE_X;
	TitleLogoWk[i].vertexWk[0].vtx.y = TitleLogoWk[i].pos.y - TITLELOGO_SIZE_Y;
	TitleLogoWk[i].vertexWk[0].vtx.z = 0.0f;

	TitleLogoWk[i].vertexWk[1].vtx.x = TitleLogoWk[i].pos.x + TITLELOGO_SIZE_X;
	TitleLogoWk[i].vertexWk[1].vtx.y = TitleLogoWk[i].pos.y - TITLELOGO_SIZE_Y;
	TitleLogoWk[i].vertexWk[1].vtx.z = 0.0f;

	TitleLogoWk[i].vertexWk[2].vtx.x = TitleLogoWk[i].pos.x - TITLELOGO_SIZE_X;
	TitleLogoWk[i].vertexWk[2].vtx.y = TitleLogoWk[i].pos.y + TITLELOGO_SIZE_Y;
	TitleLogoWk[i].vertexWk[2].vtx.z = 0.0f;

	TitleLogoWk[i].vertexWk[3].vtx.x = TitleLogoWk[i].pos.x + TITLELOGO_SIZE_X;
	TitleLogoWk[i].vertexWk[3].vtx.y = TitleLogoWk[i].pos.y + TITLELOGO_SIZE_Y;
	TitleLogoWk[i].vertexWk[3].vtx.z = 0.0f;
}


void SetVertexTitle(int i)
{
	// 頂点座標の設定
	TitleTitleWk[i].vertexWk[0].vtx.x = TitleTitleWk[i].pos.x - TITLETITLE_SIZE_X;
	TitleTitleWk[i].vertexWk[0].vtx.y = TitleTitleWk[i].pos.y - TITLETITLE_SIZE_Y;
	TitleTitleWk[i].vertexWk[0].vtx.z = 0.0f;
	
	TitleTitleWk[i].vertexWk[1].vtx.x = TitleTitleWk[i].pos.x + TITLETITLE_SIZE_X;
	TitleTitleWk[i].vertexWk[1].vtx.y = TitleTitleWk[i].pos.y - TITLETITLE_SIZE_Y;
	TitleTitleWk[i].vertexWk[1].vtx.z = 0.0f;						 
																	 
	TitleTitleWk[i].vertexWk[2].vtx.x = TitleTitleWk[i].pos.x - TITLETITLE_SIZE_X;
	TitleTitleWk[i].vertexWk[2].vtx.y = TitleTitleWk[i].pos.y + TITLETITLE_SIZE_Y;
	TitleTitleWk[i].vertexWk[2].vtx.z = 0.0f;						 
																	 
	TitleTitleWk[i].vertexWk[3].vtx.x = TitleTitleWk[i].pos.x + TITLETITLE_SIZE_X;
	TitleTitleWk[i].vertexWk[3].vtx.y = TitleTitleWk[i].pos.y + TITLETITLE_SIZE_Y;
	TitleTitleWk[i].vertexWk[3].vtx.z = 0.0f;
}


void SetTextureTitleLogo(int i, int cntPattern, int index)
{
	int x = cntPattern % TITLELOGO_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TITLELOGO_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TITLELOGO_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TITLELOGO_TEXTURE_PATTERN_DIVIDE_Y;

	TitleLogoWk[i].vertexWk[0].tex = { (float)(x)* sizeX, (float)(index)* sizeY };
	TitleLogoWk[i].vertexWk[1].tex = { (float)(x)* sizeX + sizeX, (float)(index)* sizeY };
	TitleLogoWk[i].vertexWk[2].tex = { (float)(x)* sizeX, (float)(index)* sizeY + sizeY };
	TitleLogoWk[i].vertexWk[3].tex = { (float)(x)* sizeX + sizeX, (float)(index)* sizeY + sizeY };
}


void SetTextureTitleTitle(int i, int cntPattern, int index)
{
	int x = cntPattern % TITLELOGO_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TITLELOGO_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TITLELOGO_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TITLELOGO_TEXTURE_PATTERN_DIVIDE_Y;

	TitleTitleWk[i].vertexWk[0].tex = { (float)(x)* sizeX, (float)(index)* sizeY };
	TitleTitleWk[i].vertexWk[1].tex = { (float)(x)* sizeX + sizeX, (float)(index)* sizeY };
	TitleTitleWk[i].vertexWk[2].tex = { (float)(x)* sizeX, (float)(index)* sizeY + sizeY };
	TitleTitleWk[i].vertexWk[3].tex = { (float)(x)* sizeX + sizeX, (float)(index)* sizeY + sizeY };

}


int *RetNowStruct()
{
	return &NowStruct;
}
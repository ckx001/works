//=============================================================================
//
// UIショートカットバー処理[UiGoods.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "UiGoods.h"
#include "GlobalFunction.h"
#include "UiPlayerHp.h"
#include "UiTitleMenu.h"
#include "sound.h"
#include "input.h"

#define MAX_UI_GOODS_NUM (5)

static HRESULT MakeVertexUiGoodsBg(int i);
static void SetUiGoods(int i);
static void SetVertexUiGoodsBg(int i);
static void UseGood(int type);
static void SetUiGoodsSeletedUi(int index);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;	// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		g_pD3DTextureUiRed = NULL;
static LPDIRECT3DTEXTURE9       g_pD3DTextureUiSelected = NULL;
static LPDIRECT3DTEXTURE9       g_pD3DTextureUiSelectedUi = NULL;
static VERTEX_2D				g_vertexWk[MAX_UI_GOODS_NUM][NUM_VERTEX];	// 頂点情報格納ワーク
static VERTEX_2D                g_vertexGoodsWk[NUM_VERTEX];
static VERTEX_2D                g_vertexGoodsSelectedUiWk[NUM_VERTEX];

static int GoodsSeletedIconIndex;  //选择框指针索引 0-4

struct GOOD
{
	bool bNowGoodIndexUse; //此物品栏是否有物品
	int type;  //物品对应的类型
};

GOOD Good[MAX_UI_GOODS_NUM];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUiGoods(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			UIGOODS_TEXTURE_FILE,		// ファイルの名前
			&g_pD3DTexture);		// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			UIGOODS_TEXTURE_RED_FILE,		// ファイルの名前
			&g_pD3DTextureUiRed);		// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			UIGOODS_TEXTURE_RED_FILE,		// ファイルの名前
			&g_pD3DTextureUiSelected);		// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			UIGOODS_TEXTURE_SELECTED_FILE,		// ファイルの名前
			&g_pD3DTextureUiSelectedUi);		// 読み込むメモリー
	}

	// 頂点情報の作成
	for (int i = 0; i < MAX_UI_GOODS_NUM; i++)
	{
		MakeVertexUiGoodsBg(i);
	
		SetVertexUiGoodsBg(i);
		//bNowGoodIndexUse[i] = false;
		Good[i].bNowGoodIndexUse = false;
	}

	GoodsSeletedIconIndex = 0;
	SetUiGoodsSeletedUi(GoodsSeletedIconIndex);
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUiGoods(void)
{
	SAFE_RELEASE(g_pD3DTexture);
	SAFE_RELEASE(g_pD3DTextureUiRed);
	SAFE_RELEASE(g_pD3DTextureUiSelected);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUiGoods(void)
{
	int *NowStruct = RetNowStruct();

	if (*NowStruct != STRUCT_TYPE_MENU)
	{
		for (int i = 0; i < MAX_UI_GOODS_NUM; i++)
		{
			SetVertexUiGoodsBg(i);

			g_vertexWk[i][0].rhw =
				g_vertexWk[i][1].rhw =
				g_vertexWk[i][2].rhw =
				g_vertexWk[i][3].rhw = 1.0f;

			// 反射光の設定
			g_vertexWk[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
			g_vertexWk[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
			g_vertexWk[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
			g_vertexWk[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);

			// テクスチャ座標の設定
			g_vertexWk[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
			g_vertexWk[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
			g_vertexWk[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
			g_vertexWk[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		if (IsButtonTriggered(0, BUTTON_CROSS_RIGHT))
		{
			if (GoodsSeletedIconIndex < MAX_UI_GOODS_NUM - 1)
				GoodsSeletedIconIndex++;
		}
		if (IsButtonTriggered(0, BUTTON_CROSS_LEFT))
		{
			if (GoodsSeletedIconIndex > 0)
				GoodsSeletedIconIndex--;
		}
		SetUiGoodsSeletedUi(GoodsSeletedIconIndex);

		//手柄使用物品
		if (IsButtonTriggered(0, BUTTON_X))
		{
			if (Good[GoodsSeletedIconIndex].bNowGoodIndexUse)
			{
				UseGood(Good[GoodsSeletedIconIndex].type);
				Good[GoodsSeletedIconIndex].bNowGoodIndexUse = false;
			}
		}

		if (GetKeyboardPress(DIK_1))
		{
			if (Good[0].bNowGoodIndexUse)
			{
				UseGood(Good[0].type);
				Good[0].bNowGoodIndexUse = false;
			}
		}
		if (GetKeyboardPress(DIK_2))
		{
			if (Good[1].bNowGoodIndexUse)
			{
				UseGood(Good[1].type);
				Good[1].bNowGoodIndexUse = false;
			}
		}
		if (GetKeyboardPress(DIK_3))
		{
			if (Good[2].bNowGoodIndexUse)
			{
				UseGood(Good[2].type);
				Good[2].bNowGoodIndexUse = false;
			}
		}
		if (GetKeyboardPress(DIK_4))
		{
			if (Good[3].bNowGoodIndexUse)
			{
				UseGood(Good[3].type);
				Good[3].bNowGoodIndexUse = false;
			}
		}
		if (GetKeyboardPress(DIK_5))
		{
			if (Good[4].bNowGoodIndexUse)
			{
				UseGood(Good[4].type);
				Good[4].bNowGoodIndexUse = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUiGoods(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int *NowStruct = RetNowStruct();

	if (*NowStruct != STRUCT_TYPE_MENU)
	{
		for (int i = 0; i < MAX_UI_GOODS_NUM; i++)
		{
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk[i], sizeof(VERTEX_2D));

			if (Good[i].bNowGoodIndexUse)
			{
				SetUiGoods(i);
				pDevice->SetTexture(0, g_pD3DTextureUiSelected);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexGoodsWk, sizeof(VERTEX_2D));
			}
		}

		pDevice->SetTexture(0, g_pD3DTextureUiSelectedUi);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexGoodsSelectedUiWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexUiGoodsBg(int i)
{
	// 頂点座標の設定
	SetVertexUiGoodsBg(i);

	// rhwの設定
	g_vertexWk[i][0].rhw =
	g_vertexWk[i][1].rhw =
	g_vertexWk[i][2].rhw =
	g_vertexWk[i][3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWk[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
	g_vertexWk[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
	g_vertexWk[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
	g_vertexWk[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);

	// テクスチャ座標の設定
	g_vertexWk[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWk[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWk[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWk[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexUiGoodsBg(int i)
{
	// 頂点座標の設定
	g_vertexWk[i][0].vtx = D3DXVECTOR3(i * UIGOODS_POS_SPACE + UIGOODS_POS_X, UIGOODS_POS_Y, 0.0f);
	g_vertexWk[i][1].vtx = D3DXVECTOR3(i * UIGOODS_POS_SPACE + UIGOODS_POS_X + UIGOODS_SIZE_W, UIGOODS_POS_Y, 0.0f);
	g_vertexWk[i][2].vtx = D3DXVECTOR3(i * UIGOODS_POS_SPACE + UIGOODS_POS_X, UIGOODS_POS_Y + UIGOODS_SIZE_H, 0.0f);
	g_vertexWk[i][3].vtx = D3DXVECTOR3(i * UIGOODS_POS_SPACE + UIGOODS_POS_X + UIGOODS_SIZE_W, UIGOODS_POS_Y + UIGOODS_SIZE_H, 0.0f);
}

void SetUiGoods(int index)
{
	// 頂点座標の設定
	g_vertexGoodsWk[0].vtx = D3DXVECTOR3(index * UIGOODS_POS_SPACE + UIGOODS_POS_X, UIGOODS_POS_Y, 0.0f);
	g_vertexGoodsWk[1].vtx = D3DXVECTOR3(index * UIGOODS_POS_SPACE + UIGOODS_POS_X + UIGOODS_SIZE_W, UIGOODS_POS_Y, 0.0f);
	g_vertexGoodsWk[2].vtx = D3DXVECTOR3(index * UIGOODS_POS_SPACE + UIGOODS_POS_X, UIGOODS_POS_Y + UIGOODS_SIZE_H, 0.0f);
	g_vertexGoodsWk[3].vtx = D3DXVECTOR3(index * UIGOODS_POS_SPACE + UIGOODS_POS_X + UIGOODS_SIZE_W, UIGOODS_POS_Y + UIGOODS_SIZE_H, 0.0f);

	// rhwの設定
	g_vertexGoodsWk[0].rhw =
	g_vertexGoodsWk[1].rhw =
	g_vertexGoodsWk[2].rhw =
	g_vertexGoodsWk[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexGoodsWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 230);
	g_vertexGoodsWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 230);
	g_vertexGoodsWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 230);
	g_vertexGoodsWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 230);

	// テクスチャ座標の設定
	g_vertexGoodsWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexGoodsWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexGoodsWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexGoodsWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


void SetUiGoodsSeletedUi(int index)
{
	// 頂点座標の設定
	g_vertexGoodsSelectedUiWk[0].vtx = D3DXVECTOR3(index * UIGOODS_POS_SPACE + UIGOODS_POS_X, UIGOODS_POS_Y, 0.0f);
	g_vertexGoodsSelectedUiWk[1].vtx = D3DXVECTOR3(index * UIGOODS_POS_SPACE + UIGOODS_POS_X + UIGOODS_SIZE_W, UIGOODS_POS_Y, 0.0f);
	g_vertexGoodsSelectedUiWk[2].vtx = D3DXVECTOR3(index * UIGOODS_POS_SPACE + UIGOODS_POS_X, UIGOODS_POS_Y + UIGOODS_SIZE_H, 0.0f);
	g_vertexGoodsSelectedUiWk[3].vtx = D3DXVECTOR3(index * UIGOODS_POS_SPACE + UIGOODS_POS_X + UIGOODS_SIZE_W, UIGOODS_POS_Y + UIGOODS_SIZE_H, 0.0f);

	// rhwの設定
	g_vertexGoodsSelectedUiWk[0].rhw =
		g_vertexGoodsSelectedUiWk[1].rhw =
		g_vertexGoodsSelectedUiWk[2].rhw =
		g_vertexGoodsSelectedUiWk[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexGoodsSelectedUiWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
	g_vertexGoodsSelectedUiWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
	g_vertexGoodsSelectedUiWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);
	g_vertexGoodsSelectedUiWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 160);

	// テクスチャ座標の設定
	g_vertexGoodsSelectedUiWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexGoodsSelectedUiWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexGoodsSelectedUiWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexGoodsSelectedUiWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//exist: false 不判定是否存在
//       true  如果道具栏存在至少一个 将不增加
void SetGoodsAdd(int type, bool exist)
{

	if (exist)
	{
		for (int i = 0; i < MAX_UI_GOODS_NUM; i++)
		{
			if (Good[i].bNowGoodIndexUse)
				return;
		}
	}

	for (int i = 0; i < MAX_UI_GOODS_NUM; i++)
	{
		if (Good[i].bNowGoodIndexUse)
		{
			continue;
		}
		else
		{
			Good[i].bNowGoodIndexUse = true;
			Good[i].type = type;
			break;
		}
		
	}
}


void ClearAllGoods()
{
	for (int i = 0; i < MAX_UI_GOODS_NUM; i++)
	{
		if (Good[i].bNowGoodIndexUse)
		{
			Good[i].bNowGoodIndexUse = false;
		}
	}
	//return;
}


//使用物品产生效果
void UseGood(int type)
{
	if (type == GOODS_TYPE_RED)
	{
		g_pD3DTextureUiSelected = g_pD3DTextureUiRed;
		PlaySound(SOUND_LABEL_SE_USE_RED);
		//赋予效果
		PlayerHpAdd(25);
	}
}
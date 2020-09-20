#pragma once

// マクロ定義
#define UIGOODS_TEXTURE_FILE	("data/TEXTURE/UI/Ui_square1.png")	// サンプル用画像
#define UIGOODS_TEXTURE_SELECTED_FILE	("data/TEXTURE/UI/Ui_Seleted.png")	// サンプル用画像
#define UIGOODS_TEXTURE_RED_FILE	("data/TEXTURE/UI/Ui_red.png")	// サンプル用画像

#define UIGOODS_SIZE_W			(75)	// テクスチャサイズ
#define UIGOODS_SIZE_H			(75)	// 同上
#define UIGOODS_POS_X			(SCREEN_WIDTH/ 5)		// ポリゴンの初期位置X(左上)
#define UIGOODS_POS_Y			(SCREEN_HEIGHT / 5 * 4)		// 同上
#define UIGOODS_POS_SPACE       (100)     //横向间隔
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUiGoods(int type);
void UninitUiGoods(void);
void UpdateUiGoods(void);
void DrawUiGoods(void);
void SetGoodsAdd(int type, bool exist);
void ClearAllGoods(void);
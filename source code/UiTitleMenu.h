//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_UI_TTILE_START		("data/TEXTURE/Ui/Ui_Start.png")			// 読み込むテクスチャファイル名
#define TEXTURE_UI_TTILE_TITLE      ("data/TEXTURE/Ui/Ui_Title.png")

#define	TITLELOGO_POS_X			(100)								// タイトルの表示位置
#define	TITLELOGO_POS_Y			(80)								// タイトルの表示位置
#define	TITLELOGO_SIZE_X		(128)							// タイトルの幅
#define	TITLELOGO_SIZE_Y		(128)							// タイトルの高さ

#define	TITLETITLE_SIZE_X		(148)							// タイトルの幅
#define	TITLETITLE_SIZE_Y		(148)							// タイトルの高さ

#define TITLELOGO_TEXTURE_PATTERN_DIVIDE_X	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TITLELOGO_TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define TITLELOGO_ANIM_PATTERN_NUM			(TITLELOGO_TEXTURE_PATTERN_DIVIDE_X)	// アニメーションパターン数
//#define TITLELOGO_TIME_ANIMATION			(4)	// アニメーションの切り替わるカウント
#define TITLELOGO_TIME_DELAY (30)
#define TITLELOGO_MAX						(1) // 敵の最大数
typedef struct	// プレイヤー構造体
{
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3         pos;
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	int png_index;
	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D    		    vertexWk[NUM_VERTEX];

} TITLELOGO;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(int type);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
int *RetNowStruct(void);


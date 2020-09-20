#pragma once

#define UI_PLAYER_HP_MAX (100)
#define UI_PLAYER_HP_TEXTURE_SIZE_X	(3)		// テクスチャサイズ
#define UI_PLAYER_HP_TEXTURE_SIZE_Y	(15)		// 同上

typedef struct
{
	bool			use;		// true:使用  false:未使用
	D3DXVECTOR3		pos;
	VERTEX_2D_DIFFUSE    	vertexWk[NUM_VERTEX];
	//int  now_hp;
} UI_PLAYER_HP;

HRESULT InitUiPlayerUp(int type);
void UninitUiPlayerUp(void);
void UpdateUiPlayerUp(void);
void DrawUiPlayerUp(void);
void PlayerHpReduce(int hp);
void PlayerHpAdd(int hp);
#pragma once

#define MAX_UI_SMALL_MAP_X (160)
#define MAX_UI_SMALL_MAP_Y (160)
#define UI_SMALL_MAP_TEXTURE_SIZE_X	(2)		// テクスチャサイズ
#define UI_SMALL_MAP_TEXTURE_SIZE_Y	(2)		// 同上

typedef struct
{
	bool			use;		// true:使用  false:未使用
	D3DXVECTOR3		pos;
	VERTEX_2D_DIFFUSE    	vertexWk[NUM_VERTEX];
} UI_SMALL_MAP_SHADOW;

typedef struct
{
	bool			use;
	D3DXVECTOR3		pos;
	VERTEX_2D_DIFFUSE    	vertexWk[NUM_VERTEX];
} UI_SMALL_MAP;


HRESULT InitUiSmallMap(int type, int SceneId);
void UpdateUiSmallMap(void);
void DrawUiSmallMap(void);
void UninitUiSmallMap(void);
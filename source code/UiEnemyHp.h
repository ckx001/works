#pragma once

#define UI_ENEMY_HP_MAX (50)
#define UI_ENEMY_HP_TEXTURE_SIZE_X	(3)		// テクスチャサイズ
#define UI_ENEMY_HP_TEXTURE_SIZE_Y	(15)		// 同上

typedef struct
{
	bool			use;		// true:使用  false:未使用
	D3DXVECTOR3		pos;
	VERTEX_2D_DIFFUSE    	vertexWk[NUM_VERTEX];
} UI_ENEMY_HP;

typedef struct
{
	D3DXVECTOR3		pos;
	VERTEX_2D_DIFFUSE    	vertexWk[NUM_VERTEX];
} UI_ENEMY_HP_BG;

HRESULT InitUiEnemyUp(int type);
void UninitUiEnemyUp(void);
void UpdateUiEnemyUp(void);
void DrawUiEnemyUp(void);
void EnemyHpAdd(int hp);
void EnemyHpReduce(int ENEMY_TYPE, int index, int hp);
//int *RetEnemyNowHp(void);
void ReInitEnemyHpRect(void);
void SetDisPlayEnemyHp(bool b);
void ReSetEnemyHpRectFromHp(int index, int ENEMY_TYPE);
void ReInitEnemyHp(void);
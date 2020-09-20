//=============================================================================
//
// 地面処理 [BoxCollider.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <d3dx9.h>


#define	TEXTURE_NUM_BOXCOLLIDER	"data/TEXTURE/field000.jpg"						// 読み込むテクスチャファイル名
#define	TEXTURE_NUM_BOXCOLLIDER_1	"data/TEXTURE/bg.jpg"						// 読み込むテクスチャファイル名
#define NORMAL_SIZE_X (20.0f)
#define NORMAL_SIZE_Y (20.0f)
#define NORMAL_SIZE_Z (20.0f)
//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum BOXCOLLIDER_ENUM
{
	SCENE_PORTAL_COLLIDER,       //区域传送触发器
	SCENE_EVENT_COLLIDER,       //区域事件触发器
	MAX_BOXCOLLIDER_NUM = 2
};

struct BOXCOLLIDER
{
	D3DXVECTOR3		pos;		// ポリゴンの位置
	D3DXVECTOR3		rot;		// ポリゴンの向き(回転)
	D3DXVECTOR3		scl;		// ポリゴンの大きさ(スケール)
	D3DXVECTOR3     nor; //法向量
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	//VERTEX_3D		vertexWk[NUM_VERTEX];
	bool use;
	bool active;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBoxCollider(void);
void UninitBoxCollider(void);
void UpdateBoxCollider(void);
void DrawBoxCollider(void);
int CreateBoxCollider(int Index ,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl,bool bStatic);
void SetPositionBoxCollider(int Index, int nIdxBoxCollider, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
BOXCOLLIDER *GetBoxCollider(int Index);
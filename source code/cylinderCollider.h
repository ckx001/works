//=============================================================================
//
// カプセル体処理 [cylinderCollider.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
//#include <d3dx9.h>

#define Cylinder_R 10.0f
#define Cylinder_H 30.0f
//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct CYLINDERCOLLIDER
{
	D3DXVECTOR3		pos;		// ポリゴンの位置
	D3DXVECTOR3		rot;		// ポリゴンの向き(回転)
	D3DXVECTOR3		scl;		// ポリゴンの大きさ(スケール)
	D3DXVECTOR3     nor; //法向量
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	
	D3DXVECTOR3    sphereUpPos;
	D3DXVECTOR3    sphereDownPos;
	bool use;
	bool active;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCylinderCollider(void);
void UninitCylinderCollider(void);
void UpdateCylinderCollider(void);
void DrawCylinderCollider(void);
int CreateCylinderCollider(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
void SetPositionCylinderCollider(int nIdxCylinderCollider, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
CYLINDERCOLLIDER *GetCylinderCollider(void);
void DeleteCylinderCollider(int index);
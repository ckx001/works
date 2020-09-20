//=============================================================================
//
// 球体処理 [sphereCollider.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#define SPHERE_R 10.0f
//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct SPHERECOLLIDER
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
HRESULT InitSphereCollider(void);
void UninitSphereCollider(void);
void UpdateSphereCollider(void);
void DrawSphereCollider(void);
int CreateSphereCollider(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
void SetPositionSphereCollider(int nIdxSphereCollider, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
SPHERECOLLIDER *GetSphereCollider(void);
void SetSphereColliderActive(int index, bool judge);
void DeleteSphereCollider(int index);
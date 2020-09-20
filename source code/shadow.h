//=============================================================================
//
// 影処理[shadow.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define MAX_SHADOW 50000

struct SHADOW
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


HRESULT InitShadow(void);
//void DrawShadow(void);
void DrawShadow(int ShadowIndex,int type, int objIndex);
int CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool bStatic);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
SHADOW *RetShadow(void);

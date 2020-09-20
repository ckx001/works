//=============================================================================
//
// プレイヤー　デバッグ処理 [player.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************

#define MAX_PLAYER		(1)					// 僾儗僀儎乕偺悢
#define PLAYER_VALUE_MOVE (5.0f)

struct PLAYER
{
	D3DXVECTOR3			pos;				// 儌僨儖偺埵抲
	D3DXVECTOR3         old_pos;
	D3DXVECTOR3			rot;				// 儌僨儖偺岦偒(夞揮)
	D3DXVECTOR3			scl;				// 儌僨儖偺戝偒偝(僗働乕儖)

	float				spd;				// 堏摦僗僺乕僪
	
	LPDIRECT3DTEXTURE9	pD3DTexture;		// 僥僋僗僠儍傊偺億僀儞僞
	LPD3DXMESH			pD3DXMesh;			// 儊僢僔儏忣曬傊偺億僀儞僞
	LPD3DXBUFFER		pD3DXBuffMat;		// 儅僥儕傾儖忣曬傊偺億僀儞僞
	DWORD				nNumMat;			// 儅僥儕傾儖忣曬偺悢

	D3DXMATRIX			mtxWorld;			// 儚乕儖僪儅僩儕僢僋僗
	int                 SphereColliderIdx;
	bool                ColliderHit;
	int                 face_dir;
};

//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void BackUpPlayer(void);
HRESULT RestartPlayer(void);

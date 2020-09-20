//=============================================================================
//
// SkyBox処理 [MeshSky_Ex.h]
// Author : 
//
//=============================================================================
#pragma once


HRESULT InitMeshSky_Ex(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int nNumBlockX, int nNumBlockZ,
	float fRadius, float fRotY, int nTexType = -1, bool bReverse = false);
void UninitMeshSky_Ex(void);
void UpdateMeshSky_Ex(void);
void DrawMeshSky_Ex(void);


//=============================================================================
//
// 当たり判定関数 [hit_event.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool CheckHitCC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
bool CheckHitRC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
int CheckHitRectCylinder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
int CheckHitCircleCylinder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
bool CheckHitPointCylinder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2);
bool CheckHitCylinderCylinder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
//=============================================================================
//
// Ai処理 [GameAi.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once


void InitGameAi(void);
void UpdateGameAi(void);
D3DXVECTOR3 *RetCurrentCubeBarrierList(void);
void SetCurrentCubeBarrierListIndex(int index);
void SetbEnemyAstart(int index, bool judge);
float RetRotValue(D3DXVECTOR2 p1, D3DXVECTOR2 p2);
void ResetEnemyAi(int num);
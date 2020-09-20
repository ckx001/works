//=============================================================================
//
// quad tree処理 [quarTree.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once


struct QUARTREE
{
	D3DXVECTOR2 MaxValue;
	D3DXVECTOR2 MinValue;
	int CheckIndexList[5000]; //匹配的索引列
	int CheckIndexListLen = -1; //匹配的索引的长度
};


//震地动作影响地面变色
struct HITCUBE
{
	int Cubeindex; //地形砖索引
	bool use;
	D3DXVECTOR3 col = D3DXVECTOR3{ 2.0f, 0.0f, 0.0f };
	D3DXVECTOR3 colStart = D3DXVECTOR3{ 2.0f, 0.0f, 0.0f };
};


void InitQuarTree(float pos_x, float pos_z, int size_x, int size_z);
void ClearAllQuarTree(void);
void UpdateQuarTree(void);
int RetPlayerNowCubeIndex(void);
void InitHitCube(void);
HITCUBE *RetHitCube(void);
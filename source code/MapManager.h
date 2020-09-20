//=============================================================================
//
// 地形変更処理 [MapManager.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define MAX_MAP_CUBE_Z 256
#define MAX_MAP_CUBE_X 256

void OrderMapData(void);
int FindTypeCubeIndex(int z, int x, int sceneId, int CubeType);
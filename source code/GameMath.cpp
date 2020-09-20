//=============================================================================
//
// 数学関数 [GameMath.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "GameMath.h"

//t >=0 && t <= 1;
D3DXVECTOR3 Lerp(D3DXVECTOR3 from, D3DXVECTOR3 to, float t)
{
	D3DXVECTOR3 v = from + (to - from) * t;
	if (t < 0)
		v = from;
	if (t >= 1)
		v = to;
	return v;
}

D3DXVECTOR2 D2_Lerp(D3DXVECTOR2 from, D3DXVECTOR2 to, float t)
{
	D3DXVECTOR2 v = from + (to - from) * t;
	if (t < 0)
		v = from;
	if (t >= 1)
		v = to;
	return v;
}

float F_Lerp(float from, float to, float t)
{
	float v = from + (to - from) * t;
	if (t < 0)
		v = from;
	if (t >= 1)
		v = to;
	return v;
}

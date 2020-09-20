//=============================================================================
//
// 当たり判定関数 [hit_event.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "hit_event.h"
#include "sphereCollider.h"
#include "BoxCollider.h"
#include "cylinderCollider.h"
#include "debugproc.h"
#include "GlobalFunction.h"
#include <math.h>

int ObbSphereCollisionDetect(D3DXVECTOR3& obbCenter,
	D3DXVECTOR3& halfExtents,
	D3DXVECTOR3& sphereCenter,
	float sphereRadius);

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
//=============================================================================
//球体与球体碰撞
bool CheckHitCC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	//pos1とpos2をつなぐベクトルを作る
	float circle_r_1 = size1.x * SPHERE_R;
	float circle_r_2 = size2.x * SPHERE_R;

	D3DXVECTOR3 vDistance = pos1 - pos2;
	//vDistanceのベクトル長を得る
	//length = D3DXVec3Length(&vDistance);
	float length = D3DXVec3Length(&vDistance);

	if (length < (circle_r_1 + circle_r_2))
	{
		return true;
	}
	return false;
}

//立方体与球体碰撞检测
//暂不考虑旋转之间的碰撞
//参数 1 立方体
//参数 2 球体
/////////碰撞条件2可能有bug 无法从上往下碰撞
bool CheckHitRC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	D3DXVECTOR3 min1, max1;
	D3DXVECTOR3 vtx1_1, vtx1_2;

	float circle_r = size2.x * SPHERE_R;

	min1.x = pos1.x - size1.x * NORMAL_SIZE_X;
	min1.y = pos1.y - size1.y * NORMAL_SIZE_Y;
	min1.z = pos1.z - size1.z * NORMAL_SIZE_Z;
	max1.x = pos1.x + size1.x * NORMAL_SIZE_X;
	max1.y = pos1.y + size1.y * NORMAL_SIZE_Y;
	max1.z = pos1.z + size1.z * NORMAL_SIZE_Z;

	//条件满足其1可发生碰撞
	//碰撞条件1 立方体四边+半径 延申出两个矩形 圆心在矩形中则发生碰撞
	//立方体1
	if (pos2.x > min1.x - circle_r && pos2.x < max1.x + circle_r
		&& pos2.z > min1.z  && pos2.z < max1.z
		&& pos2.y > min1.y && pos2.y < max1.y
		)
		return true;
	//立方体2
	if (pos2.x > min1.x  && pos2.x < max1.x
		&& pos2.z > min1.z - circle_r && pos2.z < max1.z + circle_r
		&& pos2.y > min1.y && pos2.y < max1.y
		)
		return true;

	vtx1_1.x = pos1.x - size1.x * NORMAL_SIZE_X;
	vtx1_1.y = pos1.y - size1.y * NORMAL_SIZE_Y;
	vtx1_1.z = pos1.z + size1.z * NORMAL_SIZE_Z;

	vtx1_2.x = pos1.x + size1.x * NORMAL_SIZE_X;
	vtx1_2.y = pos1.y - size1.y * NORMAL_SIZE_Y;
	vtx1_2.z = pos1.z - size1.z * NORMAL_SIZE_Z;
	////碰撞条件 2 球体和距离它最近的一个立方体顶点的点到点之间的距离是否小于等于R
	//D3DXVECTOR3 vDistance_1 = min1;
	//D3DXVECTOR3 vDistance_2 = vtx1_1;
	//D3DXVECTOR3 vDistance_3 = vtx1_2;
	//D3DXVECTOR3 vDistance_4 = max1;
	//float length1;
	//float length2;
	//float length3;
	//float length4;
	//length1 = D3DXVec3Length(&min1);
	//length2 = D3DXVec3Length(&vtx1_1);
	//length3 = D3DXVec3Length(&vtx1_2);
	//length4 = D3DXVec3Length(&max1);
	//if (length1 < circle_r  || length2 < circle_r || length3 < circle_r || length4 < circle_r)
	//	return true;
	//return false;

	//圆碰撞
	int resultDown = (ObbSphereCollisionDetect(D3DXVECTOR3{ pos1.x, pos1.y,pos1.z },
		D3DXVECTOR3{ size1.x * NORMAL_SIZE_X, size1.y * NORMAL_SIZE_Y, size1.z * NORMAL_SIZE_Z },
		D3DXVECTOR3{ pos2.x ,pos2.y , pos2.z },
		circle_r));
	if (resultDown != 0)
		return true;

}


//立方体与胶囊体检测
//暂不考虑旋转之间的碰撞
//参数 1 立方体
//参数 2 胶囊体
//返回值 0 False 1 True 碰撞成功 2 True 上方向向量碰撞
int CheckHitRectCylinder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	D3DXVECTOR3 min1, max1;
	//立方体的最大值和最小值
	min1.x = pos1.x - size1.x * NORMAL_SIZE_X;
	min1.y = pos1.y - size1.y * NORMAL_SIZE_Y;
	min1.z = pos1.z - size1.z * NORMAL_SIZE_Z;
	max1.x = pos1.x + size1.x * NORMAL_SIZE_X;
	max1.y = pos1.y + size1.y * NORMAL_SIZE_Y;
	max1.z = pos1.z + size1.z * NORMAL_SIZE_Z;

	//圆的半径
	float circle_r = size2.x * Cylinder_R;

	//立方体1
	if (pos2.x > min1.x - circle_r && pos2.x < max1.x + circle_r
		&& pos2.z > min1.z  && pos2.z < max1.z
		&& ((pos2.y - Cylinder_H / 2 >= min1.y && pos2.y - Cylinder_H / 2 <= max1.y) ||
		(pos2.y + Cylinder_H / 2 >= min1.y && pos2.y + Cylinder_H / 2 <= max1.y))
		)
		return 1;
	//立方体2
	if (pos2.x > min1.x  && pos2.x < max1.x
		&& pos2.z > min1.z - circle_r && pos2.z < max1.z + circle_r
		&& ((pos2.y - Cylinder_H / 2 >= min1.y && pos2.y - Cylinder_H / 2 <= max1.y) ||
		(pos2.y + Cylinder_H / 2 >= min1.y && pos2.y + Cylinder_H / 2 <= max1.y))
		)
		return 1;

	//上圆的圆心位置
	float circleUpPos = pos2.y + Cylinder_H / 2;
	//下圆的圆心位置
	float circleDownPos = pos2.y - Cylinder_H / 2;

	//下圆碰撞
	int resultDown = (ObbSphereCollisionDetect(D3DXVECTOR3{ pos1.x, pos1.y,pos1.z },
		D3DXVECTOR3{ size1.x * NORMAL_SIZE_X, size1.y * NORMAL_SIZE_Y, size1.z * NORMAL_SIZE_Z },
		D3DXVECTOR3{ pos2.x ,circleDownPos , pos2.z },
		circle_r));
	if( resultDown != 0)
		return resultDown;

	//上圆碰撞
	int resultUp = ObbSphereCollisionDetect(D3DXVECTOR3{ pos1.x, pos1.y,pos1.z },
		D3DXVECTOR3{ size1.x * NORMAL_SIZE_X, size1.y * NORMAL_SIZE_Y, size1.z * NORMAL_SIZE_Z },
		D3DXVECTOR3{ pos2.x ,circleUpPos , pos2.z },
		circle_r);
	if( resultUp != 0 )
		return resultUp;
	
	return 0;
}


//圆与胶囊体检测
//暂不考虑旋转之间的碰撞
//暂不考虑胶囊体上下圆的碰撞
//参数 1 圆
//参数 2 胶囊体
//返回值 0 False 1 True 碰撞成功
int CheckHitCircleCylinder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
    //圆的半径
	float circle1_r = size1.x * Cylinder_R;
	//圆柱的半径
	float circle2_r = size2.x * Cylinder_R;

	D3DXVECTOR3 vDistance = pos1 - pos2;

	float length = D3DXVec3Length(&vDistance);

	if (length < (circle1_r + circle2_r))
		return 1;

	return 0;
}


//胶囊体与胶囊体检测
//暂不考虑旋转之间的碰撞
//暂不考虑胶囊体上下圆的碰撞
//参数 1 胶囊体
//参数 2 胶囊体
//返回值 0 False 1 True 碰撞成功
bool CheckHitCylinderCylinder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	//obj1圆柱的半径
	float circle1_r = size1.x * Cylinder_R;
	//obj2圆柱的半径
	float circle2_r = size2.x * Cylinder_R;

	//高的一半
	float Obj1High = Cylinder_H / 2 * size1.y;
	float Obj2High = Cylinder_H / 2 * size2.y;

	D3DXVECTOR3 vDistance = D3DXVECTOR3(pos1.x - pos2.x, 0, pos1.z - pos2.z);

	float length = D3DXVec3Length(&vDistance);

	if (pos1.y + Obj1High >= pos2.y - Obj2High && pos1.y - Obj1High <= pos2.y + Obj2High)
	{
		if (length < (circle1_r + circle2_r))
			return true;
	}

	return false;
}


//点与胶囊体的碰撞
//pos1 Point pos
//pos2 胶囊体pos
//size2  胶囊体size
bool CheckHitPointCylinder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{
	//圆柱的半径
	float circle2_r = size2.x * Cylinder_R;

	D3DXVECTOR3 vDistance = D3DXVECTOR3(pos1.x - pos2.x, 0 , pos1.z - pos2.z);
	float length = D3DXVec3Length(&vDistance);

	//点在圆柱内
	if (pos1.y <= pos2.y + Cylinder_H / 2 * size2.y && pos1.y >= pos1.y - Cylinder_H / 2 * size2.y)
	{
		if(length < circle2_r)
		    return true;
	}
	return false;
}

//obbCenter表示包围盒的中心位置
//obbAxes[3]表示包围盒XYZ三个坐标轴方向上的朝向, 应该是正交的, 否则意思着立方体有扭曲变形.并且应该是单位向量
//halfExtents表示包围盒三个方向上的长度.
//返回值 0 False 1 True 碰撞成功 2 True 上方向向量碰撞
int ObbSphereCollisionDetect(D3DXVECTOR3& obbCenter,
	D3DXVECTOR3& halfExtents,
	D3DXVECTOR3& sphereCenter,
	float sphereRadius)
{
	D3DXVECTOR3 obbAxes[] = {
	D3DXVECTOR3{ 0, 1.0f, 0 },
	D3DXVECTOR3{ 1.0f, 0, 0 },
	D3DXVECTOR3{ 0, 0, 1.0f },
	};

	D3DXVECTOR3 kDiff = sphereCenter - obbCenter;

	float fSide[3] = { 1.0f, 1.0f, 1.0f };
	float fS[3] = { 0.0f, 0.0f, 0.0f };
	float fD = 0.0f;
	int result = 0;

	if (D3DXVec3Dot(&kDiff, &obbAxes[0]) < 0.0f)
	{
		fS[0] = D3DXVec3Dot(&(sphereCenter - (obbCenter - (obbAxes[0] * halfExtents.x))), &obbAxes[0]);

		if (fS[0] <= -sphereRadius)
			return false;

		else if (fS[0] < 0.0f)
			fD += fS[0] * fS[0];

		fS[0] += sphereRadius;

		result = 2;
	}
	else
	{
		fS[0] = D3DXVec3Dot(&((obbCenter + (obbAxes[0] * halfExtents.x)) - sphereCenter), &obbAxes[0]);

		if (fS[0] <= -sphereRadius)
			return false;

		else if (fS[0] < 0.0f)
			fD += fS[0] * fS[0];

		fS[0] += sphereRadius;
		fSide[0] = -1.0f;

		result = 2;
	}

	if (D3DXVec3Dot(&kDiff, &obbAxes[1]) < 0.0f)
	{
		fS[1] = D3DXVec3Dot(&(sphereCenter - (obbCenter - (obbAxes[1] * halfExtents.y))), &obbAxes[1]);

		if (fS[1] <= -sphereRadius)
			return false;

		else if (fS[1] < 0.0f)
			fD += fS[1] * fS[1];

		fS[1] += sphereRadius;
	}
	else
	{
		fS[1] = D3DXVec3Dot(&((obbCenter + (obbAxes[1] * halfExtents.y)) - sphereCenter), &obbAxes[1]);

		if (fS[1] <= -sphereRadius)
			return false;

		else if (fS[1] < 0.0f)
			fD += fS[1] * fS[1];

		fS[1] += sphereRadius;
		fSide[1] = -1.0f;
	}

	if (D3DXVec3Dot(&kDiff, &obbAxes[2]) < 0.0f)
	{
		fS[2] = D3DXVec3Dot(&(sphereCenter - (obbCenter - (obbAxes[2] * halfExtents.z))), &obbAxes[2]);

		if (fS[2] <= -sphereRadius)
			return false;

		else if (fS[2] < 0.0f)
			fD += fS[2] * fS[2];

		fS[2] += sphereRadius;
	}
	else
	{
		fS[2] = D3DXVec3Dot(&((obbCenter + (obbAxes[2] * halfExtents.z)) - sphereCenter), &obbAxes[2]);

		if (fS[2] <= -sphereRadius)
			return false;

		else if (fS[2] < 0.0f)
			fD += fS[2] * fS[2];

		fS[2] += sphereRadius;
		fSide[2] = -1.0f;
	}

	//return (fD <= (sphereRadius * sphereRadius));
	if (fD <= (sphereRadius * sphereRadius))
	{
		if (result == 2)
			return result;
		else
			return 1;
	}
	return 0;
}
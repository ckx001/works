//=============================================================================
//
// カプセル体処理 [cylinderCollider.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera_Ex.h"
//#include "light.h"
#include "CylinderCollider.h"
#include "GlobalFunction.h"
#include "debugproc.h"
//#include "shadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE		(5.0f)											// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 回転量
#define MAX_CYLINDERCOLLIDER (500)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexCylinderCollider(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR col);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CYLINDERCOLLIDER				g_CylinderCollider[MAX_CYLINDERCOLLIDER];						// ポリゴンデータ
static LPD3DXMESH g_cube = NULL;
static LPD3DXMESH g_sphereUp = NULL;
static LPD3DXMESH g_sphereDown = NULL;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCylinderCollider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//参数2 前面半径
	//参数3 后面半径
	//参数4 高
	//参数 5 Slices
	//参数6 Stacks
	D3DXCreateCylinder(pDevice, Cylinder_R, Cylinder_R, Cylinder_H, 64, 3, &g_cube, NULL);	//创建圆柱
	if (FAILED(D3DXCreateSphere(pDevice, Cylinder_R, 20, 20, &g_sphereUp, NULL)))	//球体的创建
		return E_FAIL;
	if (FAILED(D3DXCreateSphere(pDevice, Cylinder_R, 20, 20, &g_sphereDown, NULL)))	//球体的创建
		return E_FAIL;
	for (int i = 0; i < MAX_CYLINDERCOLLIDER; i++)
	{
		g_CylinderCollider[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_CylinderCollider[i].rot = D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f);
		g_CylinderCollider[i].use = false;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCylinderCollider(void)
{
	g_sphereUp->Release();
	g_sphereDown->Release();
	g_cube->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCylinderCollider(void)
{

	for (int i = 0; i < MAX_CYLINDERCOLLIDER; i++)
	{
		if (g_CylinderCollider[i].use == true)
		{
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCylinderCollider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, g_mtxWorld;
	///////////透明//////////
	//int						g_nAlpha = 0x0;
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, g_nAlpha);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	///////////////////////////////
	//float cc = ::timeGetTime();
	//PrintDebugProc("a?:%f\n", cc);
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
	for (int i = 0; i < MAX_CYLINDERCOLLIDER; i++)
	{
		if (g_CylinderCollider[i].use)
		{
			if (g_CylinderCollider[i].active == true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWorld);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, g_CylinderCollider[i].scl.x, g_CylinderCollider[i].scl.y, g_CylinderCollider[i].scl.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_CylinderCollider[i].rot.y, g_CylinderCollider[i].rot.x, g_CylinderCollider[i].rot.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, g_CylinderCollider[i].pos.x, g_CylinderCollider[i].pos.y, g_CylinderCollider[i].pos.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
#ifdef COLLIDER_SWITCH
				g_cube->DrawSubset(0);
#endif
				D3DXMatrixIdentity(&g_mtxWorld);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, g_CylinderCollider[i].scl.x, g_CylinderCollider[i].scl.y, g_CylinderCollider[i].scl.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_CylinderCollider[i].rot.y, g_CylinderCollider[i].rot.x, g_CylinderCollider[i].rot.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, g_CylinderCollider[i].sphereUpPos.x, g_CylinderCollider[i].sphereUpPos.y, g_CylinderCollider[i].sphereUpPos.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
#ifdef COLLIDER_SWITCH
				g_sphereUp->DrawSubset(0);
#endif
				D3DXMatrixIdentity(&g_mtxWorld);
				// スケールを反映
				D3DXMatrixScaling(&mtxScl, g_CylinderCollider[i].scl.x, g_CylinderCollider[i].scl.y, g_CylinderCollider[i].scl.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_CylinderCollider[i].rot.y, g_CylinderCollider[i].rot.x, g_CylinderCollider[i].rot.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, g_CylinderCollider[i].sphereDownPos.x, g_CylinderCollider[i].sphereDownPos.y, g_CylinderCollider[i].sphereDownPos.z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
#ifdef COLLIDER_SWITCH
				g_sphereDown->DrawSubset(0);
#endif
			}
		}
	}
	pDevice->SetRenderState(D3DRS_ZENABLE, false);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCylinderCollider(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR col)
{
	return S_OK;
}


int CreateCylinderCollider(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	int nIdxCylinderCollider = -1;

	for (int nCntCylinderCollider = 0; nCntCylinderCollider < MAX_CYLINDERCOLLIDER; nCntCylinderCollider++)
	{
		if (!g_CylinderCollider[nCntCylinderCollider].use)
		{
			g_CylinderCollider[nCntCylinderCollider].pos = pos;
			g_CylinderCollider[nCntCylinderCollider].rot = rot;
			g_CylinderCollider[nCntCylinderCollider].scl = scl;
			g_CylinderCollider[nCntCylinderCollider].use = true;
			g_CylinderCollider[nCntCylinderCollider].active = false;

			g_CylinderCollider[nCntCylinderCollider].sphereUpPos = D3DXVECTOR3{ pos.x, pos.y + Cylinder_H / 2 * scl.y, pos.z };
			g_CylinderCollider[nCntCylinderCollider].sphereDownPos = D3DXVECTOR3{ pos.x, pos.y + Cylinder_H / 2 * scl.y, pos.z };
			nIdxCylinderCollider = nCntCylinderCollider;
			break;
		}
	}
	return nIdxCylinderCollider;
}

void SetPositionCylinderCollider(int nIdxCylinderCollider, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	if (nIdxCylinderCollider < 0) return;

	g_CylinderCollider[nIdxCylinderCollider].active = true;
	g_CylinderCollider[nIdxCylinderCollider].pos = pos;
	g_CylinderCollider[nIdxCylinderCollider].rot = rot;
	g_CylinderCollider[nIdxCylinderCollider].scl = scl;

	g_CylinderCollider[nIdxCylinderCollider].sphereUpPos = D3DXVECTOR3{ pos.x, pos.y + Cylinder_H / 2 * scl.y, pos.z };
	g_CylinderCollider[nIdxCylinderCollider].sphereDownPos = D3DXVECTOR3{ pos.x, pos.y - Cylinder_H / 2 * scl.y, pos.z };
}

CYLINDERCOLLIDER *GetCylinderCollider(void)
{
	return &(g_CylinderCollider[0]);
}


//删除指定索引的触发器
void DeleteCylinderCollider(int index)
{
	g_CylinderCollider[index].use = false;
}
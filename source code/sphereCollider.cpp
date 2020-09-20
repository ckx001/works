//=============================================================================
//
// 球体処理 [sphereCollider.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera_Ex.h"
#include "light_Ex.h"
#include "SphereCollider.h"
#include "debugproc.h"
#include "GlobalFunction.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE		(5.0f)											// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 回転量
#define MAX_SPHERECOLLIDER (500)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSphereCollider(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR col);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static SPHERECOLLIDER				g_SphereCollider[MAX_SPHERECOLLIDER];						// ポリゴンデータ
//D3DXMATRIX				g_mtxWorldSphereCollider[MAX_SPHERECOLLIDER];
static LPD3DXMESH g_sphere = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSphereCollider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//参数 2 前面半径
	//参数 3 Slices
	//参数 4 Stacks
	if(FAILED(D3DXCreateSphere(pDevice, SPHERE_R, 20, 20, &g_sphere, NULL)))	//球体的创建
		return E_FAIL;
	for (int i = 0; i < MAX_SPHERECOLLIDER; i++)
	{
		g_SphereCollider[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_SphereCollider[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_SphereCollider[i].use = false;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSphereCollider(void)
{
	g_sphere->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSphereCollider(void)
{

	for (int i = 0; i < MAX_SPHERECOLLIDER; i++)
	{
		if (g_SphereCollider[i].use == true)
		{
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSphereCollider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX	g_mtxWorld;
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
	for (int i = 0; i < MAX_SPHERECOLLIDER; i++)
	{
		if (g_SphereCollider[i].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_SphereCollider[i].scl.x, g_SphereCollider[i].scl.y, g_SphereCollider[i].scl.z);
			D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_SphereCollider[i].rot.y, g_SphereCollider[i].rot.x, g_SphereCollider[i].rot.z);
			D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_SphereCollider[i].pos.x, g_SphereCollider[i].pos.y, g_SphereCollider[i].pos.z);
			D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			if (g_SphereCollider[i].active == true)
			{
#ifdef COLLIDER_SWITCH
			    g_sphere->DrawSubset(0);
#endif 
			}
		}
	}
	pDevice->SetRenderState(D3DRS_ZENABLE, false);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSphereCollider(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR col)
{
	return S_OK;
}


int CreateSphereCollider(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	int nIdxSphereCollider = -1;

	for (int nCntSphereCollider = 0; nCntSphereCollider < MAX_SPHERECOLLIDER; nCntSphereCollider++)
	{
		if (!g_SphereCollider[nCntSphereCollider].use)
		{
			nCntSphereCollider;
			g_SphereCollider[nCntSphereCollider].pos = pos;
			g_SphereCollider[nCntSphereCollider].rot = rot;
			g_SphereCollider[nCntSphereCollider].scl = scl;
			g_SphereCollider[nCntSphereCollider].use = true;
			g_SphereCollider[nCntSphereCollider].active = false;

			nIdxSphereCollider = nCntSphereCollider;
			break;
		}
	}
	return nIdxSphereCollider;
}

void SetPositionSphereCollider(int nIdxSphereCollider, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	if (nIdxSphereCollider < 0) return;

	g_SphereCollider[nIdxSphereCollider].active = true;
	g_SphereCollider[nIdxSphereCollider].pos = pos;
	g_SphereCollider[nIdxSphereCollider].rot = rot;
	g_SphereCollider[nIdxSphereCollider].scl = scl;

}

SPHERECOLLIDER *GetSphereCollider(void)
{
	return &(g_SphereCollider[0]);
}


void SetSphereColliderActive( int index, bool judge )
{
	g_SphereCollider[index].active = judge;
}



//删除指定索引的触发器
void DeleteSphereCollider(int index)
{
	g_SphereCollider[index].use = false;
}
//=============================================================================
//
// ポリゴン表示処理 [BoxCollider.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera_Ex.h"
#include "light_Ex.h"
#include "BoxCollider.h"
#include "GlobalFunction.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE		(5.0f)											// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 回転量
#define MAX_BOXCOLLIDER (100)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBoxCollider(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR col);
//void UpdateVertex(int index, D3DXCOLOR col);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;	// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		g_pD3DTexture_1 = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff = NULL;	// 頂点バッファへのポインタ

static BOXCOLLIDER				g_BoxCollider[MAX_BOXCOLLIDER_NUM][MAX_BOXCOLLIDER];						// ポリゴンデータ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoxCollider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_NUM_BOXCOLLIDER,				// ファイルの名前
		&g_pD3DTexture);				// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_NUM_BOXCOLLIDER_1,				// ファイルの名前
		&g_pD3DTexture_1);				// 読み込むメモリー

	// 位置・回転・スケールの初期設定
	//g_BoxCollider[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_BoxCollider[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_BoxCollider[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
#ifdef _DEBUG
	MakeVertexBoxCollider(pDevice, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#else
	MakeVertexBoxCollider(pDevice, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
#endif
	for (int num = 0; num < MAX_BOXCOLLIDER_NUM; num++)
	{
		for (int i = 0; i < MAX_BOXCOLLIDER; i++)
		{
			g_BoxCollider[num][i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_BoxCollider[num][i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BoxCollider[num][i].use = false;
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoxCollider(void)
{
	if (g_pD3DTexture != NULL)
	{// テクスチャの開放
		g_pD3DTexture->Release();
		g_pD3DTexture = NULL;
	}

	if (g_pD3DTexture_1 != NULL)
	{// テクスチャの開放
		g_pD3DTexture_1->Release();
		g_pD3DTexture_1 = NULL;
	}

	if (g_pD3DVtxBuff != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuff->Release();
		g_pD3DVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoxCollider(void)
{

	//for (int i = 0; i < MAX_BOXCOLLIDER; i++)
	//{
	//	if (g_BoxCollider[i].use == true)
	//	{
	//		//UpdateVertex(i);
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoxCollider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX	g_mtxWorld;
	///////////////////////////////
	for (int num = 0; num < MAX_BOXCOLLIDER_NUM; num++)
	{
		for (int i = 0; i < MAX_BOXCOLLIDER; i++)
		{
			if (g_BoxCollider[num][i].use)
			{
				if (g_BoxCollider[num][i].active == true)
				{
				    // ワールドマトリックスの初期化
				    D3DXMatrixIdentity(&g_mtxWorld);
				    
				    // スケールを反映
				    //D3DXMatrixScaling(&mtxScl, g_BoxCollider[i].scl.x, g_BoxCollider[i].scl.y, g_BoxCollider[i].scl.z);
				    //D3DXMatrixMultiply(&g_BoxCollider[i].mtxWorld, &g_BoxCollider[i].mtxWorld, &mtxScl);
				    // スケールを反映
				    D3DXMatrixScaling(&mtxScl, g_BoxCollider[num][i].scl.x, g_BoxCollider[num][i].scl.y, g_BoxCollider[num][i].scl.z);
				    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);
				    
				    // 回転を反映
				    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_BoxCollider[num][i].rot.y, g_BoxCollider[num][i].rot.x, g_BoxCollider[num][i].rot.z);
				    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);
				    
				    // 移動を反映
				    D3DXMatrixTranslation(&mtxTranslate, g_BoxCollider[num][i].pos.x, g_BoxCollider[num][i].pos.y, g_BoxCollider[num][i].pos.z);
				    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);
				    
				    // ワールドマトリックスの設定
				    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
				    
				    // 頂点バッファをデバイスのデータストリームにバインド
				    pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));
				    
				    // 頂点フォーマットの設定
				    pDevice->SetFVF(FVF_VERTEX_3D);
				    
				    // テクスチャの設定
				    //pDevice->SetTexture(0, g_pD3DTexture);
				    
				    pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

					// ポリゴンの描画
#ifdef COLLIDER_SWITCH
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, NUM_POLYGON);
					//pDevice->SetTexture(0, g_pD3DTexture_1);
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);
#endif
				}
			}
		}
	}
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBoxCollider(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR col)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * 6,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuff,				// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}
	// 创建索引缓存
	//if (FAILED(pDevice->CreateIndexBuffer(6 * sizeof(WORD), 0,
	//	D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL)))
	//{
	//	return E_FAIL;
	//}

	{//頂点バッファの中身を埋める
		for (int index = 0; index < MAX_BOXCOLLIDER; index++)
		{
			VERTEX_3D *pVtx;
			g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			//////////底/////////
			// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(-g_BoxCollider[i].size.x, -g_BoxCollider[i].size.y, g_BoxCollider[i].size.z);
			//pVtx[1].vtx = D3DXVECTOR3(g_BoxCollider[i].size.x, -g_BoxCollider[i].size.y, g_BoxCollider[i].size.z);
			//pVtx[2].vtx = D3DXVECTOR3(-g_BoxCollider[i].size.x, -g_BoxCollider[i].size.y, -g_BoxCollider[i].size.z);
			//pVtx[3].vtx = D3DXVECTOR3(g_BoxCollider[i].size.x, -g_BoxCollider[i].size.y, -g_BoxCollider[i].size.z);
			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 拡散光の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;
			//////后//////
			//頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 拡散光の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//////////////
			pVtx += 4;
			//////右//////
			//頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 拡散光の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//////////////
			pVtx += 4;
			//////前//////
			//頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);


			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 拡散光の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//////////////
			pVtx += 4;
			//////左//////
			//頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 拡散光の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//////////////
			pVtx += 4;
			//////上//////
			//頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 拡散光の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//////////////
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuff->Unlock();
	}
	return S_OK;
}


//STATIC 是否为静态 是就不需要执行SetBoxCollider函数
int CreateBoxCollider(int Index , D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 scl,bool bStatic)
{
	int nIdxBoxCollider = -1;

	for (int nCntBoxCollider = 0; nCntBoxCollider < MAX_BOXCOLLIDER; nCntBoxCollider++)
	{
		if (!g_BoxCollider[Index][nCntBoxCollider].use)
		{
			g_BoxCollider[Index][nCntBoxCollider].pos = pos;
			g_BoxCollider[Index][nCntBoxCollider].rot = rot;
			g_BoxCollider[Index][nCntBoxCollider].scl = scl;
			g_BoxCollider[Index][nCntBoxCollider].use = true;
			if(bStatic)
				g_BoxCollider[Index][nCntBoxCollider].active = true;
			else
			    g_BoxCollider[Index][nCntBoxCollider].active = false;

			nIdxBoxCollider = nCntBoxCollider;
			break;
		}
	}
	return nIdxBoxCollider;
}

void SetPositionBoxCollider(int Index, int nIdxBoxCollider, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 scl)
{
	if (nIdxBoxCollider < 0) return;	

	g_BoxCollider[Index][nIdxBoxCollider].active = true;
	g_BoxCollider[Index][nIdxBoxCollider].pos = pos;
	g_BoxCollider[Index][nIdxBoxCollider].rot = rot;
	g_BoxCollider[Index][nIdxBoxCollider].scl = scl;
//ifdef _DEBUG
//	UpdateVertex(nIdxBoxCollider, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//else
//	UpdateVertex(nIdxBoxCollider, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
//endif
}


//void UpdateVertex( int index, D3DXCOLOR col )
//{
//	//for (int index = 0; index < MAX_BOXCOLLIDER; index++)
//	//{
//		if (g_BoxCollider[index].use)
//		{
//			VERTEX_3D *pVtx;
//			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//			g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//			//////////底/////////
//			// 頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[1].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[2].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//
//			// 法線ベクトルの設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//			// 拡散光の設定
//			pVtx[0].diffuse = col;
//			pVtx[1].diffuse = col;
//			pVtx[2].diffuse = col;
//			pVtx[3].diffuse = col;
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//			pVtx += 4;
//			//////后//////
//			//頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			pVtx[1].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			pVtx[2].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			// 法線ベクトルの設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//			// 拡散光の設定
//			pVtx[0].diffuse = col;
//			pVtx[1].diffuse = col;
//			pVtx[2].diffuse = col;
//			pVtx[3].diffuse = col;
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//			//////////////
//			pVtx += 4;
//			//////右//////
//			//頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[1].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			pVtx[2].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//
//			// 法線ベクトルの設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//			// 拡散光の設定
//			pVtx[0].diffuse = col;
//			pVtx[1].diffuse = col;
//			pVtx[2].diffuse = col;
//			pVtx[3].diffuse = col;
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//			//////////////
//			pVtx += 4;
//			//////前//////
//			//頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[1].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[2].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//
//
//			// 法線ベクトルの設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//			// 拡散光の設定
//			pVtx[0].diffuse = col;
//			pVtx[1].diffuse = col;
//			pVtx[2].diffuse = col;
//			pVtx[3].diffuse = col;
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//			//////////////
//			pVtx += 4;
//			//////左//////
//			//頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[1].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[2].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			pVtx[3].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, -NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//
//			// 法線ベクトルの設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//			// 拡散光の設定
//			pVtx[0].diffuse = col;
//			pVtx[1].diffuse = col;
//			pVtx[2].diffuse = col;
//			pVtx[3].diffuse = col;
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//			//////////////
//			pVtx += 4;
//			//////上//////
//			//頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			pVtx[1].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, NORMAL_SIZE_Z);
//			pVtx[2].vtx = D3DXVECTOR3(-NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//			pVtx[3].vtx = D3DXVECTOR3(NORMAL_SIZE_X, NORMAL_SIZE_Y, -NORMAL_SIZE_Z);
//
//			// 法線ベクトルの設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//			// 拡散光の設定
//			pVtx[0].diffuse = col;
//			pVtx[1].diffuse = col;
//			pVtx[2].diffuse = col;
//			pVtx[3].diffuse = col;
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//			//////////////
//			g_pD3DVtxBuff->Unlock();
//		}
//	//}
//}

BOXCOLLIDER *GetBoxCollider(int Index)
{
	return &(g_BoxCollider[Index][0]);
}
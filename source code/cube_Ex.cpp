//=============================================================================
//
// 立方体処理[cube_Ex.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "cube_Ex.h"
#include "GlobalFunction.h"
#include "input.h"
#include "camera_Ex.h"
#include "light_Ex.h"
#include "debugproc.h"
#include "Serialize.h"
#include "DataBuf.h"
#include "debugproc.h"
#include "GameEvent.h"
#include "shadow.h"
#include "player1.h"
#include "Scene.h"
#include "Enemy_wolf.h"
#include "EnemyCannibal.h"
#include "EnemyBossWolf.h"
#include "quarTree.h"
#include "GameMath.h"
//#include "model.h"
#include <atlstr.h>
#include <vector>

using namespace std;
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	TEXTURE_OBJ_CUBE	"data/TEXTURE/desert_test.jpg"						// 撉傒崬傓僥僋僗僠儍僼傽僀儖柤
#define	TEXTURE_OBJ_CUBE_GRASS_SIDE	"data/TEXTURE/grass_side.jpg"
#define OBJ_CUBE_SHADER_EX_PATH "data/SHADER/D3DObjCube.fx"

#define VALUE_AMBIENT_COEFFICIENT (1.0f)

#define	VALUE_MOVE		(5.0f)											// 堏摦検
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 夞揮検

#define	SIZE_X			(20.0f)										// 抧柺偺僒僀僘(X曽岦)
#define	SIZE_Y			(20.0f)										// 抧柺偺僒僀僘(Y曽岦)
#define	SIZE_Z			(20.0f)										// 抧柺偺僒僀僘(Z曽岦)						

#define HITCUBE_COL_TIME (0.01f) //渐变速度
//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
static HRESULT MakeVertexObjCube(LPDIRECT3DDEVICE9 pDevice);
static bool LoadObjCubeShader_Ex();
//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;	// 僥僋僗僠儍傊偺億僀儞僞
static LPDIRECT3DTEXTURE9		g_pD3DTexture_grass_side = NULL;
//static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff = NULL;	// 捀揰僶僢僼傽傊偺億僀儞僞
static LPDIRECT3DTEXTURE9		g_pD3DTextureBackup = NULL;
static D3DXHANDLE ViewProjMatrixHandle = NULL;
static D3DXHANDLE PixTextureHandle = NULL;
static D3DXHANDLE TransparencyFloatHandle = NULL;
static D3DXHANDLE LightPositionHandle = NULL;
static D3DXHANDLE LightDiffuseHandle = NULL;
static D3DXHANDLE LightAmbientHandle = NULL;
static D3DXHANDLE ViewMatViewHandle = NULL;
static D3DXHANDLE AmbientCoefficientHandle = NULL;
static ID3DXEffect* g_pEffect = NULL;

static OBJ_CUBE_EX					g_ObjCube[OBJ_CUBE_MAX];						// 億儕僑儞僨乕僞

static float HitCubeColTime[100];
static int EnemyBossWolfSceneCubeIndex[300];
static int EnemyBossWolfSceneCubeIndexNum = 0;
//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitObjCube_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 僥僋僗僠儍偺撉傒崬傒
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_OBJ_CUBE,				
		&g_pD3DTexture);			
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_OBJ_CUBE_GRASS_SIDE,
		&g_pD3DTexture_grass_side);

	MakeVertexObjCube(pDevice);

	//for (int i = 0; i < OBJ_CUBE_MAX; i++)
	//{
	//	if (g_ObjCube[i].use)
	//	{
	//		int ChgListIndex = RetTexuturePointListMapIndex(g_ObjCube[i].TexturePathNameLen, g_ObjCube[i].TexturePathName);
	//		ReLoadObjCube_Ex(i, ChgListIndex);
	//	}
	//}
	if (FAILED(LoadObjCubeShader_Ex()))
	{
		return E_FAIL;
	}


	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		if (g_ObjCube[i].use)
		{
			g_ObjCube[i].ShadowIdx = CreateShadow(g_ObjCube[i].pos, g_ObjCube[i].rot, g_ObjCube[i].scl, true);
		}
	}

	for (int a = 0; a < 100; a++)
	{
		HitCubeColTime[a] = 0.0f;
	}
	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitObjCube_Ex(void)
{
	if (g_pD3DTexture != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_pD3DTexture->Release();
		g_pD3DTexture = NULL;
	}
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		//SAFE_RELEASE(g_ObjCube[i].g_pD3DTexture);
		//SAFE_DELETE(g_ObjCube[i].g_pD3DTexture);
		SAFE_RELEASE(g_ObjCube[i].g_pD3DVtxBuff);
	}
	//SAFE_RELEASE(g_pEffect);
	g_pEffect->Release();

}

//=============================================================================
// 峏怴張棟
//=============================================================================
void UpdateObjCube_Ex(void)
{
	CAMERA_EX *cam = GetCamera_Ex();
	D3DLIGHT9 *Light = RetLight();
	HITCUBE *HitCube = RetHitCube();

	for (int a = 0; a < 100; a++)
	{
		if (HitCube[a].use)
		{
			HitCubeColTime[a] += HITCUBE_COL_TIME;
			D3DXVECTOR3 Col = Lerp(HitCube[a].colStart, D3DXVECTOR3{ Light[0].Ambient.r, Light[0].Ambient.g,Light[0].Ambient.b }, HitCubeColTime[a]);

			HitCube[a].col = Col;
			
			if (HitCubeColTime[a] >= 1.0f)
			{
				HitCube[a].use = false;
				HitCube[a].col = HitCube[a].colStart;
				HitCubeColTime[a] = 0.0f;
			}
		}
	}
}


//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawObjCube_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SHADOW *Shadow = RetShadow();
	HITCUBE *HitCube = RetHitCube();
	int CurrentSceneIndex = RetCurrentSceneIndex();
	D3DLIGHT9 *Light = RetLight();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX matWorld, matView, matProj;

	pDevice->SetRenderState(D3DRS_ZENABLE, true);


	for (int i = 0; i < OBJ_CUBE_EX_MAX; i++)
	{
		if (g_ObjCube[i].use)
		{
			if (CubeInFrustum(g_ObjCube[i].pos))
			{
				///shadow绘制顺序 0.0f地面立方体 > shadow > 超过0.0f的立方体
				if (g_ObjCube[i].pos.y <= 10.0f)
				{
					if (!strncmp(g_ObjCube[i].TexturePathName, "brige.jpg", 9))
					{
						g_ObjCube[i].rot.y = 0.0f;
						g_ObjCube[i].rot.x = 0.0f;
						g_ObjCube[i].rot.z = 0.0f;
					}

					// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
					D3DXMatrixIdentity(&matWorld);

					// 僗働乕儖傪斀塮
					D3DXMatrixScaling(&mtxScl, g_ObjCube[i].scl.x, g_ObjCube[i].scl.y, g_ObjCube[i].scl.z);
					D3DXMatrixMultiply(&matWorld, &matWorld, &mtxScl);

					// 夞揮傪斀塮
					D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ObjCube[i].rot.y, g_ObjCube[i].rot.x, g_ObjCube[i].rot.z);
					D3DXMatrixMultiply(&matWorld, &matWorld, &mtxRot);

					// 堏摦傪斀塮
					D3DXMatrixTranslation(&mtxTranslate, g_ObjCube[i].pos.x, g_ObjCube[i].pos.y, g_ObjCube[i].pos.z);
					D3DXMatrixMultiply(&matWorld, &matWorld, &mtxTranslate);

					pDevice->GetTransform(D3DTS_VIEW, &matView);
					pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

					D3DXMATRIX matWVP = matWorld * matView * matProj;
					if (AmbientCoefficientHandle)
						g_pEffect->SetFloat(AmbientCoefficientHandle, VALUE_AMBIENT_COEFFICIENT);
					if (ViewMatViewHandle)
						g_pEffect->SetMatrix(ViewMatViewHandle, &(matView));
					if (LightPositionHandle)
						g_pEffect->SetVector("fvLightPosition", &D3DXVECTOR4{ Light[0].Direction, 0.0f });
					if (LightDiffuseHandle)
						g_pEffect->SetVector("fvDiffuse", &D3DXVECTOR4{ Light[0].Diffuse.r,Light[0].Diffuse.g,Light[0].Diffuse.b,1.0f });
					if (LightAmbientHandle)
					{
						g_pEffect->SetVector("fvAmbient", &D3DXVECTOR4{ Light[0].Ambient.r,Light[0].Ambient.g,Light[0].Ambient.b,1.0f });
						for (int a = 0; a < 100; a++)
						{
							if (HitCube[a].use)
							{
								if (HitCube[a].Cubeindex == i)
									g_pEffect->SetVector("fvAmbient", &D3DXVECTOR4{ HitCube[a].col, 1.0f});
							}
						}
						if (CurrentSceneIndex == SCENE_SCENE_HOME)
						{
							if(i == 702 || i == 738 || i == 666)
							    g_pEffect->SetVector("fvAmbient", &D3DXVECTOR4{ 0, 0, 2.0f,1.0f });
						}
					}
					if (ViewProjMatrixHandle)
						g_pEffect->SetMatrix(ViewProjMatrixHandle, &matWVP);
					if (PixTextureHandle)
					{
						if (g_ObjCube[i].g_pD3DTexture != NULL)
						{
							g_pEffect->SetTexture("g_txScene", g_ObjCube[i].g_pD3DTexture);
						}
					}
					if (TransparencyFloatHandle)
					{
						g_pEffect->SetFloat("Transparency", 1.0f);
						if (CurrentSceneIndex == SCENE_SCENE_GRASS)
						{
							if (!strncmp(g_ObjCube[i].TexturePathName, "brige.jpg", 9))
							{
								g_pEffect->SetFloat("Transparency", 0.5f);
							}
						}
					}
					g_pEffect->SetTechnique("TextureTech1");
					UINT cPasses = 0;
					g_pEffect->Begin(&cPasses, 0);
					for (UINT p = 0; p < cPasses; ++p)
					{
						g_pEffect->BeginPass(p);
						g_pEffect->CommitChanges();
						pDevice->SetStreamSource(0, g_ObjCube[i].g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

						pDevice->SetFVF(FVF_VERTEX_3D);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);
						if (CurrentSceneIndex == SCENE_SCENE_GRASS)
						{
							if (strncmp(g_ObjCube[i].TexturePathName, "brige.jpg", 9))
							{
								if (PixTextureHandle)
								{
									g_pEffect->SetTexture("g_txScene", g_pD3DTexture_grass_side);
								}
							}
							g_pEffect->CommitChanges();
						}
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);
					}
					g_pEffect->End();
				}
			}
		}
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	// 开启模板写，可以写模板和后台缓存，为了防止二次融合
	pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x0);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	// 深度检测失败时候，保留深度值
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	// 模板检测失败，保留值
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	// 模板检测设置为增长
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR); // increment to 1

	// 融合因子取决于alpha值
	DWORD dwArg1Value = 0;
	pDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwArg1Value);
	DWORD dwOpValue = 0;
	pDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwOpValue);
	// 默认是alpha来自于D3DTA_TEXTURE;  D3DTSS_ALPHAOP来自于D3DTSS_COLORARG1;
	pDevice->SetRenderState(D3DRS_ZENABLE, false);

	for (int i = 0; i < OBJ_CUBE_EX_MAX; i++)
	{
		if (g_ObjCube[i].use)
		{
			if (CubeInFrustum(Shadow[g_ObjCube[i].ShadowIdx].pos))
			{
				if (g_ObjCube[i].pos.y >= 10.0f)
					DrawShadow(g_ObjCube[i].ShadowIdx, OBJ_EN_TERRAIN_TYPE, i);
			}
		}
	}
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	pDevice->SetRenderState(D3DRS_STENCILENABLE, false);

	///////Player Shadow/////////////
	PLAYER1 *Player = GetPlayer1();
	pDevice->SetRenderState(D3DRS_ZENABLE, false);
	if (Player->use)
	{
		if(Player->down_dir)
		    DrawShadow(Player->ShadowIdx, OBJ_EN_PLAYER_TYPE, 0);
	}
	pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	////////////////////////////////
	///////Enemy Wolf Shadow///////
	ENEMY_WOLF *EnemyWolf = GetEnemyWolf();
	ENEMY_CANNIBAL *EnemyCannibal = GetEnemyCannibal();
	ENEMY_BOSSWOLF *EnemyBossWolf = GetEnemyBossWolf();
	pDevice->SetRenderState(D3DRS_ZENABLE, false);
	for (int i = 0; i < ENEMY_WOLF_MAX_NUM; i++)
	{
		if (EnemyWolf[i].use)
		{
			if (CubeInFrustum(Shadow[EnemyWolf[i].ShadowIdx].pos))
			{
				if (EnemyWolf[i].pos.y > 10.0f)
				{
					DrawShadow(EnemyWolf[i].ShadowIdx, OBJ_EN_ENEMY_WOLF_TYPE, i);
					pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
				}
			}
		}
	}

	pDevice->SetRenderState(D3DRS_ZENABLE, false);
	for (int i = 0; i < ENEMY_CANNIBAL_MAX_NUM; i++)
	{
		if (EnemyCannibal[i].use)
		{
			if (CubeInFrustum(Shadow[EnemyCannibal[i].ShadowIdx].pos))
			{
				DrawShadow(EnemyCannibal[i].ShadowIdx, OBJ_EN_ENEMY_CANNIBAL_TYPE, i);
				pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
			}
		}
	}
	pDevice->SetRenderState(D3DRS_ZENABLE, false);
	for (int i = 0; i < ENEMY_BOSSWOLF_MAX_NUM; i++)
	{
		if (EnemyBossWolf[i].use)
		{
			if (CubeInFrustum(Shadow[EnemyBossWolf[i].ShadowIdx].pos))
			{
				DrawShadow(EnemyBossWolf[i].ShadowIdx, OBJ_EN_ENEMY_BOSS_WOLF_TYPE, i);
				pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
			}
		}
	}
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
	/////////////////////////////////
	for (int i = 0; i < OBJ_CUBE_EX_MAX; i++)
	{
		if (g_ObjCube[i].use)
		{
			if (CubeInFrustum(g_ObjCube[i].pos))
			{
				if (g_ObjCube[i].pos.y > 10.0f)
				{
					// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
					D3DXMatrixIdentity(&matWorld);

					// 僗働乕儖傪斀塮
					D3DXMatrixScaling(&mtxScl, g_ObjCube[i].scl.x, g_ObjCube[i].scl.y, g_ObjCube[i].scl.z);
					D3DXMatrixMultiply(&matWorld, &matWorld, &mtxScl);

					// 夞揮傪斀塮
					D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ObjCube[i].rot.y, g_ObjCube[i].rot.x, g_ObjCube[i].rot.z);
					D3DXMatrixMultiply(&matWorld, &matWorld, &mtxRot);

					// 堏摦傪斀塮
					D3DXMatrixTranslation(&mtxTranslate, g_ObjCube[i].pos.x, g_ObjCube[i].pos.y, g_ObjCube[i].pos.z);
					D3DXMatrixMultiply(&matWorld, &matWorld, &mtxTranslate);

					pDevice->GetTransform(D3DTS_VIEW, &matView);
					pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

					D3DXMATRIX matWVP = matWorld * matView * matProj;
					if (ViewMatViewHandle)
						g_pEffect->SetMatrix(ViewMatViewHandle, &(matView));
					if (AmbientCoefficientHandle)
						g_pEffect->SetFloat(AmbientCoefficientHandle, VALUE_AMBIENT_COEFFICIENT);
					if (LightPositionHandle)
						g_pEffect->SetVector("fvLightPosition", &D3DXVECTOR4{ Light[0].Direction, 0.0f });
					if(LightDiffuseHandle)
						g_pEffect->SetVector("fvDiffuse", &D3DXVECTOR4{ Light[0].Diffuse.r,Light[0].Diffuse.g,Light[0].Diffuse.b,Light[0].Diffuse.a });
					if (LightAmbientHandle)
						g_pEffect->SetVector("fvAmbient", &D3DXVECTOR4{ Light[0].Ambient.r,Light[0].Ambient.g,Light[0].Ambient.b,Light[0].Ambient.a });
					if (ViewProjMatrixHandle)
						g_pEffect->SetMatrix(ViewProjMatrixHandle, &matWVP);
					if (PixTextureHandle)
					{
						if (g_ObjCube[i].g_pD3DTexture != NULL)
						{
							g_pEffect->SetTexture("g_txScene", g_ObjCube[i].g_pD3DTexture);
						}
					}
					if (TransparencyFloatHandle)
						g_pEffect->SetFloat("Transparency", 1.0f);
					g_pEffect->SetTechnique("TextureTech1");
					UINT cPasses = 0;
					g_pEffect->Begin(&cPasses, 0);
					for (UINT p = 0; p < cPasses; ++p)
					{
						g_pEffect->BeginPass(p);
						g_pEffect->CommitChanges();
						pDevice->SetStreamSource(0, g_ObjCube[i].g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

						pDevice->SetFVF(FVF_VERTEX_3D);

						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);
					}
					g_pEffect->End();
				}
			}
		}
	}
	pDevice->SetRenderState(D3DRS_ZENABLE, false);
}

//=============================================================================
// 捀揰偺嶌惉
//=============================================================================
HRESULT MakeVertexObjCube(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		// 僆僽僕僃僋僩偺捀揰僶僢僼傽傪惗惉
		if (FAILED(pDevice->CreateVertexBuffer(6 * sizeof(VERTEX_3D) * NUM_VERTEX,	// 捀揰僨乕僞梡偵妋曐偡傞僶僢僼傽僒僀僘(僶僀僩扨埵)
			D3DUSAGE_WRITEONLY,			
			FVF_VERTEX_3D,				
			D3DPOOL_MANAGED,			
			&g_ObjCube[i].g_pD3DVtxBuff,
			NULL)))						
		{
			MessageBox(0,"Cube 创建顶点失败", 0, 0);
			return E_FAIL;
		}

		{//捀揰僶僢僼傽偺拞恎傪杽傔傞
			VERTEX_3D *pVtx;

			// 捀揰僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
			g_ObjCube[i].g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//下面青の矩形
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, -SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, SIZE_Z);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;

			//上面白の矩形
			pVtx[0].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, SIZE_Z);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;

			//右側面緑の矩形
			pVtx[0].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, -SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, SIZE_Z);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;
			//左側面黄の矩形
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;
			//前面赤の矩形 
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, SIZE_Z);

			//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;
			//裏面紫の矩形 
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, -SIZE_Z);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// 奼嶶岝偺愝掕
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 捀揰僨乕僞傪傾儞儘僢僋偡傞
			g_ObjCube[i].g_pD3DVtxBuff->Unlock();
		}
	}

	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		// 僆僽僕僃僋僩偺捀揰僶僢僼傽傪惗惉
		if (FAILED(pDevice->CreateVertexBuffer(6 * sizeof(VERTEX_3D_DIFFUSE) * NUM_VERTEX,	// 捀揰僨乕僞梡偵妋曐偡傞僶僢僼傽僒僀僘(僶僀僩扨埵)
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D_DIFFUSE,
			D3DPOOL_MANAGED,
			&g_ObjCube[i].g_pD3DVtxBuffShadow,
			NULL)))
		{
			MessageBox(0, "Cube 创建顶点失败", 0, 0);
			return E_FAIL;
		}

		{//捀揰僶僢僼傽偺拞恎傪杽傔傞
			VERTEX_3D_DIFFUSE *pVtx;

			// 捀揰僨乕僞偺斖埻傪儘僢僋偟丄捀揰僶僢僼傽傊偺億僀儞僞傪庢摼
			g_ObjCube[i].g_pD3DVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

			//下面青の矩形
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, -SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, SIZE_Z);

			//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx += 4;

			//上面白の矩形
			pVtx[0].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, SIZE_Z);

			//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx += 4;

			//右側面緑の矩形
			pVtx[0].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, -SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, SIZE_Z);

			//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx += 4;
			//左側面黄の矩形
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z);

			//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx += 4;
			//前面赤の矩形 
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, SIZE_Z);

			//Vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			//Vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			//Vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			//Vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			pVtx[0].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx += 4;
			//裏面紫の矩形 
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_X, SIZE_Y, -SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_X, -SIZE_Y, -SIZE_Z);

			//pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			//pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			//pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			//pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			pVtx[0].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			// 捀揰僨乕僞傪傾儞儘僢僋偡傞
			g_ObjCube[i].g_pD3DVtxBuffShadow->Unlock();
		}
	}
	return S_OK;
}

OBJ_CUBE_EX *RetObjCube_Ex()
{
	return &g_ObjCube[0];
}

//重载模型
void ReLoadObjCube_Ex(int index, int ChgListIndex)
{
	if (ChgListIndex != -1)
	{
		LPDIRECT3DTEXTURE9 *TexturePointList = RetTexturePointList();
		g_ObjCube[index].g_pD3DTexture = TexturePointList[ChgListIndex];
	}
	else
		g_ObjCube[index].g_pD3DTexture = NULL;
}


bool LoadObjCubeShader_Ex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	DWORD dwShaderFlags = 0;

	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
	dwShaderFlags |= D3DXSHADER_NO_PRESHADER;

	HRESULT hr = 0;
	LPD3DXBUFFER errorBuffer = 0;
	// Read the D3DX effect file
	hr = D3DXCreateEffectFromFile(
		pDevice,
		OBJ_CUBE_SHADER_EX_PATH,
		NULL, // CONST D3DXMACRO* pDefines,
		NULL, // LPD3DXINCLUDE pInclude,
		0,
		NULL, // LPD3DXEFFECTPOOL pPool,
		&g_pEffect,
		&errorBuffer);

	if (errorBuffer)
	{
		//::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		MessageBox(NULL, (char *)
			(errorBuffer->GetBufferPointer()), "ERROR", MB_OK);
		errorBuffer->Release();
		return FALSE;
	}
	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", 0, 0);
		return FALSE;
	}
	ViewProjMatrixHandle = g_pEffect->GetParameterByName(NULL, "ViewProjMatrix");
	PixTextureHandle = g_pEffect->GetParameterByName(NULL, "g_txScene");
	TransparencyFloatHandle = g_pEffect->GetParameterByName(NULL, "Transparency");
	LightPositionHandle = g_pEffect->GetParameterByName(NULL, "fvLightPosition");
	LightDiffuseHandle = g_pEffect->GetParameterByName(NULL, "fvDiffuse");
	LightAmbientHandle = g_pEffect->GetParameterByName(NULL, "fvAmbient");
	ViewMatViewHandle = g_pEffect->GetParameterByName(NULL, "matView");
	AmbientCoefficientHandle = g_pEffect->GetParameterByName(NULL, "fvAmbientCoefficient");
	return TRUE;
}


void ClearAllCube()
{
	for (int i = 0; i < OBJ_CUBE_MAX; i++)
	{
		if (g_ObjCube[i].use)
		{
			g_ObjCube[i].pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
			g_ObjCube[i].rot = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
			g_ObjCube[i].scl = D3DXVECTOR3{ 1.0f,1.0f,1.0f };
			g_ObjCube[i].TexturePathNameLen = 0;
			strcpy(g_ObjCube[i].TexturePathName, "");
			g_ObjCube[i].use = false;
		}
	}
}

 
void DrawObjCubeShadow( int index )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetStreamSource(0, g_ObjCube[index].g_pD3DVtxBuffShadow, 0, sizeof(VERTEX_3D_DIFFUSE));

	pDevice->SetFVF(FVF_VERTEX_3D_DIFFUSE);
	//下
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	//上
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);
	//右
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, NUM_POLYGON);
	//左
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	//前
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	//后
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);

}


//BossWolf Scene Cube Index
void FindEnemyBossWolfSceneCubeIndex()
{
	for (int i = 0; i < OBJ_CUBE_EX_MAX; i++)
	{
		if (g_ObjCube[i].use)
		{
			if (g_ObjCube[i].pos.y < 10.0f
				&& g_ObjCube[i].pos.y > -10.0f)
			{
				if (g_ObjCube[i].pos.z < 5670.0f
					&& g_ObjCube[i].pos.z > 5094.0f
					&& g_ObjCube[i].pos.x > -400.0f
					&& g_ObjCube[i].pos.x < 273.0f)
				{
					EnemyBossWolfSceneCubeIndex[EnemyBossWolfSceneCubeIndexNum] = i;
					EnemyBossWolfSceneCubeIndexNum++;
				}
			}
		}
	}
}



//发现wolf boss区域周围的方块并返回索引
//dir 0 上1 下2 左 3 右
int FindEnemyBossWolfSceneCubeIndexSide(int index, int dir)
{
	for (int i = 0; i < EnemyBossWolfSceneCubeIndexNum; i++)
	{
		if (g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].use)
		{
			if (dir == 0)
			{
				if (g_ObjCube[index].pos.z + GRID_SIZE == g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].pos.z
					&& g_ObjCube[index].pos.x == g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].pos.x)
					return EnemyBossWolfSceneCubeIndex[i];
			}
			if (dir == 1)
			{
				if (g_ObjCube[index].pos.z - GRID_SIZE == g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].pos.z
					&& g_ObjCube[index].pos.x == g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].pos.x)
					return EnemyBossWolfSceneCubeIndex[i];
			}
			if (dir == 2)
			{
				if (g_ObjCube[index].pos.z  == g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].pos.z
					&& g_ObjCube[index].pos.x - GRID_SIZE == g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].pos.x)
					return EnemyBossWolfSceneCubeIndex[i];
			}
			if (dir == 3)
			{
				if (g_ObjCube[index].pos.z  == g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].pos.z
					&& g_ObjCube[index].pos.x + GRID_SIZE == g_ObjCube[EnemyBossWolfSceneCubeIndex[i]].pos.x)
					return EnemyBossWolfSceneCubeIndex[i];
			}
		}
	}
	return -1;
}

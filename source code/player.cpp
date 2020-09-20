//=============================================================================
//
// プレイヤー　デバッグ処理 [player.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "GlobalFunction.h"
#include "camera_Ex.h"
#include "sphereCollider.h"
#include "debugproc.h"
#include "GameMath.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/airplane000.x"		// 撉傒崬傓儌僨儖柤

#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 夞揮検

//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************


//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************

static PLAYER				g_Player;						// 僾儗僀儎乕
static PLAYER               g_PlayerBak;

//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

								// 埵抲丒夞揮丒僗働乕儖偺弶婜愝掕
	g_Player.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Player.old_pos = g_Player.pos;

	g_Player.spd = 0.0f;		// 堏摦僗僺乕僪僋儕傾

	g_Player.pD3DTexture = NULL;
	g_Player.pD3DXMesh = NULL;
	g_Player.pD3DXBuffMat = NULL;
	g_Player.nNumMat = 0;

	// X僼傽僀儖偺撉傒崬傒
	if(FAILED(D3DXLoadMeshFromX(MODEL_PLAYER,			// 撉傒崬傓儌僨儖僼傽僀儖柤(X僼傽僀儖)
								D3DXMESH_SYSTEMMEM,		// 儊僢僔儏偺嶌惉僆僾僔儑儞傪巜掕
								pDevice,				// IDirect3DDevice9僀儞僞乕僼僃僀僗傊偺億僀儞僞
								NULL,					// 椬愙惈僨乕僞傪娷傓僶僢僼傽傊偺億僀儞僞
								&g_Player.pD3DXBuffMat,	// 儅僥儕傾儖僨乕僞傪娷傓僶僢僼傽傊偺億僀儞僞
								NULL,					// 僄僼僃僋僩僀儞僗僞儞僗偺攝楍傪娷傓僶僢僼傽傊偺億僀儞僞
								&g_Player.nNumMat,		// D3DXMATERIAL峔憿懱偺悢
								&g_Player.pD3DXMesh)))	// ID3DXMesh僀儞僞乕僼僃僀僗傊偺億僀儞僞偺傾僪儗僗
	{
		return E_FAIL;
	}

#if 0
	// 僥僋僗僠儍偺撉傒崬傒
	D3DXCreateTextureFromFile(pDevice,					// 僨僶僀僗傊偺億僀儞僞
		TEXTURE_FILENAME,		// 僼傽僀儖偺柤慜
		&g_Player.pD3DTexture);	// 撉傒崬傓儊儌儕乕
#endif

	g_Player.SphereColliderIdx = CreateSphereCollider(g_Player.pos, g_Player.rot, g_Player.scl);
	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitPlayer(void)
{
	if(g_Player.pD3DTexture != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_Player.pD3DTexture->Release();
		g_Player.pD3DTexture = NULL;
	}

	if(g_Player.pD3DXMesh != NULL)
	{// 儊僢僔儏偺奐曻
		g_Player.pD3DXMesh->Release();
		g_Player.pD3DXMesh = NULL;
	}

	if(g_Player.pD3DXBuffMat != NULL)
	{// 儅僥儕傾儖偺奐曻
		g_Player.pD3DXBuffMat->Release();
		g_Player.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// 峏怴張棟
//=============================================================================
void UpdatePlayer(void)
{

	CAMERA_EX *cam = GetCamera_Ex();

	// 堏摦張棟
	int		dir = 0;	// 侽丗岦偒偼曄偊側偄

	if (GetKeyboardPress(DIK_LEFT))
	{
		dir |= 8;
		g_Player.spd = PLAYER_VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		dir |= 4;
		g_Player.spd = PLAYER_VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		dir |= 2;
		g_Player.spd = PLAYER_VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		dir |= 1;
		g_Player.spd = PLAYER_VALUE_MOVE;
	}

	if (GetKeyboardPress(DIK_SPACE))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		dir = 1;
		g_Player.spd = 0.0f;
	}
	if (GetKeyboardPress(DIK_K))
	{
		g_Player.pos.y -= PLAYER_VALUE_MOVE;
	}
	if (GetKeyboardPress(DIK_I))
	{
		g_Player.pos.y += PLAYER_VALUE_MOVE;
	}
	// 擖椡偝傟偨僉乕偵崌傢偣偰岦偒傪寛傔傞
	float roty = 0.0f;
	switch (dir)
	{
	case 1:	// 壓岦偒
		roty = 0.0f;
		break;

	case 2:	// 忋岦偒
		roty = D3DX_PI;
		break;

	case 4:	// 塃岦偒
		roty = -D3DX_PI / 2;
		break;

	case 8:	// 嵍岦偒
		roty = D3DX_PI / 2;
		break;

	case 5:	// 塃壓岦偒
		roty = -D3DX_PI / 4;
		break;

	case 6:	// 塃忋岦偒
		roty = -D3DX_PI / 4 * 3;
		break;

	case 9:	// 嵍壓岦偒
		roty = D3DX_PI / 4;
		break;

	case 10: // 嵍壓忋岦偒
		roty = D3DX_PI / 4 * 3;
		break;

	case 0:
		roty = g_Player.rot.y;
		break;

	}

	if (g_Player.face_dir == dir && g_Player.ColliderHit == true)
	{
		g_Player.pos = g_Player.old_pos;
		g_Player.spd = 0.0f;
	}

	if (g_Player.ColliderHit == false)
	{
		if (g_Player.old_pos != g_Player.pos)
		{
			g_Player.old_pos = g_Player.pos;
		}
	}

	//	// Key擖椡偑偁偭偨傜堏摦張棟偡傞
	//	if (dir > 0)
	{
		// 僇儊儔偵懳偟偰擖椡偺偁偭偨曽岦傊僾儗僀儎乕傪岦偐偣偰堏摦偝偣傞
		g_Player.rot.y = roty;
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;

		// 僇儊儔偺拲帇揰偲帇揰傪庡恖岞偵捛廬偝偣傞
		cam->at.x = g_Player.pos.x;
		cam->at.z = g_Player.pos.z;
		cam->pos.x = cam->at.x - sinf(cam->rot.y) * cam->len;
		cam->pos.z = cam->at.z - cosf(cam->rot.y) * cam->len;

	}

	g_Player.spd *= 0.0f;

	SetPositionSphereCollider(g_Player.SphereColliderIdx, D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 10.0f, g_Player.pos.z),
		D3DXVECTOR3(g_Player.rot.x, g_Player.rot.y, g_Player.rot.z),
		g_Player.scl);
	// 塭傕僾儗僀儎乕偺埵抲偵崌傢偣傞
	PrintDebugProc("player pos x:%f, y: %f, z: %f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	
	g_Player.face_dir = dir;
	g_Player.ColliderHit = false;
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// 儚乕儖僪儅僩儕僢僋僗偺弶婜壔
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// 僗働乕儖傪斀塮
	D3DXMatrixScaling(&mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxScl);

	// 夞揮傪斀塮
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	// 堏摦傪斀塮
	D3DXMatrixTranslation(&mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTranslate);

	// 儚乕儖僪儅僩儕僢僋僗偺愝掕
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	// 尰嵼偺儅僥儕傾儖傪庢摼
	pDevice->GetMaterial(&matDef);

	// 儅僥儕傾儖忣曬偵懳偡傞億僀儞僞傪庢摼
	pD3DXMat = (D3DXMATERIAL*)g_Player.pD3DXBuffMat->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_Player.nNumMat; nCntMat++)
	{
		// 儅僥儕傾儖偺愝掕
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// 僥僋僗僠儍偺愝掕
		pDevice->SetTexture(0, g_Player.pD3DTexture);

		// 昤夋
		g_Player.pD3DXMesh->DrawSubset(nCntMat);
	}

	// 儅僥儕傾儖傪僨僼僅儖僩偵栠偡
	pDevice->SetMaterial(&matDef);
}


//=============================================================================
// 僾儗僀儎乕忣曬傪庢摼
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

void BackUpPlayer(void)
{
	g_PlayerBak = g_Player;
}

//重新加载Player
HRESULT RestartPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Player = g_PlayerBak;

	if (FAILED(D3DXLoadMeshFromX(
		MODEL_PLAYER,
		D3DXMESH_SYSTEMMEM,		
		pDevice,				
		NULL,					
		&g_Player.pD3DXBuffMat,	
		NULL,					
		&g_Player.nNumMat,		
		&g_Player.pD3DXMesh)))
	{
		return E_FAIL;
	}

#if 0
	// 僥僋僗僠儍偺撉傒崬傒
	D3DXCreateTextureFromFile(pDevice,					// 僨僶僀僗傊偺億僀儞僞
		TEXTURE_FILENAME,		// 僼傽僀儖偺柤慜
		&g_Player.pD3DTexture);	// 撉傒崬傓儊儌儕乕
#endif

	g_Player.SphereColliderIdx = CreateSphereCollider(g_Player.pos, g_Player.rot, g_Player.scl);
	return S_OK;
}


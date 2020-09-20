//=============================================================================
//
// 僇儊儔張棟 [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "camera_Ex.h"
#include "UiTitleMenu.h"
#include "GlobalFunction.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	POS_X_CAM		(0.0f)			// 僇儊儔偺弶婜埵抲(X嵗昗)
#define	POS_Y_CAM		(430.0f)			// 僇儊儔偺弶婜埵抲(Y嵗昗)
#define	POS_Z_CAM		(-260.0f)		// 僇儊儔偺弶婜埵抲(Z嵗昗)

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// 價儏乕暯柺偺帇栰妏
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// 價儏乕暯柺偺傾僗儁僋僩斾	
#define	VIEW_NEAR_Z		(10.0f)											// 價儏乕暯柺偺NearZ抣
#define	VIEW_FAR_Z		(10000.0f)										// 價儏乕暯柺偺FarZ抣
#define VIEW_FAR_Z_C	(1500.0f)

#define	VALUE_MOVE_CAMERA_EX	(2.0f)										// 僇儊儔偺堏摦検
#define	VALUE_ROTATE_CAMERA_EX	(D3DX_PI * 0.01f)							// 僇儊儔偺夞揮検

void InitFrustum_Ex();
//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
CAMERA_EX			g_Camera_Ex;				// 僇儊儔僨乕僞
static D3DXPLANE		CamPlanes_Ex[6];	//0:NEAR 1:FAR 2:LEFT 3:RIGHT 4:TOP 5:BOTTOM
//=============================================================================
// 弶婜壔張棟
//=============================================================================
void InitCamera_Ex(void)
{
	g_Camera_Ex.pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	g_Camera_Ex.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera_Ex.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera_Ex.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 帇揰偲拲帇揰偺嫍棧傪寁嶼
	float vx, vz;
	vx = g_Camera_Ex.pos.x - g_Camera_Ex.at.x;
	vz = g_Camera_Ex.pos.z - g_Camera_Ex.at.z;
	g_Camera_Ex.len = sqrtf(vx * vx + vz * vz);

	vx = g_Camera_Ex.at.x;
	vz = g_Camera_Ex.at.z;
	g_Camera_Ex.lenAt = sqrtf(vx * vx + vz * vz);
}


//=============================================================================
// 僇儊儔偺廔椆張棟
//=============================================================================
void UninitCamera_Ex(void)
{

}


//=============================================================================
// 僇儊儔偺峏怴張棟
//=============================================================================
void UpdateCamera_Ex(void)
{
	int *NowStruct = RetNowStruct();

	if (*NowStruct != STRUCT_TYPE_MENU)
	{
		if (GetKeyboardPress(DIK_Z) || IsButtonPressed(0, BUTTON_ROT_LEFT))
		{// 帇揰慁夞乽嵍乿
			g_Camera_Ex.rot.y += VALUE_ROTATE_CAMERA_EX;
			if (g_Camera_Ex.rot.y > D3DX_PI)
			{
				g_Camera_Ex.rot.y -= D3DX_PI * 2.0f;
			}

			g_Camera_Ex.pos.x = g_Camera_Ex.at.x - sinf(g_Camera_Ex.rot.y) * g_Camera_Ex.len;
			g_Camera_Ex.pos.z = g_Camera_Ex.at.z - cosf(g_Camera_Ex.rot.y) * g_Camera_Ex.len;
		}

		if (GetKeyboardPress(DIK_C) || IsButtonPressed(0, BUTTON_ROT_RIGHT))
		{// 帇揰慁夞乽塃乿
			g_Camera_Ex.rot.y -= VALUE_ROTATE_CAMERA_EX;
			if (g_Camera_Ex.rot.y < -D3DX_PI)
			{
				g_Camera_Ex.rot.y += D3DX_PI * 2.0f;
			}

			g_Camera_Ex.pos.x = g_Camera_Ex.at.x - sinf(g_Camera_Ex.rot.y) * g_Camera_Ex.len;
			g_Camera_Ex.pos.z = g_Camera_Ex.at.z - cosf(g_Camera_Ex.rot.y) * g_Camera_Ex.len;
		}
#ifdef CAMERA_ROT_ON_SWITCH
		if (GetKeyboardPress(DIK_Y))
		{// 帇揰堏摦乽忋乿
			g_Camera_Ex.pos.y += VALUE_MOVE_CAMERA_EX;
		}

		if (GetKeyboardPress(DIK_N))
		{// 帇揰堏摦乽壓乿
			g_Camera_Ex.pos.y -= VALUE_MOVE_CAMERA_EX;
		}

		if (GetKeyboardPress(DIK_Q))
		{// 拲帇揰慁夞乽嵍乿
			g_Camera_Ex.rot.y -= VALUE_ROTATE_CAMERA_EX;
			if (g_Camera_Ex.rot.y < -D3DX_PI)
			{
				g_Camera_Ex.rot.y += D3DX_PI * 2.0f;
			}
		
			g_Camera_Ex.at.x = g_Camera_Ex.pos.x + sinf(g_Camera_Ex.rot.y) * g_Camera_Ex.len;
			g_Camera_Ex.at.z = g_Camera_Ex.pos.z + cosf(g_Camera_Ex.rot.y) * g_Camera_Ex.len;
		}
		
		if (GetKeyboardPress(DIK_E))
		{// 拲帇揰慁夞乽塃乿
			g_Camera_Ex.rot.y += VALUE_ROTATE_CAMERA_EX;
			if (g_Camera_Ex.rot.y > D3DX_PI)
			{
				g_Camera_Ex.rot.y -= D3DX_PI * 2.0f;
			}
		
			g_Camera_Ex.at.x = g_Camera_Ex.pos.x + sinf(g_Camera_Ex.rot.y) * g_Camera_Ex.len;
			g_Camera_Ex.at.z = g_Camera_Ex.pos.z + cosf(g_Camera_Ex.rot.y) * g_Camera_Ex.len;
		}

		if (GetKeyboardPress(DIK_T))
		{// 拲帇揰堏摦乽忋乿
			g_Camera_Ex.at.y += VALUE_MOVE_CAMERA_EX;
		}
		
		if (GetKeyboardPress(DIK_B))
		{// 拲帇揰堏摦乽壓乿
			g_Camera_Ex.at.y -= VALUE_MOVE_CAMERA_EX;
		}

		if (GetKeyboardPress(DIK_U))
		{// 嬤偯偔
			g_Camera_Ex.len -= VALUE_MOVE_CAMERA_EX;
		}
		
		if (GetKeyboardPress(DIK_M))
		{// 棧傟傞
			g_Camera_Ex.len += VALUE_MOVE_CAMERA_EX;
		}

		 //僇儊儔傪弶婜偵栠偡
		if (GetKeyboardPress(DIK_SPACE))
		{
			UninitCamera_Ex();
			InitCamera_Ex();
		}
#endif // CAMERA_ROT_ON_SWITCH
	}
}


//=============================================================================
// 僇儊儔偺峏怴
//=============================================================================
void SetCamera_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 價儏乕儅僩儕僢僋僗偺弶婜壔
	D3DXMatrixIdentity(&g_Camera_Ex.mtxView);

	// 價儏乕儅僩儕僢僋僗偺嶌惉
	D3DXMatrixLookAtLH(&g_Camera_Ex.mtxView,
		&g_Camera_Ex.pos,		// 僇儊儔偺帇揰(埵抲)
		&g_Camera_Ex.at,		// 僇儊儔偺拲帇揰
		&g_Camera_Ex.up);		// 僇儊儔偺忋曽岦儀僋僩儖

	// 價儏乕儅僩儕僢僋僗偺愝掕
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera_Ex.mtxView);

	// 僾儘僕僃僋僔儑儞儅僩儕僢僋僗偺弶婜壔
	D3DXMatrixIdentity(&g_Camera_Ex.mtxProjection);

	// 僾儘僕僃僋僔儑儞儅僩儕僢僋僗偺嶌惉
	D3DXMatrixPerspectiveFovLH(&g_Camera_Ex.mtxProjection,
		VIEW_ANGLE,			// 價儏乕暯柺偺帇栰妏
		VIEW_ASPECT,		// 價儏乕暯柺偺傾僗儁僋僩斾
		VIEW_NEAR_Z,		// 價儏乕暯柺偺NearZ抣
		VIEW_FAR_Z);		// 價儏乕暯柺偺FarZ抣

	InitFrustum_Ex();
	// 僾儘僕僃僋僔儑儞儅僩儕僢僋僗偺愝掕(摟帇曄姺偺愝掕)
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera_Ex.mtxProjection);
}


//=============================================================================
// 僇儊儔偺庢摼
//=============================================================================
CAMERA_EX *GetCamera_Ex(void)
{
	return &g_Camera_Ex;
}

void InitFrustum_Ex()
{
	D3DXMatrixMultiply(&g_Camera_Ex.mtxLoCombo, &g_Camera_Ex.mtxView, &g_Camera_Ex.mtxProjection);
	// calculate the planes
	// Near
	CamPlanes_Ex[0].a = g_Camera_Ex.mtxLoCombo._14 + g_Camera_Ex.mtxLoCombo._13;
	CamPlanes_Ex[0].b = g_Camera_Ex.mtxLoCombo._24 + g_Camera_Ex.mtxLoCombo._23;
	CamPlanes_Ex[0].c = g_Camera_Ex.mtxLoCombo._34 + g_Camera_Ex.mtxLoCombo._33;
	CamPlanes_Ex[0].d = g_Camera_Ex.mtxLoCombo._44 + g_Camera_Ex.mtxLoCombo._43;
	D3DXPlaneNormalize(&CamPlanes_Ex[0], &CamPlanes_Ex[0]);
	// Far
	CamPlanes_Ex[1].a = g_Camera_Ex.mtxLoCombo._14 - g_Camera_Ex.mtxLoCombo._13;
	CamPlanes_Ex[1].b = g_Camera_Ex.mtxLoCombo._24 - g_Camera_Ex.mtxLoCombo._23;
	CamPlanes_Ex[1].c = g_Camera_Ex.mtxLoCombo._34 - g_Camera_Ex.mtxLoCombo._33;
	//CamPlanes_Ex[1].d = g_Camera_Ex.mtxLoCombo._44 - g_Camera_Ex.mtxLoCombo._43;
 	D3DXVECTOR3 t = g_Camera_Ex.at - g_Camera_Ex.pos;
	t /= sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
	t = g_Camera_Ex.pos + t * VIEW_FAR_Z_C;
	CamPlanes_Ex[1].d = -CamPlanes_Ex[1].a * t.x - CamPlanes_Ex[1].b * t.y - CamPlanes_Ex[1].c * t.z;

	D3DXPlaneNormalize(&CamPlanes_Ex[1], &CamPlanes_Ex[1]);
	//Left
	CamPlanes_Ex[2].a = g_Camera_Ex.mtxLoCombo._14 + g_Camera_Ex.mtxLoCombo._11;
	CamPlanes_Ex[2].b = g_Camera_Ex.mtxLoCombo._24 + g_Camera_Ex.mtxLoCombo._21;
	CamPlanes_Ex[2].c = g_Camera_Ex.mtxLoCombo._34 + g_Camera_Ex.mtxLoCombo._31;
	CamPlanes_Ex[2].d = g_Camera_Ex.mtxLoCombo._44 + g_Camera_Ex.mtxLoCombo._41;
	D3DXPlaneNormalize(&CamPlanes_Ex[2], &CamPlanes_Ex[2]);
	// Right
	CamPlanes_Ex[3].a = g_Camera_Ex.mtxLoCombo._14 - g_Camera_Ex.mtxLoCombo._11;
	CamPlanes_Ex[3].b = g_Camera_Ex.mtxLoCombo._24 - g_Camera_Ex.mtxLoCombo._21;
	CamPlanes_Ex[3].c = g_Camera_Ex.mtxLoCombo._34 - g_Camera_Ex.mtxLoCombo._31;
	CamPlanes_Ex[3].d = g_Camera_Ex.mtxLoCombo._44 - g_Camera_Ex.mtxLoCombo._41;
	D3DXPlaneNormalize(&CamPlanes_Ex[3], &CamPlanes_Ex[3]);
	// Top
	CamPlanes_Ex[4].a = g_Camera_Ex.mtxLoCombo._14 - g_Camera_Ex.mtxLoCombo._12;
	CamPlanes_Ex[4].b = g_Camera_Ex.mtxLoCombo._24 - g_Camera_Ex.mtxLoCombo._22;
	CamPlanes_Ex[4].c = g_Camera_Ex.mtxLoCombo._34 - g_Camera_Ex.mtxLoCombo._32;
	CamPlanes_Ex[4].d = g_Camera_Ex.mtxLoCombo._44 - g_Camera_Ex.mtxLoCombo._42;
	D3DXPlaneNormalize(&CamPlanes_Ex[4], &CamPlanes_Ex[4]);
	// Bottom
	CamPlanes_Ex[5].a = g_Camera_Ex.mtxLoCombo._14 + g_Camera_Ex.mtxLoCombo._12;
	CamPlanes_Ex[5].b = g_Camera_Ex.mtxLoCombo._24 + g_Camera_Ex.mtxLoCombo._22;
	CamPlanes_Ex[5].c = g_Camera_Ex.mtxLoCombo._34 + g_Camera_Ex.mtxLoCombo._32;
	CamPlanes_Ex[5].d = g_Camera_Ex.mtxLoCombo._44 + g_Camera_Ex.mtxLoCombo._42;
	D3DXPlaneNormalize(&CamPlanes_Ex[5], &CamPlanes_Ex[5]);
}


D3DXPLANE *GetCamPlane_Ex(void)
{
	return CamPlanes_Ex;
}
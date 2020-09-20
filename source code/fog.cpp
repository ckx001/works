//=============================================================================
//
// 霧処理[fog.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "fog.h"

//void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density);
void StartFog();

void InitFog()
{
	//SetupVertexFog(0x00FFFFFF, D3DFOG_LINEAR, true, 0.1f);

	StartFog();
}


void StartFog()
{
	LPDIRECT3DDEVICE9 g_D3DDevice = GetDevice();

	D3DCAPS9 caps;
	g_D3DDevice->GetDeviceCaps(&caps);

	// Start and end distance of the fog.
	float start = 2, end = 8;

	// Set fog properties.
	g_D3DDevice->SetRenderState(D3DRS_FOGENABLE, true);
	g_D3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB(128, 128, 128));
	g_D3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	g_D3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
	g_D3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));

	// Pixel Fog
	g_D3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	// Can only use if hardware supports it.
	if (caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE)
		g_D3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);
}

//void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density)
//	{
//		LPDIRECT3DDEVICE9 pDevice = GetDevice();
//		float Start =500.5f,    // Linear fog distances
//			End = 1000.8f;
//
//		// Enable fog blending.
//		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
//
//		// Set the fog color.
//		pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);
//
//		// Set fog parameters.
//		if (D3DFOG_LINEAR == Mode)
//		{
//			pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
//			pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
//			pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
//		}
//		else
//		{
//			pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
//			pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
//		}
//
//		// Enable range-based fog if desired (only supported for
//		//   vertex fog). For this example, it is assumed that UseRange
//		//   is set to a nonzero value only if the driver exposes the 
//		//   D3DPRASTERCAPS_FOGRANGE capability.
//		// Note: This is slightly more performance intensive
//		//   than non-range-based fog.
//		if (UseRange)
//			pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
//	}
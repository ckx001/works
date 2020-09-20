//=============================================================================
//
// fate [fade.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "fade.h"
#include "debugproc.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define	FADE_RATE		(0.02f)		// 僼僃乕僪學悢

//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
HRESULT MakeVertexFade(void);
void SetColor(D3DCOLOR col);

//*****************************************************************************
// 僌儘乕僶儖曄悢
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_p3DTextureFade = NULL;		// 僥僋僗僠儍傊偺億僀儞僞
static VERTEX_2D				g_vertexWkFade[NUM_VERTEX];		// 捀揰忣曬奿擺儚乕僋

static D3DXCOLOR				g_color = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
static FADE						g_eFade = FADE_NONE;
//static int						g_eScene = SCENE_TITLE;			
static int						g_sno = -1;

//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT InitFade(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int DeviceStatus = RetCurrentDeviceStatus();
	// 捀揰忣曬偺愝掕
	MakeVertexFade();

	//SetColor(g_color);

	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void UninitFade(void)
{
	if(g_p3DTextureFade != NULL)
	{// 僥僋僗僠儍偺奐曻
		g_p3DTextureFade->Release();
	}
}

//=============================================================================
// 峏怴張棟
//=============================================================================
void UpdateFade(void)
{
	if(g_eFade != FADE_NONE)
	{// 僼僃乕僪張棟拞
		if(g_eFade == FADE_OUT)
		{// 僼僃乕僪傾僂僩張棟
			g_color.a += FADE_RATE;		// 兛抣傪壛嶼偟偰夋柺傪徚偟偰偄偔
			if(g_color.a >= 1.0f)
			{
				// 忬懺傪愗傝懼偊
				//SetScene(g_eScene);

				// 僼僃乕僪僀儞張棟偵愗傝懼偊
				g_color.a = 1.0f;

				g_eFade = FADE_NONE;
				// BGM掆巭
				//StopSound();
			}

			// 怓傪愝掕
			SetColor(g_color);
		}
		else if (g_eFade == FADE_MUSIC_STOP)
		{
			// BGM嵞惗
			if (g_sno > -1)
			{
				//PlaySound(g_sno);
			}

			g_eFade = FADE_IN;
		}
		else if(g_eFade == FADE_IN)
		{// 僼僃乕僪僀儞張棟
			g_color.a -= FADE_RATE;		// 兛抣傪尭嶼偟偰夋柺傪晜偒忋偑傜偣傞
			if(g_color.a <= 0.0f)
			{
				// 僼僃乕僪張棟廔椆
				g_color.a = 0.0f;
				g_eFade = FADE_NONE;

			}

			// 怓傪愝掕
			SetColor(g_color);
		}
	}
	else
	{
		g_color.a = 0.0f;
		//g_color = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		SetColor(g_color);
	}
	
}

//=============================================================================
// 僼僃乕僪夋柺
//=============================================================================
void DrawFade()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	// 捀揰僼僅乕儅僢僩偺愝掕
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 僥僋僗僠儍偺愝掕
	pDevice->SetTexture(0, NULL);

	// 億儕僑儞偺昤夋
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkFade, sizeof(VERTEX_2D));
}

//=============================================================================
// 捀揰偺嶌惉
//=============================================================================
HRESULT MakeVertexFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 僥僋僗僠儍偺僷乕僗儁僋僥傿僽僐儗僋僩梡
	g_vertexWkFade[0].rhw =
	g_vertexWkFade[1].rhw =
	g_vertexWkFade[2].rhw =
	g_vertexWkFade[3].rhw = 1.0f;

	// 斀幩岝偺愝掕
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_vertexWkFade[0].diffuse = g_color;
	g_vertexWkFade[1].diffuse = g_color;
	g_vertexWkFade[2].diffuse = g_color;
	g_vertexWkFade[3].diffuse = g_color;

	// 捀揰嵗昗偺愝掕
	g_vertexWkFade[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkFade[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	g_vertexWkFade[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	g_vertexWkFade[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// 僥僋僗僠儍嵗昗偺愝掕
	g_vertexWkFade[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkFade[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkFade[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkFade[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

	return S_OK;
}

//=============================================================================
// 怓傪愝掕
//=============================================================================
void SetColor(D3DCOLOR col)
{
	// 斀幩岝偺愝掕
	g_vertexWkFade[0].diffuse = col;
	g_vertexWkFade[1].diffuse = col;
	g_vertexWkFade[2].diffuse = col;
	g_vertexWkFade[3].diffuse = col;
}

//=============================================================================
// 僼僃乕僪偺忬懺愝掕
//=============================================================================
void SetFade(FADE fade )
{
	g_eFade = fade;
	//g_eScene = next;
	//g_sno = sno;
	
}

//=============================================================================
// 僼僃乕僪偺忬懺庢摼
//=============================================================================
FADE GetFade(void)
{
	return g_eFade;
}

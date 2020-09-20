//=============================================================================
//
// MAIN [main.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "GlobalFunction.h"
#include "DataBuf.h"
#include "Serialize.h"
#include "camera_Ex.h"
#include "light_Ex.h"
#include "obj_plant_Ex.h"
#include "model_Ex.h"
#include "cube_Ex.h"
#include "ObjObjModel_Ex.h"
#include "MeshSky_Ex.h"
#include "player.h"
#include "quarTree.h"
#include "UiFontPrint.h"
#include "sphereCollider.h"
#include "BoxCollider.h"
#include "Scene.h"
#include "bullet.h"
#include "SoulEffect.h"
#include "EnemyCannibal.h"
#include "GameEvent.h"
#include "UiEnemyHp.h"
#include "fade.h"
#include "player1.h"
#include "EnemyBossWolf.h"
#include "Enemy_wolf.h"
#include "cylinderCollider.h"
#include "GameAi.h"
#include "UiPlayerHp.h"
#include "shadow.h"
#include "fog.h"
#include "UiSmallMap.h"
#include "UiGoods.h"
#include "effect.h"
#include "sound.h"
#include "UiTitleMenu.h"
//*****************************************************************************
// 儅僋儘掕媊
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// 僂僀儞僪僂偺僋儔僗柤
#define WINDOW_NAME		"美少女"		// 僂僀儞僪僂偺僉儍僾僔儑儞柤


//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void InitGame_Preprocess();
void Uninit(void);
void Update(void);
void Draw(void);
//void ResizeD3DScene(int width, int height);
void OnLostDevice(void);
HRESULT ResetDevice(void);
void RestoreDeviceObjects(void);
void InitGameEvent(void);
void ReloadRender(void);
//*****************************************************************************
// 僌儘乕僶儖曄悢:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D 僆僽僕僃僋僩
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Device僆僽僕僃僋僩(昤夋偵昁梫)
float ScreenPoint_X = 0.0f;
float ScreenPoint_Y = 0.0f;
//#ifdef _DEBUG
int					g_nCountFPS;			// FPS僇僂儞僞
//#endif

//当前设备状态
int DeviceStatus = SYS_DEVICERUN;


DWORD dwExecLastTime;
DWORD dwFPSLastTime;
DWORD dwCurrentTime;
DWORD dwFrameCount;

D3DPRESENT_PARAMETERS d3dpp;
HWND hWnd;
HINSTANCE g_hInstance;
//=============================================================================
// 儊僀儞娭悢
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 柍偔偰傕椙偄偗偳丄寈崘偑弌傞乮枹巊梡愰尵乯
	UNREFERENCED_PARAMETER(lpCmdLine);		// 柍偔偰傕椙偄偗偳丄寈崘偑弌傞乮枹巊梡愰尵乯

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	MSG msg;
	
	// 僂傿儞僪僂僋儔僗偺搊榐
	RegisterClassEx(&wcex);

	// 僂傿儞僪僂偺嶌惉
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// 弶婜壔張棟(僂傿儞僪僂傪嶌惉偟偰偐傜峴偆)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}
	g_hInstance = hInstance;
//#ifdef ENGINE_MODE
//	InitUi(hInstance, hWnd);
//#endif
	//僼儗乕儉僇僂儞僩弶婜壔
	timeBeginPeriod(1);				// 暘夝擻傪愝掕
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// 僂僀儞僪僂偺昞帵(弶婜壔張棟偺屻偵屇偽側偄偲懯栚)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// 儊僢僙乕僕儖乕僾
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()偑屇偽傟偨傜儖乕僾廔椆
				break;
			}
			else
			{
				// 儊僢僙乕僕偺東栿偲僨傿僗僷僢僠
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5昩偛偲偵幚峴
			{
//#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
//#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
//#ifdef _DEBUG
				// FPS昞帵
				PrintDebugProc("FPS:%d\n", g_nCountFPS);
//#endif
				dwExecLastTime = dwCurrentTime;

				// 峏怴張棟
				Update();

				// 昤夋張棟
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// 僂傿儞僪僂僋儔僗偺搊榐傪夝彍
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 廔椆張棟
	Uninit();

	timeEndPeriod(1);				// 暘夝擻傪栠偡

	return (int)msg.wParam;
}

//=============================================================================
// 僾儘僔乕僕儍
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef ENGINE_MODE
	RAYCHECK *RayCheck = RetRayCheck();
#endif
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		//case VK_ESCAPE:
		//	DestroyWindow(hWnd);
		//	break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 弶婜壔張棟
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//D3DPRESENT_PARAMETERS d3dpp;
	//d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3D僆僽僕僃僋僩偺嶌惉
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		MessageBox(0, "Direct3D Create Failed", 0, 0);
		return E_FAIL;
	}

	// 尰嵼偺僨傿僗僾儗僀儌乕僪傪庢摼
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(0, "显示器适配初始化失败", 0, 0);
		return E_FAIL;
	}

	// 僨僶僀僗偺僾儗僛儞僥乕僔儑儞僷儔儊乕僞偺愝掕
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// 儚乕僋傪僛儘僋儕傾
	//d3dpp.BackBufferCount			= 1;						// 僶僢僋僶僢僼傽偺悢
	//d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// 僎乕儉夋柺僒僀僘(暆)
	//d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// 僎乕儉夋柺僒僀僘(崅偝)
	//d3dpp.BackBufferFormat			= d3ddm.Format;				// 僶僢僋僶僢僼傽僼僅乕儅僢僩偼僨傿僗僾儗僀儌乕僪偵崌傢偣偰巊偆
	//d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 塮憸怣崋偵摨婜偟偰僼儕僢僾偡傞
	//d3dpp.Windowed					= bWindow;					// 僂傿儞僪僂儌乕僪
	//d3dpp.EnableAutoDepthStencil	= TRUE;						// 僨僾僗僶僢僼傽乮倅僶僢僼傽乯偲僗僥儞僔儖僶僢僼傽傪嶌惉
	//d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// 僨僾僗僶僢僼傽偲偟偰16bit傪巊偆
	//
	//if(bWindow)
	//{// 僂傿儞僪僂儌乕僪
	//	d3dpp.FullScreen_RefreshRateInHz = 0;								// 儕僼儗僢僔儏儗乕僩
	//	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// 僀儞僞乕僶儖
	//}
	//else
	//{// 僼儖僗僋儕乕儞儌乕僪
	//	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// 儕僼儗僢僔儏儗乕僩
	//	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// 僀儞僞乕僶儖
	//}
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// デプスバッファとして16bitを使う
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// デプスバッファとして16bitを使う				// カラーモードの指定

	if (bWindow)
	{// ウィンドウモード
		//d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		MessageBox(0, "HAL HARDWARE_VERTEXPROCESSING Init Failed",0,0);
		// 忋婰偺愝掕偑幐攕偟偨傜
		// [僨僶僀僗嶌惉惂屼]<昤夋>傪僴乕僪僂僃傾偱峴偄丄<捀揰張棟>偼CPU偱峴側偆
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			MessageBox(0, "HAL SOFTWARE_VERTEXPROCESSING Init Failed", 0, 0);
			// 忋婰偺愝掕偑幐攕偟偨傜
			// [僨僶僀僗嶌惉惂屼]<昤夋>偲<捀揰張棟>傪CPU偱峴側偆
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// 弶婜壔幐攕
				MessageBox(0, "REF SOFTWARE_VERTEXPROCESSING Init Failed", 0, 0);
				return E_FAIL;
			}
		}
	}

	// 儗儞僟乕僗僥乕僩僷儔儊乕僞偺愝掕
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	

	// 僒儞僾儔乕僗僥乕僩僷儔儊乕僞偺愝掕
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//设置多级渐进纹理过滤
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	////////损失性能提高渲染过滤///////
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 10);
	//////////////////////
	// 僥僋僗僠儍僗僥乕僕僗僥乕僩偺愝掕
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// 傾儖僼傽僽儗儞僨傿儞僌張棟
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 嵟弶偺傾儖僼傽堷悢
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// 俀斣栚偺傾儖僼傽堷悢
	InitFontPrint();
	InitInput(hInstance, hWnd);
	//TextureBuff();
	InitGame_Preprocess(); //预处理
	InitDebugProc();
	//InitFog();
	InitBoxCollider();
	InitCylinderCollider();
	InitSphereCollider();
	InitSceneData();
	InitCamera_Ex();
	InitLight_Ex();
	InitMeshSky_Ex(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(3.14f,0.0f, 0.0f), 16, 8, 7000.0f, true);
	InitObjPlant_Ex();
	//InitPlayer();
	InitPlayer1();
	InitEnemyWolf();
	InitEnemyCannibal();
	InitEnemyBossWolf();
	InitModel_Ex();
	InitObjCube_Ex();
	InitGameEvent();
	InitObjObjModel_Ex();
	InitFade(0);
	InitGameAi();
	InitEnemyEvent();
	InitEffect();
	InitSoul();
	InitBullet();
	InitHitCube();
	InitUiPlayerUp(0);
	InitUiEnemyUp(0);
	InitUiSmallMap(0, SCENE_SCENE_HOME);
	InitUiGoods(0);
	InitShadow();
	InitTitle(0);
	InitSound(hWnd);
	return S_OK;
}

//=============================================================================
// 廔椆張棟
//=============================================================================
void Uninit(void)
{
	UninitInput();
	UninitDebugProc();
	UninitUiPlayerUp();
	UninitCamera_Ex();
	UninitObjPlant_Ex();
	UninitModel_Ex();
	//UninitPlayer();
	UninitPlayer1();
	UninitEnemyWolf();
	UninitEnemyBossWolf();
	UninitEnemyCannibal();
	UninitObjCube_Ex();
	UninitObjObjModel_Ex();
	UninitMeshSky_Ex();
	UninitSphereCollider();
	UninitCylinderCollider();
	UninitBoxCollider();
	UninitUiEnemyUp();
	UninitFade();
	UninitFontPrint();
	UninitSound();
	UninitEffect();
	UninitUiSmallMap();
	UninitUiGoods();
	UninitTitle();

	if(g_pD3DDevice != NULL)
	{// 僨僶僀僗偺奐曻
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3D僆僽僕僃僋僩偺奐曻
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

//=============================================================================
// 峏怴張棟
//=============================================================================
void Update(void)
{
	int *NowStruct = RetNowStruct();
	UpdateInput();
	UpdateUiPlayerUp();
	UpdateUiEnemyUp();
	UpdateUiSmallMap();
	UpdateFontPrint();
	UpdateCamera_Ex();
	UpdateMeshSky_Ex();
	UpdateObjPlant_Ex();
	//UpdatePlayer();
	UpdatePlayer1();
	UpdateEnemyWolf();
	UpdateEnemyCannibal();
	UpdateEnemyBossWolf();
	UpdateBullet();
	UpdateModel_Ex();
	UpdateObjCube_Ex();
	UpdateSphereCollider();
	UpdateCylinderCollider();
	UpdateQuarTree();
	UpdateObjObjModel_Ex();
	UpdateGameEvent();
	if (DeviceStatus == SYS_DEVICERUN)
	    UpdateFade();
	UpdateSoul();
	UpdateEffect();
	UpdateGameAi();
	UpdateUiGoods();
	if(*NowStruct == STRUCT_TYPE_MENU)
	    UpdateTitle();
	
}

//=============================================================================
// 昤夋張棟
//=============================================================================
void Draw(void)
{
	int *NowStruct = RetNowStruct();
	// 僶僢僋僶僢僼傽仌倅僶僢僼傽偺僋儕傾
	//g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ), D3DCOLOR_RGBA(192, 192, 192, 0), 1.0f, 0);
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 191, 255, 0), 1.0f, 0);
	// Direct3D偵傛傞昤夋偺奐巒
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		DrawDebugProc();
		SetCamera_Ex();
		//DrawMeshSky_Ex();
		DrawObjPlant_Ex();
		DrawModel_Ex();
		DrawSphereCollider();
		DrawBoxCollider();
		DrawCylinderCollider();
		//DrawPlayer();
		DrawObjCube_Ex();
		DrawPlayer1();
		DrawEnemyWolf();
		DrawEnemyCannibal();
		DrawEnemyBossWolf();
		DrawObjObjModel_Ex();
		if(DeviceStatus == SYS_DEVICERUN)
		    DrawFade();
		DrawUiSmallMap();
		DrawUiPlayerUp();
		DrawUiEnemyUp();
		DrawUiGoods();
		DrawFontPrint();
		if(*NowStruct == STRUCT_TYPE_MENU)
		    DrawTitle();
		DrawEffect();
		// Direct3D偵傛傞昤夋偺廔椆
		g_pD3DDevice->EndScene();
	}

	// 僶僢僋僶僢僼傽偲僼儘儞僩僶僢僼傽偺擖傟懼偊
	HRESULT hr = g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST)
	{
		DeviceStatus = SYS_DEVICELOST;
		if (g_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			OnLostDevice();
			ResetDevice();
		}
	}
}

//=============================================================================
// 僨僶僀僗偺庢摼
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}


void InitGame_Preprocess()
{
	GetSysPath();
	GetFilePathList();
	//加载X Model 目录
	GetModelFilePathList(CURRENT_MODEL_PATH, CURRENT_MODEL_PATH_CHECK, OBJ_MODEL_TYPE);
	//加载Obj Model 目录
	GetModelFilePathList(CURRENT_OBJMODEL_PATH, CURRENT_OBJMODEL_PATH_CHECK, OBJ_OBJMODEL_TYPE);
}


// Reset device
HRESULT ResetDevice()
{
	HRESULT hr = g_pD3DDevice->Reset(&d3dpp);
	if (SUCCEEDED(hr))
	{
		RestoreDeviceObjects();
		DeviceStatus = SYS_DEVICERUN;
	}
	else
	{
		//const WCHAR* errorString = DXGetErrorString(hr);
		const char* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return E_FAIL;
	}
	return S_OK;
}

void UninitBuffer()
{
	g_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	g_pD3DDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	g_pD3DDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, 0);
}

void OnLostDevice(void)
{
	UninitFontPrint();
	UninitDebugProc();
	UninitInput();
	UninitCamera_Ex();
	UninitObjPlant_Ex();
	UninitModel_Ex();
	BackUpPlayer1();
	UninitPlayer();
	UninitPlayer1();
	BackUpEnemyWolf();
	UninitEnemyWolf();
	BackUpEnemyCannibal();
	UninitEnemyCannibal();
	UninitObjCube_Ex();
	UninitObjObjModel_Ex();
	UninitMeshSky_Ex();
	UninitCylinderCollider();
	UninitSphereCollider();
	UninitBoxCollider();
	UninitUiPlayerUp();
	//UninitFade();

	UninitBuffer();
}

//重启设备
void RestoreDeviceObjects()
{
	ReloadRender();
	InitDebugProc();
	InitInput(g_hInstance, hWnd);
	InitCylinderCollider();
	InitSphereCollider();
	InitBoxCollider();
	//RestartPlayer();
	RestartPlayer1();
	RestartEnemyWolf();
	InitObjCube_Ex();
	//InitFade(0);
}


void InitGameEvent()
{
	//32*32 -660.0f 0.0f
	//128*128 -2580.0f 0.0f
	InitQuarTree(-660.0f, 0.0f, 64, 64);
	InitTimeEvent();
}


int RetCurrentTime(void)
{
	return dwCurrentTime;
}

int RetCurrentDeviceStatus()
{
	return DeviceStatus;
}


void ReloadRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 僒儞僾儔乕僗僥乕僩僷儔儊乕僞偺愝掕
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// 僥僋僗僠儍僗僥乕僕僗僥乕僩偺愝掕
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// 傾儖僼傽僽儗儞僨傿儞僌張棟
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 嵟弶偺傾儖僼傽堷悢
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// 俀斣栚偺傾儖僼傽堷悢
}


void ExitGame()
{
	DestroyWindow(hWnd);
}
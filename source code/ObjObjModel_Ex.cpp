//=============================================================================
//
// OBJモデル処理 [ObjObjModel_Ex.cpp]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "ObjObjModel_Ex.h"
//#include "D3DObjMesh.h"
#include "DataBuf.h"
#include "ObjLoader.h"
#include "GlobalFunction.h"
#include "sphereCollider.h"
#include "Scene.h"
#include <strsafe.h>


static TCHAR g_sObjFileName[MAX_PATH];
static LPDIRECT3DTEXTURE9 pD3DTexture;

OBJOBJMODEL_EX g_ObjObjModel[OBJ_OBJMODEL_EX_MAX];

#define	ORI_MODEL_FILENAME	"data/MODEL/Reserve_model/cactus0.obj"		// 撉傒崬傓僥僋僗僠儍僼傽僀儖柤

#define	TEXTURE_FILENAME	"data/TEXTURE/shilianchang_static08-10_01.jpg"		// 撉傒崬傓僥僋僗僠儍僼傽僀儖柤



//CD3DMesh* g_pD3DMesh = NULL; // The loaded object. Can be NULL.

static D3DXHANDLE ViewProjMatrixHandle = NULL;
static D3DXHANDLE PixTextureHandle = NULL;

static LPD3DXEFFECT g_pEffect = NULL;

HRESULT InitObjObjModel_Ex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice,					// 僨僶僀僗傊偺億僀儞僞
		TEXTURE_FILENAME,		// 僼傽僀儖偺柤慜
		&pD3DTexture)))	// 撉傒崬傓儊儌儕乕
		return E_FAIL;


	//g_ObjObjModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_ObjObjModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_ObjObjModel.scl = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	HRESULT hr;

	// Load the effect from file.
	LPD3DXBUFFER pErrors = NULL;
	hr = D3DXCreateEffectFromFile(pDevice, TEXT("data\\SHADER\\D3DObjViewer.fx"), NULL, NULL,
		0, NULL, &g_pEffect, &pErrors);
	if (FAILED(hr))
	{
		if (pErrors)
		{
			MessageBox(0, "Effect error", 0, 0);
			//MessageBoxA(g_hWnd, (LPCSTR)pErrors->GetBufferPointer(), "Effect error", MB_OK | MB_ICONSTOP);
			pErrors->Release();
		}
		return hr;
	}
	return S_OK;
}

void UninitObjObjModel_Ex()
{
	SAFE_RELEASE(pD3DTexture);
	//SAFE_DELETE(g_pD3DMesh);
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		SAFE_DELETE(g_ObjObjModel[i].g_pD3DMesh);
	}
	//SAFE_RELEASE(g_pEffect);

}


void UpdateObjObjModel_Ex()
{
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (g_ObjObjModel[i].use == true)
		{
			if (strncmp(g_ObjObjModel[i].tag, "red", strlen(g_ObjObjModel[i].tag)) == NULL)
			{
				SetPositionSphereCollider(g_ObjObjModel[i].SphereColliderIdx,
					D3DXVECTOR3{ g_ObjObjModel[i].pos.x, g_ObjObjModel[i].pos.y + 20.0f, g_ObjObjModel[i].pos.z },
					D3DXVECTOR3{ g_ObjObjModel[i].rot.x,g_ObjObjModel[i].rot.y,g_ObjObjModel[i].rot.z },
					D3DXVECTOR3{ 1.0f, 1.0f, 1.0f });
			}
		}
	}
}


void DrawObjObjModel_Ex()
{
	LPDIRECT3DDEVICE9 g_pD3DDevice = GetDevice();
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);

	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (g_ObjObjModel[i].use == true)
		{
			if (CubeInFrustum(g_ObjObjModel[i].pos))
			{
				UINT passes = 0;
				if (g_ObjObjModel[i].g_pD3DMesh && SUCCEEDED(g_pEffect->Begin(&passes, 0)))
				{
					g_pEffect->BeginPass(0);

					CD3DMesh_Ex& d3dMesh = *g_ObjObjModel[i].g_pD3DMesh;

					D3DXMATRIX mT, mR, mS;
					D3DXMATRIX mWVP, mWIT, mView, mProj;
					D3DXMATRIX mWorld;

					D3DXMatrixIdentity(&mWorld);

					D3DXMatrixScaling(&mS, g_ObjObjModel[i].scl.x, g_ObjObjModel[i].scl.y, g_ObjObjModel[i].scl.z);
					D3DXMatrixMultiply(&mWorld, &mWorld, &mS);

					D3DXMatrixRotationYawPitchRoll(&mR, g_ObjObjModel[i].rot.y, g_ObjObjModel[i].rot.x, g_ObjObjModel[i].rot.z);
					D3DXMatrixMultiply(&mWorld, &mWorld, &mR);

					D3DXMatrixTranslation(&mT, g_ObjObjModel[i].pos.x, g_ObjObjModel[i].pos.y, g_ObjObjModel[i].pos.z);
					D3DXMatrixMultiply(&mWorld, &mWorld, &mT);

					g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
					g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &mProj);

					mWVP = mWorld * mView * mProj;

					//if (g_ObjObjModel[i].pD3DTexture != NULL)
					//{
					//	g_pEffect->SetTexture("texDiffuse", g_ObjObjModel[i].pD3DTexture);
					//}
					//else
					//	g_pEffect->SetTexture("texDiffuse", NULL);
					g_pEffect->SetBool("useDiffuseTexture", true);

					g_pEffect->SetMatrix("mWorld", &mWorld);
					g_pEffect->SetMatrix("mWVP", &mWVP);
					g_pEffect->SetMatrix("mWIT", &mWIT);
					g_pEffect->SetFloat("Tf", 1.0f);

					g_pEffect->CommitChanges();

					g_pD3DDevice->SetStreamSource(0, d3dMesh.pVB, 0, d3dMesh.vertexSize);
					g_pD3DDevice->SetFVF(d3dMesh.FVF);
					int triIndex = 0;

					for (DWORD groupNum = 0; groupNum < (DWORD)g_ObjObjModel[i].ObjGroupNum; groupNum++)
					{
						if (g_ObjObjModel[i].pD3DTexture == NULL)
						{
							if (g_ObjObjModel[i].pD3DTextureList[groupNum] != NULL)
							{
								g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, false);
								g_pEffect->SetTexture("texDiffuse", g_ObjObjModel[i].pD3DTextureList[groupNum]);
								g_pEffect->SetFloat("Tf", g_ObjObjModel[i].Tf[groupNum][0]);

							}
						}
						else
							g_pEffect->SetTexture("texDiffuse", g_ObjObjModel[i].pD3DTexture);
						g_pEffect->CommitChanges();
						g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, triIndex * 3, g_ObjObjModel[i].TrianglesList[groupNum]);

						triIndex += g_ObjObjModel[i].TrianglesList[groupNum];
						//g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, d3dMesh.triCount);
					}
					//g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, d3dMesh.triCount);

					g_pEffect->EndPass();
					g_pEffect->End();
				}
			}
		}
	}
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, false);
}


BOOL LoadObjObjFile_Ex(int index, LPCTSTR sFileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	TObjMesh objMesh;
	if (0 > LoadObj(sFileName, &objMesh))
	{
		MessageBox(0, "Failed to load the specified obj file!",0,0);
		return FALSE;
	}

	CD3DMesh_Ex* pD3DMesh = new CD3DMesh_Ex;
	if (FAILED(pD3DMesh->Create(pDevice, objMesh)))
	{
		MessageBox(0, "Failed to Create", 0, 0);
		MessageBox(0, sFileName, 0, 0);
		delete pD3DMesh;
		return FALSE;
	}

	StringCchCopy(g_sObjFileName, MAX_PATH, sFileName);

	SAFE_DELETE(g_ObjObjModel[index].g_pD3DMesh);
	g_ObjObjModel[index].g_pD3DMesh = pD3DMesh;

	// Now update the mesh info text.
	TCHAR sMeshInfo[MAX_PATH];
	sMeshInfo[0] = 0;

	///////获取组数////
	g_ObjObjModel[index].ObjGroupNum = objMesh.groups.size();
	g_ObjObjModel[index].matGroups = objMesh.matGroups.size();

	//for (int i = 0; i < g_ObjObjModel[index].ObjGroupNum; i++)
	//{
	//	g_ObjObjModel[index].TrianglesList[i] = objMesh.numTrianglesList[i];
	//}

	//创建纹理数组
	if (objMesh.materials.size() > 0)
	{
		for (int i = 0; i < g_ObjObjModel[index].matGroups; i++)
		{
			string Path = CURRENT_OBJMODEL_PATH_TEXTURE;
			//漫反射纹理
			string texFile(objMesh.materials[i]->map_Kd);
			//透明度赋值
			for (int t = 0; t < 3; t++)
				g_ObjObjModel[index].Tf[i][t] = objMesh.materials[i]->Tf[t];
			Path.append(texFile);
			strcpy(g_ObjObjModel[index].TextureListPath[i].TextureListPathName, texFile.c_str());
			g_ObjObjModel[index].TextureListPath[i].TextureListPathNameLen = strlen(texFile.c_str());
			D3DXCreateTextureFromFile(
				pDevice,
				Path.c_str(),
				&g_ObjObjModel[index].pD3DTextureList[i]);
			//MessageBox(0,Path.c_str(), 0, 0);
		}
	}
	else
	{
		for (int a = 0; a < 100; a++)
			g_ObjObjModel[index].pD3DTextureList[a] = NULL;
	}

	StringCchPrintf(sMeshInfo, MAX_PATH,
		TEXT("Triangles: %d. Faces: %d. Verts: %d. Normals: %d. UVs: %d. ")
		TEXT("Materials: %d. Mat groups: %d"),
		objMesh.numTriangles, objMesh.faces.size(), objMesh.vertices.size(),
		objMesh.normals.size(), objMesh.texCoords.size(),
		objMesh.materials.size(), objMesh.matGroups.size());
	//SetDlgItemText(g_hMainDlg, IDC_LBL_MESH_INFO, sMeshInfo);

	return TRUE;
}



OBJOBJMODEL_EX *RetObjObjModel_Ex()
{
	return &g_ObjObjModel[0];
}


//重载贴图
void ReLoadObjObjModelTexture_Ex(int index, int ChgListIndex, int type, int ListIndex)
{
	if (type == OBJOBJ_TEXTURE_TYPE_TEXTURE)
	{
		if (ChgListIndex != -1)
		{
			LPDIRECT3DTEXTURE9 *TexturePointList = RetTexturePointList();
			g_ObjObjModel[index].pD3DTexture = TexturePointList[ChgListIndex];
		}
		else
			g_ObjObjModel[index].pD3DTexture = NULL;
	}
	else if (type == OBJOBJ_TEXTURE_TYPE_TEXTURELIST)
	{
		if (ChgListIndex != -1)
		{
			LPDIRECT3DTEXTURE9 *TexturePointList = RetTexturePointList();
			g_ObjObjModel[index].pD3DTextureList[ListIndex] = TexturePointList[ChgListIndex];
		}
		else
			g_ObjObjModel[index].pD3DTextureList[ListIndex] = NULL;
	}
}


//重载模型
bool ReLoadObjObjModelModel_Ex(int index, LPCSTR ModelPathAndName)
{
	if (FAILED(LoadObjObjFile_Ex(index, ModelPathAndName)))
	{
		MessageBox(0, "OBJ模型加载失败", 0, 0);
		return false;
	}

	return true;
}


//重置Obj Moedel
void ReSetObjObjModel(int index)
{
	g_ObjObjModel[index].g_pD3DMesh = NULL;
	g_ObjObjModel[index].pos = D3DXVECTOR3{ 0,0,0 };
	g_ObjObjModel[index].rot = D3DXVECTOR3{ 0,0,0 };
	g_ObjObjModel[index].scl = D3DXVECTOR3{ 1.0f,1.0f,1.0f };
	g_ObjObjModel[index].pD3DTexture = NULL;
	g_ObjObjModel[index].g_pD3DVtxBuff = NULL;
	g_ObjObjModel[index].RelativePos = { 0,0,0 };
	g_ObjObjModel[index].TexturePathNameLen = 0;
	g_ObjObjModel[index].TexturePathNameUiIndex = -1;
	strcpy(g_ObjObjModel[index].TexturePathName, "");
	g_ObjObjModel[index].ModelPathNameLen = 0;
	g_ObjObjModel[index].ModelPathNameUiIndex = -1;
	strcpy(g_ObjObjModel[index].ModelPathName, "");
	g_ObjObjModel[index].use = false;
}


void ClearAllObjObjModel()
{
	for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
	{
		if (g_ObjObjModel[i].use)
		{
			g_ObjObjModel[i].pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
			g_ObjObjModel[i].rot = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
			g_ObjObjModel[i].scl = D3DXVECTOR3{ 1.0f,1.0f,1.0f };
			g_ObjObjModel[i].TexturePathNameLen = 0;
			strcpy(g_ObjObjModel[i].TexturePathName, "");
			strcpy(g_ObjObjModel[i].tag, "");
			for (int c = 0; c < 100; c++)
			{
				g_ObjObjModel[i].TrianglesList[c] = 0;
				for (int d = 0; d < 3; d++)
					g_ObjObjModel[i].Tf[c][d] = 0;

			}
			for (int group = 0; group < g_ObjObjModel[i].matGroups; group++)
			{
				strcpy(g_ObjObjModel[i].TextureListPath[group].TextureListPathName, "");
				g_ObjObjModel[i].TextureListPath[group].TextureListPathNameLen = 0;
			}
			g_ObjObjModel[i].ObjGroupNum = 0;
			g_ObjObjModel[i].matGroups = 0;

			//关闭触发器
			DeleteSphereCollider(g_ObjObjModel[i].SphereColliderIdx);
			g_ObjObjModel[i].SphereColliderIdx = -1;
			g_ObjObjModel[i].use = false;
		}
	}
}


//
//GOODS_TYPE_RED
//GOODS_TYPE_TREE
int CreateObjObjModel(int ObjType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	GLOBAL_OBJ_OBJMODEL *GlobalObjObjModel = RetGlobalObjObjModel(SCENE_SCENE_HOME);

	int nIdxObjObjModel = -1;

	for (int num = 0; num < OBJ_OBJMODEL_EX_MAX; num++)
	{
		if (!g_ObjObjModel[num].use)
		{
			for (int i = 0; i < OBJ_OBJMODEL_EX_MAX; i++)
			{
				if (GlobalObjObjModel[i].use)
				{
					if (ObjType == GOODS_TYPE_RED)
					{
						if (strncmp(GlobalObjObjModel[i].tag, "red", 3) == NULL)
						{
							g_ObjObjModel[num].pos = pos;
							g_ObjObjModel[num].rot = rot;
							g_ObjObjModel[num].scl = scl;
							g_ObjObjModel[num].TexturePathNameLen = GlobalObjObjModel[i].TexturePathNameLen;
							strcpy(g_ObjObjModel[num].TexturePathName, GlobalObjObjModel[i].TexturePathName);
							g_ObjObjModel[num].ModelPathNameLen = GlobalObjObjModel[i].ModelPathNameLen;
							strcpy(g_ObjObjModel[num].ModelPathName, GlobalObjObjModel[i].ModelPathName);
							g_ObjObjModel[num].ObjGroupNum = GlobalObjObjModel[i].ObjGroupNum;
							g_ObjObjModel[num].matGroups = GlobalObjObjModel[i].matGroups;
							strcpy(g_ObjObjModel[num].tag, GlobalObjObjModel[i].tag);
							for (int c = 0; c < 100; c++)
							{
								g_ObjObjModel[num].TrianglesList[c] = GlobalObjObjModel[i].TrianglesList[c];
								for (int d = 0; d < 3; d++)
									g_ObjObjModel[num].Tf[c][d] = GlobalObjObjModel[i].Tf[c][d];

							}
							for (int group = 0; group < g_ObjObjModel[num].matGroups; group++)
							{
								strcpy(g_ObjObjModel[num].TextureListPath[group].TextureListPathName, GlobalObjObjModel[i].TextureListPath[group].TextureListPathName);
								g_ObjObjModel[num].TextureListPath[group].TextureListPathNameLen = GlobalObjObjModel[i].TextureListPath[group].TextureListPathNameLen;
							}

							int ChgListIndex = RetTexuturePointListMapIndex(g_ObjObjModel[num].TexturePathNameLen, g_ObjObjModel[num].TexturePathName);
							ReLoadObjObjModelTexture_Ex(num, ChgListIndex, OBJOBJ_TEXTURE_TYPE_TEXTURE, 0);

							//重载Texture数组
							//如果自定义纹理没有设置
							for (int group = 0; group < g_ObjObjModel[num].matGroups; group++)
							{
								int ChgTextureListIndex = RetTexuturePointListMapIndex(g_ObjObjModel[num].TextureListPath[group].TextureListPathNameLen, g_ObjObjModel[num].TextureListPath[group].TextureListPathName);
								ReLoadObjObjModelTexture_Ex(num, ChgTextureListIndex, OBJOBJ_TEXTURE_TYPE_TEXTURELIST, group);
							}
							ReLoadObjObjModelModel_Ex(num, g_ObjObjModel[num].ModelPathName);

							g_ObjObjModel[num].use = true;
							nIdxObjObjModel = num;
							break;
						}
					}
					if (ObjType == GOODS_TYPE_TREE)
					{
						if (strncmp(GlobalObjObjModel[i].tag, "tree", 4) == NULL)
						{
							g_ObjObjModel[num].pos = pos;
							g_ObjObjModel[num].rot = rot;
							g_ObjObjModel[num].scl = scl;
							g_ObjObjModel[num].TexturePathNameLen = GlobalObjObjModel[i].TexturePathNameLen;
							strcpy(g_ObjObjModel[num].TexturePathName, GlobalObjObjModel[i].TexturePathName);
							g_ObjObjModel[num].ModelPathNameLen = GlobalObjObjModel[i].ModelPathNameLen;
							strcpy(g_ObjObjModel[num].ModelPathName, GlobalObjObjModel[i].ModelPathName);
							g_ObjObjModel[num].ObjGroupNum = GlobalObjObjModel[i].ObjGroupNum;
							g_ObjObjModel[num].matGroups = GlobalObjObjModel[i].matGroups;
							strcpy(g_ObjObjModel[num].tag, GlobalObjObjModel[i].tag);
							for (int c = 0; c < 100; c++)
							{
								g_ObjObjModel[num].TrianglesList[c] = GlobalObjObjModel[i].TrianglesList[c];
								for (int d = 0; d < 3; d++)
									g_ObjObjModel[num].Tf[c][d] = GlobalObjObjModel[i].Tf[c][d];

							}
							for (int group = 0; group < g_ObjObjModel[num].matGroups; group++)
							{
								strcpy(g_ObjObjModel[num].TextureListPath[group].TextureListPathName, GlobalObjObjModel[i].TextureListPath[group].TextureListPathName);
								g_ObjObjModel[num].TextureListPath[group].TextureListPathNameLen = GlobalObjObjModel[i].TextureListPath[group].TextureListPathNameLen;
							}

							int ChgListIndex = RetTexuturePointListMapIndex(g_ObjObjModel[num].TexturePathNameLen, g_ObjObjModel[num].TexturePathName);
							ReLoadObjObjModelTexture_Ex(num, ChgListIndex, OBJOBJ_TEXTURE_TYPE_TEXTURE, 0);

							//重载Texture数组
							//如果自定义纹理没有设置
							for (int group = 0; group < g_ObjObjModel[num].matGroups; group++)
							{
								int ChgTextureListIndex = RetTexuturePointListMapIndex(g_ObjObjModel[num].TextureListPath[group].TextureListPathNameLen, g_ObjObjModel[num].TextureListPath[group].TextureListPathName);
								ReLoadObjObjModelTexture_Ex(num, ChgTextureListIndex, OBJOBJ_TEXTURE_TYPE_TEXTURELIST, group);
							}
							ReLoadObjObjModelModel_Ex(num, g_ObjObjModel[num].ModelPathName);

							g_ObjObjModel[num].use = true;
							nIdxObjObjModel = num;
							break;
						}
					}
				}
			}
			break;
		}
	}
	return nIdxObjObjModel;
}
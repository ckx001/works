#pragma once

//=============================================================================
// Desc: CAllocateHierarchy.h
//=============================================================================

#include <d3d9.h>
#include <d3dx9.h>
//#include "D3DUtil.h"


//-----------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: 
//-----------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};


//-----------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: 
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;            
	LPD3DXMESH           pOrigMesh;             
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD                NumAttributeGroups;    
	DWORD                NumInfl;               
	LPD3DXBUFFER         pBoneCombinationBuf;   
	D3DXMATRIX**         ppBoneMatrixPtrs;      
	D3DXMATRIX*          pBoneOffsetMatrices;   
	DWORD                NumPaletteEntries;     
	bool                 UseSoftwareVP;         
};


//-----------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: 
//-----------------------------------------------------------------------------
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR              Name, 
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD                     NumMaterials, 
		CONST DWORD *             pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);    
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};




//-----------------------------------------------------------------------------
// Desc: 
//-----------------------------------------------------------------------------
void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame , int type);
void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase ,int type);
HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot );
void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
D3DXVECTOR3 *GetPos(void);

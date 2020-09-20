//=============================================================================
// Desc: AllocateHierarchyClass.cpp
//=============================================================================
#include "main.h"
#include "AllocateHierarchyClass.h"
#include "debugproc.h"
#include "GlobalFunction.h"
#include "input.h"

//崪偺埵抲傪庢摼
D3DXVECTOR3 g_headPos;
D3DXVECTOR3 *GetPos(void);
//--------------------------------------------------------------------------------------
// Name: AllocateName()
// Desc: 僼傽僀儖傪撉傒崬傓
//--------------------------------------------------------------------------------------
HRESULT AllocateName( LPCSTR Name, LPSTR* pNewName )
{
	UINT cbLength;

	if( Name != NULL )
	{
		cbLength = ( UINT )strlen( Name ) + 1;
		*pNewName = new CHAR[cbLength];
		memcpy( *pNewName, Name, cbLength * sizeof( CHAR ) );
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: GenerateSkinnedMesh
// Desc: 僗僉儖儊僢僔儏惗惉
//--------------------------------------------------------------------------------------
HRESULT GenerateSkinnedMesh( IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer )
{
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps( &d3dCaps );

	if( pMeshContainer->pSkinInfo == NULL )
		return S_OK;

	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );


	if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
		pMeshContainer->pOrigMesh, 
		D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
		pMeshContainer->pAdjacency,
		NULL, NULL, NULL,
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups,
		&pMeshContainer->pBoneCombinationBuf,
		&pMeshContainer->MeshData.pMesh)))
		return E_FAIL;
	return S_OK;
}



//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateFrame()
// Desc: 僼儗乕儉傪嶌惉
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame( LPCSTR Name, LPD3DXFRAME* ppNewFrame )
{
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED *pFrame;

	*ppNewFrame = NULL;

	pFrame = new D3DXFRAME_DERIVED; 
	if (FAILED(AllocateName(Name, (LPSTR*)&pFrame->Name)))
	{
		delete pFrame;
		return hr;
	}

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer   = NULL;
	pFrame->pFrameSibling    = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

	return hr;
}


//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateMeshContainer()
// Desc: 儊僢僔儏僐儞僥僫傪嶌惉
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateMeshContainer( LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer )
{
	HRESULT hr;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;

	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	//偙偺僒儞僾儖偼僷僢僠儊僢僔儏傪張棟偟側偄偨傔丄尒偮偐偭偨応崌偼幐攕偟傑偡	
	if( pMeshData->Type != D3DXMESHTYPE_MESH )
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// pMesh僀儞僞乕僼僃僀僗億僀儞僞乕傪儊僢僔儏僨乕僞峔憿偐傜庢摼偟傑偡
	pMesh = pMeshData->pMesh;

	//偙偺僒儞僾儖偼FVF屳姺偺儊僢僔儏偱偼側偄偨傔丄儊僢僔儏偑尒偮偐偭偨偲偒偵幐攕偟傑偡
	if( pMesh->GetFVF() == 0 )
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	//僆乕僶乕儘乕僪偝傟偨峔憿傪妱傝摉偰偰丄D3DXMESH CONTAINER偲偟偰曉偟傑偡
	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	memset( pMeshContainer, 0, sizeof( D3DXMESHCONTAINER_DERIVED ) );

	//柤慜傪妋擣偟偰僐僺乕偟傑偡丅 擖椡偲偟偰慡偰偺儊儌儕偑敪怣幰偵懏偟丄僀儞僞乕僼僃僀僗偼偵傛偭偰懳張偡傞偙偲偑偱偒傑偡
	hr = AllocateName( Name, &pMeshContainer->Name );
	if( FAILED( hr ) )
		goto e_Exit;

	pMesh->GetDevice( &pd3dDevice );
	NumFaces = pMesh->GetNumFaces();

	//儊僢僔儏偵朄慄偑側偄応崌偼捛壛偟傑偡
	if( !( pMesh->GetFVF() & D3DFVF_NORMAL ) )
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		//儊僢僔儏傪僐僺乕偟偰朄慄梡偺僗儁乕僗傪嶌傝傑偡
		hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pMeshContainer->MeshData.pMesh );
		if( FAILED( hr ) )
			goto e_Exit;

		//巊梡偡傞儊僢僔儏僐儞僥僫偐傜怴偟偄pMesh億僀儞僞傪庢摼偟傑偡
        //拲丗傑偩嶲徠偟偰偄側偄偨傔丄pMesh傪儕儕乕僗偟傑偣傫
		pMesh = pMeshContainer->MeshData.pMesh;

		// pmesh偺朄慄傪惗惉偟傑偡
		D3DXComputeNormals( pMesh, NULL );
	}
	else  //朄慄偑側偄応崌丄儊僢僔儏僐儞僥僫偺儊僢僔儏傊偺嶲徠傪捛壛偟傑偡
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	//嵽椏忣曬傪奿擺偡傞偨傔偵儊儌儕傪妱傝摉偰傑偡丅 偙偺僒儞僾儖偺巊梡
	pMeshContainer->NumMaterials = max( 1, NumMaterials );
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces*3];

	memcpy( pMeshContainer->pAdjacency, pAdjacency, sizeof( DWORD ) * NumFaces*3 );
	memset( pMeshContainer->ppTextures, 0, sizeof( LPDIRECT3DTEXTURE9 ) * pMeshContainer->NumMaterials );

	//嵽椏偑採嫙偝傟偰偄傞応崌丄偦傟傜傪僐僺乕偟傑偡
	if( NumMaterials > 0 )
	{
		memcpy( pMeshContainer->pMaterials, pMaterials, sizeof( D3DXMATERIAL ) * NumMaterials );

		for( iMaterial = 0; iMaterial < NumMaterials; iMaterial++ )
		{
			if( pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL )
			{
				if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, pMeshContainer->pMaterials[iMaterial].pTextureFilename,
					&pMeshContainer->ppTextures[iMaterial] ) ) )
					pMeshContainer->ppTextures[iMaterial] = NULL;

				//摦揑儊儌儕傊偺億僀儞僞傪妎偊偰偄側偄丄儘乕僪屻偵柤慜傪朰傟傞
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else //儅僥儕傾儖偑採嫙偝傟側偄応崌丄僨僼僅儖僩偺儅僥儕傾儖傪巊梡偟傑偡
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset( &pMeshContainer->pMaterials[0].MatD3D, 0, sizeof( D3DMATERIAL9 ) );
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

    //僗僉僯儞僌忣曬偑偁傞応崌丄昁梫側僨乕僞傪曐懚偟偰偐傜丄HW僗僉僯儞僌偺僙僢僩傾僢僾
	if( pSkinInfo != NULL )
	{
		//丄SkinInfo偲尦偺儊僢僔儏僨乕僞傪曐懚偟傑偡
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		//捀揰傪恾嬻娫偐傜崪偺嬻娫偵堏摦偡傞偵偼僆僼僙僢僩峴楍偺攝楍偑昁梫偵側傝傑偡
		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];

		//奺儃乕儞僆僼僙僢僩儅僩儕僢僋僗傪庢摼偟偰丄屻偱庢摼偡傞昁梫偑側偄傛偆偵偟傑偡
		for( iBone = 0; iBone < cBones; iBone++ )
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *( pMeshContainer->pSkinInfo->GetBoneOffsetMatrix( iBone ) );
		}

		// GenerateSkinnedMesh偼丄堦斒揑側僗僉僯儞僌忣曬傪庢摼偟丄偦傟傪HW僼儗儞僪儕乕僶乕僕儑儞偵曄姺偟傑偡
		hr = GenerateSkinnedMesh( pd3dDevice, pMeshContainer );
		if( FAILED( hr ) )
			goto e_Exit;
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

e_Exit:
	SAFE_RELEASE( pd3dDevice );

	// Destroy娭悢傪屇傃弌偟偰丄妱傝摉偰傜傟偨儊儌儕傪揔愗偵僋儕乕儞傾僢僾偟傑偡
	if( pMeshContainer != NULL )
	{
		DestroyMeshContainer( pMeshContainer );
	}

	return hr;
}

//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyFrame()
// Desc: 僼儗乕儉偺奐曻
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame( LPD3DXFRAME pFrameToFree )
{
	SAFE_DELETE_ARRAY( pFrameToFree->Name );
	SAFE_DELETE( pFrameToFree );
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyMeshContainer()
// Desc: 儊僢僔儏僐儞僥僫偺奐曻
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase )
{
	UINT iMaterial;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = ( D3DXMESHCONTAINER_DERIVED* )pMeshContainerBase;

	SAFE_DELETE_ARRAY( pMeshContainer->Name );
	SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
	SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
	SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );

	// release all the allocated textures
	if( pMeshContainer->ppTextures != NULL )
	{
		for( iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++ )
		{
			SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
		}
	}

	SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pSkinInfo );
	SAFE_RELEASE( pMeshContainer->pOrigMesh );
	SAFE_DELETE( pMeshContainer );
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: SetupBoneMatrixPointers()
// Desc: 崪偺埵抲傪惂掕
//--------------------------------------------------------------------------------------
HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot )
{
	if( pFrameBase->pMeshContainer != NULL )
	{
		D3DXFRAME_DERIVED* pFrame = NULL;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrameBase->pMeshContainer;

		//僗僉儞儊僢僔儏偑偁傞応崌偼丄崪儅僩儕僢僋僗傪愝掕偟傑偡
		if (pMeshContainer->pSkinInfo != NULL)
		{
			UINT cBones = pMeshContainer->pSkinInfo->GetNumBones();
			pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
			for (UINT iBone = 0; iBone < cBones; iBone++)
			{
				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
				if (pFrame == NULL) return E_FAIL;

				pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}
	}

	if (pFrameBase->pFrameSibling != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameSibling, pFrameRoot)))
			return E_FAIL;
	}

	if (pFrameBase->pFrameFirstChild != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameFirstChild, pFrameRoot)))
			return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: DrawMeshContainer()
// Desc: 儊僢僔儏僐儞僥僫偺昤夋
//--------------------------------------------------------------------------------------
void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, int type)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = ( D3DXMESHCONTAINER_DERIVED* )pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = ( D3DXFRAME_DERIVED* )pFrameBase;
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps( &d3dCaps );

	//僗僉僯儞僌偺嵟弶偺僠僃僢僋
	if( pMeshContainer->pSkinInfo != NULL )
	{
		AttribIdPrev = UNUSED32;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>( pMeshContainer->pBoneCombinationBuf->GetBufferPointer() );

		//僨僶僀僗偺僨僼僅儖僩偺vtx張棟乮捠忢偼HW乯傪巊梡偟偰昤夋偟傑偡

		//PrintDebugProc("pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] %f\n", pMeshContainer->pBoneOffsetMatrices[iMatrixIndex]);
		//PrintDebugProc("ppMeshContainer->NumInfl %d\n", pMeshContainer->NumInfl);

		for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++ )
		{
			NumBlend = 0;
			for( DWORD i = 0; i < pMeshContainer->NumInfl; ++i )
			{
				if( pBoneComb[iAttrib].BoneId[i] != UINT_MAX )
				{
					NumBlend = i;
				}
			}

			if( d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1 )
			{
				//嵟弶偵尰嵼偺僽儗儞僪僂僃僀僩偺僙僢僩偺儚乕儖僪儅僩儕僢僋僗傪寁嶼偟丄僽儗儞僪悢偺惓妋側僇僂儞僩傪庢摼偟傑偡
				for( DWORD i = 0; i < pMeshContainer->NumInfl; ++i )//
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[i];


					if( iMatrixIndex != UINT_MAX )
					{

						D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
						if (type == 0)
						{
							pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
						}
						if (type == 1)
						{
							// position shadow
							//D3DXVECTOR4 lightDirection(-0.5f, -0.6f, -0.3f, 0.0f);
							D3DXVECTOR4 lightDirection(-0.707f, -0.707f, -0.707f, 0.0f);
							D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 21.0f);

							D3DXMATRIX S;
							D3DXMatrixShadow(
								&S,
								&lightDirection,
								&groundPlane);


							D3DXMATRIX W = matTemp * S;

							//W._42 = 1.0f;

							pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &W);
						}

						if (iAttrib == 40 && i == 3)
						{

							//PrintDebugProc("iMatrixIndex %d\n", iMatrixIndex);
							//PrintDebugProc("ppBoneMatrixPtrs[%d] %f,%f,%f,%f\n", i, matTemp._11, matTemp._12, matTemp._13, matTemp._14);
							//PrintDebugProc("ppBoneMatrixPtrs[%d] %f,%f,%f,%f\n", i, matTemp._21, matTemp._22, matTemp._23, matTemp._24);
							//PrintDebugProc("ppBoneMatrixPtrs[%d] %f,%f,%f,%f\n", i, matTemp._31, matTemp._32, matTemp._33, matTemp._34);
							//PrintDebugProc("ppBoneMatrixPtrs[%d] %f,%f,%f,%f\n", i, matTemp._41, matTemp._42, matTemp._43, matTemp._44);

							g_headPos = D3DXVECTOR3(matTemp._41, matTemp._42, matTemp._43);

						}

					}
				}

				pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, NumBlend );

				//face偺偙偺僒僽僙僢僩偵巊梡偝傟傞儅僥儕傾儖傪専嶕偟傑偡
				if( ( AttribIdPrev != pBoneComb[iAttrib].AttribId ) || ( AttribIdPrev == UNUSED32 ) )
				{
					pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
					pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
					AttribIdPrev = pBoneComb[iAttrib].AttribId;
				}
				/////////////////////////


				/////////////////////////
				//惓偟偄儅僥儕傾儖偲儅僩儕僢僋僗偑儘乕僪偝傟偨偺偱丄僒僽僙僢僩傪昤夋偟傑偡
				if (type == 1)
				{
					pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, true);
					pd3dDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
					pd3dDevice->SetRenderState(D3DRS_STENCILREF, 0x0);
					pd3dDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
					pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
					pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
					pd3dDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
					pd3dDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR); // increment to 1

					// alpha blend the shadow
					pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

					//D3DMATERIAL9 mtrl = InitMtrl(BLACK_COLOR, BLACK_COLOR, BLACK_COLOR, BLACK_COLOR, 0.0f);
					//mtrl.Diffuse.a = 0.9f;    // 50% transparancy
					
					DWORD dwArg1Value = 0;
					pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwArg1Value);
					DWORD dwOpValue = 0;
					pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwOpValue);
					// 默认是alpha来自于D3DTA_TEXTURE;  D3DTSS_ALPHAOP来自于D3DTSS_COLORARG1;
					pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);

					D3DMATERIAL9 mtrl;

					mtrl.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					mtrl.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					mtrl.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					mtrl.Power = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					mtrl.Diffuse.a = 0.5f; // 50% transparency.


					// Disable depth buffer so that z-fighting doesn't occur when we
					// render the shadow on top of the floor.
					//pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);

					pd3dDevice->SetMaterial(&mtrl);
					pd3dDevice->SetTexture(0, 0);
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);

					pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
					//pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
					//pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, false);

				}
				if (type == 0)
				{
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
				}

			}
		}
		
		//pMeshContainer->MeshData.pMesh->DrawSubset(player->index);
		//PrintDebugProc("player->index[%d]\n", player->index);

		pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, 0 );


	}
	else  //昗弨儊僢僔儏丄儅僥儕傾儖僾儘僷僥傿傪愝掕偟偨屻偵昤夋偡傞
	{
		pd3dDevice->SetTransform( D3DTS_WORLD, &pFrame->CombinedTransformationMatrix );

		for( iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++ )
		{
			pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
			pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
			pMeshContainer->MeshData.pMesh->DrawSubset( iMaterial );
		}
	}
}


//---------------------------------------------------------------------------------------
// Name: DrawFrame()
// Desc: 僼儗乕儉偺昤夋
//--------------------------------------------------------------------------------------
void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame, int type)
{
	if (pFrame == NULL) return;
	LPD3DXMESHCONTAINER pMeshContainer;
	pMeshContainer = pFrame->pMeshContainer;                   
	while( pMeshContainer != NULL )                     //2 
	{
		DrawMeshContainer(pd3dDevice, pMeshContainer, pFrame, type);
		pMeshContainer = pMeshContainer->pNextMeshContainer;    
	}
	//56
	DrawFrame(pd3dDevice, pFrame->pFrameSibling, type);
	DrawFrame(pd3dDevice, pFrame->pFrameFirstChild, type);
}

//--------------------------------------------------------------------------------------
// Name: UpdateFrameMatrices()0
// Desc: 僼儗乕儉峴楍偺峏怴

//--------------------------------------------------------------------------------------
void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix )
{
	if (pFrameBase == NULL || pParentMatrix == NULL) return;
	D3DXFRAME_DERIVED* pFrame = ( D3DXFRAME_DERIVED* )pFrameBase;

	D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);

	UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);                             
	UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);   
}

//--------------------------------------------------------------------------------------
// Name: GetPos()
// Desc: 崪偺埵抲傪庢摼
//--------------------------------------------------------------------------------------

D3DXVECTOR3 *GetPos(void)
{
	return(&g_headPos);
}

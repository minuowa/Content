#include "GGameDemoHeader.h"
#include "GAnimationResource.h"
#include "GAllocateHierarchy.h"
#include "GD9Device.h"
HRESULT GAnimationResource::SetupBoneMatrixPointers ( LPD3DXFRAME pFrame )
{
    HRESULT hr = S_FALSE;

    if ( pFrame == NULL )
    {
        return S_FALSE;
    }

    if ( pFrame->pMeshContainer != NULL )
    {
        hr = SetupBoneMatrixPointersOnMesh ( pFrame->pMeshContainer );

        if ( FAILED ( hr ) )
        {
            return hr;
        }
    }

    if ( pFrame->pFrameSibling != NULL )
    {
        hr = SetupBoneMatrixPointers ( pFrame->pFrameSibling );

        if ( FAILED ( hr ) )
        {
            return hr;
        }
    }

    if ( pFrame->pFrameFirstChild != NULL )
    {
        hr = SetupBoneMatrixPointers ( pFrame->pFrameFirstChild );

        if ( FAILED ( hr ) )
        {
            return hr;
        }
    }

    return S_OK;
}
GBoneLinker * GAnimationResource::getBoneInfo ( const char*sBoneName )
{
    GBoneLinker *pBoneInfo = NULL;

    CXASSERT ( sBoneName );
    GBoneLinkerMap::iterator i = mBoneInfos.find ( sBoneName );

    if ( i != mBoneInfos.end() )
    {
        return i->second;
    }

    return pBoneInfo;
}
HRESULT GAnimationResource::SetupBoneMatrixPointersOnMesh ( LPD3DXMESHCONTAINER pMeshContainerBase )
{
    D3DXFrameEX *pFrameEx = NULL;

    if ( pMeshContainerBase == NULL || pMeshContainerBase->pSkinInfo == NULL )
    {
        return S_FALSE;
    }

    D3DXMeshContainerEX *pMeshContainerEx = ( D3DXMeshContainerEX* ) pMeshContainerBase;

    DWORD dwNumBone = pMeshContainerEx->pSkinInfo->GetNumBones();

    pMeshContainerEx->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBone];

    if ( pMeshContainerEx->ppBoneMatrixPtrs == NULL )
    {
        return E_OUTOFMEMORY;
    }

    for ( DWORD i = 0; i < dwNumBone; i++ )
    {
        GString sBoneName = pMeshContainerEx->pSkinInfo->GetBoneName ( i );

        pFrameEx = ( D3DXFrameEX* ) D3DXFrameFind ( mFrameRoot, sBoneName );

        if ( pFrameEx == NULL )
        {
            return S_FALSE;
        }

        addBoneInfo ( sBoneName, & ( pFrameEx->matCombinedTransformation ) );


        pMeshContainerEx->ppBoneMatrixPtrs[i] = & ( pFrameEx->matCombinedTransformation );
    }

    return S_OK;

}
int GAnimationResource::addBoneInfo ( const char*sBoneName, D3DXMATRIX *pmat )
{
    CXASSERT ( sBoneName );
    GBoneLinker*  linker = new GBoneLinker;
    linker->mName = sBoneName;
    linker->mTransform = pmat;

    if ( !mBoneInfos.Insert ( sBoneName, linker ) )
    {
        delete linker;
        CXASSERT ( 0 );
        return 0;
    }

    return mBoneInfos.size();
}

void GAnimationResource::updateBones()
{
    SetupBoneMatrixPointers ( mFrameRoot );
}
bool GAnimationResource::createFromFile ( const char* name )
{
    CXASSERT ( name );
    GAllocateHierarchy Alloc ( D9Device->GetDvc(), name );
    HRESULT hr  = D3DXLoadMeshHierarchyFromXA ( name, D3DXMESH_MANAGED, D9Device->GetDvc(), &Alloc, NULL,
                  &mFrameRoot, &mAnimationController );
    CXASSERT_RESULT_FALSE ( hr );
    updateBones();
    return true;
}

GAnimationResource::~GAnimationResource()
{
    GAllocateHierarchy alloctor;
	D3DXFrameDestroy ( mFrameRoot, &alloctor );
    dSafeRelease ( mAnimationController );
    dSafeDeleteMap2 ( mBoneInfos );
}

D3DXMeshContainerEX::~D3DXMeshContainerEX()
{
    //dSafeDelete ( ppBoneMatrixPtrs );
}

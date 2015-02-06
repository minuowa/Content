#include "GGameDemoHeader.h"
#include "GMeshData.h"
#include "GD9Device.h"
#include "GTexture.h"


void GMetrialData::set()
{
    D9Device->GetDvc()->SetTexture ( 0, mTexture->getTexture() );
    D9Device->GetDvc()->SetMaterial ( &mMat );
}

void GMetrialData::setTexture ( const char* fileName )
{
    mTexture = TextureMgr->getResource ( fileName );
    CXASSERT ( mTexture );
}

void GMetrialData::setMetiral ( const D3DMATERIAL9& d9matrial )
{
    mMat = d9matrial;
    //dMemoryCopy ( &mMat, ( void* ) &d9matrial, sizeof ( d9matrial ) );
}

GMetrialData::GMetrialData ( void )
    : mTexture ( 0 )
{
    dMemoryZero ( &mMat, sizeof ( mMat ) );
}

D3DMATERIAL9 GMetrialData::mDefaultWhite =
{
    {0.6f, 0.6f, 0.6f, 0.6f,},
    {1.0f, 1.0f, 1.0f, 1.0f,},
    {0.6f, 0.6f, 0.6f, 0.6f,},
    {0.6f, 0.6f, 1.0f, 1.0f,},
    5.f,
};

void GGraphVertexBuffer::set()
{
    D9Device->GetDvc()->SetStreamSource ( 0, mD9VertexBuffer, 0, mElementSize );
    D9Device->GetDvc()->SetFVF ( mFVF );
}

void GGraphVertexBuffer::release()
{
    dSafeRelease ( mD9VertexBuffer );
}

DWORD GGraphVertexBuffer::allSize()
{
    return mElementCount * mElementSize;
}

bool GGraphVertexBuffer::recreate ( D3DPOOL poolType )
{
    release();

    CXASSERT_RETURN_FALSE ( mElementSize );
    CXASSERT_RETURN_FALSE ( mElementCount );
    CXASSERT_RETURN_FALSE ( mFVF );

    D9Device->GetDvc()->CreateVertexBuffer (
        allSize()
        , 0
        , mFVF
        , poolType
        , &mD9VertexBuffer
        , 0
    );
    CXASSERT_RETURN_FALSE ( mD9VertexBuffer );
    return true;
}



bool GGraphVertexBuffer::fill ( void* data )
{
    CXCheck ( mD9VertexBuffer );
    void* pdata;
    CXASSERT_RESULT_FALSE ( mD9VertexBuffer->Lock ( 0, allSize(), &pdata, 0 ) );
    dMemoryCopy ( pdata, data, allSize() );
    mD9VertexBuffer->Unlock();
    return true;
}

bool GGraphVertexBuffer::isValid()
{
    return mElementSize > 0 && mD9VertexBuffer != nullptr && mFVF != 0 && mElementCount;
}

GGraphVertexBuffer::GGraphVertexBuffer()
    : mD9VertexBuffer ( nullptr )
    , mElementSize ( 0 )
    , mFVF ( 0 )
    , mElementCount ( 0 )
{

}

void GGraphIndexBuffer::set()
{
    D9Device->GetDvc()->SetIndices ( mD9IndexBuffer );
}

void GGraphIndexBuffer::release()
{
    dSafeRelease ( mD9IndexBuffer );
}

bool GGraphIndexBuffer::recreate ( D3DPOOL poolType )
{
    CXCheck ( mIndexCount );
    D9Device->GetDvc()->CreateIndexBuffer (
        mIndexCount * sizeof ( DWORD )
        , 0
        , D3DFMT_INDEX32
        , poolType/*D3DPOOL_MANAGED*/
        , &mD9IndexBuffer
        , 0
    );
    CXCheck ( mD9IndexBuffer );
    return mD9IndexBuffer != nullptr;
}

GGraphIndexBuffer::GGraphIndexBuffer()
    : mD9IndexBuffer ( nullptr )
    , mIndexCount ( 0 )
{

}

bool GGraphIndexBuffer::isValid()
{
    return mIndexCount > 2 && mD9IndexBuffer != nullptr;
}

bool GGraphIndexBuffer::fill ( void* data )
{
    DWORD* pVBCoordIndex = NULL;

    CXASSERT_RESULT_FALSE ( mD9IndexBuffer->Lock ( 0, 6 * sizeof ( DWORD ), ( void** ) &pVBCoordIndex, 0 ) );
    dMemoryCopy ( pVBCoordIndex, data, mIndexCount * sizeof ( DWORD ) );
    mD9IndexBuffer->Unlock();
    return true;
}

GGraphPrimitive::GGraphPrimitive()
    : mVertexCount ( nullptr )
{
    mType = D3DPT_TRIANGLELIST;
    mVertexCount = 0;
}

void GGraphPrimitive::draw()
{
    CXCheck ( mVertexCount );
    D9Device->GetDvc()->DrawIndexedPrimitive ( mType, 0, 0, *mVertexCount, 0, mPrimitiveCount );
}

GGraphPrimitive::~GGraphPrimitive()
{
}

void GGraphPrimitive::setVB ( const GGraphVertexBuffer& vb )
{
    mVertexCount = (u16*)&vb.mElementCount;
}

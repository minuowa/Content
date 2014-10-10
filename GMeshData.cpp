#include "GGameDemoHeader.h"
#include "GMeshData.h"
#include "GD9Device.h"
#include "GTexture.h"


void GMetrialData::Render()
{
    D9DEVICE->GetDvc()->SetTexture ( 0, mTexture->getTexture() );
    D9DEVICE->GetDvc()->SetMaterial ( &mMat );
}

void GMetrialData::SetTexture ( const char* fileName )
{
    mTexture = TextureMgr->getResource ( fileName );
    CXASSERT ( mTexture );
}

void GMetrialData::SetMetiral ( const D3DMATERIAL9& d9matrial )
{
    dMemoryCopy ( &mMat, ( void* ) &d9matrial, sizeof ( d9matrial ) );
}

GMetrialData::GMetrialData ( void )
    : mTexture ( 0 )
{
    dMemoryZero ( &mMat, sizeof ( mMat ) );
}

void GGraphVertexBuffer::Set()
{
    D9DEVICE->GetDvc()->SetStreamSource ( 0, mD9VertexBuffer, 0, mDataSize );
    D9DEVICE->GetDvc()->SetFVF ( mFVF );
}

void GGraphIndexBuffer::Set()
{
    D9DEVICE->GetDvc()->SetIndices ( mD9IndexBuffer );
}

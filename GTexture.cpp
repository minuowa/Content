#include "GGameDemoHeader.h"
#include "GTexture.h"
#include "GD9Device.h"
#include "Content.h"

GTexture::GTexture()
    : mD3DTexture ( 0 )
{

}

bool GTexture::createFromFile ( const char* name )
{
    mFileName = name;
    dSafeRelease ( mD3DTexture );
    D3DXCreateTextureFromFileExA (
         Content::Device.getD9Device(),
        name,
        D3DX_FROM_FILE,
        D3DX_FROM_FILE,
        D3DX_FROM_FILE,
        0,
        D3DFMT_UNKNOWN,
        D3DPOOL_MANAGED,
        D3DX_FILTER_TRIANGLE,
        D3DX_FILTER_TRIANGLE,
        0,
        NULL,
        NULL,
        &mD3DTexture );
	if (mD3DTexture)
	{
		D3DSURFACE_DESC surfaceDesc;    //高度图纹理表面
		mD3DTexture->GetLevelDesc ( 0, &surfaceDesc );
		mHeight = surfaceDesc.Height;
		mWidth = surfaceDesc.Width;
	}
    return nullptr != mD3DTexture;
}

GTexture::~GTexture()
{
    dSafeRelease ( mD3DTexture );
}

void GTexture::destory()
{
    delete this;
}

void GTexture::set ( int stage )
{
    bool useTexture = mD3DTexture != nullptr;

     Content::Device.getD9Device()->SetTexture ( stage,  mD3DTexture );

     Content::Device.getD9Device()->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
     Content::Device.getD9Device()->SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
     Content::Device.getD9Device()->SetTextureStageState ( 0, D3DTSS_COLOROP, useTexture ? D3DTOP_MODULATE : D3DTOP_SELECTARG2 );

     Content::Device.getD9Device()->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
     Content::Device.getD9Device()->SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
     Content::Device.getD9Device()->SetTextureStageState ( 0, D3DTSS_ALPHAOP, useTexture ? D3DTOP_MODULATE : D3DTOP_SELECTARG2 );
}

int GTexture::width() const
{
	return mWidth;
}

int GTexture::height() const
{
	return mHeight;
}



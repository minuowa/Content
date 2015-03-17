#include "GGameDemoHeader.h"
#include "GHeightMap.h"
#include "GTexture.h"
#include "Content.h"


GHeightMap::GHeightMap ( float Width, float Length, float MinHeight, float MaxHeight, const char* FileName )
{
    mFileName = FileName;
    mDatas = nullptr;
    mWidth = 0;
    mHeight = 0;
    mRanageX = Width;
    mRanageZ = Length;
    mMinHeight = MinHeight;
    mMaxHeight = MaxHeight;
}

float GHeightMap::GetHeight ( float X, float Z )
{
    //ZIndex越大越靠近高度图上边
    int XIndex = ( int ) ( ( X + mRanageX / 2.0f ) / mRanageX * ( mWidth - 1 ) );
    int ZIndex = ( int ) ( ( Z + mRanageZ / 2.0f ) / mRanageZ * ( mHeight - 1 ) );
    CXASSERT ( XIndex <= mWidth );
    CXASSERT ( ZIndex <= mHeight );
    CXColor color;
    color.Color = mDatas[XIndex + mWidth * ( mHeight - 1 - ZIndex )];
    return color.B * ( mMaxHeight - mMinHeight ) / 255.0f;
}

void GHeightMap::SetHeight ( float X, float Z, float Y )
{
    byte b = ( byte ) ( Y / ( float ) ( mMaxHeight - mMinHeight ) * 255.0f );
    if ( b > 255 )
    {
        b = 255;
    }
    int XIndex = ( int ) ( ( X + mRanageX / 2.0f ) / mRanageX * ( mWidth - 1 ) );
    int ZIndex = ( int ) ( ( Z + mRanageZ / 2.0f ) / mRanageZ * ( mHeight - 1 ) );
    CXColor color;
    color.A = 255;
    color.R = b;
    color.G = b;
    color.B = b;
    mDatas[XIndex + mWidth * ( mHeight - 1 - ZIndex )] = color;
}

void GHeightMap::Save()
{
    //Bitmap HeightMap = new Bitmap ( Width, Height, PixelFormat.Format32bppArgb );
    //for ( int i = 0; i < Height; i++ )
    //{
    //	for ( int j = 0; j < Width; j++ )
    //		HeightMap.SetPixel ( i, j, Color.FromArgb ( Datas[j * Width + i] ) );
    //}
    //HeightMap.Save ( FileName );
}

bool GHeightMap::load()
{
    D3DSURFACE_DESC surfaceDesc;    //高度图纹理表面

    D3DLOCKED_RECT rcLock;	//锁定的表面
    GTexture* texture =  Content::TextureMgr.getResource ( mFileName.c_str() );
    CXASSERT ( nullptr != texture );
    IDirect3DTexture9* d9texture = texture->getTexture();

    d9texture->GetLevelDesc ( 0, &surfaceDesc );

    mHeight = surfaceDesc.Height;
    mWidth = surfaceDesc.Width;

    CXASSERT_RESULT_FALSE ( d9texture->LockRect ( 0, &rcLock, NULL, 0 ) );

    DWORD dwPitch = rcLock.Pitch / sizeof ( DWORD );
    dSafeDelete ( mDatas );
    DWORD *pBit = ( DWORD* ) rcLock.pBits;
    mDatas = new DWORD[mWidth * mHeight];
    dMemoryCopy ( mDatas, pBit, mWidth * mHeight * sizeof ( DWORD ) );
    d9texture->UnlockRect ( 0 );
    return true;
}

GHeightMap::~GHeightMap()
{
    dSafeDelete ( mDatas );
}

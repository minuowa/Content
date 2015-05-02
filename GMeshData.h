#pragma once
#include "GTexture.h"
#include "GRender.h"
class GGraphIndexBuffer
{
public:
    GGraphIndexBuffer();
    void set();
    void release();
    bool recreate ( D3DPOOL poolType );
    bool fill ( void* data );
    bool isValid();
public:
    DWORD mIndexCount;
protected:
    IDirect3DIndexBuffer9* mD9IndexBuffer;
};
class GGraphVertexBuffer
{
public:
    GGraphVertexBuffer();
    void set();
    void release();
    DWORD allSize();
    bool recreate ( D3DPOOL poolType );
	bool fill ( void* data,u32 elementCnt=0 );
    bool isValid();
    DWORD mFVF;
    WORD mElementCount;
    WORD mElementSize;
protected:
    IDirect3DVertexBuffer9*	mD9VertexBuffer;
};
class GGraphPrimitive
{
public:
    GGraphPrimitive();
    ~GGraphPrimitive();
    void draw();
    void setVB ( const GGraphVertexBuffer& vb );

    D3DPRIMITIVETYPE mType;
    u16* mVertexCount;
    u32 mPrimitiveCount;
	GRenderTech mRenderTech;
};
class GMetrialData
{
public:
    static D3DMATERIAL9 mDefaultWhite;
public:
    GMetrialData ( void );
    void	setTexture ( const char* fileName );
    void	setMetiral ( const D3DMATERIAL9& d9matrial );
    void	set();
private:
    GTexture*			mTexture;
    D3DMATERIAL9		mMat;

};
typedef CXDynaArray<GMetrialData*>	GRenderDataArr;

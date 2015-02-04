#include "GGameDemoHeader.h"

#include "GWorldCorrd.h"
#include "GD9Device.h"
//地图坐标系
#define FVF_XYZDIFUSE D3DFVF_XYZ|D3DFVF_DIFFUSE
struct VertexXYZAndColor
{
	float x, y, z;
	D3DCOLOR color;
};


GWorldCorrd::GWorldCorrd ( void )
	: mLineCount ( 50 )
	, mWidth ( 100.f )
{
	mIBCoord = NULL;
	mIBLines = NULL;
	mVBCoord = NULL;
	mVBLines = NULL;

	D9DEVICE->mOnLostDevice += this;
	D9DEVICE->mOnResetDevice += this;
}

GWorldCorrd::~GWorldCorrd ( void )
{
	dSafeRelease ( mIBCoord );
	dSafeRelease ( mIBLines );
	dSafeRelease ( mVBCoord );
	dSafeRelease ( mVBLines );

	D9DEVICE->mOnLostDevice -= this;
	D9DEVICE->mOnResetDevice -= this;
}

bool GWorldCorrd::recreate()
{
	if ( !__super::recreate() )
		return false;
	return recreateAll();
}

bool GWorldCorrd::render()
{
	if ( !__super::render() )
		return false;

	D9DEVICE->GetDvc()->SetStreamSource ( 0, mVBCoord, 0, sizeof ( VertexXYZAndColor ) );
	D9DEVICE->GetDvc()->SetFVF ( FVF_XYZDIFUSE );
	D9DEVICE->GetDvc()->SetIndices ( mIBCoord );
	D9DEVICE->GetDvc()->DrawIndexedPrimitive ( D3DPT_LINELIST, 0, 0, 6, 0, 3 );
	D9DEVICE->GetDvc()->SetStreamSource ( 0, mVBLines, 0, sizeof ( VertexXYZAndColor ) );
	D9DEVICE->GetDvc()->SetFVF ( FVF_XYZDIFUSE );
	D9DEVICE->GetDvc()->SetIndices ( mIBLines );
	D9DEVICE->GetDvc()->SetTexture ( 0, nullptr );
	D9DEVICE->GetDvc()->DrawIndexedPrimitive ( D3DPT_LINELIST, 0, 0, 4 * mLineCount, 0, 2 * mLineCount );

	return true;
}

void GWorldCorrd::operator= ( GWorldCorrd cd )
{
	mWidth = cd.mWidth;
	mVBCoord = cd.mVBCoord;
	mVBLines = cd.mVBLines;
	mIBLines = cd.mIBLines;
}

void GWorldCorrd::onCallBack ( const CXDelegate& delgate, CXEventArgs* )
{
	if ( delgate == D9DEVICE->mOnLostDevice )
	{
		//CXSafeRelease(mIBLines);
		//CXSafeRelease(mIBCoord);
		//CXSafeRelease(mVBLines);
		//CXSafeRelease(mVBCoord);
	}
	else if ( delgate == D9DEVICE->mOnResetDevice )
	{
		//recreate();
	}
}

bool GWorldCorrd::recreateAll()
{
	VertexXYZAndColor aCoord[6] =
	{
		{100.0f, 0.0f, 0.0f, 0xffffff00}, //x轴黄色
		{ -100.0f, 0.0f, 0.0f, 0xffffff00},
		{0.0f, 100.0f, 0.0f, 0xffff0000}, //y轴红色
		{0.0f, -100.0f, 0.0f, 0xffff0000},
		{0.0f, 0.0f, 100.0f, 0xff00ff00}, //z轴绿色
		{0.0f, 0.0f, -100.0f, 0xff00ff00},
	};
	VertexXYZAndColor* pVBCoord;
	D9DEVICE->GetDvc()->CreateVertexBuffer ( 6 * sizeof ( VertexXYZAndColor ), 0, FVF_XYZDIFUSE, D3DPOOL_MANAGED/*D3DPOOL_MANAGED*/, &mVBCoord, 0 );
	CXASSERT_RETURN_FALSE ( mVBCoord );
	mVBCoord->Lock ( 0, 6 * sizeof ( VertexXYZAndColor ), ( void** ) &pVBCoord, 0 );
	memcpy ( pVBCoord, aCoord, 6 * sizeof ( VertexXYZAndColor ) );
	mVBCoord->Unlock();
	DWORD aIndex[6] =
	{
		0, 1,
		2, 3,
		4, 5,
	};
	DWORD* pVBCoordIndex = NULL;
	//DVC->GetDVC()->CreateIndexBuffer(2*3*sizeof(DWORD),0,D3DFMT_INDEX32,D3DPOOL_MANAGED,&mIBCoord,0);
	D9DEVICE->GetDvc()->CreateIndexBuffer ( 6 * sizeof ( DWORD ), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED/*D3DPOOL_MANAGED*/, &mIBCoord, 0 );
	mIBCoord->Lock ( 0, 6 * sizeof ( DWORD ), ( void** ) &pVBCoordIndex, 0 );
	memcpy ( pVBCoordIndex, aIndex, 6 * sizeof ( DWORD ) );
	mIBCoord->Unlock();

	DWORD  dwOffsetOfLines = mLineCount * 2;
	VertexXYZAndColor* pVBLine;
	D9DEVICE->GetDvc()->CreateVertexBuffer ( mLineCount * 2 * 2 * sizeof ( VertexXYZAndColor ), 0, FVF_XYZDIFUSE, D3DPOOL_MANAGED, &mVBLines, 0 );
	mVBLines->Lock ( 0, mLineCount * 2 * 2 * sizeof ( VertexXYZAndColor ), ( void** ) &pVBLine, 0 );

	static const D3DCOLOR colorX = D3DCOLOR_ARGB ( 255, 0, 100, 0 );
	static const D3DCOLOR colorZ = D3DCOLOR_ARGB ( 255, 100, 100, 100 );

	for ( int u = 0; u < mLineCount; u++ )
	{
		pVBLine[2 * u].x = - ( mLineCount - 1 ) * mWidth / 2.0f;
		pVBLine[2 * u].y = 0.0f;
		pVBLine[2 * u].z = ( u - ( mLineCount - 1 ) / 2.0f ) * mWidth;
		pVBLine[2 * u].color = colorX;

		pVBLine[2 * u + 1].x = ( mLineCount - 1 ) * mWidth / 2.0f;
		pVBLine[2 * u + 1].y = 0.0f;
		pVBLine[2 * u + 1].z = ( u - ( mLineCount - 1 ) / 2.0f ) * mWidth;
		pVBLine[2 * u + 1].color = colorX;
	}

	for ( int v = 0; v < mLineCount; v++ )
	{
		pVBLine[dwOffsetOfLines + 2 * v].x = ( v - ( mLineCount - 1 ) / 2.0f ) * mWidth;
		pVBLine[dwOffsetOfLines + 2 * v].y = 0.0f;
		pVBLine[dwOffsetOfLines + 2 * v].z = - ( mLineCount - 1 ) * mWidth / 2.0f;
		pVBLine[dwOffsetOfLines + 2 * v].color = colorZ;

		pVBLine[dwOffsetOfLines + 2 * v + 1].x = ( v - ( mLineCount - 1 ) / 2.0f ) * mWidth;
		pVBLine[dwOffsetOfLines + 2 * v + 1].y = 0.0f;
		pVBLine[dwOffsetOfLines + 2 * v + 1].z = ( mLineCount - 1 ) * mWidth / 2.0f;
		pVBLine[dwOffsetOfLines + 2 * v + 1].color = colorZ;
	}
	mVBLines->Unlock();

	DWORD* pIB;
	//DWORD pIB[mLineCount*4];

	D9DEVICE->GetDvc()->CreateIndexBuffer ( mLineCount * 4 * sizeof ( DWORD ), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &mIBLines, 0 );
	mIBLines->Lock ( 0, mLineCount * 4 * sizeof ( DWORD ), ( void** ) &pIB, 0 );
	for ( int n = 0; n < 2 * mLineCount; n++ )
	{
		pIB[2 * n] = 2 * n;
		pIB[2 * n + 1] = 2 * n + 1;
	}
	//memcpy(pIB,pIB,mLineCount*4sizeof(DWORD));
	mIBLines->Unlock();
	return true;
}

#include "GGameDemoHeader.h"
#include "GTerrainCommon.h"

EXVertex::EXVertex ( float x, float y, float z, float nx, float ny, float nz, int color, float tu, float tv )
    : Pos ( x, y, z )
    , Normal ( nx, ny, nz )
    //, Txt1 ( 0, 0, 0, 0 )
{
    Color = color;
    Tu = tu;
    Tv = tv;
}

EXVertex::EXVertex()
{
    dMemoryZeroStruct ( this );
}
GCubeBound::GCubeBound()
{
	mMinX = FLOAT_MAX;
	mMaxX = FLOAT_MIN;

	mMinY = FLOAT_MAX;
	mMaxY = FLOAT_MIN;

	mMinZ = FLOAT_MAX;
	mMaxZ = FLOAT_MIN;

	mRadius = 0;
}

void GCubeBound::updateRadius()
{
	mRadius = 0.25 * sqrt ( dSequare ( mMaxX - mMinX ) + dSequare ( mMaxY - mMinY ) + dSequare ( mMaxZ - mMinZ ) );
}

void GCubeBound::updateCenter()
{
	mCenter.x = ( mMaxX + mMinX ) / 2.0f;
	mCenter.y = ( mMinY + mMaxY ) / 2.0f;
	mCenter.z = ( mMinZ + mMaxZ ) / 2.0f;
}

HitInfo::HitInfo()
{
	Hited = false;
	dMemoryZeroArray ( Index );
	U = 0.0f;
	V = 0.0f;
	Distance = 0.f;
	TriangleIndex = 0;
}

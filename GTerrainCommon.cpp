#include "GGameDemoHeader.h"
#include "GTerrainCommon.h"

EXVertex::EXVertex ( float x, float y, float z, float nx, float ny, float nz, int color, float tu, float tv )
    : Pos ( x, y, z )
    , Normal ( nx, ny, nz )
    , Txt1 ( 0, 0, 0, 0 )
{
    Color = color;
    Tu = tu;
    Tv = tv;
}

EXVertex::EXVertex()
{
    dMemoryZeroStruct ( this );
}

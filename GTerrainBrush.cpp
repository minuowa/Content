#include "GGameDemoHeader.h"
#include "GTerrainBrush.h"
#include "GTexture.h"
#include "Content.h"

struct GBrushString
{
    const char* mName;
    eTerrainBrushType mType;
    static eTerrainBrushType getType ( const char* name );
};



static const GBrushString gBrushString[] =
{
    {"A", eTerrainBrushType_A,},
    {"R", eTerrainBrushType_R,},
    {"G", eTerrainBrushType_G,},
    {"B", eTerrainBrushType_B,},
};

eTerrainBrushType GBrushString::getType ( const char* name )
{
for ( auto & s: gBrushString )
    {
        if ( dStrEqual ( name, s.mName ) )
        {
            return s.mType;
        }
    }
    return eTerrainBrushType_Count;
}
GTerrainBrush::GTerrainBrush ( void )
{
    dMemoryZeroArray ( mTexture );
}


GTerrainBrush::~GTerrainBrush ( void )
{
}

void GTerrainBrush::setXMLFile ( const char* name )
{
    mXMLFile = name;
}

bool GTerrainBrush::recreate()
{
    this->clear();

    xml_load ( mXMLFile.c_str() );
    CXFileName filename ( mXMLFile.c_str() );
    xml_get_node ( "Brush" )
    {
        GString stype;
        xml_get_attr ( "Color", stype );
        GString file;
        xml_get_attr ( "File", file );
        file.insert ( 0, 1, CXFileName::PathSpliter );
        file.insert ( 0, filename.GetRelativePath() );
        mTexture[GBrushString::getType ( stype.c_str() )] =  Content::TextureMgr.getResource ( file );
    }

    return true;
}

void GTerrainBrush::clear()
{
for ( auto t: mTexture )
    {
        if ( t != nullptr )
        {
             Content::TextureMgr.destoryResource ( t->mFileName );
        }
    }
}



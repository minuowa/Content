#include "GGameDemoHeader.h"
#include "GFilmFrame.h"


GFilmFrame::GFilmFrame ( void )
    : mLifeTime ( 0 )
    , mElapsedTime ( 0 )
    , mNext ( nullptr )
{
}


GFilmFrame::~GFilmFrame ( void )
{
}

void GFilmFrame::setInfo ( CXMillSecond lifeMS, u32 id )
{
    mLifeTime = lifeMS;
    mID = id;
}

void GFilmFrame::tail ( GFilmFrame* next )
{
    CXASSERT ( next );
    mNext = next;
}

bool GFilmFrame::advanceTime ( u32 timeMS )
{
    mElapsedTime += timeMS;
    if ( mElapsedTime > mLifeTime )
    {
        mElapsedTime = 0;
        return false;
    }
    return true;
}

void GFilmFrame::start()
{
    luacpp::call<void> ( gLuaScript.getState(), "playFrame", mID );
}



#include "GGameDemoHeader.h"
#include "GFilmFrame.h"
#include "GFilmPlayer.h"


GFilmFrame::GFilmFrame ( void )
    : mLifeTime ( 0 )
    , mElapsedTime ( 0 )
{
    do
    {
        int id = mIDGenerator.alloc();
        mName.Format ( "FilmFrame%d", id );
    }
    while ( nullptr != FilmPlayer->getFrame ( mName.c_str() ) );

}


GFilmFrame::~GFilmFrame ( void )
{
}

void GFilmFrame::setInfo ( CXMillSecond life, const char* name)
{
    mLifeTime = life;
    mName = name;
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
    luacpp::call<void> ( gLuaScript.getState(), "playFrame", mName.c_str() );
}

const char* GFilmFrame::getName()
{
    return mName.c_str();
}

void GFilmFrame::setName ( const char* name )
{
    assert ( name );
    mName = name;
}

const char* GFilmFrame::getNext()
{
    return mNextFrame.c_str();
}

void GFilmFrame::setNext ( const char* next )
{
    mNextFrame = next;
}

const char* GFilmFrame::getPrev()
{
    return mPrevFrame;
}

void GFilmFrame::setPrev ( const char* prev )
{
    mPrevFrame = prev;
}

CXIDGenerator GFilmFrame::mIDGenerator ( 1, 500 );



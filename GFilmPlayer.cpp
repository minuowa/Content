#include "GGameDemoHeader.h"
#include "GFilmPlayer.h"
#include "GFilmFrame.h"
#include "GTimer.h"


GFilmPlayer::GFilmPlayer ( void )
    : mFilmMode ( eFilmModePlay )
{
    mEnd = false;
    mCurFrame = nullptr;
}


GFilmPlayer::~GFilmPlayer ( void )
{
    mNameMap.destroySecond();
}

bool GFilmPlayer::play ( const char* filename )
{
    if ( mFilmMode != eFilmModePlay )
        return false;

    this->clear();

    gLuaScript.doFile ( filename );
    gLuaScript.doString ( "main:playFilm()" );

    CXASSERT_RETURN_FALSE ( mNameMap.size() > 0 );

    this->setAllPrev();

    mCurFrame = updateHeader();

    {
        GFilmFrame* cur = mCurFrame;
        while ( cur )
        {
            GFilmAddFrameEvent event;
            event.mFrameName = cur->getName();
            event.mPrevName = cur->getPrev();
            mDelegateAddFrame.trigger ( &event );
            cur = getFrame ( cur->getNext() );
        }
    }
    if ( mCurFrame )
        mCurFrame->start();

    return mCurFrame != nullptr;
}



void GFilmPlayer::addFrame ( const char* name, const char* nextName, int lifeSeconds )
{
    GFilmFrame* frame = new GFilmFrame;
    frame->setInfo ( lifeSeconds * 1000, name );
    frame->setNext ( nextName );
    mNameMap.Insert ( frame->getName(), frame );
}

void GFilmPlayer::addFrameBehind ( const char* prevName )
{
    GFilmFrame* pPrev = getFrame ( prevName );
    GFilmFrame* pNew = new GFilmFrame;
    GFilmAddFrameEvent event;
    event.mFrameName = pNew->getName();
    event.mPrevName = prevName;
    mDelegateAddFrame.trigger ( &event );
    if ( pPrev )
    {
        pNew->setPrev ( prevName );
        pNew->setNext ( pPrev->getNext() );
        GFilmFrame* pNext = getFrame ( pPrev->getNext() );
        if ( pNext )
            pNext->setPrev ( pNew->getName() );
        pPrev->setNext ( pNew->getName() );
    }
    mNameMap.Insert ( pNew->getName(), pNew );
}

void GFilmPlayer::process()
{
    if ( mCurFrame == nullptr )
        return;
    DWORD frametime = TheTimer->getFrameTimems();
    if ( !mCurFrame->advanceTime ( frametime ) )
    {
        mCurFrame = getFrame ( mCurFrame->getNext() );
        if ( mCurFrame != nullptr )
            mCurFrame->start();
    }
}

void GFilmPlayer::save()
{

}

GFilmFrame* GFilmPlayer::getFrame ( const char* name )
{
    GFilmFrame* frame = nullptr;
    mNameMap.Get ( name, frame );
    return frame;
}

void GFilmPlayer::deleteFrame ( const char* frame )
{
    GFilmFrame* pFrame = getFrame ( frame );

for ( auto p: mNameMap )
    {
        if ( dStrEqual ( frame, p.second->getPrev() ) )
        {
            p.second->setPrev ( pFrame->getPrev() );
        }
        else if ( dStrEqual ( frame, p.second->getNext() ) )
        {
            p.second->setNext ( pFrame->getNext() );
        }
    }

    GFilmDeleteFrameEvent event;
    event.mFrameName = pFrame->getName();
    mDelegateDeleteFrame.trigger ( &event );
    dSafeDelete ( pFrame );
    mNameMap.erase ( frame );
}

void GFilmPlayer::renameFrame ( const char* name, const char* newName )
{
    assert ( name );
    assert ( newName );
    assert ( getFrame ( newName ) == nullptr );
    GFilmFrame* frame = getFrame ( name );
    CXASSERT_RETURN ( frame );

    GFilmFrame* prev = getFrame ( frame->getPrev() );
    if ( prev )
        prev->setNext ( newName );
    GFilmFrame* next = getFrame ( frame->getNext() );
    if ( next )
        next->setPrev ( newName );

    frame->setName ( newName );
    mNameMap.erase ( name );
    mNameMap.Insert ( newName, frame );
}

void GFilmPlayer::updateList()
{

}

GFilmFrame* GFilmPlayer::updateHeader()
{
    GFilmFrame* headerFrame = nullptr;
for ( auto & pair: mNameMap )
    {
        GFilmFrame* frame = pair.second;
        if ( dStrEqual ( "", frame->getPrev() ) )
        {
            headerFrame = frame;
        }
    }
    CXASSERT_RETURN_FALSE ( headerFrame );
    return headerFrame;
}

void GFilmPlayer::clear()
{
for ( auto & p: mNameMap )
    {
        GFilmDeleteFrameEvent event;
        event.mFrameName = p.second->getName();
        mDelegateDeleteFrame.trigger ( &event );
    }
    mNameMap.destroySecond();
    mCurFrame = nullptr;
}

void GFilmPlayer::setAllPrev()
{
for ( auto & p: mNameMap )
    {
        GFilmFrame* cur = p.second;
        const char* next = cur->getNext();
        if ( !dStrEqual ( next, "" ) )
        {
            GFilmFrame* frame = getFrame ( next );
            CXASSERT_RETURN ( frame );
            frame->setPrev ( p.second->getName() );
        }
    }
}


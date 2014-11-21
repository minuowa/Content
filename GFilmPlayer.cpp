#include "GGameDemoHeader.h"
#include "GFilmPlayer.h"
#include "GFilmFrame.h"
#include "GTimer.h"


GFilmPlayer::GFilmPlayer ( void )
{
    mEnd = false;
    mLastFrame = nullptr;
    mCurFrame = nullptr;
}


GFilmPlayer::~GFilmPlayer ( void )
{
    dSafeDeleteVector ( mFilmSequence );
}

bool GFilmPlayer::play ( const char* filename )
{
    mLastFrame = nullptr;
    dSafeDeleteVector ( mFilmSequence );
    gLuaScript.doFile ( filename );

    CXASSERT_RETURN_FALSE ( mFilmSequence.size() > 0 );

    mCurFrame = mFilmSequence[0];
    if ( mCurFrame )
        mCurFrame->start();

    return mCurFrame != nullptr;
}

DeclareFilmTool void GFilmPlayer::addFrame( u32 id, int lifeSeconds )
{
    GFilmFrame* frame = new GFilmFrame;
    frame->setInfo ( lifeSeconds * 1000, id );
    mFilmSequence.push_back ( frame );
    if ( mLastFrame )
        mLastFrame->tail ( frame );
    mLastFrame = frame;
}

void GFilmPlayer::process()
{
    if ( mCurFrame == nullptr )
        return;
    DWORD frametime = TheTimer->getFrameTimems();
    if ( !mCurFrame->advanceTime ( frametime ) )
    {
        mCurFrame = mCurFrame->next();
        if ( mCurFrame != nullptr )
            mCurFrame->start();
    }
}


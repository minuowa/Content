#include "GGameDemoHeader.h"
#include "GTimer.h"
#include "GText.h"
#include "Content.h"
//--------------------------------------------------------------------------------------------------
GTimer::GTimer ( void )
    : mAccuTime ( 0 )
    , mFrameTime ( ~0 )
    , mLastTime ( 0 )
    , mFPSFrameCount ( 0 )
{
    Content::Text.addText ( &mFPSString );
}

GTimer::~GTimer ( void )
{
    Content::Text.removeText ( &mFPSString );
}


DWORD GTimer::getFrameTimems()
{
    return mFrameTime;
}

void GTimer::update()
{
    DWORD now = timeGetTime();
    mFrameTime = now - mLastTime;
    mAccuTime += mLastTime == 0 ? 0 : mFrameTime;
    mLastTime = now;

    if ( FPSCount == mFPSFrameCount )
    {
        mFPS = ( DWORD ) ( 1000.0f * FPSCount / mFPSAccuTime );
        mFPSString.Format ( "FPS:%d", mFPS );

        mFPSFrameCount = 0;
        mFPSAccuTime = 0;
    }
    mFPSAccuTime += mFrameTime;
    mFPSFrameCount++;
}

float GTimer::getFrameTimeSec()
{
    return mFrameTime * 0.001f;
}

DWORD GTimer::getAccuTime()
{
    return mAccuTime;
}



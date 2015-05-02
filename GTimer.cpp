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
#ifdef _DEBUG
	Content::Text.addText ( &mFPSString );
#endif
}

GTimer::~GTimer ( void )
{
#ifdef _DEBUG
	Content::Text.removeText ( &mFPSString );
#endif
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
        mFPSString.format( "FPS:%d", mFPS );

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

DWORD GTimer::getFPS() const
{
	return mFPS;
}



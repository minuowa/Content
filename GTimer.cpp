#include "GGameDemoHeader.h"
#include "GTimer.h"
//--------------------------------------------------------------------------------------------------
FiImplateSingleton(GTimer)
	//--------------------------------------------------------------------------------------------------

	GTimer::GTimer(void)
	:mAccuTime(0)
	,mFrameTime(~0)
	,mLastTime(0)
{
	FiMarkSingleton();
}

GTimer::~GTimer(void)
{
}


DWORD GTimer::getFrameTimems()
{
	return mFrameTime;
}

void GTimer::update()
{
	DWORD now =timeGetTime();
	mFrameTime = now - mLastTime;
	mAccuTime += mLastTime == 0 ? 0 : mFrameTime;
	mLastTime = now;
}

float GTimer::getFrameTimeSec()
{
	return mFrameTime*0.001f;
}

DWORD GTimer::getAccuTime()
{
	return mAccuTime;
}



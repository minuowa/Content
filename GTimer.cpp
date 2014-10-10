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

void GTimer::Update()
{
	DWORD now =timeGetTime();
	mFrameTime = now - mLastTime;
	mAccuTime += mLastTime == 0 ? 0 : mFrameTime;
	mLastTime = now;
}

float GTimer::GetFrameTimeSec()
{
	return mFrameTime/1000.0f;
}



#pragma once


class GTimer
{
	FiDeclareSingleton(GTimer);
public:
    GTimer(void);
    ~GTimer(void);

    DWORD getAccuTime();

    DWORD getFrameTimems();	    //两次获取当前时间的间隔

	float getFrameTimeSec();

	void update();
private:

	DWORD mAccuTime;

	DWORD mFrameTime;

    DWORD mLastTime;		    //上一次时间
};

#define TheTimer GTimer::GetSingletonPtr()
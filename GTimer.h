#pragma once


class GTimer
{
public:

    static const int FPSCount = 100;

    GTimer ( void );
    ~GTimer ( void );

    DWORD getAccuTime();

    DWORD getFrameTimems();	    //两次获取当前时间的间隔

    float getFrameTimeSec();

    void update();

    DWORD getFPS() const;

private:

    DWORD mAccuTime;

    DWORD mFrameTime;

    DWORD mLastTime;		    //上一次时间

	DWORD mFPSAccuTime;

	DWORD mFPSFrameCount;

    DWORD mFPS;

	GString mFPSString;
};
#pragma once


class GTimer
{
public:

    static const int FPSCount = 100;

    GTimer ( void );
    ~GTimer ( void );

    DWORD getAccuTime();

    DWORD getFrameTimems();	    //���λ�ȡ��ǰʱ��ļ��

    float getFrameTimeSec();

    void update();

    DWORD getFPS() const;

private:

    DWORD mAccuTime;

    DWORD mFrameTime;

    DWORD mLastTime;		    //��һ��ʱ��

	DWORD mFPSAccuTime;

	DWORD mFPSFrameCount;

    DWORD mFPS;

	GString mFPSString;
};
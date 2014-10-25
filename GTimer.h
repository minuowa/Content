#pragma once


class GTimer
{
	FiDeclareSingleton(GTimer);
public:
    GTimer(void);
    ~GTimer(void);

    DWORD getAccuTime();

    DWORD getFrameTimems();	    //���λ�ȡ��ǰʱ��ļ��

	float getFrameTimeSec();

	void update();
private:

	DWORD mAccuTime;

	DWORD mFrameTime;

    DWORD mLastTime;		    //��һ��ʱ��
};

#define TheTimer GTimer::GetSingletonPtr()
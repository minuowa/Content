#pragma once


class GTimer
{
	FiDeclareSingleton(GTimer);
public:
    GTimer(void);
    ~GTimer(void);

    DWORD GetAccuTime();

    DWORD GetFrameTimems();	    //���λ�ȡ��ǰʱ��ļ��

	float GetFrameTimeSec();

	void Update();
private:

	DWORD mAccuTime;

	DWORD mFrameTime;

    DWORD mLastTime;		    //��һ��ʱ��
};

#define TIMER GTimer::GetSingleton()
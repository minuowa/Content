#pragma once


class GFrameWork:public CXCallBack
{
public:
    GFrameWork(void);
    virtual ~GFrameWork(void);
public:
	void Active(bool active);
	bool IsActive() const;
    virtual bool InitBase(HWND mainWnd);//��ʼ����Ϸ  
	HWND GetMainWin() const { return mMainWin; }
	void resize(int w,int h);
public:
    HWND mMainWin;//�����ھ��
	HINSTANCE mInst;//Game���� 
	bool mIsActive;
};

//���ں�������
 LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
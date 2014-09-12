#pragma once


class GFrameWork:public CXCallBack
{
public:
    GFrameWork(void);
    virtual ~GFrameWork(void);
public:
	void active(bool active);
	bool isActive() const;
    virtual bool initBase(HWND mainWnd);//��ʼ����Ϸ  
	HWND getMainWin() const { return mMainWin; }
	void resize(int w,int h);
public:
    HWND mMainWin;//�����ھ��
	HINSTANCE mInst;//Game���� 
	bool mIsActive;
};

//���ں�������
 LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
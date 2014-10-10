#pragma once


class GFrameWork:public CXCallBack
{
public:
    GFrameWork(void);
    virtual ~GFrameWork(void);
public:
	virtual bool initBase(HWND mainWnd);//��ʼ����Ϸ  
	void active(bool active);
	bool isActive() const;
	HWND getMainWin() const { return mMainWin; }
	void resize(int w,int h);
public:
    HWND mMainWin;//�����ھ��
	HINSTANCE mInst;//Game���� 
	bool mIsActive;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

//���ں�������
#ifndef GFrameWork_h__
#define GFrameWork_h__
class GFrameWork: public CXCallBack
{
public:
    GFrameWork ( void );
    virtual ~GFrameWork ( void );
public:
    virtual bool init ( HWND mainWnd ); //≥ı ºªØ”Œœ∑
    void active ( bool active );
    bool isActive() const;
    HWND getMainWin() const;
    void resize ( int w, int h );
public:
    HWND mMainWin;
    HINSTANCE mInst;
    bool mIsActive;

    static LRESULT CALLBACK WndProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
};
#endif // GFrameWork_h__
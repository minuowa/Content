#pragma once

#include "GD9Device.h"
#include "XSingleton.h"
enum eInputState:
unsigned char
{
    eInputStateNone,
    eInputStateBe = 0X80,
};
enum eInputAction
{
    eInputAction_None,
    eInputAction_Pressing = BIT ( 0 ),
    eInputAction_Up = BIT ( 1 ),
    eInputAction_Down = BIT ( 2 ) | eInputAction_Pressing,
};
eInputAction operator- ( eInputState state, eInputAction );

enum eButtonType
{
    eButtonType_LeftButton,
    eButtonType_RightButton,
    eButtonType_MiddleButton,
    eButtonType_Count,
};
enum eCtrlKey
{
    eCtrlKey_LCtrl = BIT ( 0 ),
    eCtrlKey_RCtrl = BIT ( 1 ),
    eCtrlKey_LShift = BIT ( 2 ),
    eCtrlKey_RShift = BIT ( 3 ),
    eCtrlKey_LAtl = BIT ( 4 ),
    eCtrlKey_RAtl = BIT ( 5 ),
    eCtrlKey_ButtonPressing = BIT ( 6 ),
    eCtrlKey_ButtonDown = BIT ( 7 ),
    eCtrlKey_ButtonUp = BIT ( 8 ),
    eCtrlKey_LButton = BIT ( 9 ),
    eCtrlKey_RButton = BIT ( 10 ),
    eCtrlKey_MButton = BIT ( 11 ),
    eCtrlKey_Count,
};
typedef void ( *KeyCallBack ) ();
struct GKeyItem
{
    byte mKey;
	eInputAction mKeyAct;
    int mCtrl;
    KeyCallBack mCallBack;
};
class  GD9Device;
class GD8Input
{
public:
    CXDelegate mDelegateMouseZoom;
    CXDelegate mDelegateMouseMove;
    CXDelegate mDelegateMouseUp;
    CXDelegate mDelegateMouseDown;
    CXDelegate mDelegateMouseDownAndMoved;
public:
    enum {KEY_COUNT = 256,};

    GD8Input ( void );
    ~GD8Input ( void );
    void reset();

	bool isKeyPressing ( byte key );
	bool isKeyDown ( byte key );
    bool iskeyUp ( byte key );

    bool isButtonPressing ( eButtonType bt );
    bool isButtonDown ( eButtonType bt ) const;
    bool isButtonUp ( eButtonType bt ) const;
    //////////////////////////////////////////////////////////////////////////
    /////位置
    bool isWheelMButton();
    POINT getMousePoint() const;
    POINT getMouseMove();
    D3DVECTOR getMouseMoveEX();
    int getMouseWheel();

    void freezeMouse ( bool bFreeze );
    void update();
    void active ( bool active );
    bool init ( const GD9Device& device, HINSTANCE hInst, HWND hWin );
    bool isMouseInDevice() const;


    //************************************
    // Method:    addInputKey
    // FullName:  GD8Input::addInputKey
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: byte key DIK
    // Parameter: int ctrl eCtrlKey 组合
    // Parameter: KeyCallBack callback
    //************************************
    void addInputKey ( byte key, eInputAction keyact, int ctrl, KeyCallBack callback );
private:
    void updateState();
    void checkEvent();
    bool isCtrlOk ( int ctrlkeys );
    HINSTANCE mInstance;
    HWND mWinID;
    LPDIRECTINPUT8 mDI;
    LPDIRECTINPUTDEVICE8 mKboard;
    LPDIRECTINPUTDEVICE8 mMouse;

    DIMOUSESTATE mMouseData;
    POINT mCurMousePositon;

    eInputAction mMouseAction[eButtonType_Count];//0左键，1右键，2中键
    eInputAction mMouseActionOld[eButtonType_Count];

    eInputAction mKBoardAction[KEY_COUNT];
    eInputAction mKBoardActionOld[KEY_COUNT];

    eInputState mKeyData[KEY_COUNT];
    bool mActive;
    bool mNeedClearMouseMoveDelta;

    CXDynaArray<GKeyItem*> mInputArray;
};
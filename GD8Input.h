#pragma once

#include "GD9Device.h"
#include "XSingleton.h"

#define DI_BUTTONNULL 0
#define DI_BUTTONTRUE 1
#define DI_BUTTONUP 3
#define DI_BUTTONDOWN 5
#define DI_BUTTONCLICK 11
#define DI_BUTTONDBCLICK 27

enum eButtonType
{
	eButtonType_LeftButton, 
	eButtonType_RightButton,
	eButtonType_MiddleButton,
	eButtonType_Count,
};
class  GD9Device;
class GD8Input
{
public:
	CXDelegate mDelegateMouseMove;
	CXDelegate mDelegateMouseUp;
	CXDelegate mDelegateMouseDown;
	CXDelegate mDelegateMouseDownAndMoved;
public:
	enum {KEY_COUNT=256,};

	GD8Input ( void );
	~GD8Input ( void );
	void Reset();
	/////×´Ì¬
	//bool IsKeyDown(byte key);
	bool IskeyUp ( byte key );
	bool IsPressKey ( byte key );
	//bool IsPressing(byte key);


	////////¶¯×÷
	bool isPressingButton ( eButtonType bt );
	byte GetButtonAction ( eButtonType bt ) const;
	byte getKeyAction ( int key );

	bool isButtonDown(eButtonType bt) const;
	bool isButtonUp(eButtonType bt) const;
	//////////////////////////////////////////////////////////////////////////
	/////Î»ÖÃ
	bool IsWheelMButton();
	POINT getMousePoint() const;
	POINT GetMouseMove();
	D3DVECTOR GetMouseMoveEX();
	int getMouseWheel();

	void FreezeMouse ( bool bFreeze );
	void Update();
	void Active ( bool active );
	bool init ( const GD9Device& device, HINSTANCE hInst, HWND hWin );
	bool isMouseInDevice() const;
private:
	void UpdateState();
	void checkEvent();

	HINSTANCE mInstance;
	HWND mWinID;
	LPDIRECTINPUT8 mDI;
	LPDIRECTINPUTDEVICE8 mKboard;
	LPDIRECTINPUTDEVICE8 mMouse;

	DIMOUSESTATE mMouseData;
	POINT mCurMousePositon;
	byte mMouseButtonStateOld[eButtonType_Count];
	byte mMouseButtonState[eButtonType_Count];//0×ó¼ü£¬1ÓÒ¼ü£¬2ÖÐ¼ü
	byte mKBoardState[KEY_COUNT];
	byte mKBoardStateOld[KEY_COUNT];
	BYTE mKeyData[KEY_COUNT];
	bool mActive;
	bool mNeedClearMouseMoveDelta;
};
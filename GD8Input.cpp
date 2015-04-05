#include "GGameDemoHeader.h"
#include "GD8Input.h"
#include "GD9Device.h"
#include "Content.h"


GD8Input::GD8Input ( void )
    : mActive ( true )
    , mInstance ( 0 )
    , mWinID ( 0 )
    , mNeedClearMouseMoveDelta ( false )
{
    mKboard = NULL;
    mMouse = NULL;
    mDI = NULL;

    reset();
}

GD8Input::~GD8Input ( void )
{
    dSafeDeleteVector ( mInputArray );
}



bool GD8Input::init ( const GD9Device& device, HINSTANCE hInst, HWND hWin )
{
    CXUnuse ( device );

    mInstance = hInst;
    mWinID = hWin;
    CoInitialize ( NULL );
    HRESULT hr = DirectInput8Create ( mInstance, DIRECTINPUT_HEADER_VERSION, IID_IDirectInput8, ( LPVOID* ) &mDI, NULL );
    CXASSERT_RESULT_FALSE ( hr );

    hr = mDI->CreateDevice ( GUID_SysKeyboard, &mKboard, NULL );
    CXASSERT_RESULT_FALSE ( hr );
    mKboard->SetDataFormat ( &c_dfDIKeyboard );

#ifdef _DEBUG
    mKboard->SetCooperativeLevel ( mWinID, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );
#else
    mKboard->SetCooperativeLevel ( mWinID, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );
#endif

    //调试用DISCL_NONEXCLUSIVE|DISCL_BACKGROUND
    //正式版本用：DISCL_EXCLUSIVE|DISCL_
    hr = mDI->CreateDevice ( GUID_SysMouse, &mMouse, NULL );
    CXASSERT_RESULT_FALSE ( hr );
    mMouse->SetDataFormat ( &c_dfDIMouse );
#ifdef _DEBUG
    //mMouse->SetCooperativeLevel(mHwin,DISCL_BACKGROUND|DISCL_NONEXCLUSIVE);
    mMouse->SetCooperativeLevel ( mWinID, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
#else
    mMouse->SetCooperativeLevel ( mWinID, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );
#endif
    mKboard->Acquire();
    mMouse->Acquire();
    return true;
}

void GD8Input::update()
{
    if ( !mActive )
        return;

    //-------------------------------------------------------------------------
    // 此处多做一次updateState()是为了清除鼠标中数据的增量
    //-------------------------------------------------------------------------

    if ( mNeedClearMouseMoveDelta )
    {
        updateState();
        mNeedClearMouseMoveDelta = false;
    }

    updateState();

    checkEvent();
}

bool GD8Input::isKeyPressing ( byte key )
{
    return mKBoardAction[key] == eInputAction_Pressing;
}



POINT GD8Input::getMouseMove()
{
    POINT Pt;
    Pt.x = mMouseData.lX;
    Pt.y = mMouseData.lY;
    return Pt;
}

D3DVECTOR GD8Input::getMouseMoveEX()
{
    D3DVECTOR vPos;
    dVector ( vPos, mMouseData.lX, mMouseData.lY , mMouseData.lZ );
    return vPos;
}

POINT GD8Input::getMousePoint() const
{
    POINT pt;
    GetCursorPos ( &pt );
    ScreenToClient ( mWinID, &pt );

    RECT rcNow;
    GetClientRect ( mWinID, &rcNow );

    float fWidth = ( float ) ( rcNow.right - rcNow.left );
    float fHeight = ( float ) ( rcNow.bottom - rcNow.top );

    pt.x *= ( ( float )  Content::Device.mWidth ) / fWidth;
    pt.y *= ( ( float )  Content::Device.mHeight ) / fHeight;

    return pt;
}

int GD8Input::getMouseWheel()
{
    return mMouseData.lZ;
}


bool GD8Input::isButtonPressing ( eButtonType bt )
{
    return mMouseAction[bt] == eInputAction_Pressing;
}


bool GD8Input::iskeyUp ( byte key )
{
    return mKBoardAction[key] == eInputAction_Up;
}

bool GD8Input::isKeyDown ( byte key )
{
    return mKBoardAction[key] == eInputAction_Down;
}

void GD8Input::active ( bool active )
{
    mActive = active;
    mNeedClearMouseMoveDelta = true;
}

void GD8Input::reset()
{
    dMemoryZero ( &mMouseData, sizeof ( mMouseData ) );
    dMemoryZero ( mMouseActionOld, sizeof ( mMouseActionOld ) );

    dMemoryZero ( mKeyData, sizeof ( mKeyData ) );
    dMemoryZero ( mKBoardActionOld, sizeof ( mKBoardActionOld ) );
    mCurMousePositon.x = 0;
    mCurMousePositon.y = 0;
}

void GD8Input::updateState()
{
    if ( mKboard->GetDeviceState ( KEY_COUNT, mKeyData ) == DIERR_INPUTLOST )
    {
        mKboard->Acquire();
        mKboard->GetDeviceState ( KEY_COUNT, mKeyData );
    }

    if ( mMouse->GetDeviceState ( ( sizeof ( mMouseData ) ), &mMouseData ) == DIERR_INPUTLOST )
    {
        mMouse->Acquire();
        mMouse->GetDeviceState ( ( sizeof ( mMouseData ) ), &mMouseData );
    }

    for ( int i = 0; i < eButtonType_Count; i++ )
    {
        mMouseAction[i] = eInputState ( mMouseData.rgbButtons[i] ) - mMouseActionOld[i];
        mMouseActionOld[i] = mMouseAction[i];
    }

    for ( int i = 0; i < KEY_COUNT; i++ )
    {
        mKBoardAction[i] = mKeyData[i] - mKBoardActionOld[i];
        mKBoardActionOld[i] = mKBoardAction[i];
    }
}

void GD8Input::checkEvent()
{
    if ( !isMouseInDevice() )
        return;
    D3DVECTOR v = getMouseMoveEX();
    if ( v.x != 0 || v.y != 0 )
    {
        mDelegateMouseMove.trigger();

        for ( int i = 0; i < eButtonType_Count; ++i )
        {
            if ( isButtonPressing ( ( eButtonType ) i ) )
            {
                mDelegateMouseDownAndMoved.trigger();
            }
        }
    }
    if ( v.z != 0 )
    {
        mDelegateMouseZoom.trigger();
    }
    for ( int i = 0; i < eButtonType_Count; ++i )
    {
        if ( isButtonDown ( eButtonType ( i ) ) )
        {
            mDelegateMouseDown.trigger();
        }
        else if ( isButtonUp ( eButtonType ( i ) ) )
        {
            mDelegateMouseUp.trigger();
        }
    }
for ( auto i: mInputArray )
    {
        if ( i->mCtrl > 0 )
        {
            if ( !isCtrlOk ( i->mCtrl ) )
                continue;
        }
        if ( i->mKey > 0 && i->mKeyAct != eInputAction_None )
        {
            if ( i->mKeyAct != mKBoardAction[i->mKey] )
                continue;
        }
        i->mCallBack();
        break;
    }
}

bool GD8Input::isButtonDown ( eButtonType bt ) const
{
    return mMouseAction[bt] == eInputAction_Down; //发生按下动作
}

bool GD8Input::isMouseInDevice() const
{
    POINT pt =  getMousePoint();
    //鼠标在客户区外面就不执行GetInput
    if ( pt.x < 0 || pt.x >  Content::Device.mWidth || pt.y < 0 || pt.y >  Content::Device.mHeight )
    {
        return false;
    }
    return true;
}

bool GD8Input::isButtonUp ( eButtonType bt ) const
{
    return mMouseAction[bt] == eInputAction_Up; //发生按下动作
}
bool SortFunc ( GKeyItem* left, GKeyItem* right )
{
    return left->mCtrl > right->mCtrl;
}

eInputAction operator- ( eInputState state, eInputAction act )
{
    eInputAction action = eInputAction_None;
    switch ( state )
    {
    case eInputStateNone:
    {
        if ( act == eInputAction_Down || act == eInputAction_Pressing )
            action = eInputAction_Up ;
    }
    break;
    case eInputStateBe:
    {
        action = eInputAction_Pressing;
        if ( act == eInputAction_None )
            action = eInputAction_Down;
    }
    break;
    }
    return action;
}

void GD8Input::addInputKey ( byte key, eInputAction keyact, int ctrl, KeyCallBack callback )
{
    CXASSERT_RETURN ( key > 0 || ctrl > 0 );
#ifdef _DEBUG
for ( auto i: mInputArray )
    {
        if ( i->mCtrl == ctrl && i->mKey == key )
        {
            CXASSERT_RETURN ( 0 && "Exist key!" );
        }
    }
#endif
    GKeyItem* item = new GKeyItem;
    item->mKey = key;
    item->mKeyAct = keyact;
    item->mCtrl = ctrl;
    item->mCallBack = callback;
    mInputArray.push_back ( item );
    ::sort ( mInputArray.begin(), mInputArray.end(), SortFunc );
}

bool GD8Input::isCtrlOk ( int ctrlkeys )
{
    static const byte CTRLKEYMAP[] =
    {
        DIK_LCONTROL,
        DIK_RCONTROL,
        DIK_LSHIFT,
        DIK_RSHIFT,
        DIK_LALT,
        DIK_RALT,
    };
    enum eButtonState
    {
        eButtonState_Pressing,
        eButtonState_Down,
        eButtonState_Up,
        eButtonState_Count,
    };
    eButtonState buttonState = eButtonState_Count;
    bool downkey = false;
    for ( int key = eCtrlKey_LCtrl, offset = 0; key < eCtrlKey_Count; key = 1 << ( ++offset ) )
    {
        switch ( key )
        {
        case eCtrlKey_LCtrl:
        case eCtrlKey_RCtrl:
        case eCtrlKey_LShift:
        case eCtrlKey_RShift:
        case eCtrlKey_LAtl:
        case eCtrlKey_RAtl:
        {
            if (  ctrlkeys & key )
            {
                if ( !isKeyPressing ( CTRLKEYMAP[offset] ) )
                    return false;
                else
                {
                    downkey = true;
                }
            }
        }
        break;
        case eCtrlKey_ButtonDown:
        case eCtrlKey_ButtonPressing:
        case eCtrlKey_ButtonUp:
        {
            if (  ctrlkeys & key )
            {
                CXASSERT ( buttonState == eButtonState_Count );
                buttonState = ( eButtonState ) ( offset - 6 );
            }
        }
        break;
        case eCtrlKey_LButton:
        case eCtrlKey_RButton:
        case eCtrlKey_MButton:
        {
            if ( buttonState == eButtonState_Count )
                break;
            if (  ctrlkeys & key )
            {
                if ( buttonState == eButtonState_Down )
                {
                    if ( !isButtonDown ( eButtonType ( offset - 9 ) ) )
                    {
                        int n = 0;
                        ++n;
                        return false;
                    }

                }
                else if ( buttonState == eButtonState_Pressing )
                {
                    if ( !isButtonPressing (  eButtonType ( offset - 9 ) ) )
                        return false;
                }
                else if ( buttonState == eButtonState_Up )
                {
                    if ( !isButtonUp (  eButtonType ( offset - 9 ) ) )
                        return false;
                }
            }
        }
        break;
        case eCtrlKey_Count:
        {
            CXASSERT_RETURN_FALSE ( 0 );
        }
        break;
        default:
            break;
        }
    }
    return true;
}


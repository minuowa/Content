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

    Reset();
}

GD8Input::~GD8Input ( void )
{

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

    //������DISCL_NONEXCLUSIVE|DISCL_BACKGROUND
    //��ʽ�汾�ã�DISCL_EXCLUSIVE|DISCL_
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

void GD8Input::Update()
{
    //���������㣬Ȼ���ȡ��ǰ״̬

    if ( !mActive )
        return;

    //-------------------------------------------------------------------------
    // �˴�����һ��UpdateState()��Ϊ�������������ݵ�����
    //-------------------------------------------------------------------------

    if ( mNeedClearMouseMoveDelta )
    {
        UpdateState();
        mNeedClearMouseMoveDelta = false;
    }

    UpdateState();

    checkEvent();
}

bool GD8Input::IsPressKey ( byte key )
{
    return mKeyData[key] == 0x80;
}



POINT GD8Input::GetMouseMove()
{
    POINT Pt;
    Pt.x = mMouseData.lX;
    Pt.y = mMouseData.lY;
    return Pt;
}

D3DVECTOR GD8Input::GetMouseMoveEX()
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

byte GD8Input::GetButtonAction ( eButtonType bt ) const
{
    return mMouseButtonState[bt];
}

bool GD8Input::isPressingButton ( eButtonType bt )
{
    if ( bt == eButtonType_LeftButton )
    {
        return mMouseData.rgbButtons[eButtonType_LeftButton];
    }
    else if ( bt == eButtonType_MiddleButton )
    {
        return mMouseData.rgbButtons[eButtonType_MiddleButton];
    }
    else
    {
        return mMouseData.rgbButtons[eButtonType_RightButton];
    }
}

byte GD8Input::getKeyAction ( int key )
{
    return mKBoardState[key];
}

bool GD8Input::IskeyUp ( byte key )
{
    return getKeyAction ( key ) == DI_BUTTONUP;
}
void GD8Input::Active ( bool active )
{
    mActive = active;
    mNeedClearMouseMoveDelta = true;
}

void GD8Input::Reset()
{
    ZeroMemory ( &mMouseData, sizeof ( mMouseData ) );
    ZeroMemory ( mMouseButtonStateOld, sizeof ( mMouseButtonStateOld ) );

    ZeroMemory ( mKeyData, sizeof ( mKeyData ) );
    ZeroMemory ( mKBoardStateOld, sizeof ( mKBoardStateOld ) );
    mCurMousePositon.x = 0;
    mCurMousePositon.y = 0;
}

void GD8Input::UpdateState()
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

    //////ȷ����궯��
    for ( int i = 0; i < eButtonType_Count; i++ )
    {
        if ( !mMouseButtonStateOld[i] )
        {
            if ( mMouseData.rgbButtons[i] == 0x80 )
            {
                mMouseButtonState[i] = DI_BUTTONDOWN; //�������¶���
            }
            else
            {
                mMouseButtonState[i] = DI_BUTTONNULL;
            }
        }
        else
        {
            if ( mMouseData.rgbButtons[i] == 0x80 )
            {
                mMouseButtonState[i] = DI_BUTTONNULL;
            }
            else
            {
                mMouseButtonState[i] = DI_BUTTONUP; //����������
            }
        }
        mMouseButtonStateOld[i] = ( mMouseData.rgbButtons[i] == 0x80 );
    }

    //////ȷ�����̶���
    for ( int i = 0; i < KEY_COUNT; i++ )
    {
        if ( !mKBoardStateOld[i] )
        {
            if ( mKeyData[i] == 0x80 )
            {
                mKBoardState[i] = DI_BUTTONDOWN; //�������¶���
            }
            else
            {
                mKBoardState[i] = DI_BUTTONNULL;
            }
        }
        else
        {
            if ( mKeyData[i] == 0x80 )
            {
                mKBoardState[i] = DI_BUTTONNULL;
            }
            else
            {
                mKBoardState[i] = DI_BUTTONUP; //����������
            }
        }
        mKBoardStateOld[i] = ( mKeyData[i] & 0x80 );
    }
}

void GD8Input::checkEvent()
{
    if ( !isMouseInDevice() )
        return;
	POINT v = GetMouseMove();
	if ( v.x != 0 || v.y != 0 )
	{
		mDelegateMouseMove.trigger();

		for ( int i = 0; i < eButtonType_Count; ++i )
		{
			if ( isPressingButton ( ( eButtonType ) i ) )
			{
				mDelegateMouseDownAndMoved.trigger();
			}
		}
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
}

bool GD8Input::isButtonDown ( eButtonType bt ) const
{
    return mMouseButtonState[bt] == DI_BUTTONDOWN; //�������¶���
}

bool GD8Input::isMouseInDevice() const
{
    POINT pt =  getMousePoint();
    //����ڿͻ�������Ͳ�ִ��GetInput
    if ( pt.x < 0 || pt.x >  Content::Device.mWidth || pt.y < 0 || pt.y >  Content::Device.mHeight )
    {
        return false;
    }
    return true;
}

bool GD8Input::isButtonUp ( eButtonType bt ) const
{
    return mMouseButtonState[bt] == DI_BUTTONUP; //�������¶���
}

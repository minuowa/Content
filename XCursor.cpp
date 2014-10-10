#include "GGameDemoHeader.h"
#include "XCursor.h"

CXCursor gCursor;

CXCursor::CXCursor ( void )
{
    mhNowCursor = NULL;

    ZeroMemory ( mhCursor, sizeof ( mhCursor ) );
}

CXCursor::~CXCursor ( void )
{

}

bool CXCursor::Init()
{

    HCURSOR hCursor = NULL;

    GString sFlieName;

    for ( int i = 0; i < CURSORNUM; i++ )
    {
        sFlieName.Format ( "res\\cursor\\%03d.cur", i );

        hCursor = LoadCursorFromFileA ( sFlieName );

        if ( hCursor == NULL )
        {
            return false;
        }

        mhCursor[i] = hCursor;
    }

    SetNowCursor ( curNormal );

    //mhCursor[11]=LoadCursorFromFile("res\\cursor\\012.ani");

    return true;
}

void CXCursor::SetNowCursor ( eCursor cur )
{
    HCURSOR hCursor = NULL;

    switch ( cur )
    {

    case curNormal:		//正常
        hCursor = mhCursor[0];
        break;

    case curAttack:		//攻击
        hCursor = mhCursor[1];
        break;

    case curTalk:		//对话
        hCursor = mhCursor[2];
        break;

    case curTouch:		//抚摸
        hCursor = mhCursor[3];
        break;

    case curNotCanBeSelected:	//不可选择
        hCursor = mhCursor[4];
        break;

    case curGrasp:		//抓住
        hCursor = mhCursor[5];
        break;

    case curNotMine:	//不是本人的
        hCursor = mhCursor[6];
        break;

    case curRepair:		//修理
        hCursor = mhCursor[7];
        break;

    case curTrade:		//交易
        hCursor = mhCursor[8];
        break;

    case curSystem:		//系统
        hCursor = mhCursor[9];
        break;

        //case curNoUse:		//不可用
        //	hCursor=mhCursor[10];
        //	break;

        //case curDynamic:		//动态的
        //	hCursor=mhCursor[11];
        //	break;
    }

    if ( hCursor != NULL )
    {
        mhNowCursor = hCursor;

        ::SetCursor ( hCursor );
    }
}

HCURSOR CXCursor::GetNowCursor()
{
    if ( mhNowCursor != NULL )
    {
        return mhNowCursor;
    }

    return NULL;
}

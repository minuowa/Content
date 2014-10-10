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

    case curNormal:		//����
        hCursor = mhCursor[0];
        break;

    case curAttack:		//����
        hCursor = mhCursor[1];
        break;

    case curTalk:		//�Ի�
        hCursor = mhCursor[2];
        break;

    case curTouch:		//����
        hCursor = mhCursor[3];
        break;

    case curNotCanBeSelected:	//����ѡ��
        hCursor = mhCursor[4];
        break;

    case curGrasp:		//ץס
        hCursor = mhCursor[5];
        break;

    case curNotMine:	//���Ǳ��˵�
        hCursor = mhCursor[6];
        break;

    case curRepair:		//����
        hCursor = mhCursor[7];
        break;

    case curTrade:		//����
        hCursor = mhCursor[8];
        break;

    case curSystem:		//ϵͳ
        hCursor = mhCursor[9];
        break;

        //case curNoUse:		//������
        //	hCursor=mhCursor[10];
        //	break;

        //case curDynamic:		//��̬��
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

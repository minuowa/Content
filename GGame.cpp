#include "GGameDemoHeader.h"
#include "GGame.h"
#include "XString.h"
#include <locale.h>
#include "XEvent.h"
#include "GGameOption.h"
#include "GComponentFactory.h"
#include "GResourceManager.h"
#include "GTerrain.h"

/******************************************************************/
//�����MeshBuffer�У�����͵�ͼ����
//��ա����桢��ͼ��������׶�ü�
/******************************************************************/
FiImplateSingleton ( GGame );

GGame::GGame ( void )
    : mSceneMgr ( NULL )
    , mpSelectObj ( NULL )
    , mpSelectAnim ( NULL )
    , mFinished ( false )
{
}

GGame::~GGame ( void )
{
    dSafeDelete ( mSceneMgr );
	CXSingleton<GResourceManager<GTexture>>::destoryInstance();
	CXSingleton<GResourceManager<GAnimationResource>>::destoryInstance();
	CXSingleton<GMeshManager>::destoryInstance();
	CXSingleton<GGameOption>::destoryInstance();
	CXSingleton<GComponentFactory>::destoryInstance();

	CXSingleton<GD8Input>::destoryInstance();
	CXSingleton<GD9Device>::destoryInstance();
}

void GGame::loop()
{
    TheTimer->update();

    if ( mIsActive )
    {
        INPUTSYSTEM.Update();
        getInput();
    }

    update();

    render();
}

void GGame::getInput()
{
    float fPass = TheTimer->getFrameTimeSec();


    if ( INPUTSYSTEM.IskeyUp ( DIK_ESCAPE ) )
    {
        exit ( 0 );
    }

    static DWORD dwGetInput = 0;

    eGameScene gs = mSceneMgr->mSceneMachine.GetNowScene();

    switch ( gs )
    {
    case gsLoading:

        break;
    case gslogin:
        break;

    case  gsGame:

        if ( INPUTSYSTEM.getKeyAction ( DIK_F9 ) == DI_BUTTONUP )
        {
        }

        //if (INPUTSYSTEM.GetKeyAction(DIK_TAB)==DI_BUTTONUP)
        //{
        //	dwGetInput++;
        //	if (0==dwGetInput%2)
        //	{
        //		mpSelectAnim=&gAnimMesh[0];
        //	}
        //	else
        //	{
        //		mpSelectAnim=&gAnimMesh[1];
        //	}
        //}

        if ( mpSelectAnim != NULL )
        {
            mpSelectAnim->getInput ( fPass );
        }

        GMeshBaseObj *pMeshBaseObj = NULL;

        POINT pt = INPUTSYSTEM.GetMousePoint();

        bool bHit = false;

        static bool bFlag = true;

        //pMeshBaseObj = ( CXMeshBaseObj* )( GMeshManager::GetSingleton().GetInput( fPass ) );

        if ( pMeshBaseObj != NULL )
        {
            mpSelectObj = pMeshBaseObj;
            //        if ( mEditor )
            //        {
            //CXASSERT(0);
            //            //mEditor->SetObject ( pMeshBaseObj );
            //        }
        }

        if ( mpSelectObj != NULL )
        {
            if ( mpSelectAnim != NULL )
            {
                if ( !mpSelectAnim->getTrans().mCanMoveStep )
                {
                    mpSelectObj->getInput ( fPass );
                }
                else
                {
                    mpSelectObj = NULL;
                }
            }

        }

        if ( mpSelectObj != NULL )
        {
            if ( INPUTSYSTEM.IsPressKey ( DIK_COMMA ) )
            {
                //>�����ٶ�
                mpSelectObj->getTrans().mSpeedMove += 0.8f;
            }

            if ( INPUTSYSTEM.IsPressKey ( DIK_PERIOD ) )
            {
                //<��С�ٶ�
                mpSelectObj->getTrans().mSpeedMove -= 0.8f;
            }

            if ( DI_BUTTONUP == INPUTSYSTEM.getKeyAction ( DIK_DELETE ) )
            {
                mSceneMgr->destroy ( ( CGameStaticObj* ) mpSelectObj );

            }

        }


        //���Ŵ󣬡���С
        if ( INPUTSYSTEM.IsPressKey ( DIK_UPARROW ) )
        {
            if ( mpSelectObj != NULL )
            {
                mpSelectObj->getTrans().mZoom += D3DXVECTOR3 ( 0.01f, 0.01f, 0.01f );
            }

            if ( mpSelectAnim != NULL )
            {
                mpSelectAnim->getTrans().mSpeedMove += 0.5f;
            }

        }

        if ( INPUTSYSTEM.IsPressKey ( DIK_DOWNARROW ) )
        {
            if ( mpSelectObj != NULL )
            {
                mpSelectObj->getTrans().mZoom -= D3DXVECTOR3 ( 0.01f, 0.01f, 0.01f );
            }

            if ( mpSelectAnim != NULL )
            {
                mpSelectAnim->getTrans().mSpeedMove -= 0.5f;
            }

        }

        if ( INPUTSYSTEM.getKeyAction ( DIK_F2 ) == DI_BUTTONUP )
        {

            if ( bFlag )
            {
                D9DEVICE->GetDvc()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );
            }
            else
            {
                D9DEVICE->GetDvc()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
            }

            toggle ( bFlag );

            //OPENCLOSE( mpNowMap->mLighting );
        }

        if ( INPUTSYSTEM.getKeyAction ( DIK_F3 ) == DI_BUTTONUP )
        {
            //gAnimMesh[0].mXPos.mvPos = D3DXVECTOR3( 0, 0, 0 );
            //gAnimMesh[0].UpdateForForceOnMap();
        }

        mSceneMgr->getInput ( fPass );
        break;
    }

}

void GGame::update( )
{

    if ( INPUTSYSTEM.IskeyUp ( DIK_TAB ) )
    {
        mSceneMgr->saveScene ( "gameSceneEditor.xml" );
        //DWORD FillMode = 0;
        //D9DEVICE->GetDvc()->GetRenderState( D3DRS_FILLMODE, &FillMode );
        //D9DEVICE->GetDvc()->SetRenderState( D3DRS_FILLMODE, FillMode == D3DFILL_WIREFRAME ? D3DFILL_SOLID : D3DFILL_WIREFRAME );
    }


    eGameScene gs = mSceneMgr->mSceneMachine.GetNowScene();

    float fPass = TheTimer->getFrameTimeSec();

    switch ( gs )
    {

    case gsLoading:

        break;

    case gsGame:
        mSceneMgr->update ( fPass );
        //gAnimMesh[0].Update();
        //mSea.Update( fPass );

        break;
    }


}

void GGame::render( )
{
    float fPass = TheTimer->getFrameTimeSec();

    eGameScene gs = mSceneMgr->mSceneMachine.GetNowScene();

    mSceneMgr->setView();
    switch ( D9DEVICE->TestDevice() )
    {
    case D3D_OK:
    case D3DERR_DEVICELOST:
        break;
    case D3DERR_DEVICENOTRESET:
    {
        D9DEVICE->OnDeviceLost();
    }
    break;
    default:
    {
        CXASSERT ( 0 );
    }
    break;
    }
    if ( D9DEVICE->BeginRender() )
    {
        switch ( gs )
        {

        case gsLoading:

            //mpUIMgr->RenderLoading( fPass );

            break;

        case  gsGame:

            gameRender ( fPass );

            break;

        case gslogin:

            //mpUIMgr->RenderLogin(fPass);

            break;
        }

        D9DEVICE->EndRender();
    }

}

bool GGame::init ( HWND mainWnd )
{
    //setlocale( LC_ALL, "zh_CN.UTF-8" );

    //��ʼ�����500ms
    HRESULT hr = NULL;

    bool bSuccess = false;

    int nSuccess = ERR_INT;



    CoInitialize ( NULL );
    //��ʼ�����
    bSuccess = gCursor.Init();
    gCursor.SetNowCursor ( curNormal );

    //��ʼ�����
    CXASSERT_RETURN_FALSE ( __super::init ( mainWnd ) );
    //��ʼ��D3D�豸
    CXASSERT_RETURN_FALSE (
        GSingletonD9Device::getInstance()->Init ( mMainWin )
    );
    CXASSERT_RETURN_FALSE (
        GSingletonD8Input::GetSingleton().Init ( GSingletonD9Device::GetSingleton(),
                mInst, mMainWin )
    );

    //��ʼ������������
    mSceneMgr = new GSceneManager;

    bSuccess = mSceneMgr->init ();

    if ( !bSuccess )
        return false;

    MeshMgr.Init();

    D9DEVICE->ResetRenderState();

    bSuccess = gEffect.Create ( "..\\Data\\Effect\\SimpleEffect.fx" );

    gEffect.m_hWorldViewProj = gEffect.mD3DEffect->GetParameterByName ( 0, "matWorldViewProj" );

    gEffect.m_hWorld = gEffect.mD3DEffect->GetParameterByName ( 0, "matWorld" );

    gEffect.m_Tech = gEffect.mD3DEffect->GetTechniqueByName ( "TShader" );

    gEffect.m_hUseMaterialOnly = gEffect.mD3DEffect->GetParameterByName ( 0, "bUseMaterialOnly" );

    gEffect.m_hTexture = gEffect.mD3DEffect->GetParameterByName ( 0, "TexObj" );

    gEffect.m_hMtrlAmbient = gEffect.mD3DEffect->GetParameterByName ( 0, "materialAmbient" );

    gEffect.m_hMtrlDiffuse = gEffect.mD3DEffect->GetParameterByName ( 0, "materialDiffuse" );

    gEffect.m_hOpenLight = gEffect.mD3DEffect->GetParameterByName ( 0, "bOpenLight" );


    //������Ⱦ������3000ms

    CoUninitialize();

    gEvent.Create ( "Load" );

    //gEvent.SetUsed();

    //mMTLoadObj.Init(&LoadObj,(LPVOID)this,true);
    loadObj ( this );

    mSceneMgr->mSceneMachine.ChangeToScene ( gsGame );


    return true;
}

void GGame::shutDown()
{

}

DWORD WINAPI loadObj ( LPVOID pParam )
{
    CoInitialize ( NULL );

    GGame *pGame = ( GGame* ) pParam;

    if ( pGame == NULL )
    {
        return ERR_INT;
    }

    //����ͶӰ����
	//TheSceneMgr->loadScene("gameSceneEditor.xml");
	TheSceneMgr->setProj();
	if ( 1 )
    {
        //������������ϵ
		//GWorldCorrd* corrd = new GWorldCorrd();
		//corrd->setNodeName ( "World Corrd" );
		//CXASSERT_RETURN_FALSE ( corrd->recreate() );
		//TheSceneMgr->addStaticObj ( corrd );
    }



    if ( 1 )
    {
		GAnimMeshObj *pAnimMesh = new GAnimMeshObj;
		pAnimMesh->setMediaFile ( "..\\Data\\res\\Anim\\AnimMesh0002\\A0002.X" );
		CXASSERT_RETURN_FALSE ( pAnimMesh->recreate() );
		TheSceneMgr->addDynaObj ( pAnimMesh );
    }
    if ( 1 )
    {
        //MeshPara seaPara (  80.0f, 0, 64, "..\\Data\\res\\water\\BlueShort\\A21C_000.jpg", NULL );

        //GWater* sea = new GWater;
        //sea->setParam ( seaPara );
        //sea->mCanSelect = false;
        ////sea->addQuakePoint ( -50, 0, 10.0f, 2.8f );
        ////sea->addQuakePoint ( 50, 0, 10.0f, 2.8f );
        //sea->recreate();
        //sea->setWorldTranslate ( D3DXVECTOR3 ( 0, 1, 0 ) );
        //TheSceneMgr->addDynaObj ( sea );
    }

	if (1)
	{
		GameEditor::GxMap* xmap=new GameEditor::GxMap(5,50);
		TheSceneMgr->addDynaObj ( xmap );
	}
    //TheSceneMgr->mEye.InitTrack( &gAnimMesh[0] );

    //gEvent.WaitForUse(-1);

    TheSceneMgr->mSceneMachine.ChangeToScene ( gsGame );

    return TRUE_INT;
}




void GGame::gameRender ( float fPass )
{
    //GMeshManager::GetSingleton().Render( fPass );

    mSceneMgr->mSceneRootNode->draw();

    //gAnimMesh[0].Render( fPass );

    renderEye ( fPass );

    //mpUIMgr->Render( fPass );
}

void GGame::renderEye ( float fPass )
{
    /**************************************************************************************

    ����Ļ����ʾ�۾���Ϣ

    **************************************************************************************/
    //RECT rcEye = {5, 100, 150, 150};

    //char strEye[128];

    //ZeroMemory( strEye, sizeof( strEye ) );

    ////sprintf(strEye,"�۾�λ��--X��%.2f,Y:%.2f,Z:%.2f",mpSceneMgr->mText.mEye.mXPos.mvPos.x,mpSceneMgr->mText.mEye.mXPos.mvPos.y,mpSceneMgr->mText.mEye.mXPos.mvPos.z);

    //sprintf( strEye, "������ٶȣ�%.2f", mSceneMgr->mEye.mXPos.mfSpeedMove );

    //mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );



    //ZeroMemory( strEye, sizeof( strEye ) );

    //if ( mpSelectAnim != NULL )
    //{
    //    sprintf( strEye, "��ţλ��--X��%.2f,Y:%.2f,Z:%.2f���ٶȣ�%.2f", mpSelectAnim->mXPos.mvPos.x, mpSelectAnim->mXPos.mvPos.y, mpSelectAnim->mXPos.mvPos.z, mpSelectAnim->mXPos.mfSpeedMove );

    //    mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );
    //}


    //if ( mpSelectObj != NULL )
    //{
    //    ZeroMemory( strEye, sizeof( strEye ) );
    //    sprintf( strEye, "��ǰ����ID��%d", ( ( CGameStaticObj* )( mpSelectObj ) )->m_nObjID );
    //    mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );

    //    ZeroMemory( strEye, sizeof( strEye ) );
    //    sprintf( strEye, "��ǰģ��ID��%d", ( ( CGameStaticObj* )( mpSelectObj ) )->LnMeshID );
    //    mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );
    //}

    //ZeroMemory( strEye, sizeof( strEye ) );
    //sprintf( strEye, "��ǰ��Ⱦ������Ŀ��%d", GMeshBuffer::GetSingleton().mRenderObjNum + 1 );
    //mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //rcEye.top += 50;
    //rcEye.bottom += 50;
    //mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );

}

void GGame::finish()
{
    mFinished = true;
}




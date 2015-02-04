#include "GGameDemoHeader.h"
#include "GGame.h"
#include "XString.h"
#include <locale.h>
#include "XEvent.h"
#include "GGameOption.h"
#include "GComponentFactory.h"
#include "GResourceManager.h"
#include "GTerrain.h"
#include "GText.h"
#include "GFilmPlayer.h"
#include "GUINode.h"
#include "GUIManager.h"

/******************************************************************/
//�����MeshBuffer�У�����͵�ͼ����
//��ա����桢��ͼ��������׶�ü�
/******************************************************************/

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
    CXSingleton<GEffectManager>::destoryInstance();
    CXSingleton<GMeshManager>::destoryInstance();
    CXSingleton<GGameOption>::destoryInstance();
    CXSingleton<GComponentFactory>::destoryInstance();
    CXSingleton<GInputEntityManager>::destoryInstance();

    CXSingleton<GFilmPlayer>::destoryInstance();

    CXSingleton<GText>::destoryInstance();

    CXSingleton<GD8Input>::destoryInstance();
    CXSingleton<GD9Device>::destoryInstance();
}

bool GGame::loop()
{
    if ( mFinished )
        return false;
    TheTimer->update();
    if ( mIsActive )
    {
        INPUTSYSTEM.Update();
        getInput();
    }
    update();
    render();
    return true;
}

void GGame::getInput()
{
    if ( INPUTSYSTEM.IskeyUp ( DIK_ESCAPE ) )
    {
        finish();
        return;
    }
    if ( INPUTSYSTEM.IskeyUp ( DIK_TAB ) )
    {
        FilmPlayer->play ( "film.lua" );
    }
    InputEntityMgr->getInput ( TheTimer->getFrameTimems() );
}

void GGame::update( )
{
    eGameScene gs = mSceneMgr->mSceneMachine.GetNowScene();

    float fPass = TheTimer->getFrameTimeSec();

    switch ( gs )
    {
    case gsLoading:
        break;
    case gsGame:
    {
        FilmPlayer->process();
        mSceneMgr->update ( fPass );
		UIMgr->processInput();
    }
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

    CoInitialize ( NULL );
    //��ʼ�����
    //CXASSERT_RETURN_FALSE ( gCursor.Init() );
    //gCursor.SetNowCursor ( curNormal );

    //��ʼ�����
    CXASSERT_RETURN_FALSE ( GFrameWork::init ( mainWnd ) );
    //��ʼ��D3D�豸
    CXASSERT_RETURN_FALSE ( GSingletonD9Device::getInstance()->Init ( mMainWin ) );
    CXASSERT_RETURN_FALSE ( GSingletonD8Input::getSingleton().Init ( GSingletonD9Device::getSingleton(), mInst, mMainWin ) );

    //��ʼ������������
    mSceneMgr = new GSceneManager;

	CXASSERT_RETURN_FALSE ( mSceneMgr->init() );

    //MeshMgr.Init();

	CXASSERT_RETURN_FALSE ( TextMgr->init() );

    //������Ⱦ������3000ms

    CoUninitialize();

    gEvent.Create ( "Load" );

    //gEvent.SetUsed();

    //mMTLoadObj.Init(&LoadObj,(LPVOID)this,true);

    gLuaScript.init();
    luaRegistAll();

    loadObj ( this );

    mSceneMgr->mSceneMachine.ChangeToScene ( gsGame );


    return true;
}

void GGame::shutDown()
{
	CXSingleton<GGame>::destoryInstance();
}



#define LuaModel(name)
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
	if (1)
	{
		//GUINode* uinode=new GUINode;
		//CXASSERT_RETURN_FALSE(uinode->recreate());
		//TheSceneMgr->addStaticObj ( uinode );
		//getUIRootNode()->addChild(uinode);
	}
    //if ( 1 )
    //{
    //    //������������ϵ
    //    //GWorldCorrd* corrd = new GWorldCorrd();
    //    //corrd->setNodeName ( "World Corrd" );
    //    //CXASSERT_RETURN_FALSE ( corrd->recreate() );
    //    //TheSceneMgr->addStaticObj ( corrd );
    //}



    //if ( 1 )
    //{
    //    GAnimEntity *pAnimMesh = new GAnimEntity;
    //    pAnimMesh->setMediaFile ( "..\\Data\\res\\Anim\\AnimMesh0002\\A0002.X" );
    //    CXASSERT_RETURN_FALSE ( pAnimMesh->recreate() );
    //    TheSceneMgr->addDynaObj ( pAnimMesh );
    //}
    //if ( 0 )
    //{
    //    MeshPara seaPara (  80.0f, 0, 64, "..\\Data\\res\\water\\BlueShort\\A21C_000.jpg", NULL );

    //    GWater* sea = new GWater;
    //    sea->setParam ( seaPara );
    //    sea->mCanSelect = false;
    //    //sea->addQuakePoint ( -50, 0, 10.0f, 2.8f );
    //    //sea->addQuakePoint ( 50, 0, 10.0f, 2.8f );
    //    sea->recreate();
    //    //sea->setWorldTranslate ( D3DXVECTOR3 ( 0, 1, 0 ) );
    //    TheSceneMgr->addDynaObj ( sea );
    //}
    ////gLuaScript.init();
    //if ( 1 )
    //{

    //    //luacpp::reg_cclass<GNode>::_reg(gLuaScript.getState(),"GNode");

    //    //luacpp::reg_cclass<GTerrain,GNode>::_reg(gLuaScript.getState(),"GTerrain")
    //    //luacpp::reg_cclass<GTerrain>::_reg(gLuaScript.getState(),"GTerrain")
    //    //	.constructor<void>()
    //    //	.function("recreate",&GTerrain::recreate);

    //    //luacpp::reg_cclass<GSceneManager>::_reg(gLuaScript.getState(),"GSceneManager")
    //    //	.function("addDynaObj",&GSceneManager::addDynaObj);
    //    //LuaModel(GTerrain)
    //    //{
    //    //	gLuaScript.regClass<GNode> ( "GNode" );
    //    //	gLuaScript.regClass<GTerrain, GNode> ( "GTerrain" );
    //    //	gLuaScript.regClassFunction<GTerrain> ( "recreate", &GTerrain::recreate );
    //    //	gLuaScript.regClassCreator<GTerrain>();
    //    //}

    //    //      gLuaScript.regClass<GSceneManager> ( "GSceneManager" );
    //    //      gLuaScript.regClassFunction<GSceneManager> ( "addDynaObj", &GSceneManager::addDynaObj );

    //    //      gLuaScript.regGlobalFun ( "getSceneMgr", getSceneMgr );
    //    //      gLuaScript.regGlobalFun ( "logInfo", logInfo );

    //    CXASSERT ( gLuaScript.doFile ( "main.lua" ) );
    //    //int n=luacpp::call<int>(gLuaScript.getState(),"addNum",3);
    //    //luacpp::call<void>(gLuaScript.getState(),"addInfo");
    //    int n = 0;
    //    gLuaScript.call ( n, "addNum", 3 );
    //    gLuaScript.call ( "addInfo" );

    //    //GTerrain* xmap=new GTerrain();
    //    //xmap->recreate();
    //    //TheSceneMgr->addDynaObj ( xmap );
    //}
    ////TheSceneMgr->mEye.InitTrack( &gAnimMesh[0] );

    ////gEvent.WaitForUse(-1);

    TheSceneMgr->mSceneMachine.ChangeToScene ( gsGame );

    return TRUE_INT;
}




void GGame::gameRender ( float fPass )
{
    mSceneMgr->mSceneRootNode->draw();
	UIMgr->draw();
    TextMgr->drawText();
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
    //sprintf( strEye, "��ǰ��Ⱦ������Ŀ��%d", GMeshBuffer::getSingleton().mRenderObjNum + 1 );
    //mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //rcEye.top += 50;
    //rcEye.bottom += 50;
    //mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );

}

void GGame::finish()
{
    mFinished = true;
}






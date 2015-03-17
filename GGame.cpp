#include "GGameDemoHeader.h"
#include "GGame.h"
#include "XCharString.h"
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
#include "Content.h"

/******************************************************************/
//天空在MeshBuffer中，海面和地图不在
//天空、海面、地图均不做视锥裁剪
/******************************************************************/

GGame::GGame ( void )
    : mpSelectObj ( NULL )
    , mpSelectAnim ( NULL )
    , mFinished ( false )
{
}

GGame::~GGame ( void )
{
    //CXSingleton<GResourceManager<GTexture>>::destoryInstance();
    //CXSingleton<GResourceManager<GAnimationResource>>::destoryInstance();
    //CXSingleton<GEffectManager>::destoryInstance();
    //CXSingleton<GMeshManager>::destoryInstance();
    //CXSingleton<GGameOption>::destoryInstance();
    //CXSingleton<GComponentFactory>::destoryInstance();
    //CXSingleton<GInputEntityManager>::destoryInstance();
    //CXSingleton<GFilmPlayer>::destoryInstance();
    //CXSingleton<GText>::destoryInstance();
    //CXSingleton<GD8Input>::destoryInstance();
    //CXSingleton<GD9Device>::destoryInstance();
}

bool GGame::loop()
{
    if ( mFinished )
        return false;
    Content::Timer.update();
    if ( mIsActive )
    {
        Content::InputSystem.Update();
        if ( !getInput() )
            return false;
    }
    update();
    render();
    return true;
}

bool GGame::getInput()
{
    if (  Content::InputSystem.IskeyUp ( DIK_ESCAPE ) )
    {
        finish();
        return false;
    }
    if (  Content::InputSystem.IskeyUp ( DIK_TAB ) )
    {
        Content::FilmPlayer.play ( "film.lua" );
    }
    Content::InputEntityMgr.getInput ( Content::Timer.getFrameTimems() );
    return true;
}

bool GGame::update()
{
    eGameScene gs =  Content::Scene.mSceneMachine.GetNowScene();

    float fPass = Content::Timer.getFrameTimeSec();

    switch ( gs )
    {
    case gsLoading:
        break;
    case gsGame:
    {
        Content::FilmPlayer.process();
        Content::Scene.update ( fPass );
        Content::UIMgr.processInput();
    }
    break;
    }
    return true;
}

void GGame::render( )
{
    float fPass = Content::Timer.getFrameTimeSec();

    eGameScene gs =  Content::Scene.mSceneMachine.GetNowScene();

    Content::Scene.setView();
    switch (  Content::Device.TestDevice() )
    {
    case D3D_OK:
    case D3DERR_DEVICELOST:
        break;
    case D3DERR_DEVICENOTRESET:
    {
        Content::Device.OnDeviceLost();
    }
    break;
    default:
    {
        CXASSERT ( 0 );
    }
    break;
    }
    if (  Content::Device.BeginRender() )
    {
        switch ( gs )
        {

        case gsLoading:

            //mpContent::UIMgr.RenderLoading( fPass );

            break;

        case  gsGame:

            gameRender ( fPass );

            break;

        case gslogin:

            //mpContent::UIMgr.RenderLogin(fPass);

            break;
        }

        Content::Device.EndRender();
    }

}

bool GGame::init ( HWND mainWnd )
{
    //setlocale( LC_ALL, "zh_CN.UTF-8" );

    //初始化大概500ms

    CoInitialize ( NULL );
    //初始化鼠标
    //CXASSERT_RETURN_FALSE ( gCursor.Init() );
    //gCursor.SetNowCursor ( curNormal );
    Content::initStatic();
    //初始化框架
    CXASSERT_RETURN_FALSE ( GFrameWork::init ( mainWnd ) );
    //初始化D3D设备
    CXASSERT_RETURN_FALSE ( Content::Device.init ( mMainWin ) );

    CXASSERT_RETURN_FALSE ( Content::InputSystem.init ( Content::Device, mInst, mMainWin ) );

    CXASSERT_RETURN_FALSE ( Content::GameOption.init() );

    //初始化场景管理器
    CXASSERT_RETURN_FALSE (  Content::Scene.init() );

    CXASSERT_RETURN_FALSE (  Content::UIMgr.init() );

    CXASSERT_RETURN_FALSE ( Content::Text.init() );

    //加载渲染对象大概3000ms

    CoUninitialize();

    gEvent.Create ( "Load" );

    //gEvent.SetUsed();

    //mMTLoadObj.Init(&LoadObj,(LPVOID)this,true);

    gLuaScript.init();
    luaRegistAll();

    loadObj ( this );

    Content::Scene.mSceneMachine.ChangeToScene ( gsGame );


    return true;
}

void GGame::shutDown()
{
    //CXSingleton<GGame>::destoryInstance();
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

    //设置投影矩阵
    // Content::Scene.loadScene("gameSceneEditor.xml");
    Content::Scene.setProj();
    if ( 1 )
    {
        //GUINode* uinode=new GUINode;
        //CXASSERT_RETURN_FALSE(uinode->recreate());
        // Content::Scene.addStaticObj ( uinode );
        //getUIRootNode()->addChild(uinode);
    }
    //if ( 1 )
    //{
    //    //创建世界坐标系
    //    //GWorldCorrd* corrd = new GWorldCorrd();
    //    //corrd->setNodeName ( "World Corrd" );
    //    //CXASSERT_RETURN_FALSE ( corrd->recreate() );
    //    // Content::Scene.addStaticObj ( corrd );
    //}



    //if ( 1 )
    //{
    //    GAnimEntity *pAnimMesh = new GAnimEntity;
    //    pAnimMesh->setMediaFile ( "..\\Data\\res\\Anim\\AnimMesh0002\\A0002.X" );
    //    CXASSERT_RETURN_FALSE ( pAnimMesh->recreate() );
    //     Content::Scene.addDynaObj ( pAnimMesh );
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
    //     Content::Scene.addDynaObj ( sea );
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
    //    // Content::Scene.addDynaObj ( xmap );
    //}
    //// Content::Scene.mEye.InitTrack( &gAnimMesh[0] );

    ////gEvent.WaitForUse(-1);

    Content::Scene.mSceneMachine.ChangeToScene ( gsGame );

    return TRUE_INT;
}




void GGame::gameRender ( float fPass )
{
    Content::Scene.mSceneRootNode->draw();
    Content::UIMgr.draw();
    Content::Text.drawText();
}

void GGame::renderEye ( float fPass )
{
    /**************************************************************************************

    在屏幕上显示眼睛信息

    **************************************************************************************/
    //RECT rcEye = {5, 100, 150, 150};

    //char strEye[128];

    //ZeroMemory( strEye, sizeof( strEye ) );

    ////sprintf(strEye,"眼睛位置--X：%.2f,Y:%.2f,Z:%.2f",mpSceneMgr->mText.mEye.mXPos.mvPos.x,mpSceneMgr->mText.mEye.mXPos.mvPos.y,mpSceneMgr->mText.mEye.mXPos.mvPos.z);

    //sprintf( strEye, "摄像机速度：%.2f",  Content::Scene.mEye.mXPos.mfSpeedMove );

    //mpContent::UIMgr.mText.SetCurrentFontSize( fs12 );

    //mpContent::UIMgr.mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );



    //ZeroMemory( strEye, sizeof( strEye ) );

    //if ( mpSelectAnim != NULL )
    //{
    //    sprintf( strEye, "铁牛位置--X：%.2f,Y:%.2f,Z:%.2f，速度：%.2f", mpSelectAnim->mXPos.mvPos.x, mpSelectAnim->mXPos.mvPos.y, mpSelectAnim->mXPos.mvPos.z, mpSelectAnim->mXPos.mfSpeedMove );

    //    mpContent::UIMgr.mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpContent::UIMgr.mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );
    //}


    //if ( mpSelectObj != NULL )
    //{
    //    ZeroMemory( strEye, sizeof( strEye ) );
    //    sprintf( strEye, "当前对象ID：%d", ( ( CGameStaticObj* )( mpSelectObj ) )->m_nObjID );
    //    mpContent::UIMgr.mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpContent::UIMgr.mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );

    //    ZeroMemory( strEye, sizeof( strEye ) );
    //    sprintf( strEye, "当前模型ID：%d", ( ( CGameStaticObj* )( mpSelectObj ) )->LnMeshID );
    //    mpContent::UIMgr.mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpContent::UIMgr.mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );
    //}

    //ZeroMemory( strEye, sizeof( strEye ) );
    //sprintf( strEye, "当前渲染对象数目：%d", GMeshBuffer::getSingleton().mRenderObjNum + 1 );
    //mpContent::UIMgr.mText.SetCurrentFontSize( fs12 );

    //rcEye.top += 50;
    //rcEye.bottom += 50;
    //mpContent::UIMgr.mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );

}

void GGame::finish()
{
    mFinished = true;
}






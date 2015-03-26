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
#include "GMaskNode.h"
#include "GPlugin.h"

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


DWORD WINAPI loadObj ( LPVOID pParam )
{
    CoInitialize ( NULL );

    //设置投影矩阵
    Content::Scene.setProj();
	Content::Game.mDelegateOnInitEnd.trigger();
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

GPlugin* GGame::getPlugin ( const char* name )
{
for ( auto & plugin: mPlugins )
    {
        if ( dStrEqual ( name, plugin->getPluginName() ) )
        {
            return plugin;
        }
    }
    return nullptr;
}

bool GGame::registerPlugin ( GPlugin* plugin )
{
    CXASSERT_RETURN_FALSE ( plugin );
    CXASSERT_RETURN_FALSE ( !getPlugin ( plugin->getPluginName() ) );
    mPlugins.push_back ( plugin );
    return true;
}

bool GGame::unregisterPlugin ( const char* name )
{
    GPlugin* plugin = getPlugin ( name );
    CXASSERT_RETURN_FALSE ( plugin != nullptr );
    mPlugins.destroyPointer( plugin );
    return true;

}






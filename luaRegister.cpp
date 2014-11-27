#include "GGameDemoHeader.h"
#include "GAnimEntity.h"
#include "GComponentTrans.h"
#include "GFilmFrame.h"
#include "GFilmPlayer.h"
#include "GGame.h"
#include "GNode.h"
#include "GObject.h"
#include "GRenderEntity.h"
#include "GSceneMgr.h"
#include "GTerrain.h"
	
int luaRegistAll()
{
	gLuaScript.regGlobalFun ( "getFilmPlayer", getFilmPlayer );
	gLuaScript.regGlobalFun ( "getSceneMgr", getSceneMgr );
	gLuaScript.regGlobalFun ( "logInfo", logInfo );

	gLuaScript.regClass<GObject>( "GObject" );
	gLuaScript.regClassCreator<GObject>( );
	gLuaScript.regClassFunction<GObject>( "setName", &GObject::setName );

	gLuaScript.regClass<GNode,GObject>( "GNode" );
	gLuaScript.regClassCreator<GNode>( );
	gLuaScript.regClassFunction<GNode>( "recreate", &GNode::recreate );
	gLuaScript.regClassFunction<GNode>( "getTrans", &GNode::getTrans );

	gLuaScript.regClass<GRenderEntity,GNode>( "GRenderEntity" );
	gLuaScript.regClassCreator<GRenderEntity>( );

	gLuaScript.regClass<GAnimEntity,GRenderEntity>( "GAnimEntity" );
	gLuaScript.regClassCreator<GAnimEntity>( );
	gLuaScript.regClassFunction<GAnimEntity>( "setMediaFile", &GAnimEntity::setMediaFile );
	gLuaScript.regClassFunction<GAnimEntity>( "play", &GAnimEntity::play );

	gLuaScript.regClass<GComponentTrans>( "GComponentTrans" );
	gLuaScript.regClassCreator<GComponentTrans>( );
	gLuaScript.regClassFunction<GComponentTrans>( "setTranslate", &GComponentTrans::setTranslate );

	gLuaScript.regClass<GFilmFrame>( "GFilmFrame" );
	gLuaScript.regClassCreator<GFilmFrame>( );

	gLuaScript.regClass<GFilmPlayer>( "GFilmPlayer" );
	gLuaScript.regClassCreator<GFilmPlayer>( );
	gLuaScript.regClassFunction<GFilmPlayer>( "addFrame", &GFilmPlayer::addFrame );

	gLuaScript.regClass<GSceneManager>( "GSceneManager" );
	gLuaScript.regClassCreator<GSceneManager>( );
	gLuaScript.regClassFunction<GSceneManager>( "addDynaObj", &GSceneManager::addDynaObj );
	gLuaScript.regClassFunction<GSceneManager>( "getObj", &GSceneManager::getObj );
	gLuaScript.regClassFunction<GSceneManager>( "moveCameraToObj", &GSceneManager::moveCameraToObj );

	gLuaScript.regClass<GTerrain,GRenderEntity>( "GTerrain" );
	gLuaScript.regClassCreator<GTerrain>( );

	return 0;
}
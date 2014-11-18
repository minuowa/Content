#include "GGameDemoHeader.h"
#include "GAnimEntity.h"
#include "GGame.h"
#include "GNode.h"
#include "GRenderEntity.h"
#include "GSceneMgr.h"
#include "GTerrain.h"
	
int luaRegistAll()
{
	gLuaScript.regGlobalFun ( "getSceneMgr", getSceneMgr );

	gLuaScript.regGlobalFun ( "logInfo", logInfo );

	gLuaScript.regClass<GAnimEntity,GRenderEntity>( "GAnimEntity" );
	gLuaScript.regClassCreator<GAnimEntity>( );
	gLuaScript.regClassFunction<GAnimEntity>( "setMediaFile", &GAnimEntity::setMediaFile );

	gLuaScript.regClass<GNode>( "GNode" );
	gLuaScript.regClassCreator<GNode>( );
	gLuaScript.regClassFunction<GNode>( "recreate", &GNode::recreate );

	gLuaScript.regClass<GRenderEntity,GNode>( "GRenderEntity" );
	gLuaScript.regClassCreator<GRenderEntity>( );

	gLuaScript.regClass<GSceneManager>( "GSceneManager" );
	gLuaScript.regClassCreator<GSceneManager>( );
	gLuaScript.regClassFunction<GSceneManager>( "addDynaObj", &GSceneManager::addDynaObj );

	gLuaScript.regClass<GTerrain,GRenderEntity>( "GTerrain" );
	gLuaScript.regClassCreator<GTerrain>( );

	return 0;
}
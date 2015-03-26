#pragma once

#include "GEffect.h"
#include "GAmmo.h"
#include "Particles.h"
#include "XCursor.h"
#include "GAnimEntity.h"
#include "GWater.h"
#include "GMeshBuffer.h"
#include "GWorldCorrd.h"
#include "GStillEntity.h"
#include "GFrameWork.h"
#include "GTimer.h"
#include "GNode.h"
#include "GCamera.h"
#include "GSceneMgr.h"
#include "XMultiThread.h"

class GPlugin;
extern DWORD WINAPI loadObj(LPVOID pParam);

class GGame:public GFrameWork
{
public:
	typedef GFrameWork Super;
public:

	CXDelegate mDelegateOnInitEnd;


	GGame(void);
	~GGame(void);

	virtual bool init(HWND mainWnd);

	virtual bool loop();
	
	void shutDown();

	void finish();

	bool registerPlugin(GPlugin* plugin);

	GPlugin* getPlugin(const char* name);

	bool unregisterPlugin(const char* name);
private:

	bool getInput();

	bool update();

	void render( );
public:
	CXMultiThread mMTLoadObj;

	GStillEntity *mpSelectObj;

	GAnimEntity *mpSelectAnim;


private:

	void gameRender(float fPass);

	void renderEye(float fPass);

	bool	mFinished;

protected:
	CXDynaArray<GPlugin*> mPlugins;
};

DeclareFilmToolGlobal inline void logInfo ( const char* s )
{
	OutputDebugStringA ( "\n" );
	OutputDebugStringA ( s );
}
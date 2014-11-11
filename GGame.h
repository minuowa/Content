#pragma once

#include "GEffect.h"
#include "GAmmo.h"
#include "Particles.h"
#include "XCursor.h"
#include "GAnimMeshObj.h"
#include "GWater.h"
#include "GMeshBuffer.h"
#include "GWorldCorrd.h"
#include "GGameMap.h"
#include "GMeshBaseObj.h"
#include "GFrameWork.h"
#include "GTimer.h"
#include "GNode.h"
#include "GCamera.h"
#include "GSceneMgr.h"
#include "XMultiThread.h"

extern DWORD WINAPI loadObj(LPVOID pParam);

class GGame:public GFrameWork
{
public:
	typedef GFrameWork Super;

	FiDeclareSingleton( GGame );
public:
	GGame(void);
	~GGame(void);

	virtual bool init(HWND mainWnd);

	virtual void loop();

	void shutDown();

	void finish();
public:

	GSceneManager* getSceneMgr( ) { return mSceneMgr;}

private:

	void getInput();

	void update();

	void render( );

public:

	CXMultiThread mMTLoadObj;

	GSceneManager *mSceneMgr;

	GMeshBaseObj *mpSelectObj;

	GAnimMeshObj *mpSelectAnim;


private:

	void gameRender(float fPass);

	void renderEye(float fPass);

	bool	mFinished;
};

#define TheGame GGame::getInstance() 
#define TheSceneMgr GGame::GetSingleton().getSceneMgr()

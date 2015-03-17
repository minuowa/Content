#pragma once
#include "GGame.h"
#include "GComponentFactory.h"
#include "GGameOption.h"
#include "GTexture.h"
#include "GResourceManager.h"
#include "GInputEntityManager.h"
#include "GMeshBuffer.h"
#include "GEffect.h"
#include "GText.h"
#include "GFilmPlayer.h"
#include "GUIManager.h"
#include "GAnimationResource.h"
#include "GTimer.h"
#include "GInitilizer.h"
class Content
{
public:

	static void initStatic();
	static void destroyStatic();

	static void initComponentFactory();
	static void initNodeFactory();

	static GInitilizer Initilizer;

	static GTimer Timer;

    static GGameOption GameOption;

	static GComponentFactory ComponentFactory;

	static GFactory<GNode> GameObjFactory;

	static GD9Device Device;

	static GD8Input InputSystem;

	static GResourceManager<GTexture> TextureMgr;
    static GEffectManager EffectMgr;
    static GMeshManager MeshMgr;
	static GResourceManager<GAnimationResource> AnimationMgr;

	static GInputEntityManager InputEntityMgr;

	static GFilmPlayer FilmPlayer;


	static GText Text;
	static GSceneManager Scene;
	static GUIManager UIMgr;

    static GGame Game;
};


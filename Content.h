#pragma once
#include "GResourceManager.h"
#include "GFactory.h"
#include "GAnimationResource.h"
#include "GTexture.h"
class GD9Device;
class GD8Input;
class GGame;
class GComponentFactory;
class GGameOption;
class GMeshManager;
class GEffect;
class GEffectManager;
class GSceneManager;
class GText;
class GFilmPlayer;
class GUIManager;
class GInputEntityManager;
class GTimer;
class GInitilizer;
class GNode;
class GRender;
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
	static GRender Render;

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


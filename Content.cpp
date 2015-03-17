#include "GGameDemoHeader.h"
#include "Content.h"
#include "GTerrain.h"
#include "GComponentMesh.h"
#include "GComponentBox.h"

GInitilizer Content::Initilizer;

GText Content::Text;

GTimer Content::Timer;

GD9Device Content::Device;

GGameOption Content::GameOption;

GComponentFactory Content::ComponentFactory;

GFactory<GNode> Content::GameObjFactory;

GD8Input Content::InputSystem;

GResourceManager<GTexture> Content::TextureMgr;
GEffectManager Content::EffectMgr;
GMeshManager Content::MeshMgr;
GResourceManager<GAnimationResource> Content::AnimationMgr;

GInputEntityManager Content::InputEntityMgr;

GFilmPlayer Content::FilmPlayer;

GSceneManager Content::Scene;
GUIManager Content::UIMgr;

GGame Content::Game;

void Content::initStatic()
{
	EffectMgr.clear();
	initComponentFactory();
	initNodeFactory();
}

void Content::destroyStatic()
{

}


#define __RegisterGameObjCreator(className) \
	Content::GameObjFactory.registerCreator(#className,className::CreateNode)

void Content::initNodeFactory()
{
	__RegisterGameObjCreator ( GNode );
	__RegisterGameObjCreator ( GAnimEntity );
	__RegisterGameObjCreator ( GStillEntity );
	__RegisterGameObjCreator ( GRenderEntity );
	__RegisterGameObjCreator ( GWater );
	__RegisterGameObjCreator ( GWorldCorrd );
	__RegisterGameObjCreator ( GTerrain );
	__RegisterGameObjCreator ( GCamera );
}

#define __RegisterComponentCreator(className) \
	Content::ComponentFactory.registerCreator(#className,className::ComponentType,className::createComponent)

void Content::initComponentFactory()
{
	__RegisterComponentCreator ( GComponentTrans );
	__RegisterComponentCreator ( GComponentMesh );
	__RegisterComponentCreator ( GComponentBox );
}



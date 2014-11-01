#include "GGameDemoHeader.h"
#include "GSceneMgr.h"
#include "GGameMap.h"
#include "GGame.h"
#include "GComponentTrans.h"
#include "GComponentMesh.h"
#include "GComponentFactory.h"
#include "GComponentBox.h"
#include <fstream>

static const char* gRootNodeName = "SceneRootNode";
static const char* gRootDynamicNodeName = "SceneRootDynamicNode";
static const char* gRootStaticNodeName = "SceneRootStaticNode";

GSceneManager::GSceneManager ( void )
    : mSceneDynamicRootNode ( 0 )
    , mSceneRootNode ( 0 )
    , mSceneStaticRootNode ( 0 )
    , mCurCamera ( 0 )
{
    mOperatoredObj = 0;
}

GSceneManager::~GSceneManager ( void )
{
    dSafeDelete ( mSceneRootNode );
}

bool GSceneManager::init()
{
    initNodeFactory();
    initComponentFactory();

    mSceneRootNode = new GNode();
    mSceneRootNode->setNodeName ( gRootNodeName );

    mSceneStaticRootNode = new GNode();
    mSceneDynamicRootNode = new GNode();
    mSceneStaticRootNode->setNodeName ( gRootStaticNodeName );
    mSceneDynamicRootNode->setNodeName ( gRootDynamicNodeName  );
    mSceneRootNode->addChild ( mSceneStaticRootNode );
    mSceneRootNode->addChild ( mSceneDynamicRootNode );

    mCurCamera = new GCamera;
    mCurCamera->setNodeName ( "Current Camera" );
    if ( mCurCamera->recreate()  )
    {
        //GComponentTrans* trans = get
        mCurCamera->getTrans().mDir = D3DXVECTOR3 ( ZEROFLOAT, -200.0f, 200.0f );
        mCurCamera->getTrans().mRight = D3DXVECTOR3 ( 1.0f, ZEROFLOAT, ZEROFLOAT );
        D3DXVec3Cross ( &mCurCamera->getTrans().mUpon, &mCurCamera->getTrans().mDir, &mCurCamera->getTrans().mRight );

        D3DXVec3Normalize ( &mCurCamera->getTrans().mDir, &mCurCamera->getTrans().mDir );
        D3DXVec3Normalize ( &mCurCamera->getTrans().mRight, &mCurCamera->getTrans().mRight );
        D3DXVec3Normalize ( &mCurCamera->getTrans().mUpon, &mCurCamera->getTrans().mUpon );

        mCurCamera->getTrans().mSpeedMove = 150.0f;

        addDynaObj ( mCurCamera );

        return true;
    }

    return false;
}

void* GSceneManager::getInput ( float fPass )
{
    mSceneDynamicRootNode->getInput ( fPass );
    return 0;
}

void GSceneManager::setView()
{
    mCurCamera->setView();
}

void GSceneManager::setProj()
{
    mCurCamera->setProj();
}

void GSceneManager::update ( float fPass )
{
    //mCurCamera->Update();
    if ( mSceneDynamicRootNode )
    {
        //mSceneRootNode->Update();
        mSceneDynamicRootNode->update();
    }
}

void GSceneManager::destroy ( CGameStaticObj *pObj )
{
    GString sConfigFile;
    sConfigFile.Format ( "Data\\StaticObj\\Save\\StaticObj%03d.txt", pObj->m_nObjID );

    if ( IsFileExist ( sConfigFile.c_str() ) )
    {
        DeleteFileA ( sConfigFile );
    }

}

bool GSceneManager::saveScene ( CChar* xmlFile )
{
    //version="1.0" encoding="UTF-8"
    CXRapidxmlDocument doc;
    doc.append_node (
        doc.allocate_node ( rapidxml::node_pi, doc.allocate_string ( "xml version=\"1.0\" encoding=\"UTF-8\"" ) )
    );
    CXRapidxmlNode* root = doc.allocate_node ( rapidxml::node_element );
    root->name ( "GameObects" );
    doc.append_node ( root );
    mSceneRootNode->linkTo ( root );
    CXRapidxmlWriter writer;
    writer.AppendChild ( &doc );
    writer.Write ( xmlFile );
    return true;
}

void GSceneManager::addStaticObj ( GNode* node )
{
    CXASSERT ( node );
    mSceneStaticRootNode->addChild ( node );
}

void GSceneManager::addDynaObj ( GNode* node )
{
    CXASSERT ( node );
    mSceneDynamicRootNode->addChild ( node );
}


GNode* GSceneManager::getNodeByName ( const char* name )
{
    if ( mSceneRootNode )
        return mSceneRootNode->getNodeByName ( name );
    return nullptr;
}

#define __RegisterGameObjCreator(className) \
	mGameObjFactory.registerCreator(#className,className::CreateNode)

void GSceneManager::initNodeFactory()
{
    __RegisterGameObjCreator ( GNode );
    __RegisterGameObjCreator ( GAnimMeshObj );
    __RegisterGameObjCreator ( GMeshBaseObj );
    __RegisterGameObjCreator ( GRenderObject );
    __RegisterGameObjCreator ( GWater );
    __RegisterGameObjCreator ( GWorldCorrd );
    __RegisterGameObjCreator ( GCamera );

#if TheEditor
    typedef GFactory<GNode>::ObjCreatorMap GNodeCreatorMap;
    const GNodeCreatorMap& nodeCreatorMap = mGameObjFactory.getCreators();
    GNodeCreatorMap::const_iterator walk = nodeCreatorMap.begin();
    GNodeCreatorMap::const_iterator end = nodeCreatorMap.end();
    for ( ; walk != end; ++walk )
    {
        mGameObjectTypeArray.push_back ( walk->first.c_str() );
    }
#endif
}


void GSceneManager::initComponentFactory()
{
    __RegisterComponentCreator ( GComponentTrans );
    __RegisterComponentCreator ( GComponentMesh );
    __RegisterComponentCreator ( GComponentBox );

    typedef GComponentFactory::ComponentCreatorMap ComponentCreatorMap;
    const ComponentCreatorMap& nodeCreatorMap =
        CXSingleton<GComponentFactory>::GetSingleton().getCreators();
    ComponentCreatorMap::const_iterator walk = nodeCreatorMap.begin();
    ComponentCreatorMap::const_iterator end = nodeCreatorMap.end();
    for ( ; walk != end; ++walk )
    {
        mObjectComponentTypeArray.push_back ( walk->first.c_str() );
    }
}

const CharStringArr& GSceneManager::getGameObjectTypes()
{
    return mGameObjectTypeArray;
}

void GSceneManager::setOperatorObj ( int objID )
{
    mOperatoredObj = objID;
}

void GSceneManager::onCallBack ( const CXDelegate& delgate )
{
}

GNode* GSceneManager::getSceneRoot() const
{
    return mSceneRootNode;
}

const CharStringArr& GSceneManager::getObjectComponentTypes()
{
    return mObjectComponentTypeArray;
}

void GSceneManager::addObj ( GNode* node, GNode* parent/*=nullptr*/ )
{
    if ( parent )
    {
        parent->addChild ( node );
    }
    else
    {
        CXASSERT ( mSceneRootNode );
        mSceneRootNode->addChild ( node );
    }
}

void GSceneManager::addObj ( const char* parentName, const char* typeName )
{
    GNode* parent = getNodeByName ( parentName );
    GNode* child = createObjByTypeName ( typeName );
    addObj ( child , parent );
}

GNode* GSceneManager::createObjByTypeName ( const char* typeName )
{
    return mGameObjFactory.create ( typeName );
}
GNode* getNodeByName ( CXDynaArray<GNode*>& list, const char* name )
{
for ( auto n: list )
    {
        if ( !strcmp ( n->getObjectName(), name ) )
        {
            return n;
        }
    }
    return nullptr;
}
GNode* getRootNode ( CXDynaArray<GNode*>& list )
{
for ( auto n: list )
    {
        if ( n->getParent() == nullptr )
        {
            return n;
        }
    }
    return nullptr;
}
bool GSceneManager::loadScene ( const char* xmlFile )
{
    CXDynaArray<GNode*> objList;

    xml_load ( xmlFile );
    xml_get_node ( "Object" )
    {
        GString stype;
        xml_get_attr ( "Type", stype );
        GString sparent;
        xml_get_attr ( "Parent", sparent );
        GNode* n = createObjByTypeName ( stype.c_str() );
        objList.push_back ( n );

        n->registerAll();

        xml_get_node ( "Category" )
        {
            xml_get_attr ( "Type", stype );
            xml_get_node ( "Property" )
            {
                GString propName, propVar;
                xml_get_attr ( "Name", propName );
                xml_get_attr ( "Value", propVar );
                n->setProperty ( stype.c_str(), propName.c_str(), propVar.c_str() );
            }
        }
        if ( !n->recreate() )
        {
            dSafeDelete ( n );
            continue;
        }

        GNode* parent = ::getNodeByName ( objList, sparent.c_str() );
        if ( parent )
            parent->addChild ( n );
    }

    GNode* newRoot = getRootNode ( objList );
    CXASSERT_RETURN_FALSE ( newRoot );
    dSafeDelete ( mSceneRootNode );
    setInnerNode ( newRoot );
    mDelegateReloadScene.trigger();
    return true;
}

bool GSceneManager::setInnerNode ( GNode* rootNode )
{
    CXASSERT_RETURN_FALSE ( rootNode );
    mSceneRootNode = rootNode;
    GNode* dynaRoot = mSceneRootNode->getNodeByName ( gRootDynamicNodeName );
    GNode* staticRoot = mSceneRootNode->getNodeByName ( gRootStaticNodeName );
    CXASSERT_RETURN_FALSE ( dynaRoot );
    CXASSERT_RETURN_FALSE ( staticRoot );
    mSceneDynamicRootNode = dynaRoot;
    mSceneStaticRootNode = staticRoot;

    mCurCamera = nullptr;
    mCurCamera = findFirstCameraInScene ( mSceneRootNode );
    mUsingCamera = mCurCamera->getObjectName();
    CXASSERT_RETURN_FALSE ( mCurCamera );
    setProj();
    return mCurCamera != nullptr;
}

GCamera* GSceneManager::findFirstCameraInScene ( GNode* n )
{
    GCamera camera;
    return ( GCamera* ) mSceneRootNode->getFirstNodeByCategoryName ( camera.categoryName() );
}

void GSceneManager::deleteObj ( const char* name )
{
    GNode* n = mSceneRootNode->getNodeByName ( name );
    if ( n )
        mSceneRootNode->deleteChild ( n );
}
class GIsCamera
{
public:
    bool operator() ( GNode*& n )
    {
        return dStrEqual ( n->categoryName(), mCamera.categoryName() );
    }
    GCamera mCamera;
};
GCamera* GSceneManager::changeToNextCamera()
{
	GNode* cur=mCurCamera;
    GIsCamera isCamera;
    GCamera* next = ( GCamera* ) dFindNextElementInTreeCycle ( mSceneRootNode, cur, isCamera );
    if ( next )
    {
		if ( mCurCamera )
			mCurCamera->setCanGetInput ( false );
		mCurCamera = next;
		mCurCamera->setCanGetInput ( true );
		mUsingCamera = next->getObjectName();
		this->setProj();
    }
    return mCurCamera;
}

void GSceneManager::moveToNextCamera()
{
	GNode* cur = mSceneRootNode->getNodeByName ( mUsingCamera );
    GIsCamera isCamera;
	GCamera* next = ( GCamera* ) dFindNextElementInTreeCycle ( mSceneRootNode, cur, isCamera );
	if ( next )
	{
		mUsingCamera=next->getObjectName();
		mCurCamera->moveTo ( next->GetWorldMatrix ( false ) );
	}

}


CSceneMachine::CSceneMachine()
{
    mgsNowScene = gsNull;
}

eGameScene CSceneMachine::GetNowScene()
{
    return mgsNowScene;
}

void CSceneMachine::ChangeToScene ( eGameScene gs )
{
    if ( gs == mgsNowScene )
    {
        return;
    }

    mgsNowScene = gs;
}

CSceneMachine::~CSceneMachine()
{

}

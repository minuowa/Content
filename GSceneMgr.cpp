#include "GGameDemoHeader.h"
#include "GSceneMgr.h"
#include "GGame.h"
#include "GComponentTrans.h"
#include "GComponentMesh.h"
#include "GComponentFactory.h"
#include "GComponentBox.h"
#include <fstream>
#include "GTerrain.h"
#include "Content.h"

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
    mSceneRootNode = new GNode();
    mSceneRootNode->setName ( gRootNodeName );

    mSceneStaticRootNode = new GNode();
    mSceneDynamicRootNode = new GNode();
    mSceneStaticRootNode->setName ( gRootStaticNodeName );
    mSceneDynamicRootNode->setName ( gRootDynamicNodeName  );
    mSceneRootNode->addChild ( mSceneStaticRootNode );
    mSceneRootNode->addChild ( mSceneDynamicRootNode );

    mCurCamera = new GCamera;
    mCurCamera->setName ( "Current Camera" );
    if ( mCurCamera->recreate()  )
    {
        //GComponentTrans* trans = get
        mCurCamera->getTrans()->setDir ( D3DXVECTOR3 ( ZEROFLOAT, -200.0f, 200.0f ) );
        mCurCamera->getTrans()->setRight ( D3DXVECTOR3 ( 1.0f, ZEROFLOAT, ZEROFLOAT ) );
        D3DXVECTOR3 upon;
        dVector3Cross ( upon, mCurCamera->getTrans()->getDir(), mCurCamera->getTrans()->getRight() );
        mCurCamera->getTrans()->setUpon ( upon );

        mCurCamera->getTrans()->normalizeRotation();

        mCurCamera->getTrans()->mSpeedMove = 150.0f;
        addDynaObj ( mCurCamera );
        return true;
    }

    return false;
}

void* GSceneManager::getInput ( float fPass )
{
    Content::InputEntityMgr.getInput ( fPass );
    //mSceneDynamicRootNode->getInput ( fPass );
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


GNode* GSceneManager::getObj ( const char* name )
{
    if ( mSceneRootNode )
        return mSceneRootNode->getNodeByName ( name );
    return nullptr;
}

void GSceneManager::setOperatorObj ( int objID )
{
    mOperatoredObj = objID;
}

void GSceneManager::onCallBack ( const CXDelegate& delgate, CXEventArgs* )
{
}

GNode* GSceneManager::getSceneRoot() const
{
    return mSceneRootNode;
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
    GNode* parent = getObj ( parentName );
    GNode* child = createObjByTypeName ( typeName );
    CXASSERT_RETURN ( child->recreate() );
    addObj ( child , parent );
}

GNode* GSceneManager::createObjByTypeName ( const char* typeName )
{
    return Content::GameObjFactory.create ( typeName );
}
GNode* getNodeByName ( CXDynaArray<GNode*>& list, const char* name )
{
for ( auto n: list )
    {
        if ( !strcmp ( n->getName(), name ) )
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
    mUsingCamera = mCurCamera->getName();
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
    GNode* cur = mCurCamera;
    GIsCamera isCamera;
    GCamera* next = ( GCamera* ) dFindNextElementInTreeCycle ( mSceneRootNode, cur, isCamera );
    if ( next )
    {
        if ( mCurCamera )
            mCurCamera->setCanGetInput ( false );
        mCurCamera = next;
        mCurCamera->setCanGetInput ( true );
        mUsingCamera = next->getName();
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
        mUsingCamera = next->getName();
        mCurCamera->moveTo ( next->getTrans()->getWorld(), 3000 );
    }

}

GCamera* GSceneManager::getCurCamera() const
{
    return mCurCamera;
}

DeclareFilmTool void GSceneManager::moveCameraToObj ( const char* name, u32 timeMS )
{
    GNode* obj = getObj ( name );
    CXASSERT_RETURN ( obj );
    mCurCamera->moveTo ( obj->getTrans()->getWorld(), timeMS );
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

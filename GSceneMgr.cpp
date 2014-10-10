#include "GGameDemoHeader.h"
#include "GSceneMgr.h"
#include "GGameMap.h"
#include "GGame.h"
#include "GComponentTrans.h"
#include "GComponentMesh.h"
#include "GComponentFactory.h"
#include "GComponentBox.h"

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

bool GSceneManager::Init ( const GD9Device& DVC )
{
    initNodeFactory();
    initComponentFactory();

    mSceneRootNode = new GNode();
    mSceneRootNode->setNodeName ( "Scene Root" );

    mSceneStaticRootNode = new GNode();
    mSceneDynamicRootNode = new GNode();
    mSceneStaticRootNode->setNodeName ( "Scene Static Root" );
    mSceneDynamicRootNode->setNodeName ( "Scene Dyna Root" );
    mSceneRootNode->addChild ( mSceneStaticRootNode );
    mSceneRootNode->addChild ( mSceneDynamicRootNode );

    mCurCamera = new GCamera;
    mCurCamera->setNodeName ( "Current Camera" );
    if ( mCurCamera->reCreate()  )
    {
        //GComponentTrans* trans = get
        mCurCamera->getTrans().mvDir = D3DXVECTOR3 ( ZEROFLOAT, -200.0f, 200.0f );
        mCurCamera->getTrans().mvRight = D3DXVECTOR3 ( 1.0f, ZEROFLOAT, ZEROFLOAT );
        D3DXVec3Cross ( &mCurCamera->getTrans().mvUpon, &mCurCamera->getTrans().mvDir, &mCurCamera->getTrans().mvRight );

        D3DXVec3Normalize ( &mCurCamera->getTrans().mvDir, &mCurCamera->getTrans().mvDir );
        D3DXVec3Normalize ( &mCurCamera->getTrans().mvRight, &mCurCamera->getTrans().mvRight );
        D3DXVec3Normalize ( &mCurCamera->getTrans().mvUpon, &mCurCamera->getTrans().mvUpon );

        mCurCamera->getTrans().mfSpeedMove = 150.0f;

        addDynaObj ( mCurCamera );

        return true;
    }

    return false;
}

void* GSceneManager::GetInput ( float fPass )
{
    mSceneDynamicRootNode->GetInput ( fPass );
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

void GSceneManager::Delete ( CGameStaticObj *pObj )
{
    GString sConfigFile;
    sConfigFile.Format ( "Data\\StaticObj\\Save\\StaticObj%03d.txt", pObj->m_nObjID );

    if ( IsFileExist ( sConfigFile.c_str() ) )
    {
        DeleteFileA ( sConfigFile );
    }

}

bool GSceneManager::SaveScene ( CChar* xmlFile )
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

//bool GSceneMgr::OnNotify ( const EditorEvent& event )
//{
//    if ( !__super::OnNotify ( event ) )
//        return false;
//    switch ( event.mType )
//    {
//    case eEditorToScene_Add:
//    {
//        String typeName = event.mArgs[0];
//        GNode* node = mGameObjFactory.Create ( typeName.c_str() );
//        mSceneRootNode->AddChild ( node );
//    }
//    break;
//    case eEditorToSecne_Select:
//    {
//        String objName = event.mArgs[0];
//        GNode* node = GetNodeByName ( objName );
//        EditorSetObjectProperty ( node );
//        EditorUpdatePopupMenu ( node );
//    }
//    break;
//    case eEditorToScene_PropertyChange:
//    {
//        String objName = event.mArgs[0];

//    }
//    break;
//    case eEditorToScene_ComponentAttach:
//    {
//        String objName = event.mArgs[0];
//        String componenttype = event.mArgs[1];
//        GNode* node = GetNodeByName ( objName );
//        if ( node )
//        {
//            node->AttachComponent ( componenttype );
//        }
//    }
//    break;
//    case eEditorToScene_ComponentDettach:
//    {
//        String objName = event.mArgs[0];
//        String componenttype = event.mArgs[1];
//        GNode* node = GetNodeByName ( objName );
//        if ( node )
//        {
//            node->DetachComponent ( componenttype );
//        }
//    }
//    break;
//    }
//    return true;
//}

CSceneMachine::CSceneMachine()
{
    mgsNowScene = gsNull;
    //ZeroMemory(&mgsContainer,sizeof(mgsContainer));
}

eGameScene CSceneMachine::GetNowScene()
{
    //return mgsContainer[0];
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

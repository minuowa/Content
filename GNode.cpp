#include "GGameDemoHeader.h"
#include "GNode.h"
#include "GMeshBuffer.h"
#include "GBound.h"
#include "GTimer.h"
#include "GGame.h"
#include "Content.h"

void GNode::getInput ( DWORD frameTimeMs )
{
    if ( 0 )
    {
        /*************************************************************
        控制规则：

        1、跳跃之后只能转动方向，不可MoveStep

        *************************************************************/
        //bool bMoveFront =  Content::InputSystem.IsPressKey ( DIK_W );				//向前移动
        //bool bMoveBack =  Content::InputSystem.IsPressKey ( DIK_S );				//向后移动
        //bool bTrunLeft =  Content::InputSystem.IsPressKey ( DIK_A );				//左转
        //bool bTrunRight =  Content::InputSystem.IsPressKey ( DIK_D );				//右转
        //bool bJump =  Content::InputSystem.IsPressKey ( DIK_SPACE );				//跳跃
        ///*************************************************************/


        //if ( bMoveFront )
        //{
        //    m_bBehaviour = true;

        //    getTrans()->MoveStep ( frameTimeMs );

        //    getTrans()->mBack = false;

        //    SetState ( oasMoving, false );

        //}
        //else if ( bMoveBack )
        //{
        //    m_bBehaviour = true;

        //    getTrans()->MoveStep ( -frameTimeMs );

        //    getTrans()->mBack = true;

        //    SetState ( oasMoving, true );
        //}


        //if ( bTrunLeft )
        //{
        //    getTrans()->TrunStepLeftRight ( -frameTimeMs / 3.0f );
        //}

        //else if ( bTrunRight )
        //{
        //    getTrans()->TrunStepLeftRight ( frameTimeMs / 3.0f );
        //}

        //if ( bJump )
        //{
        //    if ( !getTrans()->mJump )
        //    {
        //        m_bBehaviour = true;

        //        if ( bMoveBack )
        //        {
        //            getTrans()->mSpeed = -getTrans()->getDir() * getTrans()->mSpeedMove;
        //        }
        //        if ( bMoveFront )
        //        {
        //            getTrans()->mSpeed = getTrans()->getDir() * getTrans()->mSpeedMove;
        //        }
        //    }

        //    getTrans()->Jump();
        //}

        //if (  Content::InputSystem.IsPressKey ( DIK_1 ) )
        //{
        //    SetState ( oasBeAttack, false );
        //}

        //if (  Content::InputSystem.getKeyAction ( DIK_2 ) == DI_BUTTONUP )
        //{
        //    SetState ( oasAttack, false );
        //}
        //else if (  Content::InputSystem.getKeyAction ( DIK_3 ) == DI_BUTTONUP )
        //{
        //    SetState ( oasRunAttack, false );
        //}

        //if ( ! ( bMoveFront || bMoveBack || bTrunRight || bTrunLeft || bJump ||  Content::InputSystem.IsPressKey ( DIK_1 ) ) )
        //{

        //    SetState ( oasStandBy, false );
        //}
    }

}

void GNode::updateWorld()
{
    if ( mParent )
        getTrans()->updateWorld ( mParent->getTrans()->getWorld() );

for ( auto c : mChildren )
    {
        c->updateWorld();
    }

    //if ( mOpt == optByPosition )
    //{
    //    getTrans()->GetWorldMatrix ( _matWorld, bForTrans ) ;
    //    D3DXMATRIX parentMat;
    //    ( ( GNode* ) ( mParent ) )->getTrans()->GetWorldMatrix ( parentMat, true ) ;
    //    _matWorld *= parentMat;
    //}
    //else
    //{
    //    getTrans()->GetWorldMatrix ( _matWorld, bForTrans ) ;
    //    D3DXMATRIX parentMat = ( ( GNode* ) ( mParent ) )->GetWorldMatrixByBone ( mParentName, false ) ;
    //    _matWorld *= parentMat;

    //    //_matWorld=mXPos.GetScaleMatrix(false)*mXPos.GetTransLation(false)*
    //    //	(((CXWorldObj*)(mParent))->GetWorldMatrixByBone(msParentName,false))*mXPos.GetRotationMatrix(false);

    //}

}

GNode::GNode()
    : mNodeState ()
    , mLocalID ( 0 )
{
	mNodeState.setAll(true);

    mLocalID = getObjIDManager().addObj ( this );

    mParent = nullptr;
    m_fBoundRadius = 0.01f;

    m_ForceType = ftUpWithMap;

    m_bForceOnMap = true;

    m_bBehaviour = false;

    m_bBeSelected = false;

    mCanSelect = true;										//是否可选择

    m_bBlock = false;

    //m_vBlockPoint=ZEROVECTOR3;

    m_fBlockHeight = ZEROFLOAT;

    m_fBlockArea = ZEROFLOAT;

    m_bUseMatrialColor = false;

    mCulledByCamera = true;
    ZeroMemory ( &m_InsectInfo, sizeof ( m_InsectInfo ) );			//碰撞信息
    attachComponent ( eComponentType_Trans, false, false );
}

GNode::~GNode()
{
    getObjIDManager().removeObj ( mLocalID );
    dSafeDeleteVector ( mChildren );
}



D3DXMATRIX GNode::GetWorldMatrixByBone ( const char *sBoneName, bool bForTrans /*= false */ )
{
    CXASSERT ( 0 )
    //return updateWorld ( false );
    return D3DXMATRIX();
}



IntersectInfo * GNode::GetBlockPoint()
{
    IntersectInfo *HitInfo = NULL;

    //CMeshBufferNodeArr& mbnList=GMeshManager::getSingleton().GetList();
    //CMeshBufferNodeArr::iterator pEnd = mbnList.end();
    //for (CMeshBufferNodeArr::iterator i = mbnList.begin();i!=pEnd;++i)
    //{
    //	GMeshBufferNode *pMeshBufferNode =*i;
    //	CXMeshBaseObjList& objList = pMeshBufferNode->GetObjList();
    //	for (CXMeshBaseObjList::iterator obj=objList.begin();obj!=objList.end();++obj)
    //	{
    //		CXMeshBaseObj *pObj=*obj;
    //		float fHitDist=1000.0f;
    //		//返回值
    //		if (this->GetObjID()==pObj->GetObjID())
    //		{
    //			continue;
    //		}

    //		D3DXVECTOR3 vecDir=pObj->mXPos.mvPos-mXPos.mvLastPos;

    //		const float fLen=D3DXVec3Length(&vecDir);

    //		//如果两者距离大于两个包围球的半径之和与当前的位移，就不用考虑与该对象的碰撞

    //		float fPassLen=D3DXVec3Length(&(mXPos.mvLastPos-mXPos.mvPos));

    //		if (fLen>m_fBoundRadius+pObj->m_fBoundRadius+fPassLen)
    //		{
    //			continue;
    //		}
    //		D3DXVec3Normalize(&mXPos.mvDir,&mXPos.mvDir);

    //		D3DXVECTOR4 vPos(mXPos.mvLastPos.x,mXPos.mvLastPos.y,mXPos.mvLastPos.z,1);
    //		D3DXVECTOR4 vDir;

    //		vDir.x=mXPos.mvPos.x-mXPos.mvLastPos.x;
    //		vDir.y=mXPos.mvPos.y-mXPos.mvLastPos.y;
    //		vDir.z=mXPos.mvPos.z-mXPos.mvLastPos.z;
    //		vDir.w=0;
    //		D3DXVec3Normalize((D3DXVECTOR3*)&vDir,(D3DXVECTOR3*)&vDir);

    //		//碰撞的起点，物体的前边缘
    //		vPos+=vDir*m_fBoundRadius;

    //		bool bHit=pObj->CheckIntersect(vPos,vDir,true);

    //		if (bHit)
    //		{
    //			float fDist=pObj->m_InsectInfo.fDist;

    //			//碰撞的终点，物体的前边缘
    //			D3DXVECTOR3 vEndPos=mXPos.mvPos+(D3DXVECTOR3)vDir*m_fBoundRadius;

    //			float fDist2=D3DXVec3Length(&(vEndPos-(D3DXVECTOR3)vPos));

    //			if (fDist<=fDist2)
    //			{
    //				if (fHitDist>fDist)
    //				{
    //					HitInfo=new IntersectInfo;

    //					fHitDist=fDist;

    //					HitInfo->fDist=pObj->m_InsectInfo.fDist;

    //					HitInfo->vNormal=pObj->m_InsectInfo.vNormal;

    //					HitInfo->vHitPos=pObj->m_InsectInfo.vHitPos;

    //					HitInfo->pObj=pObj;
    //				}
    //			}
    //		}
    //	}

    //}
    return HitInfo;
}

void GNode::update(  )
{
    getTrans()->update();

for ( auto child: mChildren )
    {
        child->update();
    }
}

void GNode::setDir ( D3DXVECTOR3 vNormal )
{
    if ( m_ForceType == ftUpAlways )
    {
        getTrans()->SetDirWithUpon ( D3DXVECTOR3 ( 0, 1, 0 ) );
    }

    if ( m_ForceType == ftUpWithMap )
    {
        getTrans()->SetDirWithUpon ( vNormal );
    }
}

GNode* GNode::addChild ( GNode* c )
{
    CXASSERT_RETURN_FALSE ( c );
    mChildren.push_back ( c );
    c->mParent = this;

    GNodeAddArgs args;
    args.mAddNode = c->getName();
    args.mAddNodeParent = this->getName();
    mDelegateAddObj.trigger ( &args );

    return c;
}

bool GNode::draw()
{
    beginRender();
    render();
    endRender();
    return true;
}

bool GNode::removeChild ( GNode* child )
{
    GNodeArr::iterator iBegin = mChildren.begin();
    GNodeArr::iterator iEnd = mChildren.end();
    for ( ; iBegin != iEnd; ++iBegin )
    {
        GNode* n = *iBegin;
        if ( n == child )
        {
            mChildren.erase ( iBegin );
            return true;
        }
        if ( n->removeChild ( child ) )
            return true;
    }
    return false;
}

void GNode::beginRender()
{
    getTrans()->set();
}

void GNode::endRender()
{
    GNodeArr::iterator iBegin = mChildren.begin();
    GNodeArr::iterator iEnd = mChildren.end();
    for ( ; iBegin != iEnd; ++iBegin )
    {
        GNode* n = *iBegin;
        n->draw();
    }
}
bool GNode::recreate()
{
    this->clear();
    return true;
}

void GNode::registerAllProperty()
{
    __super::registerAllProperty();

    for ( int i = 0; i < eComponentType_Count; ++i )
    {
        GComponentInterface* component = mComponentOwner.getComponent ( eComponentType ( i ) );
        if ( component )
            registerProperty ( component );
    }
}
#define XML_OBJ_NODE_NULL	""
#define XML_OBJ_NODE_NAME		"Object"
#define XML_OBJ_NODE_TYPE		"Type"
#define XML_OBJ_NODE_PARENT		"Parent"
#define XML_OBJ_NODE_PROP		"Property"
#define XML_OBJ_NODE_PROP_NAME	"Name"
#define XML_OBJ_NODE_PROP_VALUE "Value"
#define XML_OBJ_NODE_CATEGORY	"Category"

void GNode::linkTo ( CXRapidxmlNode* parent )
{
    CXASSERT_RETURN ( parent );
    registerAll();
    //<Object Type="GNode" Name="" ParentName="">
    CXRapidxmlNode* me = parent->document()->allocate_node ( rapidxml::node_element );
    me->name ( XML_OBJ_NODE_NAME );
    CXRapidxmlAttr* attrType = parent->document()->allocate_attribute ( XML_OBJ_NODE_TYPE );
    CXRapidxmlAttr* attrParent = parent->document()->allocate_attribute ( XML_OBJ_NODE_PARENT );
    attrType->value ( categoryName() );
    attrParent->value ( mParent ? mParent->getName() : XML_OBJ_NODE_NULL );
    me->append_attribute ( attrType );
    me->append_attribute ( attrParent );

    parent->append_node ( me );

    const CategoryPropertyMap& otherPropMap = getPropertyMap();
    CategoryPropertyMap::const_iterator ibegin = otherPropMap.begin();
    CategoryPropertyMap::const_iterator iend = otherPropMap.end();
    for ( ; ibegin != iend; ++ibegin )
    {
        CXRapidxmlNode* category = parent->document()->allocate_node ( rapidxml::node_element );
        category->name ( XML_OBJ_NODE_CATEGORY );
        CXRapidxmlAttr* categoryName = parent->document()->allocate_attribute ( XML_OBJ_NODE_TYPE );
        categoryName->value ( ibegin->first.c_str() );
        category->append_attribute ( categoryName );
        me->append_node ( category );

        std::string spropvalue;
        const PropertyMap* propMap = ibegin->second;
        for ( PropertyMap::const_iterator walk = propMap->begin();
                walk != propMap->end(); ++walk )
        {
            EPropertyVar* evar = walk->second;
            evar->ToString ( spropvalue );
            CXRapidxmlNode* prop = parent->document()->allocate_node ( rapidxml::node_element );
            prop->name ( XML_OBJ_NODE_PROP );
            CXRapidxmlAttr* propName = parent->document()->allocate_attribute ( XML_OBJ_NODE_PROP_NAME );
            CXRapidxmlAttr* propValue = parent->document()->allocate_attribute ( XML_OBJ_NODE_PROP_VALUE );
            propName->value ( parent->document()->allocate_string ( walk->first.c_str() ) );
            propValue->value ( parent->document()->allocate_string ( spropvalue.c_str() ) );
            prop->append_attribute ( propName );
            prop->append_attribute ( propValue );

            category->append_node ( prop );
        }
    }

for ( auto i: mChildren )
        i->linkTo ( parent );
}

void GNode::makeXMLNode ( CXRapidxmlNode& node )
{
    node.name ( "Object" );
    CXRapidxmlAttr* attr = node.document()->allocate_attribute ( "Type", this->categoryName() );
    node.append_attribute ( attr );
    assert ( 0 && "undo work" );
    //CategoryPropertyMap::iterator it ( mOption.begin() );
    //CategoryPropertyMap::iterator iEnd ( mOption.end() );
    //StdString stemp;
    //for ( ; it != iEnd; ++it )
    //{
    //    CXRapidxmlNode* child = node.document()->allocate_node ( rapidxml::node_element );
    //    child->name ( it->first.c_str() );
    //    it->second->ToString ( stemp );
    //    child->value ( node.document()->allocate_string ( stemp.c_str() ) );
    //    node.append_node ( child );
    //}
}



bool GNode::render()
{
    return true;
}

GNode* GNode::getNodeByName ( const char* name )
{
    if ( mName == name )
        return this;

    GNode* target = 0;
    GNodeArr::iterator iBegin = mChildren.begin();
    GNodeArr::iterator iEnd = mChildren.end();
    for ( ; iBegin != iEnd; ++iBegin )
    {
        GNode* child = *iBegin;
        target = child->getNodeByName ( name );
        if ( target )
            return target;
    }
    return target;
}



void GNode::updateTrans()
{

    //const bool bOriginAutoMove = getTrans()->mAutoMove;


    //eHitType htMap = htNull;

    //eHitType htObj = htNull;

    //IntersectInfo *pIntersectObj = NULL;

    //IntersectInfo *pIntersectMapInfo = NULL;		//与地图的碰撞点

    //IntersectInfo *pIntersectFinalHit = NULL;		//最终碰撞信息（可能是地图或者是物体）

    //if ( !m_bForceOnMap || !m_bBeSelected || !mCanSelect )
    //{
    //    return;
    //}

    //if ( !m_bBehaviour )
    //{
    //    return;
    //}

    //if ( bOriginAutoMove )
    //{
    //    getTrans()->update();
    //}

    //const GComponentTrans cXPos = getTrans();
    ///*************************************************************************************************
    ////如果是在自动移动就在mXPos.Update(fPass)中更新Pos和LastPos：
    ////否则就在GetInput（fPass）中更新Pos和LastPos:

    //1、与地图碰撞
    //**************************************************************************************************/
    //if ( bOriginAutoMove )
    //{

    //    GGameMap *pMap = ( GGameMap* ) mForceMap;

    //    D3DXVECTOR4 vDir;
    //    vDir.x = getTrans()->getTranslate().x - getTrans()->mvLastPos.x;
    //    vDir.y = getTrans()->getTranslate().y - getTrans()->mvLastPos.y;
    //    vDir.z = getTrans()->getTranslate().z - getTrans()->mvLastPos.z;
    //    vDir.w = 0;

    //    D3DXVECTOR4 vPos ( getTrans()->mvLastPos.x, getTrans()->mvLastPos.y, getTrans()->mvLastPos.z, 1 );

    //    bool bHit = pMap->checkIntersect ( vPos, vDir, true );

    //    if ( bHit )
    //    {
    //        float fHitDist = D3DXVec3Length ( & ( pMap->m_InsectInfo.vHitPos - getTrans()->mvLastPos ) );

    //        float fDist = D3DXVec3Length ( & ( getTrans()->mMatLocal.mTranslate - getTrans()->mvLastPos ) );

    //        if ( fHitDist > fDist )
    //        {
    //            htMap = htAutoMoveHitNoMap;
    //        }
    //        else
    //        {
    //            htMap = htAutoMoveHitMap;

    //            //自动移动结束：
    //            getTrans()->mJump = false;

    //            getTrans()->mBack = false;

    //            getTrans()->mAutoMove = false;

    //            getTrans()->mCanMoveStep = true;

    //            getTrans()->mSpeed = ZEROVECTOR3;

    //            pIntersectMapInfo = & ( pMap->m_InsectInfo );
    //        }
    //    }
    //}
    //else
    //{

    //    if ( m_pOnObj != NULL )
    //    {
    //        pIntersectObj = UpdateForForceOnObj ( m_pOnObj );
    //    }

    //    getTrans() = cXPos;

    //    pIntersectMapInfo = UpdateForForceOnMap();

    //    if ( pIntersectObj != NULL )
    //    {
    //        if ( pIntersectMapInfo->vHitPos.y > pIntersectObj->vHitPos.y )
    //        {
    //            m_pOnObj = NULL;
    //        }
    //        else
    //        {
    //            getTrans()->setTranslate(pIntersectObj->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 )) ;

    //            float fTmpAng = D3DXVec3Dot ( &pIntersectObj->vNormal, &D3DXVECTOR3 ( 0, 1, 0 ) );
    //            if ( 0 < fTmpAng && fTmpAng < 0.5f )
    //            {
    //                getTrans()->getDir() = cXPos.getDir();

    //                D3DXVECTOR3 vDirTmp = getTrans()->mMatLocal.mUpon + pIntersectObj->vNormal;
    //                setDir ( vDirTmp / 2.0f );
    //            }
    //            else if ( fTmpAng >= 0.5f )
    //            {
    //                getTrans()->getDir() = cXPos.getDir();
    //                setDir ( pIntersectObj->vNormal );
    //            }
    //            else
    //            {
    //                getTrans()->setTranslate(cXPos.mvLastPos) ;

    //                UpdateForForceOnObj ( m_pOnObj );
    //            }
    //        }

    //        //}
    //    }
    //    else
    //    {
    //        m_pOnObj = NULL;

    //        IntersectInfo *pIntersect = GetBlockPoint();

    //        if ( pIntersect != NULL )
    //        {
    //            float fAng = D3DXVec3Dot ( &pIntersect->vNormal, &D3DXVECTOR3 ( 0, 1, 0 ) );

    //            if ( fAng < 0.7 )
    //            {
    //	getTrans()->setTranslate(cXPos.mvLastPos) ;
    //	UpdateForForceOnMap();
    //            }
    //            else
    //            {
    //                m_pOnObj = pIntersect->pObj;
    //                getTrans()->getDir() = cXPos.getDir();

    //	getTrans()->setTranslate(pIntersect->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 )) ;

    //                setDir ( pIntersect->vNormal );
    //            }


    //        }

    //    }

    //    return;
    //}

    ///*************************************************************************************************
    //2、与地图中场景物体碰撞
    //**************************************************************************************************/
    //getTrans() = cXPos;

    //IntersectInfo *pInterBlock = GetBlockPoint();
    ///*************************************************************************************************
    //3、判断碰撞点
    //**************************************************************************************************/

    //switch ( htMap )
    //{

    //case htAutoMoveHitNoMap:

    //    if ( pInterBlock != NULL )
    //    {
    //        m_pOnObj = pInterBlock->pObj;

    //        pIntersectFinalHit = pInterBlock;
    //    }

    //    break;

    //case htAutoMoveHitMap:

    //    if ( pInterBlock != NULL )
    //    {
    //        if ( pIntersectMapInfo->vHitPos.y < pInterBlock->vHitPos.y )
    //        {
    //            m_pOnObj = pInterBlock->pObj;

    //            pIntersectFinalHit = pInterBlock;
    //        }
    //        else
    //        {
    //            m_pOnObj = NULL;

    //            pIntersectFinalHit = pIntersectMapInfo;
    //        }
    //    }
    //    else
    //    {
    //        pIntersectFinalHit = pIntersectMapInfo;
    //    }

    //    break;

    //case htNotAutoMoveHitMap:

    //    if ( pInterBlock != NULL )
    //    {
    //        D3DXVec3Normalize ( &pInterBlock->vNormal, &pInterBlock->vNormal );

    //        if ( D3DXVec3Dot ( & ( pInterBlock->vNormal ), & ( D3DXVECTOR3 ( 0, 1, 0 ) ) ) < 0.5 )
    //        {
    //            //保持在原地，不能前进
    //            getTrans()->mMatLocal.mTranslate = getTrans()->mvLastPos;

    //            UpdateForForceOnMap();
    //        }
    //        else
    //        {
    //            m_pOnObj = pInterBlock->pObj;

    //            getTrans()->mMatLocal.mTranslate = pInterBlock->vHitPos;

    //            UpdateForForceOnObj ( m_pOnObj );
    //        }

    //    }

    //    break;

    //case htNotAutoMoveHitBelowMap:

    //    if ( pInterBlock != NULL )
    //    {
    //        D3DXVec3Normalize ( &pInterBlock->vNormal, &pInterBlock->vNormal );

    //        if ( D3DXVec3Dot ( & ( pInterBlock->vNormal ), & ( D3DXVECTOR3 ( 0, 1, 0 ) ) ) < 0.5 )
    //        {
    //            //保持在原地，不能前进
    //            getTrans()->mMatLocal.mTranslate = getTrans()->mvLastPos;

    //            UpdateForForceOnObj ( m_pOnObj );
    //        }
    //        else
    //        {
    //            m_pOnObj = pInterBlock->pObj;

    //            getTrans()->mMatLocal.mTranslate = pInterBlock->vHitPos;

    //            UpdateForForceOnObj ( m_pOnObj );
    //        }

    //    }

    //    break;

    //case htNotAutoMoveHitNoBelowMap:

    //    //if (pInterBlock!=NULL)
    //    //{
    //    //	D3DXVec3Normalize(&pInterBlock->vNormal,&pInterBlock->vNormal);

    //    //	if (D3DXVec3Dot(&(pInterBlock->vNormal),&(D3DXVECTOR3(0,1,0))) <0.5)
    //    //	{
    //    //		//保持在原地，不能前进
    //    //		mXPos.mvPos=mXPos.mvLastPos;

    //    //		UpdateForForceOnObj(m_pOnObj);
    //    //	}
    //    //	else
    //    //	{
    //    //		m_pOnObj=pInterBlock->pObj;

    //    //		mXPos.mvPos=pInterBlock->vHitPos;

    //    //		UpdateForForceOnObj(m_pOnObj);
    //    //	}
    //    //}
    //    //else
    //    //{
    //    //	if (pIntersectMapInfo!=NULL)
    //    //	{
    //    //		SetDir(pIntersectMapInfo->vNormal);

    //    //		mXPos.mvPos=pIntersectMapInfo->vHitPos;

    //    //		mXPos.mvPos.y=pIntersectMapInfo->vHitPos.y+m_fForceHeight;
    //    //	}

    //    //}

    //    break;


    //}

    //if ( pIntersectFinalHit != NULL )
    //{
    //    getTrans()->mMatLocal.mTranslate = pIntersectFinalHit->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 );

    //    m_bBehaviour = false;

    //    //自动移动结束：
    //    getTrans()->mJump = false;

    //    getTrans()->mBack = false;

    //    getTrans()->mAutoMove = false;

    //    getTrans()->mCanMoveStep = true;

    //    getTrans()->mSpeed = ZEROVECTOR3;

    //    setDir ( pIntersectFinalHit->vNormal );
    //}
}

void GNode::onComponentChange ( GComponentInterface* component,  bool canDetach , bool notifyEditor )
{
    if ( component )
    {
        component->setOwner ( this );
        component->setCanDetach ( canDetach );
    }
    if ( notifyEditor )
    {
        registerAll();
        GNodeComponentChangeArgs args;
        args.mChangedNode = this->getName();
        mDelegateComponentChange.trigger ( &args );
    }
}

void GNode::onPropertyChange ( void* pre, void* changed )
{
    __super::onPropertyChange ( pre, changed );

    for ( int i = 0; i < eComponentType_Count; ++i )
    {
        if ( mComponentOwner.getComponent ( eComponentType ( i ) ) )
        {
            mComponentOwner.getComponent ( eComponentType ( i ) )->onPropertyChange ( pre, changed );
        }
    }
}


void GNode::clear()
{

}

//void GNode::setWorldTranslate ( D3DXVECTOR3& v )
//{
//    getTrans()->setWorldTranslate ( v );
//}

void GNode::onPropertyChangeEnd ( void* cur )
{
    __super::onPropertyChangeEnd ( cur );

    for ( int i = 0; i < eComponentType_Count; ++i )
    {
        if ( mComponentOwner.getComponent ( eComponentType ( i ) ) )
        {
            mComponentOwner.getComponent ( eComponentType ( i ) )->onPropertyChangeEnd ( cur );
        }
    }
}



GNode* GNode::getFirstNodeByCategoryName ( const char* category )
{
    if ( dStrEqual ( this->categoryName(), category ) )
        return this;

    GNode* res = 0;
for ( auto child: mChildren )
    {
        res = child->getFirstNodeByCategoryName ( category );
        if ( res )
            return res;
    }
    return nullptr;
}

void GNode::deleteChild ( GNode* node )
{
    if ( node == nullptr )
        return;

    removeChild ( node );

    GNodeDeleteArgs args;
    args.mDeleteNode = node->getName();
    mDelegateDeleteObj.trigger ( &args );
    dSafeDelete ( node );
}



void GNode::setCanGetInput ( bool can )
{
    mNodeState.setBit ( eObjState_GetInput, can );
    if ( can )
         Content::InputEntityMgr.addInputObj ( this );
    else
         Content::InputEntityMgr.remove ( this->getLocalID() );
}


CXDynaArray<GNode*>& GNode::getChildren()
{
    return mChildren;
}

void GNode::setState ( eObjState state, bool b )
{
    mNodeState.setBit ( state, b );
}

bool GNode::isState ( eObjState state ) const
{
    return mNodeState[state];
}

CXIDObjectManager<GNode>& GNode::getObjIDManager()
{
	static CXIDObjectManager<GNode> objectIDManager;
	return objectIDManager;
}

CXDelegate GNode::mDelegateDeleteObj;

CXDelegate GNode::mDelegateComponentChange;

CXDelegate GNode::mDelegateAddObj;


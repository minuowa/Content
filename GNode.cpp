#include "GGameDemoHeader.h"
#include "GNode.h"
#include "GGameMap.h"
#include "GMeshBuffer.h"
#include "GBound.h"
#include "GTimer.h"
#include "GGame.h"

void GNode::getInput ( DWORD frameTimeMs )
{
	if ( mCanGetInput )
	{
		/*************************************************************
		���ƹ���

		1����Ծ֮��ֻ��ת�����򣬲���MoveStep

		*************************************************************/
		bool bMoveFront = INPUTSYSTEM.IsPressKey ( DIK_W );				//��ǰ�ƶ�
		bool bMoveBack = INPUTSYSTEM.IsPressKey ( DIK_S );				//����ƶ�
		bool bTrunLeft = INPUTSYSTEM.IsPressKey ( DIK_A );				//��ת
		bool bTrunRight = INPUTSYSTEM.IsPressKey ( DIK_D );				//��ת
		bool bJump = INPUTSYSTEM.IsPressKey ( DIK_SPACE );				//��Ծ
		/*************************************************************/


		if ( bMoveFront )
		{
			m_bBehaviour = true;

			getTrans().MoveStep ( frameTimeMs );

			getTrans().mBack = false;

			SetState ( oasMoving, false );

		}
		else if ( bMoveBack )
		{
			m_bBehaviour = true;

			getTrans().MoveStep ( -frameTimeMs );

			getTrans().mBack = true;

			SetState ( oasMoving, true );
		}


		if ( bTrunLeft )
		{
			getTrans().TrunStepLeftRight ( -frameTimeMs / 3.0f );
		}

		else if ( bTrunRight )
		{
			getTrans().TrunStepLeftRight ( frameTimeMs / 3.0f );
		}

		if ( bJump )
		{
			if ( !getTrans().mJump )
			{
				m_bBehaviour = true;

				if ( bMoveBack )
				{
					getTrans().mSpeed = -getTrans().mDir * getTrans().mSpeedMove;
				}
				if ( bMoveFront )
				{
					getTrans().mSpeed = getTrans().mDir * getTrans().mSpeedMove;
				}
			}

			getTrans().Jump();
		}

		if ( INPUTSYSTEM.IsPressKey ( DIK_1 ) )
		{
			SetState ( oasBeAttack, false );
		}

		if ( INPUTSYSTEM.getKeyAction ( DIK_2 ) == DI_BUTTONUP )
		{
			SetState ( oasAttack, false );
		}
		else if ( INPUTSYSTEM.getKeyAction ( DIK_3 ) == DI_BUTTONUP )
		{
			SetState ( oasRunAttack, false );
		}

		if ( ! ( bMoveFront || bMoveBack || bTrunRight || bTrunLeft || bJump || INPUTSYSTEM.IsPressKey ( DIK_1 ) ) )
		{

			SetState ( oasStandBy, false );
		}
	}
	GNodeArr::iterator iBegin = mChildren.begin();
	GNodeArr::iterator iEnd = mChildren.end();
	for ( ; iBegin != iEnd; ++iBegin )
	{
		GNode* n = *iBegin;
		n->getInput ( frameTimeMs );
	}
}

D3DXMATRIX GNode::GetWorldMatrix ( bool bForTrans )
{
	//�����������
	//D3DXMATRIX matMultiWorldMatix;

	if ( mParent == NULL )
	{
		_matWorld = getTrans().GetWorldMatrix ( bForTrans );
	}
	else
	{
		if ( mOpt == optByPosition )
		{
			_matWorld = ( getTrans().GetWorldMatrix ( bForTrans ) ) * ( ( ( GNode* ) ( mParent ) )->getTrans().GetWorldMatrix ( true ) );
		}
		else
		{
			_matWorld = ( getTrans().GetWorldMatrix ( false ) ) * ( ( ( GNode* ) ( mParent ) )->GetWorldMatrixByBone ( mParentName, false ) );

			//_matWorld=mXPos.GetScaleMatrix(false)*mXPos.GetTransLation(false)*
			//	(((CXWorldObj*)(mParent))->GetWorldMatrixByBone(msParentName,false))*mXPos.GetRotationMatrix(false);

		}

	}

	D9DEVICE->GetDvc()->SetTransform ( D3DTS_WORLD, &_matWorld );

	return _matWorld;
}

GNode::GNode()
	: mCanGetInput ( false )
{
	mParent = nullptr;

	m_pOnObj = nullptr;

	m_fBoundRadius = 0.01f;

	mForceMap = NULL;										//�����ĵ�ͼ��ָ��

	m_fForceHeight = Default_Force_Height;

	m_ForceType = ftUpWithMap;

	m_bForceOnMap = true;

	m_bBehaviour = false;

	m_bBeSelected = false;

	mCanSelect = true;										//�Ƿ��ѡ��

	m_bBlock = false;

	//m_vBlockPoint=ZEROVECTOR3;

	m_fBlockHeight = ZEROFLOAT;

	m_fBlockArea = ZEROFLOAT;

	mOpt = optByPosition;

	m_bUseMatrialColor = false;

	m_bHit = false;


	m_bEyeCliper = true;


	ZeroMemory ( &m_InsectInfo, sizeof ( m_InsectInfo ) );			//��ײ��Ϣ


	m_ObjAnimState = oasNULL;									//����ǰ״̬

	D3DXMatrixIdentity ( &_matWorld );							//������������

	//if ( TheEditor )
	//{
	//    AddWatcher ( TheEditor );
	//    AddWatcher ( TheSceneMgr );
	//}

	attachComponent ( eComponentType_Trans, false, false );
}

GNode::~GNode()
{
	dSafeDeleteVector ( mChildren );
	//   if ( mParent )
	//{
	//	mParent->removeChild(this);
	//	mParent = nullptr;
	//}
}

eObjAnimState GNode::SetState ( eObjAnimState oas, bool bBack )
{
	m_ObjAnimState = oas;

	return m_ObjAnimState;
}



D3DXMATRIX GNode::GetWorldMatrixByBone ( const char *sBoneName, bool bForTrans /*= false */ )
{
	return GetWorldMatrix ( false );
}



IntersectInfo * GNode::GetBlockPoint()
{
	IntersectInfo *HitInfo = NULL;

	//CMeshBufferNodeArr& mbnList=GMeshManager::GetSingleton().GetList();
	//CMeshBufferNodeArr::iterator pEnd = mbnList.end();
	//for (CMeshBufferNodeArr::iterator i = mbnList.begin();i!=pEnd;++i)
	//{
	//	GMeshBufferNode *pMeshBufferNode =*i;
	//	CXMeshBaseObjList& objList = pMeshBufferNode->GetObjList();
	//	for (CXMeshBaseObjList::iterator obj=objList.begin();obj!=objList.end();++obj)
	//	{
	//		CXMeshBaseObj *pObj=*obj;
	//		float fHitDist=1000.0f;
	//		//����ֵ
	//		if (this->GetObjID()==pObj->GetObjID())
	//		{
	//			continue;
	//		}

	//		D3DXVECTOR3 vecDir=pObj->mXPos.mvPos-mXPos.mvLastPos;

	//		const float fLen=D3DXVec3Length(&vecDir);

	//		//������߾������������Χ��İ뾶֮���뵱ǰ��λ�ƣ��Ͳ��ÿ�����ö������ײ

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

	//		//��ײ����㣬�����ǰ��Ե
	//		vPos+=vDir*m_fBoundRadius;

	//		bool bHit=pObj->CheckIntersect(vPos,vDir,true);

	//		if (bHit)
	//		{
	//			float fDist=pObj->m_InsectInfo.fDist;

	//			//��ײ���յ㣬�����ǰ��Ե
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
	if ( mComponentOwner.getComponent ( eComponentType_Trans ) )
	{
		updateTrans();
	}

	for ( GNode * child: mChildren )
	{
		child->update();
	}
}



IntersectInfo * GNode::UpdateForForceOnMap()
{
	if ( m_bForceOnMap )
	{
		if ( mForceMap != NULL )
		{
			GGameMap *pMap = ( GGameMap* ) mForceMap;

			D3DXVECTOR4 vDir ( 0, -1, 0, 0 ), vPos ( getTrans().mTranslate.x, getTrans().mTranslate.y + 5000.0f, getTrans().mTranslate.z, 1 );

			//���µ��������ͼ��ײ���϶���ײ��

			bool bHit = pMap->checkIntersect ( vPos, vDir, true );

			if ( bHit )
			{

				setDir ( pMap->m_InsectInfo.vNormal );

				getTrans().mTranslate = pMap->m_InsectInfo.vHitPos;

				getTrans().mTranslate.y = pMap->m_InsectInfo.vHitPos.y + m_fForceHeight;

				return & ( pMap->m_InsectInfo );
			}

		}

	}

	return NULL;
}

void GNode::setDir ( D3DXVECTOR3 vNormal )
{
	if ( m_ForceType == ftUpAlways )
	{
		getTrans().SetDirWithUpon ( D3DXVECTOR3 ( 0, 1, 0 ) );
	}

	if ( m_ForceType == ftUpWithMap )
	{
		getTrans().SetDirWithUpon ( vNormal );
	}
}

IntersectInfo * GNode::UpdateForForceOnObj ( void *pObj )
{
	if ( pObj == NULL )
	{
		return NULL;
	}

	GMeshBaseObj *lpObj = ( GMeshBaseObj * ) pObj;

	D3DXVECTOR4 vDir ( 0, -1, 0, 0 ), vPos ( getTrans().mTranslate.x, getTrans().mTranslate.y + 20.0f, getTrans().mTranslate.z, 1 );

	//���µ��������ͼ��ײ���϶���ײ��

	bool bHit = lpObj->checkIntersect ( vPos, vDir, true );

	if ( bHit )
	{
		//D3DXVec3Normalize(&lpObj->m_InsectInfo.vNormal,&lpObj->m_InsectInfo.vNormal);

		//float fAngle=D3DXVec3Dot(&lpObj->m_InsectInfo.vNormal,&D3DXVECTOR3(0,1,0));

		//if (fAngle>0.1)
		//{
		setDir ( lpObj->m_InsectInfo.vNormal );

		getTrans().mTranslate = lpObj->m_InsectInfo.vHitPos;

		getTrans().mTranslate.y = lpObj->m_InsectInfo.vHitPos.y + m_fForceHeight;
		//}
		//else
		//{
		//	mXPos.mvPos=mXPos.mvLastPos;
		//}

		return & ( lpObj->m_InsectInfo );
	}

	return NULL;
}

GNode* GNode::addChild ( GNode* c )
{
	CXASSERT_RETURN_FALSE ( c );
	mChildren.push_back ( c );
	c->mParent = this;

	mOperatorParentObj = this;
	mOperatorObj = c;
	mDelegateAddObj.trigger();

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
	getTrans().set();
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


void GNode::setParentBone ( GNode *Parent, const char *sName )
{
	if ( sName == NULL || strlen ( sName ) == 0 || strlen ( sName ) > 31 )
	{
		return;
	}

	mOpt = optByName;

	mParent = Parent;
	mParentName = sName;

}

int GNode::getObjID() const
{
	return _nID;
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
	attrParent->value ( mParent ? mParent->getObjectName() : XML_OBJ_NODE_NULL );
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

	GNodeArr::iterator iBegin = mChildren.begin();
	GNodeArr::iterator iEnd = mChildren.end();
	for ( ; iBegin != iEnd; ++iBegin )
	{
		GNode* n = *iBegin;
		n->linkTo ( parent );
	}
}

void GNode::MakeXMLNode ( CXRapidxmlNode& node )
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

void GNode::setNodeName ( CChar* name )
{
	mNodeName = name;
}

bool GNode::render()
{
	return true;
}

GNode* GNode::getNodeByName ( const char* name )
{
	if ( mNodeName == name )
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

GComponentTrans& GNode::getTrans() const
{
	GComponentTrans* pTrans = ( GComponentTrans* ) mComponentOwner.getComponent ( eComponentType_Trans );
	assert ( pTrans );
	return *pTrans;
}

void GNode::updateTrans()
{
	getTrans().update();

	const bool bOriginAutoMove = getTrans().mAutoMove;


	eHitType htMap = htNull;

	eHitType htObj = htNull;

	IntersectInfo *pIntersectObj = NULL;

	IntersectInfo *pIntersectMapInfo = NULL;		//���ͼ����ײ��

	IntersectInfo *pIntersectFinalHit = NULL;		//������ײ��Ϣ�������ǵ�ͼ���������壩

	if ( !m_bForceOnMap || !m_bBeSelected || !mCanSelect )
	{
		return;
	}

	if ( !m_bBehaviour )
	{
		return;
	}

	if ( bOriginAutoMove )
	{
		getTrans().update();
	}

	const GComponentTrans cXPos = getTrans();
	/*************************************************************************************************
	//��������Զ��ƶ�����mXPos.Update(fPass)�и���Pos��LastPos��
	//�������GetInput��fPass���и���Pos��LastPos:

	1�����ͼ��ײ
	**************************************************************************************************/
	if ( bOriginAutoMove )
	{

		GGameMap *pMap = ( GGameMap* ) mForceMap;

		D3DXVECTOR4 vDir;
		vDir.x = getTrans().mTranslate.x - getTrans().mvLastPos.x;
		vDir.y = getTrans().mTranslate.y - getTrans().mvLastPos.y;
		vDir.z = getTrans().mTranslate.z - getTrans().mvLastPos.z;
		vDir.w = 0;

		D3DXVECTOR4 vPos ( getTrans().mvLastPos.x, getTrans().mvLastPos.y, getTrans().mvLastPos.z, 1 );

		bool bHit = pMap->checkIntersect ( vPos, vDir, true );

		if ( bHit )
		{
			float fHitDist = D3DXVec3Length ( & ( pMap->m_InsectInfo.vHitPos - getTrans().mvLastPos ) );

			float fDist = D3DXVec3Length ( & ( getTrans().mTranslate - getTrans().mvLastPos ) );

			if ( fHitDist > fDist )
			{
				htMap = htAutoMoveHitNoMap;
			}
			else
			{
				htMap = htAutoMoveHitMap;

				//�Զ��ƶ�������
				getTrans().mJump = false;

				getTrans().mBack = false;

				getTrans().mAutoMove = false;

				getTrans().mCanMoveStep = true;

				getTrans().mSpeed = ZEROVECTOR3;

				pIntersectMapInfo = & ( pMap->m_InsectInfo );
			}
		}
	}
	else
	{

		if ( m_pOnObj != NULL )
		{
			pIntersectObj = UpdateForForceOnObj ( m_pOnObj );
		}

		getTrans() = cXPos;

		pIntersectMapInfo = UpdateForForceOnMap();

		if ( pIntersectObj != NULL )
		{
			if ( pIntersectMapInfo->vHitPos.y > pIntersectObj->vHitPos.y )
			{
				m_pOnObj = NULL;
			}
			else
			{
				getTrans().mTranslate = pIntersectObj->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 );

				float fTmpAng = D3DXVec3Dot ( &pIntersectObj->vNormal, &D3DXVECTOR3 ( 0, 1, 0 ) );
				if ( 0 < fTmpAng && fTmpAng < 0.5f )
				{
					getTrans().mDir = cXPos.mDir;

					D3DXVECTOR3 vDirTmp = getTrans().mUpon + pIntersectObj->vNormal;
					setDir ( vDirTmp / 2.0f );
				}
				else if ( fTmpAng >= 0.5f )
				{
					getTrans().mDir = cXPos.mDir;
					setDir ( pIntersectObj->vNormal );
				}
				else
				{
					getTrans().mTranslate = cXPos.mvLastPos;

					UpdateForForceOnObj ( m_pOnObj );
				}
			}

			//}
		}
		else
		{
			m_pOnObj = NULL;

			IntersectInfo *pIntersect = GetBlockPoint();

			if ( pIntersect != NULL )
			{
				float fAng = D3DXVec3Dot ( &pIntersect->vNormal, &D3DXVECTOR3 ( 0, 1, 0 ) );

				if ( fAng < 0.7 )
				{
					getTrans().mTranslate = cXPos.mvLastPos;
					UpdateForForceOnMap();
				}
				else
				{
					m_pOnObj = pIntersect->pObj;
					getTrans().mDir = cXPos.mDir;

					getTrans().mTranslate = pIntersect->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 );

					setDir ( pIntersect->vNormal );
				}


			}

		}

		return;
	}

	/*************************************************************************************************
	2�����ͼ�г���������ײ
	**************************************************************************************************/
	getTrans() = cXPos;

	IntersectInfo *pInterBlock = GetBlockPoint();
	/*************************************************************************************************
	3���ж���ײ��
	**************************************************************************************************/

	switch ( htMap )
	{

	case htAutoMoveHitNoMap:

		if ( pInterBlock != NULL )
		{
			m_pOnObj = pInterBlock->pObj;

			pIntersectFinalHit = pInterBlock;
		}

		break;

	case htAutoMoveHitMap:

		if ( pInterBlock != NULL )
		{
			if ( pIntersectMapInfo->vHitPos.y < pInterBlock->vHitPos.y )
			{
				m_pOnObj = pInterBlock->pObj;

				pIntersectFinalHit = pInterBlock;
			}
			else
			{
				m_pOnObj = NULL;

				pIntersectFinalHit = pIntersectMapInfo;
			}
		}
		else
		{
			pIntersectFinalHit = pIntersectMapInfo;
		}

		break;

	case htNotAutoMoveHitMap:

		if ( pInterBlock != NULL )
		{
			D3DXVec3Normalize ( &pInterBlock->vNormal, &pInterBlock->vNormal );

			if ( D3DXVec3Dot ( & ( pInterBlock->vNormal ), & ( D3DXVECTOR3 ( 0, 1, 0 ) ) ) < 0.5 )
			{
				//������ԭ�أ�����ǰ��
				getTrans().mTranslate = getTrans().mvLastPos;

				UpdateForForceOnMap();
			}
			else
			{
				m_pOnObj = pInterBlock->pObj;

				getTrans().mTranslate = pInterBlock->vHitPos;

				UpdateForForceOnObj ( m_pOnObj );
			}

		}

		break;

	case htNotAutoMoveHitBelowMap:

		if ( pInterBlock != NULL )
		{
			D3DXVec3Normalize ( &pInterBlock->vNormal, &pInterBlock->vNormal );

			if ( D3DXVec3Dot ( & ( pInterBlock->vNormal ), & ( D3DXVECTOR3 ( 0, 1, 0 ) ) ) < 0.5 )
			{
				//������ԭ�أ�����ǰ��
				getTrans().mTranslate = getTrans().mvLastPos;

				UpdateForForceOnObj ( m_pOnObj );
			}
			else
			{
				m_pOnObj = pInterBlock->pObj;

				getTrans().mTranslate = pInterBlock->vHitPos;

				UpdateForForceOnObj ( m_pOnObj );
			}

		}

		break;

	case htNotAutoMoveHitNoBelowMap:

		//if (pInterBlock!=NULL)
		//{
		//	D3DXVec3Normalize(&pInterBlock->vNormal,&pInterBlock->vNormal);

		//	if (D3DXVec3Dot(&(pInterBlock->vNormal),&(D3DXVECTOR3(0,1,0))) <0.5)
		//	{
		//		//������ԭ�أ�����ǰ��
		//		mXPos.mvPos=mXPos.mvLastPos;

		//		UpdateForForceOnObj(m_pOnObj);
		//	}
		//	else
		//	{
		//		m_pOnObj=pInterBlock->pObj;

		//		mXPos.mvPos=pInterBlock->vHitPos;

		//		UpdateForForceOnObj(m_pOnObj);
		//	}
		//}
		//else
		//{
		//	if (pIntersectMapInfo!=NULL)
		//	{
		//		SetDir(pIntersectMapInfo->vNormal);

		//		mXPos.mvPos=pIntersectMapInfo->vHitPos;

		//		mXPos.mvPos.y=pIntersectMapInfo->vHitPos.y+m_fForceHeight;
		//	}

		//}

		break;


	}

	if ( pIntersectFinalHit != NULL )
	{
		getTrans().mTranslate = pIntersectFinalHit->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 );

		m_bBehaviour = false;

		//�Զ��ƶ�������
		getTrans().mJump = false;

		getTrans().mBack = false;

		getTrans().mAutoMove = false;

		getTrans().mCanMoveStep = true;

		getTrans().mSpeed = ZEROVECTOR3;

		setDir ( pIntersectFinalHit->vNormal );
	}
}

void GNode::onComponentChange ( GComponentInterface* component,  bool canDetach , bool notifyEditor )
{
	if ( component )
	{
		component->SetTarget ( this );
		component->setCanDetach ( canDetach );
	}
	if ( notifyEditor )
	{
		registerAll();
		mOperatorObj = this;
		mDelegateComponentChange.trigger();
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

void GNode::setWorldTranslate ( D3DXVECTOR3& v )
{
	getTrans().mTranslate = v;
}

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

GNode* GNode::getParent() const
{
	return mParent;
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
	removeChild ( node );
	mOperatorObj = node;
	mDelegateDeleteObj.trigger();
	dSafeDelete ( node );
}

CXDynaArray<GNode*>& GNode::getChildren()
{
	return mChildren;
}

void GNode::setCanGetInput ( bool can )
{
	mCanGetInput = can;
}

CXDelegate GNode::mDelegateDeleteObj;

CXDelegate GNode::mDelegateComponentChange;

CXDelegate GNode::mDelegateAddObj;

GNode* GNode::mOperatorObj = nullptr;

GNode* GNode::mOperatorParentObj = nullptr;


CXDelegate GNode::mDelegateCreateObj;


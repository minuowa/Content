#include "GGameDemoHeader.h"
#include "GNode.h"
#include "GGameMap.h"
#include "GMeshBuffer.h"
#include "GBound.h"
#include "GTimer.h"
#include "GGame.h"



void GNode::GetInput ( DWORD frameTimeMs )
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

            mXPos.MoveStep ( frameTimeMs );

            mXPos.mbBack = false;

            SetState ( oasMoving, false );

        }
        else if ( bMoveBack )
        {
            m_bBehaviour = true;

            mXPos.MoveStep ( -frameTimeMs );

            mXPos.mbBack = true;

            SetState ( oasMoving, true );
        }


        if ( bTrunLeft )
        {
            mXPos.TrunStepLeftRight ( -frameTimeMs / 3.0f );
        }

        else if ( bTrunRight )
        {
            mXPos.TrunStepLeftRight ( frameTimeMs / 3.0f );
        }

        if ( bJump )
        {
            if ( !mXPos.mbJump )
            {
                m_bBehaviour = true;

                if ( bMoveBack )
                {
                    mXPos.mvSpeed = -mXPos.mvDir * mXPos.mfSpeedMove;
                }
                if ( bMoveFront )
                {
                    mXPos.mvSpeed = mXPos.mvDir * mXPos.mfSpeedMove;
                }
            }

            mXPos.Jump();
        }

        if ( INPUTSYSTEM.IsPressKey ( DIK_1 ) )
        {
            SetState ( oasBeAttack, false );
        }

        if ( INPUTSYSTEM.GetKeyAction ( DIK_2 ) == DI_BUTTONUP )
        {
            SetState ( oasAttack, false );
        }
        else if ( INPUTSYSTEM.GetKeyAction ( DIK_3 ) == DI_BUTTONUP )
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
        n->GetInput ( frameTimeMs );
    }
}

D3DXMATRIX GNode::GetWorldMatrix ( bool bForTrans )
{
    //�����������
    //D3DXMATRIX matMultiWorldMatix;

    if ( mParent == NULL )
    {
        _matWorld = mXPos.GetWorldMatrix ( bForTrans );
    }
    else
    {
        if ( mOpt == optByPosition )
        {
            _matWorld = ( mXPos.GetWorldMatrix ( bForTrans ) ) * ( ( ( GNode* ) ( mParent ) )->mXPos.GetWorldMatrix ( true ) );
        }
        else
        {
            _matWorld = ( mXPos.GetWorldMatrix ( false ) ) * ( ( ( GNode* ) ( mParent ) )->GetWorldMatrixByBone ( msParentName, false ) );

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
    ZeroMemory ( msParentName, sizeof ( msParentName ) );

    mParent = NULL;

    m_pOnObj = NULL;

    m_fBoundRadius = 0.01f;

    mForceMap = NULL;										//�����ĵ�ͼ��ָ��

    m_fForceHeight = Default_Force_Height;

    m_ForceType = ftUpWithMap;

    m_bForceOnMap = true;

    m_bBehaviour = false;

    m_bBeSelected = false;

    m_bCanSelect = true;										//�Ƿ��ѡ��

    m_bBlock = false;

    //m_vBlockPoint=ZEROVECTOR3;

    m_fBlockHeight = ZEROFLOAT;

    m_fBlockArea = ZEROFLOAT;



    m_bUseMatrialColor = false;

    m_bHit = false;


    m_bEyeCliper = true;


    ZeroMemory ( &m_InsectInfo, sizeof ( m_InsectInfo ) );			//��ײ��Ϣ


    m_ObjAnimState = oasNULL;									//����ǰ״̬

    D3DXMatrixIdentity ( &_matWorld );							//������������
}

GNode::~GNode()
{

}

eObjAnimState GNode::SetState ( eObjAnimState oas, bool bBack )
{
    m_ObjAnimState = oas;

    return m_ObjAnimState;
}



D3DXMATRIX GNode::GetWorldMatrixByBone ( char *sBoneName, bool bForTrans/*=false*/ )
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

void GNode::Update(  )
{
    const bool bOriginAutoMove = mXPos.mbAutoMove;


    eHitType htMap = htNull;

    eHitType htObj = htNull;

    IntersectInfo *pIntersectObj = NULL;

    IntersectInfo *pIntersectMapInfo = NULL;		//���ͼ����ײ��

    IntersectInfo *pIntersectFinalHit = NULL;		//������ײ��Ϣ�������ǵ�ͼ���������壩

    if ( !m_bForceOnMap || !m_bBeSelected || !m_bCanSelect )
    {
        return;
    }

    if ( !m_bBehaviour )
    {
        return;
    }

    if ( bOriginAutoMove )
    {
        mXPos.Update();
    }

    const GComponentTrans cXPos = mXPos;
    /*************************************************************************************************
    //��������Զ��ƶ�����mXPos.Update(fPass)�и���Pos��LastPos��
    //�������GetInput��fPass���и���Pos��LastPos:

    1�����ͼ��ײ
    **************************************************************************************************/
    if ( bOriginAutoMove )
    {

        GGameMap *pMap = ( GGameMap* ) mForceMap;

        D3DXVECTOR4 vDir;
        vDir.x = mXPos.mTranslate.x - mXPos.mvLastPos.x;
        vDir.y = mXPos.mTranslate.y - mXPos.mvLastPos.y;
        vDir.z = mXPos.mTranslate.z - mXPos.mvLastPos.z;
        vDir.w = 0;

        D3DXVECTOR4 vPos ( mXPos.mvLastPos.x, mXPos.mvLastPos.y, mXPos.mvLastPos.z, 1 );

        bool bHit = pMap->CheckIntersect ( vPos, vDir, true );

        if ( bHit )
        {
            float fHitDist = D3DXVec3Length ( & ( pMap->m_InsectInfo.vHitPos - mXPos.mvLastPos ) );

            float fDist = D3DXVec3Length ( & ( mXPos.mTranslate - mXPos.mvLastPos ) );

            if ( fHitDist > fDist )
            {
                htMap = htAutoMoveHitNoMap;
            }
            else
            {
                htMap = htAutoMoveHitMap;

                //�Զ��ƶ�������
                mXPos.mbJump = false;

                mXPos.mbBack = false;

                mXPos.mbAutoMove = false;

                mXPos.mbCanMoveStep = true;

                mXPos.mvSpeed = ZEROVECTOR3;

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

        mXPos = cXPos;

        pIntersectMapInfo = UpdateForForceOnMap();

        if ( pIntersectObj != NULL )
        {
            if ( pIntersectMapInfo->vHitPos.y > pIntersectObj->vHitPos.y )
            {
                m_pOnObj = NULL;
            }
            else
            {
                mXPos.mTranslate = pIntersectObj->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 );

                float fTmpAng = D3DXVec3Dot ( &pIntersectObj->vNormal, &D3DXVECTOR3 ( 0, 1, 0 ) );
                if ( 0 < fTmpAng && fTmpAng < 0.5f )
                {
                    mXPos.mvDir = cXPos.mvDir;

                    D3DXVECTOR3 vDirTmp = mXPos.mvUpon + pIntersectObj->vNormal;
                    SetDir ( vDirTmp / 2.0f );
                }
                else if ( fTmpAng >= 0.5f )
                {
                    mXPos.mvDir = cXPos.mvDir;
                    SetDir ( pIntersectObj->vNormal );
                }
                else
                {
                    mXPos.mTranslate = cXPos.mvLastPos;

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
                    mXPos.mTranslate = cXPos.mvLastPos;
                    UpdateForForceOnMap();
                }
                else
                {
                    m_pOnObj = pIntersect->pObj;
                    mXPos.mvDir = cXPos.mvDir;

                    mXPos.mTranslate = pIntersect->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 );

                    SetDir ( pIntersect->vNormal );
                }


            }

        }

        return;
    }

    /*************************************************************************************************
    2�����ͼ�г���������ײ
    **************************************************************************************************/
    mXPos = cXPos;

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
                mXPos.mTranslate = mXPos.mvLastPos;

                UpdateForForceOnMap();
            }
            else
            {
                m_pOnObj = pInterBlock->pObj;

                mXPos.mTranslate = pInterBlock->vHitPos;

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
                mXPos.mTranslate = mXPos.mvLastPos;

                UpdateForForceOnObj ( m_pOnObj );
            }
            else
            {
                m_pOnObj = pInterBlock->pObj;

                mXPos.mTranslate = pInterBlock->vHitPos;

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
        mXPos.mTranslate = pIntersectFinalHit->vHitPos + D3DXVECTOR3 ( 0, m_fForceHeight, 0 );

        m_bBehaviour = false;

        //�Զ��ƶ�������
        mXPos.mbJump = false;

        mXPos.mbBack = false;

        mXPos.mbAutoMove = false;

        mXPos.mbCanMoveStep = true;

        mXPos.mvSpeed = ZEROVECTOR3;

        SetDir ( pIntersectFinalHit->vNormal );
    }
    GNodeArr::iterator iBegin = mChildren.begin();
    GNodeArr::iterator iEnd = mChildren.end();
    for ( ; iBegin != iEnd; ++iBegin )
    {
        GNode* n = *iBegin;
        n->Update();
    }
}



IntersectInfo * GNode::UpdateForForceOnMap()
{
    if ( m_bForceOnMap )
    {
        if ( mForceMap != NULL )
        {
            GGameMap *pMap = ( GGameMap* ) mForceMap;

            D3DXVECTOR4 vDir ( 0, -1, 0, 0 ), vPos ( mXPos.mTranslate.x, mXPos.mTranslate.y + 5000.0f, mXPos.mTranslate.z, 1 );

            //���µ��������ͼ��ײ���϶���ײ��

            bool bHit = pMap->CheckIntersect ( vPos, vDir, true );

            if ( bHit )
            {

                SetDir ( pMap->m_InsectInfo.vNormal );

                mXPos.mTranslate = pMap->m_InsectInfo.vHitPos;

                mXPos.mTranslate.y = pMap->m_InsectInfo.vHitPos.y + m_fForceHeight;

                return & ( pMap->m_InsectInfo );
            }

        }

    }

    return NULL;
}

void GNode::SetDir ( D3DXVECTOR3 vNormal )
{
    if ( m_ForceType == ftUpAlways )
    {
        mXPos.SetDirWithUpon ( D3DXVECTOR3 ( 0, 1, 0 ) );
    }

    if ( m_ForceType == ftUpWithMap )
    {
        mXPos.SetDirWithUpon ( vNormal );
    }
}

IntersectInfo * GNode::UpdateForForceOnObj ( void *pObj )
{
    if ( pObj == NULL )
    {
        return NULL;
    }

    GMeshBaseObj *lpObj = ( GMeshBaseObj * ) pObj;

    D3DXVECTOR4 vDir ( 0, -1, 0, 0 ), vPos ( mXPos.mTranslate.x, mXPos.mTranslate.y + 20.0f, mXPos.mTranslate.z, 1 );

    //���µ��������ͼ��ײ���϶���ײ��

    bool bHit = lpObj->CheckIntersect ( vPos, vDir, true );

    if ( bHit )
    {
        //D3DXVec3Normalize(&lpObj->m_InsectInfo.vNormal,&lpObj->m_InsectInfo.vNormal);

        //float fAngle=D3DXVec3Dot(&lpObj->m_InsectInfo.vNormal,&D3DXVECTOR3(0,1,0));

        //if (fAngle>0.1)
        //{
        SetDir ( lpObj->m_InsectInfo.vNormal );

        mXPos.mTranslate = lpObj->m_InsectInfo.vHitPos;

        mXPos.mTranslate.y = lpObj->m_InsectInfo.vHitPos.y + m_fForceHeight;
        //}
        //else
        //{
        //	mXPos.mvPos=mXPos.mvLastPos;
        //}

        return & ( lpObj->m_InsectInfo );
    }

    return NULL;
}

GNode* GNode::AddChild ( GNode* c )
{
    CXASSERT_RETURN_FALSE ( c );
    mChildren.push_back ( c );

    EditorEvent event;
    event.mType = eSceneToEditor_Add;
    event.mArgs.push_back ( c->GetEditName() );
    event.mArgs.push_back ( GetEditName() );
    SendEditorEvent ( event );

    return c;
}

bool GNode::Draw()
{
    BeginRender();
    Render();
    EndRender();
    return true;
}

bool GNode::RemoveChild ( GNode* child )
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
        return n->RemoveChild ( child );
    }
    return false;
}

void GNode::BeginRender()
{

}

void GNode::EndRender()
{
    GNodeArr::iterator iBegin = mChildren.begin();
    GNodeArr::iterator iEnd = mChildren.end();
    for ( ; iBegin != iEnd; ++iBegin )
    {
        GNode* n = *iBegin;
        n->Draw();
    }
}






bool GNode::Create()
{
    return true;
}


void GNode::SetParentBone ( GNode *Parent, char *sName )
{
    if ( sName == NULL || strlen ( sName ) == 0 || strlen ( sName ) > 31 )
    {
        return;
    }

    mOpt = optByName;

    mParent = Parent;

    strcpy ( msParentName, sName );

}

int GNode::GetObjID() const
{
    return _nID;
}

void GNode::RegisterAll()
{
    __super::RegisterAll();

}

void GNode::LinkTo ( CXRapidxmlNode* parent )
{
    CXASSERT_RETURN ( parent );
    if ( !IsRegisterAll() )
        RegisterAll();

    CXRapidxmlNode* me = parent->document()->allocate_node ( rapidxml::node_element );
    parent->append_node ( me );
    MakeXMLNode ( *me );

    GNodeArr::iterator iBegin = mChildren.begin();
    GNodeArr::iterator iEnd = mChildren.end();
    for ( ; iBegin != iEnd; ++iBegin )
    {
        GNode* n = *iBegin;
        n->LinkTo ( me );
    }
}

void GNode::MakeXMLNode ( CXRapidxmlNode& node )
{
    node.name ( "Object" );
    CXRapidxmlAttr* attr = node.document()->allocate_attribute ( "Type", this->CategoryName() );
    node.append_attribute ( attr );
	assert(0 && "undo work");
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

void GNode::SetNodeName ( CChar* name )
{
    mNodeName = name;
}

bool GNode::Render()
{
    return true;
}

GNode* GNode::GetNodeByName ( const char* name )
{
    if ( mNodeName == name )
        return this;

    GNode* target = 0;
    GNodeArr::iterator iBegin = mChildren.begin();
    GNodeArr::iterator iEnd = mChildren.end();
    for ( ; iBegin != iEnd; ++iBegin )
    {
        GNode* child = *iBegin;
        target = child->GetNodeByName ( name );
        if ( target )
            return target;
    }
    return target;
}

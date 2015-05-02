#include "GGameDemoHeader.h"
#include "GTerrainNode.h"
#include "GTerrain.h"
#include "GTerrainCommon.h"
#include "GD9Device.h"
#include "GCamera.h"
#include "GFrustum.h"
CXImpleteObjectPoolN ( GTerrainNode, 4096 * 2 );
int GTerrainNode::RenderNodeCount = 0;

IDirect3DVertexBuffer9* GTerrainNode::mVertexBuffer = nullptr;

void GTerrainNode::buildBound ( GTerrain* owner )
{
    if ( mVertexBuffer == nullptr )
        return;

    mBound = new GCubeBound();
    int Stride = mConner[ChildRightBotttom] - mConner[ChildLeftBottom];
    int LineNum = ( mConner[ChildLeftTop] - mConner[ChildLeftBottom] ) / ( owner->getCellCount() + 1 );

    for ( int i = 0; i < LineNum; i++ )
    {
        for ( int j = 0; j < Stride + 1; j++ )
        {
            EXVertex& pos = mVertexData[mConner[ChildLeftBottom] + i * ( owner->getCellCount() + 1 ) + j];
            if ( pos.Pos.x < mBound->mMinX )
            {
                mBound->mMinX = pos.Pos.x;
            }
            if ( pos               .Pos.x > mBound->mMaxX )
            {
                mBound->mMaxX = pos.Pos.x;
            }
            if ( pos               .Pos.y < mBound->mMinY )
            {
                mBound->mMinY = pos.Pos.y;
            }
            if ( pos               .Pos.y > mBound->mMaxY )
            {
                mBound->mMaxY = pos.Pos.y;
            }
            if ( pos               .Pos.z < mBound->mMinZ )
            {
                mBound->mMinZ = pos.Pos.z;
            }
            if ( pos               .Pos.z > mBound->mMaxZ )
            {
                mBound->mMaxZ = pos.Pos.z;
            }
        }
    }

    mBound->updateCenter();
    mBound->updateRadius();

    for ( int i = 0; i < 4; i++ )
    {
        if ( mChildren[i] != nullptr )
            mChildren[i]->buildBound ( owner );
    }
}

GTerrainNode::GTerrainNode ()
{
    mPose = ChildCount;
    mLevel = 0;
    mCenter = 0;
    dMemoryZeroArray ( mChildren );
    dMemoryZeroArray ( mConner );
    dMemoryZeroArray ( mCulledData );
    dMemoryZeroArray ( mNeighbour );
    mParentNode = nullptr;
    mCullResult = eCullResultTypeNone;
    mRepairTimes = 0;
    mBound = nullptr;
}

void GTerrainNode::setVertexBuffer ( IDirect3DVertexBuffer9* VB, GTerrain* owner )
{
    mVertexBuffer = VB;
    if ( mVertexBuffer == nullptr )
        return;
    mVertexBuffer->Lock ( 0, 0, ( void** ) &mVertexData, D3DLOCK_DISCARD );

    buildBound ( owner );

    mVertexBuffer->Unlock();
}

void GTerrainNode::reset()
{
    mRepairTimes = 0;
    dMemoryZeroArray ( mCulledData );
    dMemoryZeroArray ( mRepairData );
    //dMemoryZeroArray ( mNeighbour );
    mCullResult = eCullResultTypeNone;

    for ( int i = 0; i < ChildCount; i++ )
    {
        if ( mChildren[i] != nullptr )
            mChildren[i]->reset();
    }
}

void GTerrainNode::addIndexToTerrain ( GTerrain* owner, bool displayRepairArea, bool displayRepairOnly, bool lodMode )
{
    uBuffer* dynamicBuffer = owner->getDynamicIndexBuffer();
    if ( !lodMode )
    {
        if ( mLevel == 1 )
            dynamicBuffer->addElement ( &mIndices[0], 8 * 3 );
    }
    else
    {
        if ( mCullResult == eCullResultTypeRender || mCullResult == eCullResultTypeLevelHigh )
        {
            if ( !displayRepairOnly )
            {
                for ( int i = 0; i < ChildCount; ++i )
                {
                    if ( !mCulledData[i] )
                        dynamicBuffer->addElement ( &mIndices[i * 6], 6 );
                }
            }

            if ( mRepairTimes > 0 && displayRepairArea )
            {
                assert ( mRepairTimes < 4 );
                dynamicBuffer->addElement ( &mIndices[G_TERRAIN_CELL_BASE_INDEX_NUM], mRepairTimes * 3 );
            }
        }
    }

    for ( int i = 0; i < 4; i++ )
    {
        if ( mChildren[i] != nullptr )
            mChildren[i]->addIndexToTerrain ( owner, displayRepairArea, displayRepairOnly, lodMode );
    }
}

float GetDistanceFromPointToLine ( D3DXVECTOR3& point, D3DXVECTOR3& orgin, D3DXVECTOR3& line )
{
    //point 为点P,过P做line的垂线，交点为Q,Q=orgin+k*line;
    //PQ _|_ line,相乘为0，可得到Q点，PQ即为所需结果
    float d = dVector3Length ( D3DXVECTOR3 ( point - orgin ) )
              - dSequare (
                  line.x * ( point.x - orgin.x ) +
                  line.y * ( point.y - orgin.y ) +
                  line.z * ( point.z - orgin.z )
              )
              / dVector3Length ( line ) ;

    return sqrt ( d );
}
void GTerrainNode::pick ( D3DXVECTOR3 orgin, D3DXVECTOR3 dir, CXDynaArray<HitInfo*>& AllHits )
{
    if ( GetDistanceFromPointToLine ( mBound->mCenter, orgin, dir ) < mBound->mRadius )
    {
        if ( mLevel == 1 )
        {
            //做碰撞检测
            EXVertex* VData;
            mVertexBuffer->Lock ( 0, 0, ( void** ) &VData, D3DLOCK_DISCARD );

            for ( int i = 0; i < 8; i++ )
            {
                D3DXVECTOR3 A = VData[mIndices[i * 3]]    .Pos;
                D3DXVECTOR3 B = VData[mIndices[i * 3 + 1]].Pos;
                D3DXVECTOR3 C = VData[mIndices[i * 3 + 2]].Pos;
                D3DXPLANE plane;
                D3DXVECTOR3 out ;
                D3DXVECTOR3 P ;
                D3DXPlaneFromPoints ( &plane, &A, &B, &C );
                D3DXVECTOR3 P2 ( orgin + dir * 1000000.0f ) ;
                D3DXPlaneIntersectLine ( &P, &plane, &out, &P2 );
                //线段与平面的交点

                D3DXVECTOR3 v0 = C - A;
                D3DXVECTOR3 v1 = B - A;
                D3DXVECTOR3 v2 = P - A;
                float dot00 = dVector3Dot ( v0, v0 );
                float dot01 = dVector3Dot ( v0, v1 );
                float dot02 = dVector3Dot ( v0, v2 );
                float dot11 = dVector3Dot ( v1, v1 );
                float dot12 = dVector3Dot ( v1, v2 );
                float inverDeno = 1 / ( dot00 * dot11 - dot01 * dot01 );

                float u = ( dot11 * dot02 - dot01 * dot12 ) * inverDeno;
                float v = ( dot00 * dot12 - dot01 * dot02 ) * inverDeno;

                if ( u >= 0 && v >= 0 && ( u + v ) <= 1 )
                {
                    HitInfo* II = new HitInfo();
                    II->U = u;
                    II->V = v;
                    II->Hited = true;
                    II->HitPoint = P;
                    II->Index[0] = mIndices[i * 3];
                    II->Index[1] = mIndices[i * 3 + 1];
                    II->Index[2] = mIndices[i * 3 + 2];
                    II->Distance = dVector3Length ( D3DXVECTOR3 ( P - orgin ) );
                    II->TriangleIndex = i;
                    AllHits.push_back ( II );
                }
            }
            mVertexBuffer->Unlock();
        }
        else
        {
            for ( int i = 0; i < 4; i++ )
            {
                if ( mChildren[i] != nullptr )
                    mChildren[i]->pick ( orgin, dir, AllHits );
            }
        }
    }
}

bool GTerrainNode::buildIndexBuffer ( GTerrain* owner )
{
    if ( mParentNode == nullptr )
    {
        int len = owner->getLineCount();
        mCenter = ( len + 1 ) * (  len - 1 ) / 2;
        mConner[ChildLeftBottom] = 0;
        mConner[ChildRightBotttom] = len - 1;
        mConner[ChildRightTop] = len * ( len - 1 ) + len - 1;
        mConner[ChildLeftTop] = len * ( len - 1 );
    }
    else
    {
        switch ( mPose )
        {
        case ChildLeftBottom:
            mConner[ChildLeftBottom] = mParentNode->mConner[ChildLeftBottom];
            mConner[ChildRightBotttom] = ( mParentNode->mConner[ChildRightBotttom] + mParentNode->mConner[ChildLeftBottom] ) / 2;
            mConner[ChildRightTop] = mParentNode->mCenter;
            mConner[ChildLeftTop] = ( mParentNode->mConner[ChildLeftTop] + mParentNode->mConner[ChildLeftBottom] ) / 2;
            break;

        case ChildRightBotttom:
            mConner[ChildLeftBottom] = ( mParentNode->mConner[ChildRightBotttom] + mParentNode->mConner[ChildLeftBottom] ) / 2;
            mConner[ChildRightBotttom] = mParentNode->mConner[ChildRightBotttom];
            mConner[ChildRightTop] = ( mParentNode->mConner[ChildRightTop] + mParentNode->mConner[ChildRightBotttom] ) / 2;
            mConner[ChildLeftTop] = mParentNode->mCenter;
            break;

        case ChildRightTop:
            mConner[ChildLeftBottom] = mParentNode->mCenter;
            mConner[ChildRightBotttom] = ( mParentNode->mConner[ChildRightTop] + mParentNode->mConner[ChildRightBotttom] ) / 2;
            mConner[ChildRightTop] = mParentNode->mConner[ChildRightTop];
            mConner[ChildLeftTop] = ( mParentNode->mConner[ChildRightTop] + mParentNode->mConner[ChildLeftTop] ) / 2;
            break;

        case ChildLeftTop:
            mConner[ChildLeftBottom] = ( mParentNode->mConner[ChildLeftTop] + mParentNode->mConner[ChildLeftBottom] ) / 2;
            mConner[ChildRightBotttom] = mParentNode->mCenter;
            mConner[ChildRightTop] = ( mParentNode->mConner[ChildRightTop] + mParentNode->mConner[ChildLeftTop] ) / 2;
            mConner[ChildLeftTop] = mParentNode->mConner[ChildLeftTop];
            break;
        }
        mCenter = ( mConner[ChildLeftTop] + mConner[ChildLeftBottom] ) / 2 + ( mConner[ChildRightTop] - mConner[ChildLeftTop] ) / 2;

    }
    int LeftCenter = ( mConner[ChildLeftTop] + mConner[ChildLeftBottom] ) / 2;
    int RightCenter = ( mConner[ChildRightTop] + mConner[ChildRightBotttom] ) / 2;
    int TopCenter = ( mConner[ChildRightTop] + mConner[ChildLeftTop] ) / 2;
    int BottomCenter = ( mConner[ChildRightBotttom] + mConner[ChildLeftBottom] ) / 2;
    mIndices[0] = mCenter;
    mIndices[1] = mConner[ChildLeftBottom];
    mIndices[2] = LeftCenter;

    mIndices[3] = mCenter;
    mIndices[4] = BottomCenter;
    mIndices[5] = mConner[ChildLeftBottom];

    mIndices[6] = mCenter;
    mIndices[7] = mConner[ChildRightBotttom];
    mIndices[8] = BottomCenter;

    mIndices[9] = mCenter;
    mIndices[10] = RightCenter;
    mIndices[11] = mConner[ChildRightBotttom];

    mIndices[12] = mCenter;
    mIndices[13] = mConner[ChildRightTop];
    mIndices[14] = RightCenter;

    mIndices[15] = mCenter;
    mIndices[16] = TopCenter;
    mIndices[17] = mConner[ChildRightTop];

    mIndices[18] = mCenter;
    mIndices[19] = mConner[ChildLeftTop];
    mIndices[20] = TopCenter;

    mIndices[21] = mCenter;
    mIndices[22] = LeftCenter;
    mIndices[23] = mConner[ChildLeftTop];

    for ( int i = G_TERRAIN_CELL_BASE_INDEX_NUM; i < G_TERRAIN_CELL_MAX_INDEX_NUM; i++ )
        mIndices[i] = CXIndex::InvalidIndex;

    //修补裂缝最多需要8个三角面,另加24个索引

    return true;
}

void GTerrainNode::build ( GTerrain* owner, int level, ChildType pose  )
{
    mLevel = level;
    mPose = pose;
    GCenter_NodesMap* sameLevelNodes = 0;
    GLevel_NodesMap& levelNodesMap = owner->getNodesMap();

    if ( !levelNodesMap.Get ( mLevel, sameLevelNodes ) )
    {
        sameLevelNodes = new GCenter_NodesMap;
        levelNodesMap.Insert ( mLevel, sameLevelNodes );
    }

    buildIndexBuffer ( owner );
    sameLevelNodes->Insert ( mCenter, this );

    if ( mLevel <= 1 )
    {
        return;
    }
    else
    {
        for ( int i = 0; i < 4; i++ )
        {
            mChildren[i] = new GTerrainNode ();
            mChildren[i]->mParentNode = this;
            mChildren[i]->build ( owner, mLevel - 1, ChildType ( i ) );
        }
    }
}


void GTerrainNode::repair()
{
    mRepairTimes = 0;
    int index = G_TERRAIN_CELL_BASE_INDEX_NUM;

    for ( int i = 0; i < RepairTypeCount; ++i )
    {
        if ( mRepairData[i] )
        {
            repairCrack ( this, ( RepairType ) i, &mIndices[index] );
            index += 3;
            mRepairTimes++;
        }
    }
    for ( int i = 0; i < ChildCount; i++ )
    {
        if ( mChildren[i] )
            mChildren[i]->repair();
    }
}

void GTerrainNode::repairCrack ( GTerrainNode* node, RepairType t , u32* buffer )
{
    switch ( t )
    {
    case RepairType::Left:
        buffer[0] = node->mConner[ChildLeftTop];
        buffer[1] = node->mConner[ChildLeftBottom];
        buffer[2] = ( buffer[0] + buffer[1] ) / 2;


        //buffer[3] = node->mConner[ChildLeftTop];
        //buffer[4] = node->mConner[ChildLeftBottom];
        //buffer[5] = ( buffer[3] + buffer[4] ) / 2;


        break;
    case RepairType::Bottom:
        buffer[0] = node->mConner[ChildLeftBottom];
        buffer[1] = node->mConner[ChildRightBotttom];
        buffer[2] = ( buffer[0] + buffer[1] ) / 2;


        //buffer[3] = node->mConner[ChildLeftBottom];
        //buffer[4] = node->mConner[ChildRightBotttom];
        //buffer[5] = ( buffer[3] + buffer[4] ) / 2;

        break;

    case RepairType::Right:
        buffer[0] = node->mConner[ChildRightTop];
        buffer[1] = node->mConner[ChildRightBotttom];
        buffer[2] = ( buffer[0] + buffer[1] ) / 2;


        //buffer[3] = node->mConner[ChildRightTop];
        //buffer[4] = node->mConner[ChildRightBotttom];
        //buffer[5] = ( buffer[3] + buffer[4] ) / 2;

        break;

    case RepairType::Top:

        buffer[0] = node->mConner[ChildLeftTop];
        buffer[1] = node->mConner[ChildRightTop];
        buffer[2] = ( buffer[0] + buffer[1] ) / 2;


        //buffer[3] = node->mConner[ChildLeftTop];
        //buffer[4] = node->mConner[ChildRightTop];
        //buffer[5] = ( buffer[3] + buffer[4] ) / 2;

        break;
    }
}

void GTerrainNode::cull ( GCamera* camera, GTerrain* owner )
{
    if ( !camera->isInCamera ( mBound ) )
    {
        mCullResult = eCullResultTypeNotInEye;
        return;
    }

    if ( mLevel == 1 )
    {
        mCullResult = eCullResultTypeRender;
        //if ( mParentNode )
        assert ( mParentNode );
        mParentNode->mCulledData[mPose] = true;
        return;
    }

    D3DXVECTOR3 cameraPos;
    camera->getObjectCorrdPos ( cameraPos, owner );
    float Distance = dVector3Length ( mBound->mCenter - cameraPos );

    float TargetLevel =
        Distance / owner->getLODFactor()  / sqrt ( mLevel );

    if ( TargetLevel < mLevel )
    {
        mCullResult = eCullResultTypeLevelHigh;
        if ( mParentNode )
            mParentNode->mCulledData[mPose] = true;
        for ( int i = 0; i < ChildCount; i++ )
        {
            if ( mChildren[i] != nullptr )
                mChildren[i]->cull ( camera, owner );
        }
    }
    else
    {
        mCullResult = eCullResultTypeRender;
        if ( mParentNode )
            mParentNode->mCulledData[mPose] = true;
    }
}

GTerrainNode::~GTerrainNode()
{
    dSafeDelete ( mBound );
for ( auto & c: mChildren )
    {
        if ( c != nullptr )
        {
            dSafeDelete ( c );
        }
    }
}
void GTerrainNode::checkShouldRepair ( GTerrain* owner )
{
    ////可以修补任意等级差的裂缝

    /** @brief
    	@1,当前可渲染节点(A),周围某同级节点B,如果如果B不可渲染并且B的不可渲染
    值为None,AB之间要做修补
    	@2,当前不可渲染节点A(eCullResultTypeLevelHigh),周围存在某同级节点B
    (eCullResultTypeNone,裁剪过程中，因其为LevelHigh的子节点，所以未做任何处
    理),AB之间要做修补
    **/
    if ( mCullResult == eCullResultTypeRender || mCullResult == eCullResultTypeLevelHigh )
    {
        for ( int i = 0; i < RepairTypeCount; ++i )
        {
            checkShouldRepair ( RepairType ( i ) );
        }
    }

    if ( mCullResult == eCullResultTypeLevelHigh )
    {
        for ( int i = 0; i < ChildCount; i++ )
        {
            if ( mChildren[i] != nullptr )
                mChildren[i]->checkShouldRepair ( owner );
        }
    }
}
void GTerrainNode::checkShouldRepair ( RepairType repairType )
{
    GTerrainNode* node = mNeighbour[repairType];
    if ( node && ( node->mCullResult == eCullResultTypeNone
                   //|| node->mCullResult == eCullResultTypeLevelHigh
                 ) )
    {
        mRepairData[repairType] = true;
    }
}

void GTerrainNode::buildNeighbour ( GTerrain* owner )
{
    int centers[ChildCount];
    centers[Left] = mCenter - ( ( int ) ( pow ( 2, mLevel ) ) );
    centers[Right] = mCenter + ( ( int ) ( pow ( 2, mLevel ) ) );
    centers[Top] = mCenter + ( ( int ) ( pow ( 2, mLevel ) ) ) * owner->getLineCount();
    centers[Bottom] = mCenter - ( ( int ) ( pow ( 2, mLevel ) ) ) * owner->getLineCount();

    GLevel_NodesMap& levelNodesMap = owner->getNodesMap();
    GCenter_NodesMap *NodeByCenter = nullptr;
    if ( levelNodesMap.Get ( mLevel, NodeByCenter ) )
    {
        for ( int i = 0; i < NeighbourCount; ++i )
        {
            GTerrainNode* n = 0;
            if ( NodeByCenter->Get ( centers[i], n ) )
                mNeighbour[i] = n;
        }
    }
    for ( int i = 0; i < ChildCount; ++i )
    {
        if ( mChildren[i] != nullptr )
            mChildren[i]->buildNeighbour ( owner );
    }
}

//CXObjectPool<GTerrainNode> GTerrainNode::mObjectPool ( 4096 * 2 );

EXVertex* GTerrainNode::mVertexData = nullptr;


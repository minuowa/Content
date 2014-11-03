#include "GGameDemoHeader.h"
#include "GTerrainNode.h"
#include "GTerrain.h"
#include "GTerrainCommon.h"
#include "GD9Device.h"
#include "GCamera.h"
#include "GFrustum.h"

int GTerrainNode::RootPos = GTerrainNode::InvalidNumber;
int GTerrainNode::RootLevel = 0;
int GTerrainNode::NodeCount = 0;

IDirect3DVertexBuffer9* GTerrainNode::mVertexBuffer = nullptr;

void GTerrainNode::buildBound ( GTerrain* owner )
{
    if ( mVertexBuffer == nullptr )
        return;

    mBound = new GCubeBound();
    int Stride = mConner[Pose_RightBotttom] - mConner[Pose_LeftBottom];
    int LineNum = ( mConner[Pose_LeftTop] - mConner[Pose_LeftBottom] ) / ( owner->getCellCount() + 1 );

    for ( int i = 0; i < LineNum; i++ )
    {
        for ( int j = 0; j < Stride + 1; j++ )
        {
            EXVertex& pos = mVertexData[mConner[Pose_LeftBottom] + i * ( owner->getCellCount() + 1 ) + j];
            if ( pos.Pos.x < mBound->mMinX )
            {
                mBound->mMinX = pos.Pos.x;
            }
            if ( pos.Pos.x > mBound->mMaxX )
            {
                mBound->mMaxX = pos.Pos.x;
            }
            if ( pos.Pos.y < mBound->mMinY )
            {
                mBound->mMinY = pos.Pos.y;
            }
            if ( pos.Pos.y > mBound->mMaxY )
            {
                mBound->mMaxY = pos.Pos.y;
            }
            if ( pos.Pos.z < mBound->mMinZ )
            {
                mBound->mMinZ = pos.Pos.z;
            }
            if ( pos.Pos.z > mBound->mMaxZ )
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

GTerrainNode::GTerrainNode ( int level, Pose pose )
    : mPose ( pose )
{
    Level = level;
    mCenter = 0;
    dMemoryZeroArray ( mChildren );
    dMemoryZeroArray ( mConner );
    mParentNode = nullptr;
    mVertexBuffer = nullptr;
    mVertexData = nullptr;
    mBeRender = true;
    mBeNeedRepair = false;
    mNotRenderReason = NotRenderReason::None;
    mRepairTimes = 0;
    mBound = nullptr;
    GCenter_NodesMap* sameLevelNodes = 0;
    if ( !mNodeMaps.Get ( level, sameLevelNodes ) )
    {
        sameLevelNodes = new GCenter_NodesMap;
        mNodeMaps.Insert ( level, sameLevelNodes );
    }
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
    mBeRender = false;
    mBeNeedRepair = false;
    mRepairTimes = 0;
    mRepairDatas.clear();

    mNotRenderReason = NotRenderReason::None;
    for ( int i = 0; i < G_TERRAIN_CHILD_NUM; i++ )
    {
        if ( mChildren[i] != nullptr )
            mChildren[i]->reset();
    }
}

void GTerrainNode::addIndexToTerrain ( GTerrain* owner )
{
    if ( mBeRender || mNotRenderReason == NotRenderReason::LevelHigh )
    {
        int VertexNum = 9;
        int PrimitiveNum = 8;
        int StartIndex = 0;

        CXBuffer* dynamicBuffer = owner->getDynamicIndexBuffer();

        if (
            ( mNotRenderReason == NotRenderReason::LevelHigh && mBeNeedRepair )
            ||
            mBeRender
        )
        {
            if ( mNotRenderReason == NotRenderReason::LevelHigh || owner->isDisplayRepairAreaOnly() )
            {
                VertexNum = mRepairTimes * 2;
                PrimitiveNum = mRepairTimes * 2;
                StartIndex = 24;
            }
            else
            {
                VertexNum = 9 + mRepairTimes * 2; ;
                PrimitiveNum = 8 + mRepairTimes * 2;
                StartIndex = 0;
            }

            //D9DEVICE->GetDvc()->DrawIndexedPrimitive ( D3DPT_TRIANGLELIST, 0, 0, VertexNum
            //        , StartIndex, PrimitiveNum );
        }
        dynamicBuffer->addElement ( &mIndices[StartIndex], PrimitiveNum * 3 );
        //GxDevice.DVC.DrawIndexedPrimitives(
        //    PrimitiveType.TriangleList,
        //    0, 0,
        //    9,
        //    0, 8);
    }
    for ( int i = 0; i < 4; i++ )
    {
        if ( mChildren[i] != nullptr )
            mChildren[i]->addIndexToTerrain ( owner );
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
        if ( Level == 1 )
        {
            //做碰撞检测
            EXVertex* VData;
            mVertexBuffer->Lock ( 0, 0, ( void** ) &VData, D3DLOCK_DISCARD );

            for ( int i = 0; i < 8; i++ )
            {
                D3DXVECTOR3 A = VData[mIndices[i * 3]].Pos;
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
        mConner[Pose_LeftBottom] = 0;
        mConner[Pose_RightBotttom] = len - 1;
        mConner[Pose_RightTop] = len * ( len - 1 ) + len - 1;
        mConner[Pose_LeftTop] = len * ( len - 1 );
    }
    else
    {
        switch ( mPose )
        {
        case Pose_LeftBottom:
            mConner[Pose_LeftBottom] = mParentNode->mConner[Pose_LeftBottom];
            mConner[Pose_RightBotttom] = ( mParentNode->mConner[Pose_RightBotttom] + mParentNode->mConner[Pose_LeftBottom] ) / 2;
            mConner[Pose_RightTop] = mParentNode->mCenter;
            mConner[Pose_LeftTop] = ( mParentNode->mConner[Pose_LeftTop] + mParentNode->mConner[Pose_LeftBottom] ) / 2;
            break;

        case Pose_RightBotttom:
            mConner[Pose_LeftBottom] = ( mParentNode->mConner[Pose_RightBotttom] + mParentNode->mConner[Pose_LeftBottom] ) / 2;
            mConner[Pose_RightBotttom] = mParentNode->mConner[Pose_RightBotttom];
            mConner[Pose_RightTop] = ( mParentNode->mConner[Pose_RightTop] + mParentNode->mConner[Pose_RightBotttom] ) / 2;
            mConner[Pose_LeftTop] = mParentNode->mCenter;
            break;

        case Pose_RightTop:
            mConner[Pose_LeftBottom] = mParentNode->mCenter;
            mConner[Pose_RightBotttom] = ( mParentNode->mConner[Pose_RightTop] + mParentNode->mConner[Pose_RightBotttom] ) / 2;
            mConner[Pose_RightTop] = mParentNode->mConner[Pose_RightTop];
            mConner[Pose_LeftTop] = ( mParentNode->mConner[Pose_RightTop] + mParentNode->mConner[Pose_LeftTop] ) / 2;
            break;

        case Pose_LeftTop:
            mConner[Pose_LeftBottom] = ( mParentNode->mConner[Pose_LeftTop] + mParentNode->mConner[Pose_LeftBottom] ) / 2;
            mConner[Pose_RightBotttom] = mParentNode->mCenter;
            mConner[Pose_RightTop] = ( mParentNode->mConner[Pose_RightTop] + mParentNode->mConner[Pose_LeftTop] ) / 2;
            mConner[Pose_LeftTop] = mParentNode->mConner[Pose_LeftTop];
            break;
        }
        mCenter = ( mConner[Pose_LeftTop] + mConner[Pose_LeftBottom] ) / 2 + ( mConner[Pose_RightTop] - mConner[Pose_LeftTop] ) / 2;

    }
    int LeftCenter = ( mConner[Pose_LeftTop] + mConner[Pose_LeftBottom] ) / 2;
    int RightCenter = ( mConner[Pose_RightTop] + mConner[Pose_RightBotttom] ) / 2;
    int TopCenter = ( mConner[Pose_RightTop] + mConner[Pose_LeftTop] ) / 2;
    int BottomCenter = ( mConner[Pose_RightBotttom] + mConner[Pose_LeftBottom] ) / 2;
    mIndices[0] = mCenter;
    mIndices[1] = mConner[Pose_LeftBottom];
    mIndices[2] = LeftCenter;

    mIndices[3] = mCenter;
    mIndices[4] = BottomCenter;
    mIndices[5] = mConner[Pose_LeftBottom];

    mIndices[6] = mCenter;
    mIndices[7] = mConner[Pose_RightBotttom];
    mIndices[8] = BottomCenter;

    mIndices[9] = mCenter;
    mIndices[10] = RightCenter;
    mIndices[11] = mConner[Pose_RightBotttom];

    mIndices[12] = mCenter;
    mIndices[13] = mConner[Pose_RightTop];
    mIndices[14] = RightCenter;

    mIndices[15] = mCenter;
    mIndices[16] = TopCenter;
    mIndices[17] = mConner[Pose_RightTop];

    mIndices[18] = mCenter;
    mIndices[19] = mConner[Pose_LeftTop];
    mIndices[20] = TopCenter;

    mIndices[21] = mCenter;
    mIndices[22] = LeftCenter;
    mIndices[23] = mConner[Pose_LeftTop];

    for ( int i = G_TERRAIN_CELL_BASE_INDEX_NUM; i < G_TERRAIN_CELL_MAX_INDEX_NUM; i++ )
        mIndices[i] = InvalidNumber;

    //修补裂缝最多需要4个三角面,另加12个索引

    return true;
}

void GTerrainNode::build ( GTerrain* owner )
{
    buildIndexBuffer ( owner );

    GCenter_NodesMap* NodeByCenter = mNodeMaps[Level];
    if ( NodeByCenter != nullptr )
    {
        NodeByCenter->Insert ( mCenter, this );
    }

    NodeCount++;

    if ( Level <= 1 )
    {
        return;
    }
    else
    {
        for ( int i = 0; i < 4; i++ )
        {
            mChildren[i] = new GTerrainNode ( Level - 1, Pose ( i ) );
            mChildren[i]->mParentNode = this;
            mChildren[i]->build ( owner );
        }
    }
}

void GTerrainNode::checkShouldRepair ( GTerrain* owner )
{
    ////可以修补任意等级差的裂缝

    ////如果当前可渲染节点(A),周围某同级节点(B)
    ////如果B不可渲染并且B的不可渲染值为None,那么A必须作AB之间的修补

    ////如果A的父节点(PA)不为空,B的父节点（PB）不可能为空（因为AB的level相同）
    ////如果PB不可渲染并且PB的不可渲染值为None,那么PA必须作PA、PB之间的修补
    if ( mBeRender )
    {
        int centers[Pose_Root];
        centers[Left] = mCenter - ( ( int ) ( pow ( 2, Level ) ) );
        centers[Right] = mCenter + ( ( int ) ( pow ( 2, Level ) ) );
        centers[Top] = mCenter + ( ( int ) ( pow ( 2, Level ) ) ) * owner->getLineCount();
        centers[Bottom] = mCenter - ( ( int ) ( pow ( 2, Level ) ) ) * owner->getLineCount();

        GCenter_NodesMap *NodeByCenter = mNodeMaps[Level];

        if ( NodeByCenter != nullptr )
        {
            mRepairDatas.clear();
            for ( int i = 0; i <= Bottom; ++i )
            {
                checkShouldRepair ( centers[i], *NodeByCenter, RepairType ( i ) );
            }
        }

        if ( mChildren != nullptr )
        {
            for ( int i = 0; i < 4; i++ )
            {
                mChildren[i]->checkShouldRepair ( owner );
            }
        }
    }
}
void GTerrainNode::repair()
{
    if ( mBeNeedRepair )
    {
        CXDynaArray<int> intList;

        mRepairTimes = 0;
for ( auto p: mRepairDatas )
        {
            static const int cnt = 6;
            int buffer[cnt];
            repairCrack ( this, p.first, buffer );
            for ( int i = 0; i < cnt; i++ )
            {
                intList.push_back ( buffer[i] );
            }
            mRepairTimes++;
        }

        for ( int i = 0; i < intList.size(); i++ )
            mIndices[G_TERRAIN_CELL_BASE_INDEX_NUM + i] = intList[i];
    }
    for ( int i = 0; i < 4; i++ )
    {
        if ( mChildren[i] )
            mChildren[i]->repair();
    }
}

void GTerrainNode::repairCrack ( GTerrainNode* node, RepairType t , int* buffer )
{
    switch ( t )
    {
    case RepairType::Left:
        buffer[0] = node->mConner[Pose_LeftTop];
        buffer[1] = node->mConner[Pose_LeftBottom];
        buffer[2] = ( buffer[0] + buffer[1] ) / 2;


        buffer[3] = node->mConner[Pose_LeftTop];
        buffer[4] = node->mConner[Pose_LeftBottom];
        buffer[5] = ( buffer[3] + buffer[4] ) / 2;


        break;
    case RepairType::Bottom:
        buffer[0] = node->mConner[Pose_LeftBottom];
        buffer[1] = node->mConner[Pose_RightBotttom];
        buffer[2] = ( buffer[0] + buffer[1] ) / 2;


        buffer[3] = node->mConner[Pose_LeftBottom];
        buffer[4] = node->mConner[Pose_RightBotttom];
        buffer[5] = ( buffer[3] + buffer[4] ) / 2;

        break;

    case RepairType::Right:
        buffer[0] = node->mConner[Pose_RightTop];
        buffer[1] = node->mConner[Pose_RightBotttom];
        buffer[2] = ( buffer[0] + buffer[1] ) / 2;


        buffer[3] = node->mConner[Pose_RightTop];
        buffer[4] = node->mConner[Pose_RightBotttom];
        buffer[5] = ( buffer[3] + buffer[4] ) / 2;

        break;

    case RepairType::Top:

        buffer[0] = node->mConner[Pose_LeftTop];
        buffer[1] = node->mConner[Pose_RightTop];
        buffer[2] = ( buffer[0] + buffer[1] ) / 2;


        buffer[3] = node->mConner[Pose_LeftTop];
        buffer[4] = node->mConner[Pose_RightTop];
        buffer[5] = ( buffer[3] + buffer[4] ) / 2;

        break;
    }
}

void GTerrainNode::clipByCamera ( GCamera* camera, GTerrain* owner )
{
    mNotRenderReason = NotRenderReason::None;
    if ( !camera->getCuller()->isInFrustum ( mBound ) )
    {
        mBeRender = false;
        mNotRenderReason = NotRenderReason::NotInEye;
        return;
    }

    if ( Level == 1 )
    {
        mBeRender = true;
        return;
    }
    D3DXVECTOR3 cameraPos;
    camera->getObjectCorrdPos ( cameraPos, owner );
    float Distance = dVector3Length ( mBound->mCenter - cameraPos );

    float TargetLevel =
        Distance / 800;
    //(float)pow(Distance / GxMap::_DisFactor, GxMap::_Power);

    if ( TargetLevel < Level )
    {
        mBeRender = false;
        mNotRenderReason = NotRenderReason::LevelHigh;
        for ( int i = 0; i < 4; i++ )
        {
            if ( mChildren[i] != nullptr )
                mChildren[i]->clipByCamera ( camera, owner );
        }
    }
    else
    {
        mBeRender = true;
    }
}

GTerrainNode::~GTerrainNode()
{
    dSafeDelete ( mBound );

for ( auto c: mChildren )
    {
        if ( c != nullptr )
        {
            dSafeDelete ( c );
        }
    }
}

void GTerrainNode::checkShouldRepair ( int center, GCenter_NodesMap & nodeMap, RepairType repairType )
{
    if ( nodeMap.findkey ( center ) )
    {
        if (
            !nodeMap[center]->mBeRender &&
            nodeMap[center]->mNotRenderReason == NotRenderReason::None
        )
        {
            mBeNeedRepair = true;
            if ( !mRepairDatas.findkey ( RepairType::Bottom ) )
                mRepairDatas.Insert ( RepairType::Bottom, true );

            GTerrainNode* OthersParent = nodeMap[center]->mParentNode;
            GTerrainNode* MyParent = mParentNode;

            while ( MyParent != nullptr )
            {
                if ( OthersParent->mNotRenderReason == NotRenderReason::None )
                {
                    MyParent->mBeNeedRepair = true;
                    if ( !MyParent->mRepairDatas.findkey ( repairType ) )
                        MyParent->mRepairDatas.Insert ( repairType, true );

                    MyParent = MyParent->mParentNode;
                    OthersParent = OthersParent->mParentNode;
                }
                else if ( OthersParent->mNotRenderReason == NotRenderReason::LevelHigh )
                {
                    break;
                }
            }
        }
    }
}

EXVertex* GTerrainNode::mVertexData = nullptr;


GTerrainNode::GLevel_NodesMap GTerrainNode::mNodeMaps;
#include "GGameDemoHeader.h"
#include "GTerrain.h"
#include "GD9Device.h"
#include "GTerrainCommon.h"
#include "GTexture.h"
#include "GBitmap.h"
#include "GHeightMap.h"

GameEditor::NodeBound::NodeBound()
{
    mMinX = FLOAT_MAX;
    mMaxX = FLOAT_MIN;

    mMinX = FLOAT_MAX;
    mMaxX = FLOAT_MIN;

    mMinX = FLOAT_MAX;
    mMaxX = FLOAT_MIN;

    Radius = 0;
}

void GameEditor::NodeBound::updateRadius()
{
    Radius = 0.25 * sqrt ( dSequare ( mMaxX - mMinX ) + dSequare ( mMaxY - mMinY ) + dSequare ( mMaxZ - mMinZ ) );
}

void GameEditor::NodeBound::updateCenter()
{
    mCenter.x = ( mMaxX + mMinX ) / 2.0f;
    mCenter.y = ( mMinY + mMaxY ) / 2.0f;
    mCenter.z = ( mMinZ + mMaxZ ) / 2.0f;
}

GameEditor::HitInfo::HitInfo()
{
    Hited = false;
    dMemoryZeroArray ( Index );
    U = 0.0f;
    V = 0.0f;
    Distance = 0.f;
    TriangleIndex = 0;
}
int GameEditor::GTerrainNode::RootPos = GameEditor::GTerrainNode::InvalidNumber;
int GameEditor::GTerrainNode::RootLevel = 0;
int GameEditor::GTerrainNode::NodeCount = 0;
int GameEditor::GTerrainNode::CellCount = 0;
int GameEditor::GTerrainNode::LineCount = 0;

IDirect3DVertexBuffer9* GameEditor::GTerrainNode::VBuffer = nullptr;

void GameEditor::GTerrainNode::BuildBound()
{
    if ( VBuffer == nullptr )
    {
        return;
    }
    dSafeDelete ( mBound );
    mBound = new NodeBound();
    int Stride = mConner[RightBotttom] - mConner[LeftBottom];
    int LineNum = ( mConner[LeftTop] - mConner[LeftBottom] ) / ( CellCount + 1 );

    for ( int i = 0; i < LineNum; i++ )
    {
        for ( int j = 0; j < Stride + 1; j++ )
        {
            EXVertex& pos = VBData[mConner[LeftBottom] + i * ( CellCount + 1 ) + j];
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
            mChildren[i]->BuildBound();
    }
}

GameEditor::GTerrainNode::GTerrainNode ( int level, int pos )
{
    mConner = nullptr;
    Pos = RootPos;
    Level = level;
    mCenter = 0;
    dMemoryZeroArray ( mChildren );
    Parent = nullptr;
    mIndexBuffer = nullptr;
    VBuffer = nullptr;
    VBData = nullptr;
    BeRender = true;
    BeNeedRepair = false;
    NotRender_Reason = NotRenderReason::None;
    RepairTimes = 0;
    mBound = nullptr;
    QNodeMap* NodeByCenter = 0;
    if ( !mNodeMaps.Get ( level, NodeByCenter ) )
    {
        NodeByCenter = new QNodeMap;
        mNodeMaps.Insert ( level, NodeByCenter );
    }

    Pos = pos;

    mConner = new int[4];

    if ( Pos == RootPos )
    {
        RootLevel = Level;
    }
}

void GameEditor::GTerrainNode::SetVertexBuffer ( IDirect3DVertexBuffer9* VB )
{
    VBuffer = VB;
    if ( VBuffer == nullptr )
    {
        return;
    }
    VBuffer->Lock ( 0, 0, ( void** ) &VBData, D3DLOCK_DISCARD );

    BuildBound();

    VBuffer->Unlock();
}

void GameEditor::GTerrainNode::Reset()
{
    BeRender = false;
    BeNeedRepair = false;
    RepairTimes = 0;
    RepairDatas.clear();

    NotRender_Reason = NotRenderReason::None;
    for ( int i = 0; i < 4; i++ )
    {
        if ( mChildren[i] != nullptr )
            mChildren[i]->Reset();
    }
}

void GameEditor::GTerrainNode::Render()
{
    if ( BeRender || NotRender_Reason == NotRenderReason::LevelHigh )
    {
        D9DEVICE->GetDvc()->SetIndices ( mIndexBuffer );
        int VertexNum = 9;
        int PrimitiveNum = 8;
        int StartIndex = 0;

        if (
            ( NotRender_Reason == NotRenderReason::LevelHigh && BeNeedRepair )
            ||
            BeRender
        )
        {
            if ( NotRender_Reason == NotRenderReason::LevelHigh || GxMap::DisplayRepairAreaOnly )
            {
                VertexNum = RepairTimes * 2;
                PrimitiveNum = RepairTimes * 2;
                StartIndex = 24;
            }
            else
            {
                VertexNum = 9 + RepairTimes * 2; ;
                PrimitiveNum = 8 + RepairTimes * 2;
                StartIndex = 0;
            }
            D9DEVICE->GetDvc()->DrawIndexedPrimitive ( D3DPT_TRIANGLELIST, 0, 0, VertexNum
                    , StartIndex, PrimitiveNum );
        }
        //GxDevice.DVC.DrawIndexedPrimitives(
        //    PrimitiveType.TriangleList,
        //    0, 0,
        //    9,
        //    0, 8);
    }
    for ( int i = 0; i < 4; i++ )
    {
        if ( mChildren[i] != nullptr )
            mChildren[i]->Render();
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
void GameEditor::GTerrainNode::Pick ( D3DXVECTOR3 orgin, D3DXVECTOR3 dir, CXDynaArray<HitInfo*>& AllHits )
{
    if ( GetDistanceFromPointToLine ( mBound->mCenter, orgin, dir ) < mBound->Radius )
    {
        if ( Level == 1 )
        {
            //做碰撞检测
            int* IData;
            mIndexBuffer->Lock ( 0, 0, ( void** ) &IData, D3DLOCK_DISCARD );
            EXVertex* VData;
            VBuffer->Lock ( 0, 0, ( void** ) &VData, D3DLOCK_DISCARD );

            for ( int i = 0; i < 8; i++ )
            {
                D3DXVECTOR3 A = VData[IData[i * 3]].Pos;
                D3DXVECTOR3 B = VData[IData[i * 3 + 1]].Pos;
                D3DXVECTOR3 C = VData[IData[i * 3 + 2]].Pos;
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
                    II->Index[0] = IData[i * 3];
                    II->Index[1] = IData[i * 3 + 1];
                    II->Index[2] = IData[i * 3 + 2];
                    II->Distance = dVector3Length ( D3DXVECTOR3 ( P - orgin ) );
                    II->TriangleIndex = i;
                    AllHits.push_back ( II );
                }
            }
            mIndexBuffer->Unlock();
            VBuffer->Unlock();
        }
        else
        {
            for ( int i = 0; i < 4; i++ )
            {
                if ( mChildren[i] != nullptr )
                    mChildren[i]->Pick ( orgin, dir, AllHits );
            }
        }
    }
}

int* GameEditor::GTerrainNode::BuildIndexBuffer()
{
    //face number=8
    //vertex number=9
    //index=face*3
    int* indices = nullptr;
    if ( Parent == nullptr )
    {
        int len = ( int ) pow ( 2, RootLevel ) + 1;
        CellCount = len - 1;
        LineCount = CellCount + 1;
        mCenter = ( len + 1 ) * ( len - 1 ) / 2;
        mConner[LeftBottom] = 0;
        mConner[RightBotttom] = len - 1;
        mConner[RightTop] = len * ( len - 1 ) + len - 1;
        mConner[LeftTop] = len * ( len - 1 );
    }
    else
    {
        switch ( Pos )
        {
        case LeftBottom:
            mConner[LeftBottom] = Parent->mConner[LeftBottom];
            mConner[RightBotttom] = ( Parent->mConner[RightBotttom] + Parent->mConner[LeftBottom] ) / 2;
            mConner[RightTop] = Parent->mCenter;
            mConner[LeftTop] = ( Parent->mConner[LeftTop] + Parent->mConner[LeftBottom] ) / 2;
            break;

        case RightBotttom:
            mConner[LeftBottom] = ( Parent->mConner[RightBotttom] + Parent->mConner[LeftBottom] ) / 2;
            mConner[RightBotttom] = Parent->mConner[RightBotttom];
            mConner[RightTop] = ( Parent->mConner[RightTop] + Parent->mConner[RightBotttom] ) / 2;
            mConner[LeftTop] = Parent->mCenter;
            break;

        case RightTop:
            mConner[LeftBottom] = Parent->mCenter;
            mConner[RightBotttom] = ( Parent->mConner[RightTop] + Parent->mConner[RightBotttom] ) / 2;
            mConner[RightTop] = Parent->mConner[RightTop];
            mConner[LeftTop] = ( Parent->mConner[RightTop] + Parent->mConner[LeftTop] ) / 2;
            break;

        case LeftTop:
            mConner[LeftBottom] = ( Parent->mConner[LeftTop] + Parent->mConner[LeftBottom] ) / 2;
            mConner[RightBotttom] = Parent->mCenter;
            mConner[RightTop] = ( Parent->mConner[RightTop] + Parent->mConner[LeftTop] ) / 2;
            mConner[LeftTop] = Parent->mConner[LeftTop];
            break;
        }
        mCenter = ( mConner[LeftTop] + mConner[LeftBottom] ) / 2 + ( mConner[RightTop] - mConner[LeftTop] ) / 2;

    }
    int IndexNum = 24 + 24;
    //此处不可用Dynamic，Dynamic锁定索引之后，原有索引清空
    D9DEVICE->GetDvc()->CreateIndexBuffer (
        sizeof ( int ) *IndexNum
        , 0
        , D3DFMT_INDEX32
        , D3DPOOL_MANAGED
        , &mIndexBuffer
        , 0
    );
    if ( mIndexBuffer != nullptr )
    {
        mIndexBuffer->Lock ( 0, 0, ( void** ) &indices, D3DLOCK_DISCARD );
        int LeftCenter = ( mConner[LeftTop] + mConner[LeftBottom] ) / 2;
        int RightCenter = ( mConner[RightTop] + mConner[RightBotttom] ) / 2;
        int TopCenter = ( mConner[RightTop] + mConner[LeftTop] ) / 2;
        int BottomCenter = ( mConner[RightBotttom] + mConner[LeftBottom] ) / 2;
        indices[0] = mCenter;
        indices[1] = mConner[LeftBottom];
        indices[2] = LeftCenter;

        indices[3] = mCenter;
        indices[4] = BottomCenter;
        indices[5] = mConner[LeftBottom];

        indices[6] = mCenter;
        indices[7] = mConner[RightBotttom];
        indices[8] = BottomCenter;

        indices[9] = mCenter;
        indices[10] = RightCenter;
        indices[11] = mConner[RightBotttom];

        indices[12] = mCenter;
        indices[13] = mConner[RightTop];
        indices[14] = RightCenter;

        indices[15] = mCenter;
        indices[16] = TopCenter;
        indices[17] = mConner[RightTop];

        indices[18] = mCenter;
        indices[19] = mConner[LeftTop];
        indices[20] = TopCenter;

        indices[21] = mCenter;
        indices[22] = LeftCenter;
        indices[23] = mConner[LeftTop];

        for ( int i = 24; i < IndexNum; i++ )
        {
            indices[i] = InvalidNumber;
        }

        //修补裂缝最多需要4个三角面,另加12个索引
        mIndexBuffer->Unlock();

    }
    return indices;
}

void GameEditor::GTerrainNode::Build()
{
    BuildIndexBuffer();

    QNodeMap* NodeByCenter = mNodeMaps[Level];
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
            mChildren[i] = new GTerrainNode ( Level - 1, i );
            mChildren[i]->Parent = this;
            mChildren[i]->Build();
        }
    }
}

void GameEditor::GTerrainNode::CheckShouldRepair()
{
    return;
    ////可以修补任意等级差的裂缝

    ////如果当前可渲染节点(A),周围某同级节点(B)
    ////如果B不可渲染并且B的不可渲染值为None,那么A必须作AB之间的修补

    ////如果A的父节点(PA)不为空,B的父节点（PB）不可能为空（因为AB的level相同）
    ////如果PB不可渲染并且PB的不可渲染值为None,那么PA必须作PA、PB之间的修补
    //if ( BeRender )
    //{
    //    int LeftNodeCenter = mCenter - ( ( int ) ( pow ( 2, Level ) ) );
    //    int RightNodeCenter = mCenter + ( ( int ) ( pow ( 2, Level ) ) );
    //    int TopNodeCenter = mCenter + ( ( int ) ( pow ( 2, Level ) ) ) * QNode::LineCount;
    //    int BottomNodeCenter = mCenter - ( ( int ) ( pow ( 2, Level ) ) ) * QNode::LineCount;

    //    QNodeMap *NodeByCenter = mNodeMaps[Level];

    //    if ( NodeByCenter != nullptr )
    //    {
    //        RepairDatas.clear();

    //        if ( NodeByCenter->findkey ( LeftNodeCenter ) )
    //        {
    //            if (
    //                !NodeByCenter[LeftNodeCenter]->BeRender &&
    //                NodeByCenter[LeftNodeCenter]->NotRender_Reason == NotRenderReason::None
    //            )
    //            {
    //                BeNeedRepair = true;
    //                if ( !RepairDatas.findkey ( RepairType::Left ) )
    //                {
    //                    RepairDatas.Insert( RepairType::Left, true );
    //                }
    //                QNode* OthersParent = NodeByCenter[LeftNodeCenter]->Parent;
    //                QNode* MyParent = Parent;

    //                while ( MyParent != nullptr )
    //                {
    //                    if ( OthersParent->NotRender_Reason == NotRenderReason::None )
    //                    {
    //                        MyParent->BeNeedRepair = true;
    //                        if ( !MyParent->RepairDatas.findkey ( RepairType::Left ) )
    //                        {
    //                            MyParent->RepairDatas.Insert ( RepairType::Left, true );
    //                        }
    //                        MyParent = MyParent->Parent;
    //                        OthersParent = OthersParent->Parent;

    //                    }
    //                    else if ( OthersParent->NotRender_Reason == NotRenderReason::LevelHigh )
    //                    {
    //                        break;
    //                    }
    //                }
    //            }
    //        }
    //        if ( NodeByCenter->findkey ( RightNodeCenter ) )
    //        {
    //            if (
    //                !NodeByCenter[RightNodeCenter]->BeRender &&
    //                NodeByCenter[RightNodeCenter]->NotRender_Reason == NotRenderReason::None
    //            )
    //            {
    //                BeNeedRepair = true;
    //                if ( !RepairDatas.findkey ( RepairType::Right ) )
    //                {
    //                    RepairDatas.Insert ( RepairType::Right, false );
    //                }


    //                QNode* OthersParent = NodeByCenter[RightNodeCenter]->Parent;
    //                QNode* MyParent = Parent;
    //                while ( MyParent != nullptr )
    //                {
    //                    if ( OthersParent->NotRender_Reason == NotRenderReason::None )
    //                    {
    //                        MyParent->BeNeedRepair = true;
    //                        if ( !MyParent->RepairDatas.findkey ( RepairType::Right ) )
    //                        {
    //                            MyParent->RepairDatas.Insert ( RepairType::Right, true );
    //                        }
    //                        MyParent = MyParent->Parent;
    //                        OthersParent = OthersParent->Parent;

    //                    }
    //                    else if ( OthersParent->NotRender_Reason == NotRenderReason::LevelHigh )
    //                    {
    //                        break;
    //                    }
    //                }
    //            }
    //        }
    //        if ( NodeByCenter->findkey ( TopNodeCenter ) )
    //        {
    //            if (
    //                !NodeByCenter[TopNodeCenter]->BeRender &&
    //                NodeByCenter[TopNodeCenter]->NotRender_Reason == NotRenderReason::None
    //            )
    //            {
    //                BeNeedRepair = true;
    //                if ( !RepairDatas.findkey ( RepairType::Top ) )
    //                {
    //                    RepairDatas.Insert ( RepairType::Top, true );
    //                }


    //                QNode* OthersParent = NodeByCenter[TopNodeCenter]->Parent;
    //                QNode* MyParent = Parent;
    //                while ( MyParent != nullptr )
    //                {
    //                    if ( OthersParent->NotRender_Reason == NotRenderReason::None )
    //                    {
    //                        MyParent->BeNeedRepair = true;
    //                        if ( !MyParent->RepairDatas.findkey ( RepairType::Top ) )
    //                        {
    //                            MyParent->RepairDatas.Insert ( RepairType::Top, true );
    //                        }
    //                        MyParent = MyParent->Parent;
    //                        OthersParent = OthersParent->Parent;

    //                    }
    //                    else if ( OthersParent->NotRender_Reason == NotRenderReason::LevelHigh )
    //                    {
    //                        break;
    //                    }
    //                }
    //            }
    //        }
    //        if ( NodeByCenter->findkey ( BottomNodeCenter ) )
    //        {
    //            if (
    //                !NodeByCenter[BottomNodeCenter]->BeRender &&
    //                NodeByCenter[BottomNodeCenter]->NotRender_Reason == NotRenderReason::None
    //            )
    //            {
    //                BeNeedRepair = true;
    //                if ( !RepairDatas.findkey ( RepairType::Bottom ) )
    //                {
    //                    RepairDatas.Insert ( RepairType::Bottom, true );
    //                }
    //                QNode* OthersParent = NodeByCenter[BottomNodeCenter]->Parent;
    //                QNode* MyParent = Parent;


    //                while ( MyParent != nullptr )
    //                {
    //                    if ( OthersParent->NotRender_Reason == NotRenderReason::None )
    //                    {
    //                        MyParent->BeNeedRepair = true;
    //                        if ( !MyParent->RepairDatas.findkey ( RepairType::Bottom ) )
    //                        {
    //                            MyParent->RepairDatas.Insert ( RepairType::Bottom, true );
    //                        }
    //                        MyParent = MyParent->Parent;
    //                        OthersParent = OthersParent->Parent;
    //                    }
    //                    else if ( OthersParent->NotRender_Reason == NotRenderReason::LevelHigh )
    //                    {
    //                        break;
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    //if ( mChildren != nullptr )
    //{
    //    for ( int i = 0; i < 4; i++ )
    //    {
    //        mChildren[i]->CheckShouldRepair();
    //    }
    //}
}

void GameEditor::GTerrainNode::Repair()
{
    return;
    //if ( BeNeedRepair )
    //{
    //    List<int> resList = new List<int>();
    //    int* res = nullptr;

    //    RepairTimes = 0;
    //    foreach ( KeyValuePair<RepairType, bool> p in RepairDatas )
    //    {
    //        res = RepairCrack ( this, p.Key );
    //        if ( res != nullptr )
    //        {
    //            for ( int i = 0; i < res.Length; i++ )
    //            {
    //                resList.Add ( res[i] );
    //            }
    //            RepairTimes++;
    //        }
    //    }

    //    int* indices ;
    //    IBuffer->Lock ( 0, 0, ( void** ) &indices, D3DLOCK_DISCARD );
    //    for ( int i = 0; i < resList.Count; i++ )
    //    {
    //        indices[24 + i] = resList[i];
    //    }
    //    IBuffer->Unlock();
    //}
    //if ( mChildren != nullptr )
    //{
    //    for ( int i = 0; i < 4; i++ )
    //    {
    //        mChildren[i].Repair();
    //    }
    //}
}

int* GameEditor::GTerrainNode::RepairCrack ( GTerrainNode* node, RepairType t )
{
    int* indices = new int[6];
    switch ( t )
    {
    case RepairType::Left:
        indices[0] = node->mConner[LeftTop];
        indices[1] = node->mConner[LeftBottom];
        indices[2] = ( indices[0] + indices[1] ) / 2;


        indices[3] = node->mConner[LeftTop];
        indices[4] = node->mConner[LeftBottom];
        indices[5] = ( indices[3] + indices[4] ) / 2;


        break;
    case RepairType::Bottom:
        indices[0] = node->mConner[LeftBottom];
        indices[1] = node->mConner[RightBotttom];
        indices[2] = ( indices[0] + indices[1] ) / 2;


        indices[3] = node->mConner[LeftBottom];
        indices[4] = node->mConner[RightBotttom];
        indices[5] = ( indices[3] + indices[4] ) / 2;

        break;

    case RepairType::Right:
        indices[0] = node->mConner[RightTop];
        indices[1] = node->mConner[RightBotttom];
        indices[2] = ( indices[0] + indices[1] ) / 2;


        indices[3] = node->mConner[RightTop];
        indices[4] = node->mConner[RightBotttom];
        indices[5] = ( indices[3] + indices[4] ) / 2;

        break;

    case RepairType::Top:

        indices[0] = node->mConner[LeftTop];
        indices[1] = node->mConner[RightTop];
        indices[2] = ( indices[0] + indices[1] ) / 2;


        indices[3] = node->mConner[LeftTop];
        indices[4] = node->mConner[RightTop];
        indices[5] = ( indices[3] + indices[4] ) / 2;

        break;

    }

    return indices;
}

void GameEditor::GTerrainNode::ClipByCamera ( GCamera* camera )
{
    return;
    //NotRender_Reason = NotRenderReason::None;
    //if ( !Camera->Frustum.IsInFrustum ( mBound ) )
    //{
    //    BeRender = false;
    //    NotRender_Reason = NotRenderReason.NotInEye;
    //    return;
    //}

    //if ( Level == 1 )
    //{
    //    BeRender = true;
    //    return;
    //}

    //float Distance = ( mBound->mCenter - Camera->Frustum.CameraPos ).Length();

    //float TargetLevel =
    //    Distance / GxMap::_DisFactor;
    ////(float)pow(Distance / GxMap::_DisFactor, GxMap::_Power);

    //if ( TargetLevel < Level )
    //{
    //    BeRender = false;
    //    NotRender_Reason = NotRenderReason::LevelHigh;
    //    for ( int i = 0; i < 4; i++ )
    //    {
    //        if ( mChildren[i] != nullptr )
    //            mChildren[i]->ClipByCamera ( camera );
    //    }
    //}
    //else
    //{
    //    BeRender = true;
    //}
}

GameEditor::GTerrainNode::~GTerrainNode()
{
    dSafeDelete ( mBound );
    dSafeDelete ( mConner );

for ( auto c: mChildren )
    {
        if ( c != nullptr )
        {
            dSafeDelete ( c );
        }
    }
}

EXVertex* GameEditor::GTerrainNode::VBData = nullptr;


GameEditor::GTerrainNode::QNodeMaps GameEditor::GTerrainNode::mNodeMaps;

GameEditor::GxMap::GxMap ( int Level, float cellWidth )
{
    File_HeightMap = "..\\Res\\Terrain\\heightmap.bmp";
    mHeightMap = nullptr;
    mRootNode = nullptr;
    Width = 1.0f;
    DisplayRepairAreaOnly = false;
    RepairMode = true;
    ReapirLevelTwo = true;
    mAlphaSplatMap = nullptr;
    BeHasAlphasplatMap = true;
    BeSaveAlphaSplat = false;
    File_AlphaSplat = "..\\Res\\Terrain\\TerrainAlphaSplat.png";
    File_Effect = "..\\Res\\Terrain\\TerrainEffect.fx";
    File_BrushConfig = "..\\Res\\Terrain\\Brushs\\Brushs.xml";
    TerrainEffect = nullptr;
    EH_Diffuse = nullptr;
    mOriginalIndexBuffer = nullptr;

    mVertexBuffer = nullptr;
    Text = nullptr;
    BeHasHeightMap = false;
    Camera = nullptr;
    //GxComponentFog TerrainFog = new GxComponentFog();
//   SetComponent ( TerrainFog );
    //GxComponentColour cc = (GxComponentColour)GetComponent(GxComponentType.COLOUR);
    //cc.UseTextureColor = false;

    //Name = typeof ( GxMap ).ToString();
    //VetexNumber=(2^Level+1)*(2^Level+1);
    //NodeNumber=4^0+4^1+4^2+...+4^(Level-1)=(1-4^Level)/(1-4);

    //UpdateEx += new GameUpdateEventHandler ( updateEx );

    //Camera = ( GxCamera ) GxGameObj.GetObjByName ( "DefaultCamera" );

    Width = cellWidth;
    Text = TextureMgr->getResource ( "..\\Res\\Box\\001.bmp" );
    //Mtrl.Diffuse = Color.FromArgb ( 255, 255, 255, 255 );
    dMakeColor ( Mtrl.Diffuse, 255, 255, 255, 255 );
    mRootNode = new GTerrainNode ( Level, GTerrainNode::RootPos );
    mRootNode->Build();

    LoadHeightMap();

    LoadAlphaMap();
    LoadBrushs();
    LoadEffect();

    CreateVertexBuffer();
    mRootNode->SetVertexBuffer ( mVertexBuffer );

    //平滑处理
    SmoothProcess();

    ComputerNormals();
}

void GameEditor::GxMap::LoadAlphaMap()
{
    if ( !IsFileExist ( File_AlphaSplat.c_str() ) )
    {
        BeHasAlphasplatMap = false;
        return;
    }
    mAlphaSplatMap = new GBitmap ( File_AlphaSplat );

    if ( mAlphaSplatMap->Width != GTerrainNode::LineCount )
    {
        //CXASSERT ( 0 && "AlphaMap的宽度必须等于地图的尺寸加一." );
    }
}

void GameEditor::GxMap::updateEx()
{
    //if ( GxInput.instance.isKeyUp ( Key.O ) )
    //{
    //    DisplayRepairAreaOnly = !DisplayRepairAreaOnly;
    //}
    //if ( GxInput.instance.isKeyUp ( Key.M ) )
    //{
    //    RepairMode = !RepairMode;
    //}
    //if ( GxInput.instance.isKeyUp ( Key.T ) )
    //{
    //    RepairMode = !ReapirLevelTwo;
    //}
    //if ( BeSaveAlphaSplat )
    //{
    //    SaveAlphaSplat();
    //    BeSaveAlphaSplat = false;
    //}
    ////GxPick.Pick(this);

    //RootNode->Reset();
    //Camera->Cliper ( this );
    //RootNode->ClipByCamera ( Camera );


    //if ( RepairMode )
    //{
    //    RootNode->CheckShouldRepair();
    //    RootNode->Repair();
    //}
}

void GameEditor::GxMap::LoadBrushs()
{
    //BrushSets = new Dictionary<string, string>();
    //Texts = new Dictionary<string, Texture>();

    //XmlDocument doc = new XmlDocument();
    //doc.Load ( File_BrushConfig );
    //XmlNode root = doc.SelectSingleNode ( "Brushs" );
    //XmlNodeList xnl = root.ChildNodes;
    //foreach ( XmlNode xnf in xnl )
    //{
    //    XmlElement xe = ( XmlElement ) xnf;
    //    string file = xe.GetAttribute ( "File" );
    //    string color = xe.GetAttribute ( "Color" );

    //    BrushSets.Add ( file, color );

    //    string fileEx = file.Insert ( 0, "..\\Res\\Terrain\\Brushs\\" );
    //    Texts.Add ( color, GxTexturePool.GetTexture ( fileEx ) );
    //}
}

void GameEditor::GxMap::LoadEffect()
{
    //TerrainEffect = GxEffect.GetEffect ( File_Effect );

    //Paras = new Dictionary<string, EffectHandle>();

    //Paras.Add ( "A", EffectHandle.FromString ( "xTexture0" ) );
    //Paras.Add ( "R", EffectHandle.FromString ( "xTexture1" ) );
    //Paras.Add ( "G", EffectHandle.FromString ( "xTexture2" ) );
    //Paras.Add ( "B", EffectHandle.FromString ( "xTexture3" ) );

    //EH_Diffuse = EffectHandle.FromString ( "Diffuse" );

    //foreach ( KeyValuePair<string, EffectHandle> ect in Paras )
    //{
    //    TerrainEffect.SetValue ( ect.Value, Texts[ect.Key] );
    //}
}

bool GameEditor::GxMap::render()
{
    if ( !__super::render() )
        return false;

    GetWorldMatrix ( false );

    //必须的，否则默认使用顶点颜色做光照
    D9DEVICE->GetDvc()->SetStreamSource ( 0, mVertexBuffer, 0, sizeof ( EXVertex ) );
    D9DEVICE->GetDvc()->SetFVF ( EXVertex::Format );
    D9DEVICE->GetDvc()->SetTexture ( 0, nullptr );
    mRootNode->Render();

    //DVC.RenderState.DiffuseMaterialSource = ColorSource.Material;
    //DVC.Material = Mtrl;
    //DVC.VertexFormat = EXVertex.Format;
    //DVC.VertexDeclaration = EXVertex.Vertex_Declaration;
    //DVC.SetStreamSource ( 0, VBuffer, 0 );
    //TerrainEffect.SetValue ( EH_Diffuse, Mtrl.DiffuseColor );
    //TerrainEffect.SetValue (
    //    EffectHandle.FromString ( "matWorldViewProj" ),
    //    Transform.getTransfrom() * GxDevice.DVC.Transform.View * GxDevice.DVC.Transform.Projection
    //);
    //TerrainEffect.Begin ( FX.None );
    //TerrainEffect.BeginPass ( 0 );
    //RootNode->Render();
    //TerrainEffect.EndPass();
    //TerrainEffect.End();
    return true;
}

void GameEditor::GxMap::CreateVertexBuffer()
{
    D9DEVICE->GetDvc()->CreateVertexBuffer (
        ( GTerrainNode::LineCount ) * ( GTerrainNode::LineCount ) * sizeof ( EXVertex )
        , 0, EXVertex::Format
        , D3DPOOL_MANAGED/*D3DPOOL_MANAGED*/
        , &mVertexBuffer
        , 0
    );

    if ( mVertexBuffer != nullptr )
    {
        EXVertex pos;
        EXVertex* data;
        mVertexBuffer->Lock ( 0, 0, ( void** ) &data, D3DLOCK_DISCARD );

        float fHeight = 0.0f;
        float fX = 0.0f;
        float fZ = 0.0f;
        for ( int i = 0; i < GTerrainNode::LineCount; i++ )
        {
            for ( int j = 0; j < GTerrainNode::LineCount; j++ )
            {
                fX = ( j - GTerrainNode::CellCount / 2.0f ) * Width;
                fZ = ( i - GTerrainNode::CellCount / 2.0f ) * Width;
                if ( BeHasHeightMap )
                {
                    fHeight = mHeightMap->GetHeight ( fX, fZ );
                }
                else
                {
                    fHeight = 0;
                }
                pos.Pos = D3DXVECTOR3 ( fX, fHeight, fZ );

                if ( BeHasAlphasplatMap )
                {
                    //CXColor cl = AlphaSplatMap->GetPixel ( j, QNode::LineCount - i - 1 );
                    //pos.Txt1 = new D3DXVECTOR4 ( cl.R / 255.0f, cl.G / 255.0f, cl.B / 255.0f, cl.A / 255.0f );
                }

                //pos.Color = Color.Red.ToArgb();
                pos.Color = 0XFFFFFFFF;
                pos.Tu = j;
                pos.Tv = GTerrainNode::CellCount - i;

                data[ ( GTerrainNode::LineCount ) * i + j] = pos;
            }
        }
        mVertexBuffer->Unlock();
    }
}

void GameEditor::GxMap::LoadHeightMap()
{
    if ( !IsFileExist ( File_HeightMap.c_str() ) )
    {
        BeHasHeightMap = false;
        return;
    }
    mHeightMap = new GxHeightMap (
        ( float ) ( Width * ( pow ( 2, GTerrainNode::RootLevel ) ) ),
        ( float ) ( Width * ( pow ( 2, GTerrainNode::RootLevel ) ) ),
        -10.0f, 40.0f,
        File_HeightMap );
    //BeHasHeightMap = false;
}

void GameEditor::GxMap::ComputerNormals()
{
    //计算每个面的法线
    if ( mOriginalIndexBuffer == nullptr )
    {
        mOriginalIndexBuffer = new CXBuffer();
        mOriginalIndexBuffer->setElementByteCount ( sizeof ( int ) );
        mOriginalIndexBuffer->reAllocateByElementCount ( GTerrainNode::CellCount * GTerrainNode::CellCount * 2 * 3 );
        int Stride = GTerrainNode::LineCount;
        for ( int i = 0; i < GTerrainNode::CellCount / 2; i++ )
        {
            for ( int j = 0; j < GTerrainNode::CellCount / 2; j++ )
            {
                u32 BaseIndex = ( i * GTerrainNode::CellCount / 2 + j ) * 24;
                mOriginalIndexBuffer->set ( BaseIndex + 0 , ( i * 2 + 1 ) * Stride + j * 2 + 1		);
                mOriginalIndexBuffer->set ( BaseIndex + 1 , ( i * 2 ) * Stride + j * 2				);
                mOriginalIndexBuffer->set ( BaseIndex + 2 , ( i * 2 + 1 ) * Stride + j * 2			);

                mOriginalIndexBuffer->set ( BaseIndex + 3 , ( i * 2 + 1 ) * Stride + j * 2 + 1		);
                mOriginalIndexBuffer->set ( BaseIndex + 4 , ( i * 2 ) * Stride + j * 2 + 1			);
                mOriginalIndexBuffer->set ( BaseIndex + 5 , ( i * 2 ) * Stride + j * 2				);

                mOriginalIndexBuffer->set ( BaseIndex + 6 , ( i * 2 + 1 ) * Stride + j * 2 + 1		);
                mOriginalIndexBuffer->set ( BaseIndex + 7 , ( i * 2 ) * Stride + ( j + 1 ) * 2		);
                mOriginalIndexBuffer->set ( BaseIndex + 8 , ( i * 2 ) * Stride + j * 2 + 1			);

                mOriginalIndexBuffer->set ( BaseIndex + 9 , ( i * 2 + 1 ) * Stride + j * 2 + 1		);
                mOriginalIndexBuffer->set ( BaseIndex + 10, ( i * 2 + 1 ) * Stride + ( j + 1 ) * 2	);
                mOriginalIndexBuffer->set ( BaseIndex + 11, ( i * 2 ) * Stride + j * 2 + 1			);

                mOriginalIndexBuffer->set ( BaseIndex + 12, ( i * 2 + 1 ) * Stride + j * 2 + 1			);
                mOriginalIndexBuffer->set ( BaseIndex + 13, ( ( i + 1 ) * 2 ) * Stride + ( j + 1 ) * 2	);
                mOriginalIndexBuffer->set ( BaseIndex + 14, ( i * 2 + 1 ) * Stride + ( j + 1 ) * 2		);

                mOriginalIndexBuffer->set ( BaseIndex + 15, ( i * 2 + 1 ) * Stride + j * 2 + 1			);
                mOriginalIndexBuffer->set ( BaseIndex + 16, ( ( i + 1 ) * 2 ) * Stride + j * 2 + 1		);
                mOriginalIndexBuffer->set ( BaseIndex + 17, ( ( i + 1 ) * 2 ) * Stride + ( j + 1 ) * 2	);

                mOriginalIndexBuffer->set ( BaseIndex + 18, ( i * 2 + 1 ) * Stride + j * 2 + 1			);
                mOriginalIndexBuffer->set ( BaseIndex + 19, ( i * 2 + 1 ) * Stride + j * 2				);
                mOriginalIndexBuffer->set ( BaseIndex + 20, ( ( i + 1 ) * 2 ) * Stride + j * 2 + 1		);

                mOriginalIndexBuffer->set ( BaseIndex + 21, ( i * 2 + 1 ) * Stride + j * 2 + 1			);
                mOriginalIndexBuffer->set ( BaseIndex + 22, ( i * 2 + 1 ) * Stride + j * 2				);
                mOriginalIndexBuffer->set ( BaseIndex + 23, ( ( i + 1 ) * 2 ) * Stride + j * 2 + 1		);

            }
        }
    }


    EXVertex* VData;
    mVertexBuffer->Lock ( 0, 0, ( void** ) &VData, D3DLOCK_DISCARD );
    int normalCnt = mOriginalIndexBuffer->size() / 3;
    D3DXVECTOR3* Normals = new D3DXVECTOR3[normalCnt];

    D3DXVECTOR3 V[3];

    for ( int i = 0; i < normalCnt; i++ )
    {
        V[0] = VData[mOriginalIndexBuffer->get<u32> ( i * 3 )].Pos;
        V[1] = VData[mOriginalIndexBuffer->get<u32> ( i * 3 + 1 )].Pos;
        V[2] = VData[mOriginalIndexBuffer->get<u32> ( i * 3 + 2 )].Pos;
        dVector3Cross ( Normals[i], V[1] - V[0], V[2] - V[0] );
        dVector3Normalize ( Normals[i] );
    }

    for ( int i = 0; i < GTerrainNode::CellCount / 2; i++ )
    {
        for ( int j = 0; j < GTerrainNode::CellCount / 2; j++ )
        {
            int VertexBaseIndex = ( i * GTerrainNode::CellCount / 2 + j ) * 24;
            int FaceBaseIndex = ( i * GTerrainNode::CellCount / 2 + j ) * 8;

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 0 + 0 )].Normal = (
                        Normals[FaceBaseIndex + 0] +
                        Normals[FaceBaseIndex + 1] +
                        Normals[FaceBaseIndex + 2] +
                        Normals[FaceBaseIndex + 3] +
                        Normals[FaceBaseIndex + 4] +
                        Normals[FaceBaseIndex + 5] +
                        Normals[FaceBaseIndex + 6] +
                        Normals[FaceBaseIndex + 7]
                    );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 0 + 0 )].Normal );

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 0 + 1 )].Normal += dVector3NormalizeSlow ( Normals[FaceBaseIndex + 0] + Normals[FaceBaseIndex + 1] );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 0 + 1 )].Normal );

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 1 + 1 )].Normal += dVector3NormalizeSlow ( ( Normals[FaceBaseIndex + 1] + Normals[FaceBaseIndex + 2] ) );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 1 + 1 )].Normal );

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 2 + 1 )].Normal += dVector3NormalizeSlow ( ( Normals[FaceBaseIndex + 3] + Normals[FaceBaseIndex + 2] ) );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 2 + 1 )].Normal );

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 3 + 1 )].Normal += dVector3NormalizeSlow ( ( Normals[FaceBaseIndex + 3] + Normals[FaceBaseIndex + 4] ) );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 3 + 1 )].Normal );

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 4 + 1 )].Normal += dVector3NormalizeSlow ( ( Normals[FaceBaseIndex + 5] + Normals[FaceBaseIndex + 4] ) );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 4 + 1 )].Normal );

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 5 + 1 )].Normal += dVector3NormalizeSlow ( ( Normals[FaceBaseIndex + 5] + Normals[FaceBaseIndex + 6] ) );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 5 + 1 )].Normal );

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 6 + 1 )].Normal += dVector3NormalizeSlow ( ( Normals[FaceBaseIndex + 7] + Normals[FaceBaseIndex + 6] ) );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 6 + 1 )].Normal );

            VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 7 + 1 )].Normal += dVector3NormalizeSlow ( ( Normals[FaceBaseIndex + 7] + Normals[FaceBaseIndex + 0] ) );
            dVector3Normalize ( VData[mOriginalIndexBuffer->get<u32> ( VertexBaseIndex + 3 * 7 + 1 )].Normal );

        }
    }
    dSafeDeleteArray ( Normals );
    dSafeDelete ( mOriginalIndexBuffer );
    mVertexBuffer->Unlock();
}

void GameEditor::GxMap::SmoothProcess()
{
    EXVertex* data;
    mVertexBuffer->Lock ( 0, 0, ( void** ) &data, D3DLOCK_DISCARD );
    for ( int i = 1; i < GTerrainNode::CellCount; i++ )
    {
        for ( int j = 1; j < GTerrainNode::CellCount; j++ )
        {
            data[i * GTerrainNode::LineCount + j].Pos.y = 1.0f / 9.0f * (
                        data[ ( i + 1 ) * GTerrainNode::LineCount + j - 1].Pos.y +
                        data[ ( i + 1 ) * GTerrainNode::LineCount + j].Pos.y +
                        data[ ( i + 1 ) * GTerrainNode::LineCount + j + 1].Pos.y +

                        data[i * GTerrainNode::LineCount + j - 1].Pos.y +
                        data[i * GTerrainNode::LineCount + j].Pos.y +
                        data[i * GTerrainNode::LineCount + j + 1].Pos.y +

                        data[ ( i - 1 ) * GTerrainNode::LineCount + j - 1].Pos.y +
                        data[ ( i - 1 ) * GTerrainNode::LineCount + j].Pos.y +
                        data[ ( i - 1 ) * GTerrainNode::LineCount + j + 1].Pos.y
                    );
        }
    }
    mVertexBuffer->Unlock();
}

void GameEditor::GxMap::AlterHeight ( HitInfo* HI, EXVertex* MyVB )
{
    if ( HI == nullptr || MyVB == nullptr || !EditHeight )
        return;

    float HeightDelta = 0.01f;

    MyVB[HI->Index[0]].Pos.y += HeightDelta;
    MyVB[HI->Index[1]].Pos.y += HeightDelta;
    MyVB[HI->Index[2]].Pos.y += HeightDelta;

    mHeightMap->SetHeight ( MyVB[HI->Index[0]].Pos.x, MyVB[HI->Index[0]].Pos.z, ( byte ) ( MyVB[HI->Index[0]].Pos.y ) );
    mHeightMap->SetHeight ( MyVB[HI->Index[1]].Pos.x, MyVB[HI->Index[1]].Pos.z, ( byte ) ( MyVB[HI->Index[1]].Pos.y ) );
    mHeightMap->SetHeight ( MyVB[HI->Index[2]].Pos.x, MyVB[HI->Index[2]].Pos.z, ( byte ) ( MyVB[HI->Index[2]].Pos.y ) );
}

void GameEditor::GxMap::CheckValue ( D3DXVECTOR4& v )
{
    //if (X>1.0f)
    //{
    //    X = 1.0f;
    //}
    float Sum = v.x + v.y + v.z + v.w;
    v.x /= Sum;
    v.y /= Sum;
    v.z /= Sum;
    v.w /= Sum;
}

bool GameEditor::GxMap::CheckIndexInvalid ( int Index )
{
    return Index >= 0 && Index < GTerrainNode::LineCount * GTerrainNode::LineCount;
}

void GameEditor::GxMap::AlterFace ( HitInfo* HI, EXVertex* MyVB )
{
    if ( HI == nullptr || MyVB == nullptr || !EditFace )
        return;
    mAlterFaceIndexListMap.clear();

    mAlterFaceIndexListMap.Insert ( 0, new AlterFaceList );

    for ( int i = 0; i < 3; i++ )
    {
        mAlterFaceIndexListMap[0]->push_back ( HI->Index[i] );
    }
    int tmp = 0;
    for ( int j = 1; j < 2; j++ )
    {
        mAlterFaceIndexListMap.Insert ( j, new AlterFaceList );
        if ( !mAlterFaceIndexListMap.findkey ( j + 1 ) )
        {
            mAlterFaceIndexListMap.Insert ( j + 1, new AlterFaceList );
        }
        for ( int i = 0; i < 3; i++ )
        {
            tmp = HI->Index[i] + 1;
            if ( !mAlterFaceIndexListMap[j - 1]->Find ( tmp ) && !mAlterFaceIndexListMap[j]->Find ( tmp ) )
            {
                mAlterFaceIndexListMap[j]->push_back ( tmp );
            }
        }
        for ( int i = 0; i < 3; i++ )
        {
            tmp = HI->Index[i] - 1;

            if ( !mAlterFaceIndexListMap[j - 1]->Find ( tmp ) && !mAlterFaceIndexListMap[j]->Find ( tmp ) )
            {
                mAlterFaceIndexListMap[j]->push_back ( tmp );
            }
        }
        for ( int i = 0; i < 3; i++ )
        {
            tmp = HI->Index[i] + GTerrainNode::LineCount;

            if ( !mAlterFaceIndexListMap[j - 1]->Find ( tmp ) && !mAlterFaceIndexListMap[j]->Find ( tmp ) )
            {
                mAlterFaceIndexListMap[j]->push_back ( tmp );
            }
        }
        for ( int i = 0; i < 3; i++ )
        {
            tmp = HI->Index[i] - GTerrainNode::LineCount;

            if ( !mAlterFaceIndexListMap[j - 1]->Find ( tmp ) && !mAlterFaceIndexListMap[j]->Find ( tmp ) )
            {
                mAlterFaceIndexListMap[j]->push_back ( tmp );
            }
        }


        //for (int i = 0; i < 3; i++)
        //{
        //    tmp=HI->Index[i] + QNode::LineCount+1;

        //    if (!IndexList[j].Contains(tmp) && !IndexList[j - 1].Contains(tmp))
        //    {
        //        IndexList[j+1].Add(tmp);
        //    }
        //}
        //for (int i = 0; i < 3; i++)
        //{
        //    tmp=HI->Index[i] + QNode::LineCount-1;

        //    if (!IndexList[j ].Contains(tmp) && !IndexList[j-1].Contains(tmp))
        //    {
        //        IndexList[j + 1].Add(tmp);
        //    }
        //}

        //for (int i = 0; i < 3; i++)
        //{
        //    tmp=HI->Index[i] - QNode::LineCount+1;

        //    if (!IndexList[j].Contains(tmp) && !IndexList[j - 1].Contains(tmp))
        //    {
        //        IndexList[j + 1].Add(tmp);
        //    }
        //}
        //for (int i = 0; i < 3; i++)
        //{
        //    tmp=HI->Index[i] - QNode::LineCount-1;

        //    if (!IndexList[j].Contains(tmp) && !IndexList[j - 1].Contains(tmp))
        //    {
        //        IndexList[j + 1].Add(tmp);
        //    }
        //}
    }

    //float Delta = 0.5f;
    //float factor = 0.5f;
    //for ( int i = 0; i < 3; i++ )
    //{

    //    switch ( GxPlugin_TerrainTool.GetSingleton().CurrentBrush )
    //    {
    //    case "A":
    //        MyVB[HI->Index[i]].Txt1.W += Delta;
    //        break;
    //    case "R":
    //        MyVB[HI->Index[i]].Txt1.X += Delta;
    //        break;
    //    case "G":
    //        MyVB[HI->Index[i]].Txt1.Y += Delta;
    //        break;
    //    case "B":
    //        MyVB[HI->Index[i]].Txt1.Z += Delta;
    //        break;
    //    }
    //    CheckValue ( ref MyVB[HI->Index[i]].Txt1 );


    //}
    //foreach ( KeyValuePair<int, List<int>> il in mAlterFaceIndexListMap )
    //{
    //    foreach ( int inx in il.Value )
    //    {
    //        if ( CheckIndexInvalid ( inx ) )
    //        {
    //            switch ( GxPlugin_TerrainTool.GetSingleton().CurrentBrush )
    //            {
    //            case "R":
    //                MyVB[inx].Txt1.X += ( float ) ( pow ( factor, il.Key ) * Delta );
    //                break;
    //            case "G":
    //                MyVB[inx].Txt1.Y += ( float ) ( pow ( factor, il.Key ) * Delta );
    //                break;
    //            case "B":
    //                MyVB[inx].Txt1.Z += ( float ) ( pow ( factor, il.Key ) * Delta );
    //                break;
    //            case "A":
    //                MyVB[inx].Txt1.W += ( float ) ( pow ( factor, il.Key ) * Delta );
    //                break;
    //            }
    //            CheckValue ( ref MyVB[inx].Txt1 );
    //        }
    //    }
    //}
}

bool GameEditor::GxMap::Pick ( D3DXVECTOR3 orgin, D3DXVECTOR3 dir )
{
    CXDynaArray<HitInfo*> HIS ;
    mRootNode->Pick ( orgin, dir, HIS );

    if ( HIS.size() == 0 )
    {
        return false;
    }
    //HIS.Sort ( HitInfo.SortByDistance );
    HitInfo* HI = HIS[0];
    bool Hited = HI->Hited;
    /*************************************************************************/
    //修改高度
    if ( Hited )
    {

        EXVertex* MyVB;
        mVertexBuffer->Lock ( 0, 0, ( void** ) &MyVB, D3DLOCK_DISCARD );
        AlterHeight ( HI,  MyVB );
        AlterFace ( HI,  MyVB );

        mVertexBuffer->Unlock();
    }

    return Hited;
}

void GameEditor::GxMap::SaveAlphaSplat()
{
    if ( mAlphaSplatMap == nullptr )
    {
        //AlphaSplatMap = new GBitmap (
        //    QNode::LineCount,
        //    QNode::LineCount,
        //    PixelFormat.Format32bppArgb
        //);
    }


    //EXVertex* VData;
    //mVertexBuffer->Lock ( 0, 0, ( void** ) &VData, D3DLOCK_DISCARD );
    ////纹理的平滑处理
    //for (int i = 1; i < QNode::CellCount; i++)
    //{
    //    for (int j = 1; j < QNode::CellCount; j++)
    //    {
    //        VData[i*QNode::LineCount+j].Txt1=1.0f / 9.0f *
    //            (
    //            VData[(i + 1) * QNode::LineCount + j - 1].Txt1 +
    //            VData[(i + 1) * QNode::LineCount + j].Txt1 +
    //            VData[(i + 1) * QNode::LineCount + j + 1].Txt1 +

    //            VData[i * QNode::LineCount + j - 1].Txt1 +
    //            VData[i * QNode::LineCount + j].Txt1 +
    //            VData[i * QNode::LineCount + j + 1].Txt1 +

    //            VData[(i - 1) * QNode::LineCount + j - 1].Txt1 +
    //            VData[(i - 1) * QNode::LineCount + j].Txt1 +
    //            VData[(i - 1) * QNode::LineCount + j + 1].Txt1
    //            );
    //    }
    //}

    //if ( VData != nullptr )
    //{
    //    D3DXVECTOR4 VTmp = new D3DXVECTOR4 ( 0, 0, 0, 0 );
    //    for ( int i = 0; i < QNode::LineCount; i++ )
    //    {
    //        for ( int j = 0; j < QNode::LineCount; j++ )
    //        {
    //            VTmp = VData[ ( QNode::LineCount - j - 1 ) * QNode::LineCount + i].Txt1;
    //            AlphaSplatMap.SetPixel (
    //                i, j,
    //                Color.FromArgb (
    //                    ( int ) ( VTmp.X * 255.0f ),
    //                    ( int ) ( VTmp.Y * 255.0f ),
    //                    ( int ) ( VTmp.Z * 255.0f ),
    //                    ( int ) ( VTmp.W * 255.0f ) )
    //            );
    //        }
    //    }
    //    string tmpFile = "tmp.png";
    //    AlphaSplatMap.Save ( tmpFile, ImageFormat.Png );
    //    AlphaSplatMap.Dispose();
    //    File.Delete ( File_AlphaSplat );
    //    File.Move ( tmpFile, File_AlphaSplat );
    //    AlphaSplatMap = new GBitmap ( File_AlphaSplat );

    //    mVertexBuffer->Unlock();
    //}
}

GameEditor::GxMap::~GxMap()
{
    dSafeDelete ( mOriginalIndexBuffer );
    dSafeDelete ( mRootNode );
    dSafeDelete ( mAlphaSplatMap );
    dSafeDelete ( mHeightMap );
    GTerrainNode::mNodeMaps.destroySecond();
}

void GameEditor::GxMap::update()
{
    __super::update();
}

bool GameEditor::GxMap::DisplayRepairAreaOnly = false;


float GameEditor::GxMap::_DisFactor = 25.6f;
float GameEditor::GxMap::_Power = 1.2f;

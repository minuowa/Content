#ifndef GTerrainCommon_h__
#define GTerrainCommon_h__
#include <hash_map>

struct EXVertex
{
    D3DXVECTOR3 Pos;
    D3DXVECTOR3 Normal;
    u32 Color;
    float TU;
    float TV;
    D3DXVECTOR4 Wights;
    EXVertex();
    EXVertex ( float x, float y, float z, float nx, float ny, float nz, int color, float tu, float tv );

    static const int Format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2 | D3DFVF_TEX1;
};
CXDefineOnce D3DVERTEXELEMENT9 gTerrainVertexDeclartion[] =
{
    { 0, 0,   D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
    { 0, 24,  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
    { 0, 28,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    { 0, 36,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
    D3DDECL_END()
};
struct GCubeBound
{
    float mMinX;
    float mMaxX;
    float mMinY;
    float mMaxY;
    float mMinZ;
    float mMaxZ;
    D3DXVECTOR3 mCenter;
    float mRadius;
    GCubeBound();
    void updateRadius();
    void updateCenter();
};
struct HitInfo
{
    bool Hited;
    D3DXVECTOR3 HitPoint;
    int Index[3];
    float U ;
    float V ;
    float Distance;
    int TriangleIndex ;
    static int SortByDistance ( HitInfo a1, HitInfo a2 )
    {
        return a1.Distance == a2.Distance ;
    }
    HitInfo();
};
class GTerrainNode;
/** @brief center,node **/
typedef CXHashMap<int, GTerrainNode*> GCenter_NodesMap;
/** @brief level,nodesMap **/
typedef CXHashMap<int, GCenter_NodesMap*> GLevel_NodesMap;
#endif // GTerrainCommon_h__

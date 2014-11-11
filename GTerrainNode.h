#include "GTerrainCommon.h"


class GHeightMap;
struct GCubeBound;
struct HitInfo;
class GCamera;
struct EXVertex;
class GTerrain;
class GTerrainNode
{
public:
	enum ChildType
	{
		ChildLeftBottom,
		ChildRightBotttom,
		ChildRightTop,
		ChildLeftTop,
		ChildRoot,
		ChildCount = ChildRoot,
	};
public:
	static const u32 G_TERRAIN_FACE_NUM = 2;
	static const u32 G_TERRAIN_INDEX_NUM = 3;
	static const u32 G_TERRAIN_CELL_BASE_INDEX_NUM = ChildCount * G_TERRAIN_FACE_NUM * G_TERRAIN_INDEX_NUM;
	static const u32 G_TERRAIN_CELL_MAX_INDEX_NUM = G_TERRAIN_CELL_BASE_INDEX_NUM * 2;
public:
    enum RepairType
    {
        Left,
        Right,
        Top,
        Bottom,
		RepairTypeCount,
		NeighbourCount=RepairTypeCount,
    };

    enum CenterType
    {
        CenterType_LeftBottom,
        CenterType_RightBotttom,
        CenterType_RightTop,
        CenterType_LeftTop,
        CenterType_Max,
    };
    enum eCullResultType
    {
		eCullResultTypeNone,
        eCullResultTypeRender,
        eCullResultTypeNotInEye,	//不在视野内
        eCullResultTypeLevelHigh,	//level的等级过高
    };
public:
    GTerrainNode();
    virtual ~GTerrainNode();

    void setVertexBuffer ( IDirect3DVertexBuffer9* VB, GTerrain* owner );
    void reset();
    /** @brief add indices into terrain indexbuffer **/
	void addIndexToTerrain ( GTerrain* owner,bool lodMode );
    void pick ( D3DXVECTOR3 orgin, D3DXVECTOR3 dir, CXDynaArray<HitInfo*>& AllHits );


    bool buildIndexBuffer ( GTerrain* owner );
    void build ( GTerrain* owner, int level, ChildType pose = ChildRoot );
    void checkShouldRepair ( GTerrain* owner );
    void checkShouldRepair ( RepairType repairType );
    void repair();
    void cull ( GCamera* camera, GTerrain* owner );
    void buildBound ( GTerrain* owner );
	void buildNeighbour(GTerrain* owner);
	void repairCrack ( GTerrainNode* node, RepairType t , u32* buffer );
public:
	static void* operator new(unsigned int n)
	{
		return mObjectPool.acquireObject();
	}
	static void operator delete(void* p)
	{
		mObjectPool.releaseObject ( p );
	}
public :
    //3---------2
    //|         |
    //0---------1
    int mConner[CenterType_Max];
    int mCenter;
    ChildType mPose;
    //level=1的为叶子节点
    int mLevel ;
	GTerrainNode* mChildren[ChildCount];
	GTerrainNode* mNeighbour[NeighbourCount];
    GTerrainNode* mParentNode;
    GCubeBound* mBound;
    u32 mIndices[G_TERRAIN_CELL_MAX_INDEX_NUM];
    eCullResultType mCullResult;
	bool mCulledIndexType[ChildCount];
	bool mRepairIndexType[RepairTypeCount];
    bool mBeNeedRepair;
	bool mBeRender;
	int mRepairTimes;

	static IDirect3DVertexBuffer9* mVertexBuffer;
	//buildBound时使用，减少锁定与解锁的时间
	static EXVertex* mVertexData;
	static int RenderNodeCount;
	static CXObjectPool<GTerrainNode> mObjectPool;
};


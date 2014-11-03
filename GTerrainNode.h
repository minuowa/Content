
static const u32 G_TERRAIN_CHILD_NUM = 4;
static const u32 G_TERRAIN_FACE_NUM = 2;
static const u32 G_TERRAIN_INDEX_NUM = 3;
static const u32 G_TERRAIN_CELL_BASE_INDEX_NUM = G_TERRAIN_CHILD_NUM * G_TERRAIN_FACE_NUM * G_TERRAIN_INDEX_NUM;
static const u32 G_TERRAIN_CELL_MAX_INDEX_NUM = G_TERRAIN_CELL_BASE_INDEX_NUM * 2;
class GxHeightMap;
struct GCubeBound;
struct HitInfo;
class GCamera;
struct EXVertex;
class GTerrain;
class GTerrainNode
{
public:
    /** @brief center,node **/
    typedef CXMap<int, GTerrainNode*> GCenter_NodesMap;
    /** @brief level,nodesMap **/
    typedef CXMap<int, GCenter_NodesMap*> GLevel_NodesMap;
public:
    enum RepairType
    {
        Left,
        Right,
        Top,
        Bottom,
    };
    enum Pose
    {
        Pose_LeftBottom,
        Pose_RightBotttom,
        Pose_RightTop,
        Pose_LeftTop,
        Pose_Root,
        Pose_Max = Pose_Root,
    };
    enum CenterType
    {
        CenterType_LeftBottom,
        CenterType_RightBotttom,
        CenterType_RightTop,
        CenterType_LeftTop,
		CenterType_Max,
	};
    enum NotRenderReason
    {
        None,
        //不在视野内
        NotInEye,
        //level的等级过高
        LevelHigh,
    };
public:
    GTerrainNode ( int level, Pose pose = Pose_Root );
    ~GTerrainNode();

    void setVertexBuffer ( IDirect3DVertexBuffer9* VB, GTerrain* owner );
    void reset();
    /** @brief add indices into terrain indexbuffer **/
    void addIndexToTerrain ( GTerrain* owner );
    void pick ( D3DXVECTOR3 orgin, D3DXVECTOR3 dir, CXDynaArray<HitInfo*>& AllHits );


	bool buildIndexBuffer ( GTerrain* owner );
    void build(GTerrain* owner);
    void checkShouldRepair ( GTerrain* owner );
    void checkShouldRepair ( int center, GCenter_NodesMap& nodeMap, RepairType repairType );
    void repair();
	void clipByCamera ( GCamera* camera,GTerrain* owner );
    void buildBound ( GTerrain* owner );

    void repairCrack ( GTerrainNode* node, RepairType t , int* buffer );
protected:
public :
    static const int InvalidNumber = ~0;



    //第一个int为level，第二个int为center，同一个level的QNode的Center必不相同
    static GLevel_NodesMap mNodeMaps;

    typedef CXMap<RepairType, bool> RepairTypeMap;
    RepairTypeMap mRepairDatas;


    //3---------2
    //|         |
    //0---------1
    int mConner[CenterType_Max];
    int mCenter;



    static int RootPos;
    static int RootLevel;
    static int NodeCount;

    Pose mPose;


    //level=1的为叶子节点
    int Level ;

    GTerrainNode* mChildren[G_TERRAIN_CHILD_NUM];
    GTerrainNode* mParentNode;
    GCubeBound* mBound;
    u32 mIndices[G_TERRAIN_CELL_MAX_INDEX_NUM];
    static IDirect3DVertexBuffer9* mVertexBuffer;

    //buildBound时使用，减少锁定与解锁的时间
    static EXVertex* mVertexData;

    bool mBeRender;
    NotRenderReason mNotRenderReason;

    bool mBeNeedRepair;
    int mRepairTimes;
};


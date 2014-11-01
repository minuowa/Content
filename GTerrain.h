#ifndef GTerrain_h__
#define GTerrain_h__
#include "GNode.h"

class GBitmap;
class GCamera;
class GTexture;
class CXEffect;
struct EXVertex;
namespace GameEditor
{
	static const u32 G_TERRAIN_CHILD_NUM = 4;
	class GxHeightMap;
struct NodeBound
{
    float mMinX;
    float mMaxX;
    float mMinY;
    float mMaxY;
    float mMinZ;
    float mMaxZ;
    D3DXVECTOR3 mCenter;
    float Radius;
    NodeBound();
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
class GTerrainNode
{
public:
    enum RepairType
    {
        Left,
        Right,
        Top,
        Bottom,
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
	GTerrainNode ( int level, int pos );
	~GTerrainNode();

    void SetVertexBuffer ( IDirect3DVertexBuffer9* VB );
    void Reset();
    void Render();
    void Pick ( D3DXVECTOR3 orgin, D3DXVECTOR3 dir, CXDynaArray<HitInfo*>& AllHits );


    int* BuildIndexBuffer();
    void Build();
    void CheckShouldRepair();
    void Repair();
    void ClipByCamera ( GCamera* camera );
    void BuildBound();

    static int* RepairCrack ( GTerrainNode* node, RepairType t );
protected:
public :
    static const int InvalidNumber = ~0;
    typedef CXMap<int, GTerrainNode*> QNodeMap;
    typedef CXMap<int, QNodeMap*> QNodeMaps;
    //第一个int为level，第二个int为center，同一个level的QNode的Center必不相同
    static QNodeMaps mNodeMaps;

    typedef CXMap<RepairType, bool> RepairTypeMap;
    RepairTypeMap RepairDatas;


    //3---------2
    //|         |
    //0---------1
    int* mConner;
    int mCenter;
    static const int LeftBottom = 0;
    static const int RightBotttom = 1;
    static const int RightTop = 2;
    static const int LeftTop = 3;


    static int RootPos;
    static int RootLevel;
    static int NodeCount;
    static int CellCount;
    static int LineCount;

    int Pos;


    //level=1的为叶子节点
    int Level ;

    GTerrainNode* mChildren[G_TERRAIN_CHILD_NUM];
    GTerrainNode* Parent;
    NodeBound* mBound;
    IDirect3DIndexBuffer9* mIndexBuffer ;

    static IDirect3DVertexBuffer9* VBuffer;

    //buildBound时使用，减少锁定与解锁的时间
    static EXVertex* VBData;

    bool BeRender;
    NotRenderReason NotRender_Reason;

    bool BeNeedRepair;
    int RepairTimes;
};

class GxMap:public GNode
{
public:
    GxMap ( int Level, float cellWidth );
	~GxMap();
    void LoadAlphaMap();
    void updateEx ( );
	virtual void update();
	void LoadBrushs();
    void LoadEffect();
	virtual bool render() override;
    void CreateVertexBuffer();

    void LoadHeightMap();

    void ComputerNormals();
    void SmoothProcess();
    void AlterHeight ( HitInfo* HI, EXVertex* MyVB );

    void AlterFace ( HitInfo* HI, EXVertex* MyVB );
    bool Pick ( D3DXVECTOR3 orgin, D3DXVECTOR3 dir );

    void SaveAlphaSplat();
    static void CheckValue ( D3DXVECTOR4& v );
    static bool CheckIndexInvalid ( int Index );
public:
	static bool DisplayRepairAreaOnly;
protected:
    GString File_HeightMap;

    GxHeightMap* mHeightMap;
    GTerrainNode* mRootNode;

    float Width;

    bool RepairMode;
    bool ReapirLevelTwo;

    GBitmap* mAlphaSplatMap ;
    bool BeHasAlphasplatMap ;
    bool BeSaveAlphaSplat ;

    GString File_AlphaSplat ;
    GString File_Effect ;
    GString File_BrushConfig ;

    CXMap<GString, GString> BrushSets;

    CXEffect* TerrainEffect ;
    CXMap<GString, HANDLE> Paras ;
    CXMap<GString, GTexture*> Texts;
    HANDLE EH_Diffuse ;

    static float _DisFactor;
    static float _Power;

    float DisFactor;
    double Power;


	bool Editable ;
    bool EditHeight ;
    bool EditFace  ;
    float FogStart;
    float FogEnd;
    float FogDensity;
    //用来存储所有顶点的索引，目前为计算法线使用
    CXBuffer* mOriginalIndexBuffer;

    IDirect3DVertexBuffer9* mVertexBuffer;
    GTexture* Text;
    bool BeHasHeightMap  ;

    D3DMATERIAL9 Mtrl;
    GCamera* Camera  ;
    typedef CXDynaArray<int> AlterFaceList;
    typedef CXMap<int, AlterFaceList*> AlterFaceIndexListMap;
    AlterFaceIndexListMap mAlterFaceIndexListMap ;
};
}
#endif // GTerrain_h__

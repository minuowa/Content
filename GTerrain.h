#ifndef GTerrain_h__
#define GTerrain_h__
#include "GTerrainCommon.h"
#include "GRenderEntity.h"

class GBitmap;
class GCamera;
class GTexture;
class GEffect;
struct EXVertex;
struct HitInfo;
class GHeightMap;
class GTerrainBrush;
class GEffect;
enum eTerrainState
{
	eTerrainState_Editable,
	eTerrainState_EditFaceable,
	eTerrainState_EditFacing,
	eTerrainState_EditHeightable,
	eTerrainState_EditHeighting,
	eTerrrinState_UsingAlphasplatMap,
	eTerrainState_Repairable,
	eTerrainState_DisplayRepairArea,
	eTerrainState_DisplayRepairAreaOnly,
};
class GTerrain: public GRenderEntity
{
    DeclareEditorType ( GTerrain );
    DeclareFilmObjBase ( GTerrain, GRenderEntity );
public:
    GTerrain();
    ~GTerrain();
    void setRootLevel ( uchar level );
    void setCellWidth ( float width );

    void LoadAlphaMap();
    void updateEx ( );
    virtual void update();
    virtual bool render() override;
    bool loadBrushs();
    bool loadEffect();
    void createVertexBuffer();

    void loadHeightMap();

    void computerNormals();
    void smoothProcess();
    void AlterHeight ( HitInfo* HI, EXVertex* MyVB );

    void AlterFace ( HitInfo* HI, EXVertex* MyVB );
    bool Pick ( D3DXVECTOR3 orgin, D3DXVECTOR3 dir );

    void SaveAlphaSplat();
    void CheckValue ( D3DXVECTOR4& v );
    bool CheckIndexInvalid ( u32 Index );

    bool isDisplayRepairAreaOnly() const;
    u32 getLineCount() const;
    u32 getCellCount() const;
    inline uBuffer* getDynamicIndexBuffer() const;
    inline GLevel_NodesMap& getNodesMap();
    inline float getLODFactor() const;
    virtual bool recreate();
protected:
    void renderNodes();
    void setEffects();
    void setEffectConst();
    bool createNodes();
    bool createVertexDeclaretion();
    void copyToIndexBuffer();
protected:
    virtual void registerAllProperty();


    virtual void onPropertyChangeEnd ( void* cur );

    virtual void clear();

    virtual void onCallBack ( const CXDelegate& , CXEventArgs*  )override;

public:

    //第一个int为level，第二个int为center，同一个level的QNode的Center必不相同
    GLevel_NodesMap mNodeMaps;
public:
    bool mDisplayRepairAreaOnly;
    bool mDisplayRepairArea;
protected:
    u32 mRootLevel;
    int mCellCount;
    int mLineCount;
    float mHeightMax;
    float mHeightMin;

    GHeightMap* mHeightMap;
    GTerrainNode* mRootNode;

    float mCellWidth;
    /** @brief targetLevel=distance/mLODFactor **/
    float mLODFactor;

    bool mLODMode;

    GBitmap* mAlphaSplatMap ;
    bool mUsingHeightMap  ;

    uString mFileAlphaSplat ;
    uString mFileEffect ;
    uString mFileBrushConfig ;
    uString mFileHeightMap;

    uString mTerrainCountString;
    GTerrainBrush* mTerrainBrush;
    CXMap<uString, uString> BrushSets;

    GEffect* mTerrainEffect ;
    CXMap<uString, HANDLE> Paras ;
    CXMap<uString, GTexture*> Texts;

    float DisFactor;
    double Power;

	CXBitArray32 mTerrainState;
    float FogStart;
    float FogEnd;
    float FogDensity;
    //用来存储所有顶点的索引，目前为计算法线使用
    uBuffer* mOriginalIndexBuffer;

    uBuffer* mDynamicIndexBuffer;

    IDirect3DVertexBuffer9* mVertexBuffer;
    IDirect3DIndexBuffer9* mIndexBuffer;
    IDirect3DVertexDeclaration9 *mVertexDeclartion;

    GTexture* mTexture;

    D3DMATERIAL9 mMtrl;
    typedef CXDynaArray<int> AlterFaceList;
    typedef CXMap<int, AlterFaceList*> AlterFaceIndexListMap;
    AlterFaceIndexListMap mAlterFaceIndexListMap ;
};

inline uBuffer* GTerrain::getDynamicIndexBuffer() const
{
    return mDynamicIndexBuffer;
}
inline GLevel_NodesMap& GTerrain::getNodesMap()
{
    return mNodeMaps;
}
inline float GTerrain::getLODFactor() const
{
    return mLODFactor;
}

#endif // GTerrain_h__

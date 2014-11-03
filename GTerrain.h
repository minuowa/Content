#ifndef GTerrain_h__
#define GTerrain_h__
#include "GNode.h"

class GBitmap;
class GCamera;
class GTexture;
class CXEffect;
struct EXVertex;
struct HitInfo;
class GxHeightMap;
class GTerrainNode;
class GTerrain: public GNode
{
public:
    GTerrain ( int Level, float cellWidth );
    ~GTerrain();
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
    void CheckValue ( D3DXVECTOR4& v );
    bool CheckIndexInvalid ( u32 Index );

    bool isDisplayRepairAreaOnly() const;
    u32 getLineCount() const;
    u32 getCellCount() const;
	inline CXBuffer* getDynamicIndexBuffer() const;
public:
    bool mDisplayRepairAreaOnly;
protected:
    u32 mRootLevel;
    int mCellCount;
    int mLineCount;

    GxHeightMap* mHeightMap;
    GTerrainNode* mRootNode;

    float mCellWidth;

    bool mRepairMode;
    bool ReapirLevelTwo;

    GBitmap* mAlphaSplatMap ;
    bool BeHasAlphasplatMap ;
    bool BeSaveAlphaSplat ;

    GString File_AlphaSplat ;
    GString File_Effect ;
    GString File_BrushConfig ;
	GString mFileHeightMap;

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

	CXBuffer* mDynamicIndexBuffer;

    IDirect3DVertexBuffer9* mVertexBuffer;
    IDirect3DIndexBuffer9* mIndexBuffer;
    GTexture* mTexture;
    bool BeHasHeightMap  ;

    D3DMATERIAL9 Mtrl;
    GCamera* mCamera  ;
    typedef CXDynaArray<int> AlterFaceList;
    typedef CXMap<int, AlterFaceList*> AlterFaceIndexListMap;
    AlterFaceIndexListMap mAlterFaceIndexListMap ;
};

inline CXBuffer* GTerrain::getDynamicIndexBuffer() const
{
	return mDynamicIndexBuffer;
}
#endif // GTerrain_h__

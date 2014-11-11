#ifndef GTerrain_h__
#define GTerrain_h__
#include "GNode.h"
#include "GTerrainCommon.h"

class GBitmap;
class GCamera;
class GTexture;
class GEffect;
struct EXVertex;
struct HitInfo;
class GHeightMap;

class GTerrain: public GNode
{
	DeclareEditorType(GTerrain);
public:
	GTerrain();
	~GTerrain();
	void setRootLevel(uchar level);
	void setCellWidth(float width);
	void LoadAlphaMap();
	void updateEx ( );
	virtual void update();
	void LoadBrushs();
	void LoadEffect();
	virtual bool render() override;
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
	inline CXBuffer* getDynamicIndexBuffer() const;
	inline GLevel_NodesMap& getNodesMap();
	inline float getLODFactor() const;
	virtual bool recreate();
protected:
	bool createNodes();
protected:
	virtual void registerAllProperty();


	virtual void onPropertyChangeEnd( void* cur );

	virtual void clear();
public:

	//第一个int为level，第二个int为center，同一个level的QNode的Center必不相同
	GLevel_NodesMap mNodeMaps;
public:
	bool mDisplayRepairAreaOnly;
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
	bool ReapirLevelTwo;

	GBitmap* mAlphaSplatMap ;
	bool BeHasAlphasplatMap ;
	bool BeSaveAlphaSplat ;
	bool mUsingHeightMap  ;

	GString File_AlphaSplat ;
	GString File_Effect ;
	GString File_BrushConfig ;
	GString mFileHeightMap;

	GString mTerrainCountString;

	CXMap<GString, GString> BrushSets;

	GEffect* TerrainEffect ;
	CXMap<GString, HANDLE> Paras ;
	CXMap<GString, GTexture*> Texts;
	HANDLE EH_Diffuse ;

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

	D3DMATERIAL9 mMtrl;
	GCamera* mCamera  ;
	typedef CXDynaArray<int> AlterFaceList;
	typedef CXMap<int, AlterFaceList*> AlterFaceIndexListMap;
	AlterFaceIndexListMap mAlterFaceIndexListMap ;
};

inline CXBuffer* GTerrain::getDynamicIndexBuffer() const
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

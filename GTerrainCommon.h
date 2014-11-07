#ifndef GTerrainCommon_h__
#define GTerrainCommon_h__

struct EXVertex
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	int Color;
	float Tu;
	float Tv;
	//D3DXVECTOR4 Txt1;
	EXVertex();
	EXVertex ( float x, float y, float z, float nx, float ny, float nz, int color, float tu, float tv );
	//private static VertexElement[] _VertexElements = null;
	//public static VertexElement[] VertexElements
	//{
	//	get
	//	{
	//		if (_VertexElements == null)
	//		{

	//			_VertexElements = new VertexElement[6];
	//			_VertexElements[0] = new VertexElement(0, 0, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Position, 0);
	//			_VertexElements[1] = new VertexElement(0, sizeof(float) * 3, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Normal, 0);
	//			_VertexElements[2] = new VertexElement(0, sizeof(float) * 6, DeclarationType.Color, DeclarationMethod.Default, DeclarationUsage.Color, 0);
	//			_VertexElements[3] = new VertexElement(0, sizeof(float) * 6+sizeof(int), DeclarationType.Float2, DeclarationMethod.Default, DeclarationUsage.TextureCoordinate, 0);
	//			_VertexElements[4] = new VertexElement(0, sizeof(float) * 8 + sizeof(int), DeclarationType.Float4, DeclarationMethod.Default, DeclarationUsage.TextureCoordinate, 1);
	//			_VertexElements[5] = VertexElement.VertexDeclarationEnd;
	//		}
	//		return _VertexElements;
	//	}
	//}
	//private static VertexDeclaration _Vertex_Declaration = null;
	//public static VertexDeclaration Vertex_Declaration
	//{
	//	get
	//	{
	//		if (_Vertex_Declaration==null)
	//		{
	//			_Vertex_Declaration=new VertexDeclaration(GxDevice.DVC,EXVertex.VertexElements);
	//		}

	//		return _Vertex_Declaration;
	//	}
	//}

	static const int Format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 /*| D3DFVF_TEX1*/;
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
typedef CXMap<int, GTerrainNode*> GCenter_NodesMap;
/** @brief level,nodesMap **/
typedef CXMap<int, GCenter_NodesMap*> GLevel_NodesMap;
#endif // GTerrainCommon_h__

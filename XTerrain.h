#pragma once
#include "XBase.h"
#include "XVector3.h"
#include "GNode.h"
class CXTerrainNode;
struct CXTerrainVertex
{
	CXVector3	Pos;
	XI32		Color;
	f32		TU;
	f32		TV;
	static const XI32 FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};
class CXTerrain :
	public GNode
{
public:
	CXTerrain ( void );
	~CXTerrain ( void );

	bool Init();
	XI32 GetLineWidth() const;
	XI32 GetCellWidth() const;
private:
	bool CreateIndexBuffer();
	bool CreateVertexBuffer();

private:
	int	mLevel;
	int mWidth;
	IDirect3DVertexBuffer9* mVertexBuffer;
	IDirect3DIndexBuffer9*	mIndexBuffer;
	CXTerrainNode* mRoot;
};
inline
	XI32 CXTerrain::GetCellWidth() const
{
	return XI32 ( ::pow ( 2.0f, mLevel ) );
}
inline
	XI32 CXTerrain::GetLineWidth() const
{
	return GetCellWidth() + 1;
}


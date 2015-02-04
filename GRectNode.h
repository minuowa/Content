#pragma once
#include "gnode.h"
#include "gnode.h"
#include "GMeshData.h"
class GRectNode :
	public GNode
{
public:
	GRectNode ( void );
	~GRectNode ( void );

	virtual bool recreate();

	void setRect ( const CXRect& rect );
	void setRect ( long x, long y, long w, long h );
	void setColor ( u32 color );
	u32 getColor() const;
	void updateRect();
	bool containPoint ( long x, long y )const;
	const CXRect& getRect() const;

protected:
	virtual bool render();
	virtual bool doGraph();

	GGraphVertexBuffer mVB;
	GGraphIndexBuffer mIB;
	GGraphPrimitive mGraph;
	CXColor mVertexColor;
	CXRect mRect;
};


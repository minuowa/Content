#pragma once
#include "gnode.h"
#include "gnode.h"
#include "GMeshData.h"
class GRectNode :
	public GNode
{
	DeclareEditorType(GRectNode)
public:
	GRectNode ( void );
	~GRectNode ( void );

	virtual bool recreate();

	void setRect ( const CXRect& rect );
	void setRect ( long x, long y, long w, long h );
	void setRelRect(const CXRect& rect);
	void setWH(long w,long h);
	void setXY(long x,long y);
	virtual void setColor ( u32 color );
	u32 getColor() const;
	void updateRect();
	bool containPoint ( long x, long y )const;
	const CXRect& getRect() const;
	void offset(long x,long y);
protected:
	virtual bool render();
	virtual bool doGraph();
	void updateAbRect();
	virtual void registerAllProperty();
	virtual void onPropertyChangeEnd ( void* cur );

	GGraphVertexBuffer mVB;
	GGraphIndexBuffer mIB;
	GGraphPrimitive mGraph;
	CXColor mVertexColor;
	CXRect mRelRect;
	CXRect mRect;
};


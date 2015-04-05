#pragma once
#include "gnode.h"
#include "gnode.h"
#include "GMeshData.h"

struct GRectVertex
{
	float mX, mY, mZ, mRHW;
	CXColor mColor;
	static const DWORD mFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE ;
};

class GRectNode :
    public GNode
{
    DeclareEditorType ( GRectNode )
public:
    GRectNode ( void );
    ~GRectNode ( void );

    virtual bool recreate();
	virtual GNode* addChild ( GRectNode* c );
    void setRect ( const CXRect& rect );
    void setRect ( long x, long y, long w, long h );
    void setRelRect ( const CXRect& rect );
    void setWH ( long w, long h );
    void setXY ( long x, long y );
    void setRB ( long r, long b );
    virtual void setColor ( u32 color );
    u32 getColor() const;
    void updateRect();
    bool containPoint ( long x, long y ) const;
    const CXRect& getRect() const;
    virtual void offset ( long x, long y );
    void correct();
    void clamp ( const CXRect& rc );
    virtual void scale ( double scale, long xref, long yref );
    double getScaleX() const;
    double getScaleY() const;
    void setScale ( float x, float y );
	void setOrignalRect ( const CXRect& rect );
protected:
    virtual bool render();
    virtual bool doGraph();
    virtual void updateAbsoluteRect();
    virtual void updateRelRect();
	void setOrignalRectWithAbsoluteRect();
protected:
	virtual void registerAllProperty();
    virtual void onPropertyChangeEnd ( void* cur );

    GGraphVertexBuffer mVB;
    GGraphIndexBuffer mIB;
    GGraphPrimitive mGraph;
    CXColor mVertexColor;
    CXRect mRelRect;
    CXRect mRect;
	CXRect mOrignalRect;
    double mScaleX;
    double mScaleY;
};


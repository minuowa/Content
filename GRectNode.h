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
    CXDelegate mDelegateOnRectChanged;
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
    virtual void scale ( double dscale, long xref, long yref );
    virtual void scale ( double dscale );
    double getScaleX() const;
    double getScaleY() const;
    void setScale ( float x, float y );
	void setOrignalRelRect ( const CXRect& relRect );
    void setRenderTech ( GRenderTech tech );
    void correctPixel();
protected:
    virtual bool render();
    virtual bool doGraph();
    virtual void updateAbsoluteRect();
    virtual void updateRelRect();
    void setOrignalRectWithRelRect();
protected:
    virtual void registerAllProperty();
    virtual void onPropertyChangeEnd ( void* cur );

    GGraphVertexBuffer mVB;
    GGraphIndexBuffer mIB;
    GGraphPrimitive mGraph;
    CXColor mVertexColor;
    CXRect mRelRect;
    CXRect mRect;
    CXRect mOrignalRelRect;
    double mScaleX;
    double mScaleY;
};


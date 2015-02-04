#pragma once
#include "GRectNode.h"
#include "GMeshData.h"
class GUINode :
    public GRectNode
{
public:
    GUINode ( void );
    ~GUINode ( void );

    virtual bool recreate();

    void setRect ( const CXRect& rect );
    void setRect ( long x, long y, long w, long h );
    void setColor ( u32 color );
    u32 getColor() const;
    void updateRect();
    bool containPoint ( long x, long y ) const;
    const CXRect& getRect() const;

    void setTexture ( const char* fileName );

    GUINode* getHoverNode ( long x, long y ) ;

protected:
    virtual bool render();
    virtual bool doGraph();

    GTexture* mTextureProp;
};


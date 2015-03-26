#pragma once
#include "GRectNode.h"
#include "GMeshData.h"
class GUINode :
    public GRectNode
{
    DeclareEditorType ( GUINode );
public:
    GUINode ( void );
    ~GUINode ( void );

    virtual bool recreate();

    void setColor ( u32 color );
    u32 getColor() const;
    void updateRect();
    bool containPoint ( long x, long y ) const;
    const CXRect& getRect() const;
    const CXRect& getRelRect() const;

    void setTexture ( const char* fileName );

    GUINode* getNode ( long x, long y, eObjState state ) ;
    void onLostCapture();
protected:

    virtual bool render();
    virtual bool doGraph();


    GTexture* mTextureProp;
};


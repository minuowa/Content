#ifndef GTapeNode_h__
#define GTapeNode_h__
#include "GGWireRectNode.h"

class GTapeNode :
    public GGWireRectNode
{
public:
    enum eCornerType
    {
        Corner_LeftTop,
        Corner_LeftMiddle,
        Corner_LeftBottom,

        Corner_MiddleTop,
        Corner_MiddleBottom,

        Corner_RigthTop,
        Corner_RightMiddle,
        Corner_RightBottom,

        CornerCount,
    };

    GTapeNode ( void );
    ~GTapeNode ( void );
    virtual bool recreate();

    virtual bool render();

    virtual bool doGraph();

    virtual void setColor ( u32 color );

    virtual void scale ( float scale, long xref, long yref );
    virtual void offset ( long x, long y );

    virtual void onCallBack ( const CXDelegate& d, CXEventArgs* e );

    void changeCursor ( eCornerType type );
protected:
    GGWireRectNode* mCorner[CornerCount];
};

#endif // GTapeNode_h__

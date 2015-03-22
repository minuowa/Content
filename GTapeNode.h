#ifndef GTapeNode_h__
#define GTapeNode_h__

#include "grectnode.h"
class GTapeNode :
    public GRectNode
{
public:
    enum
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

	virtual void setColor( u32 color );

protected:
    GRectNode* mCorner[CornerCount];
};

#endif // GTapeNode_h__

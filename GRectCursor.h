#pragma once
#include "GRectNode.h"
class GRectCursor: public GRectNode
{
public:
    GRectCursor ( void );
    ~GRectCursor ( void );

    virtual bool doGraph();

	virtual bool recreate();

	void updateByRect(const CXRect& rc);

	void setTarget(GRectNode* node);

	virtual void scale( double dscale );

	virtual void onCallBack( const CXDelegate& d, CXEventArgs* e );
	void offset ( long x, long y );

	virtual void updateAbsoluteRect();

	virtual void updateRelRect();

protected:
	GRectNode* mTarget;
};


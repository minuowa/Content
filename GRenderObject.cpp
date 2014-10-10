#include "GGameDemoHeader.h"
#include "GRenderObject.h"


GRenderObject::GRenderObject(void)
{
	mVisible=true;

	mLighting=false;

	mChangeWhenHit=false;
}


GRenderObject::~GRenderObject(void)
{
	mForceMap=NULL;
}

bool GRenderObject::render()
{
	if(!mVisible)
		return false;

	GNode::GetWorldMatrix(false);

	return true;
}

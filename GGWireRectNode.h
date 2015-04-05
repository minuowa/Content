#pragma once
#include "GUINode.h"
class GGWireRectNode :
	public GUINode
{
public:
	GGWireRectNode(void);
	~GGWireRectNode(void);

	virtual bool recreate();

	virtual bool doGraph();

	virtual bool render();


};


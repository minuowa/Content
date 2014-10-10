#pragma once
#include "GNode.h"
class GRenderObject :
	public GNode
{
public:
	GRenderObject(void);
	~GRenderObject(void);
	virtual bool render();
public:
	bool mVisible;
	bool mLighting;
	bool mChangeWhenHit;
};


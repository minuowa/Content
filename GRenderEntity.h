#pragma once
#include "GNode.h"
class GRenderEntity :
	public GNode
{
public:
	GRenderEntity(void);
	~GRenderEntity(void);
	virtual bool render();
public:
	bool mVisible;
	bool mChangeWhenPicked;
};


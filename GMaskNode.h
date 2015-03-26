#pragma once
#include "guinode.h"
class GMaskNode :
	public GUINode
{
	DeclareEditorType(GUINode);
public:
	GMaskNode(void);
	~GMaskNode(void);

	void setMaskTexture(const char* texture);

	virtual bool render();

	GTexture* mMaskTexture;
	GTexture* mMaskTexture2;
};


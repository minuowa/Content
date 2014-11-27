#pragma once
#include "GRenderEntity.h"


class GWorldCorrd:public GRenderEntity
{
	DeclareEditorType(GWorldCorrd);
private:
	LPDIRECT3DVERTEXBUFFER9 mVBLines;
	LPDIRECT3DVERTEXBUFFER9 mVBCoord;

	LPDIRECT3DINDEXBUFFER9 mIBLines;
	LPDIRECT3DINDEXBUFFER9 mIBCoord;

public:
	GWorldCorrd(void);
	virtual void onCallBack ( const CXDelegate& , CXEventArgs*  )override;
	virtual bool recreate();
	virtual bool render();
	virtual bool recreateAll();
	WORD mLineCount;
	float mWidth;
	void operator=(GWorldCorrd cd);
public:
	~GWorldCorrd(void);
};

#pragma once
#include "GMeshBaseObj.h"

class CGameStaticObj :
	public GMeshBaseObj
{
	DeclareEditorType(CGameStaticObj);
public:

	CGameStaticObj(void);

	~CGameStaticObj(void);

public:

	bool recreate(GNode *Parent,int OBJNum);


public:
	int m_nObjID;

};

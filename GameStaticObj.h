#pragma once
#include "GStillEntity.h"

class CGameStaticObj :
	public GStillEntity
{
	DeclareEditorType(CGameStaticObj);
public:
	CGameStaticObj(void);
	~CGameStaticObj(void);
public:
	int m_nObjID;

};

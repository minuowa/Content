#pragma once


#include "XArea.h"

class GBound 
{

public:

	GBound(void);

	~GBound(void);

public:

	virtual bool Init(ID3DXMesh *pMesh);

};

class CBoundAABB:public GBound
{

public:

	CBoundAABB();

	~CBoundAABB();

public:

	virtual bool Init( ID3DXMesh *pMesh );

	CAreaCube mArea;
};

class CBoundSphere:public GBound
{
public:

	CBoundSphere();

	~CBoundSphere();

public:

	virtual bool Init( ID3DXMesh *pMesh );

	float mfRadious;
};
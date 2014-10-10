#pragma once
#include "XString.h"
#include "GAnimationResource.h"
class FiGraph
{
	virtual void Update(){}
	virtual void Render(){}
};
class GAnimMeshObj;

class GTrailEffect:public FiGraph
{
public:
	GTrailEffect(void);
	~GTrailEffect(void);

	void Init(CChar* linkerName1,CChar* linkerName2);
	void Attach(GAnimMeshObj* anim);
	void Update();
public:
	void Generate(D3DXMATRIX* v1,D3DXMATRIX*v2);

	GString mLinkerName1;
	GString mLinkerName2;

	GBoneLinker* mLinker1;
	GBoneLinker* mLinker2;

	D3DXMATRIX mLastMat1;
	D3DXMATRIX mLastMat2;
	D3DXMATRIX mLastWorld;
	D3DXVECTOR3 mLastTranslate;
	D3DXVECTOR3 mLastDir;
};

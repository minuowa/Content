#pragma once
#include "uString.h"
#include "GAnimationResource.h"
class FiGraph
{
	virtual void Update(){}
	virtual void Render(){}
};
class GAnimEntity;

class GTrailEffect:public FiGraph
{
public:
	GTrailEffect(void);
	~GTrailEffect(void);

	void Init(const char* linkerName1,const char* linkerName2);
	void Attach(GAnimEntity* anim);
	void Update();
public:
	void Generate(D3DXMATRIX* v1,D3DXMATRIX*v2);

	uString mLinkerName1;
	uString mLinkerName2;

	GBoneLinker* mLinker1;
	GBoneLinker* mLinker2;

	D3DXMATRIX mLastMat1;
	D3DXMATRIX mLastMat2;
	D3DXMATRIX mLastWorld;
	D3DXVECTOR3 mLastTranslate;
	D3DXVECTOR3 mLastDir;
};

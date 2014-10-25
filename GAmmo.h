#pragma once

#include "Particles.h"
#include "GRenderObject.h"

//炮弹类
//从武器中发射，落到地面时产生效果，消亡

class GAmmo:public GRenderObject 
{
public:
	GAmmo(void);

	~GAmmo(void);

public:

	virtual bool recreate();

	void Shoot();

	void SetShooter(GNode *pShooter);

public:

	GRenderObject *mpObj;

};

class CAmmoParticles:public GAmmo
{
public:

	CAmmoParticles();

	~CAmmoParticles();

public:

	virtual bool recreate();

	void update();

	bool render();

public:
	
	CRectMesh mParticles;

};

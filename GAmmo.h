#pragma once

#include "Particles.h"
#include "GRenderEntity.h"

//炮弹类
//从武器中发射，落到地面时产生效果，消亡

class GAmmo:public GRenderEntity 
{
public:
	GAmmo(void);

	~GAmmo(void);

public:

	virtual bool recreate();

	void Shoot();

	void SetShooter(GNode *pShooter);

public:

	GRenderEntity *mpObj;

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

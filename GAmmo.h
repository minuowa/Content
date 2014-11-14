#pragma once

#include "Particles.h"
#include "GRenderEntity.h"

//�ڵ���
//�������з��䣬�䵽����ʱ����Ч��������

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

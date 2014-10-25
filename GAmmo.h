#pragma once

#include "Particles.h"
#include "GRenderObject.h"

//�ڵ���
//�������з��䣬�䵽����ʱ����Ч��������

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

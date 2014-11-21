#include "GGameDemoHeader.h"
#include "GAmmo.h"

GAmmo::GAmmo ( void )
{
	getTrans()->mSpeedMove = 150;

}

GAmmo::~GAmmo ( void )
{

}

void GAmmo::Shoot()
{
	m_bForceOnMap = true;
	getTrans()->mAutoMove = true;
	getTrans()->mSpeed = getTrans()->mSpeedMove * getTrans()->getDir();
}


bool GAmmo::recreate()
{
	if ( !__super::recreate() )
		return false;

	return true;
}

void GAmmo::SetShooter ( GNode *pShooter )
{

	//getTrans()->mMatLocal.mTranslate = pShooter->getTrans()->mMatLocal.mTranslate;

	//getTrans()->mMatLocal.mTranslate.y += 50.0f;
	//getTrans()->mMatLocal.mTranslate.z += 5.0f;

	//getTrans()->getDir() = pShooter->getTrans()->getDir();
	//getTrans()->getDir().y += 0.01f;

	//D3DXVec3Normalize ( &getTrans()->getDir(), &getTrans()->getDir() );

	//getTrans()->mMatLocal.mRight = pShooter->getTrans()->mMatLocal.mRight;

	//D3DXVec3Cross ( &getTrans()->mMatLocal.mUpon, &getTrans()->getDir(), &getTrans()->mMatLocal.mRight );

	//D3DXVec3Normalize ( &getTrans()->mMatLocal.mUpon, &getTrans()->mMatLocal.mUpon );

	mParent = NULL;
}


bool CAmmoParticles::recreate()
{
	if ( !GAmmo::recreate() )
		return false;

	mParticles.recreate();

	//ForceOnMap(pForceMap,1.0f,ftUpWithMap);

	return true;

}

void CAmmoParticles::update(  )

{
	m_bForceOnMap = false;
	GRenderEntity::update();

	mParticles.update();
}

bool CAmmoParticles::render()
{
	GAmmo::render();

	return mParticles.render();
}

CAmmoParticles::CAmmoParticles()
{

}

CAmmoParticles::~CAmmoParticles()
{

}

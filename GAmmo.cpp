#include "GGameDemoHeader.h"
#include "GAmmo.h"

GAmmo::GAmmo ( void )
{
    getTrans().mfSpeedMove = 150;

}

GAmmo::~GAmmo ( void )
{

}

void GAmmo::Shoot()
{
    m_bForceOnMap = true;
    getTrans().mbAutoMove = true;
    getTrans().mvSpeed = getTrans().mfSpeedMove * getTrans().mDir;
}


bool GAmmo::recreate()
{
    if ( !__super::recreate() )
        return false;

    return true;
}

void GAmmo::SetShooter ( GNode *pShooter )
{

    getTrans().mTranslate = pShooter->getTrans().mTranslate;

    getTrans().mTranslate.y += 50.0f;
    getTrans().mTranslate.z += 5.0f;

    getTrans().mDir = pShooter->getTrans().mDir;
    getTrans().mDir.y += 0.01f;

    D3DXVec3Normalize ( &getTrans().mDir, &getTrans().mDir );

    getTrans().mRight = pShooter->getTrans().mRight;

    D3DXVec3Cross ( &getTrans().mUpon, &getTrans().mDir, &getTrans().mRight );

    D3DXVec3Normalize ( &getTrans().mUpon, &getTrans().mUpon );

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
    GRenderObject::update();

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

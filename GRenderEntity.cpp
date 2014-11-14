#include "GGameDemoHeader.h"
#include "GRenderEntity.h"


GRenderEntity::GRenderEntity ( void )
{
    mVisible = true;

    mChangeWhenPicked = false;
}


GRenderEntity::~GRenderEntity ( void )
{
    mForceMap = NULL;
}

bool GRenderEntity::render()
{
    if ( !mVisible )
        return false;
    getTrans().set();
    return true;
}

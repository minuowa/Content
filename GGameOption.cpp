#include "GGameDemoHeader.h"
#include "GGameOption.h"
#include "GEnumTypesStrings.h"
#include "GD9Device.h"

GGameOption::GGameOption ( void )
{
    mFillMode = FillSolid;

    updateRenderState();
}


GGameOption::~GGameOption ( void )
{
}

void GGameOption::registerAllProperty()
{
    //__super::registerAllProperty();
    __RegisterPropertyEnum ( mFillMode );
}

void GGameOption::onPropertyChangeEnd ( void* cur )
{
    __super::onPropertyChangeEnd ( cur );

    if ( &mFillMode == cur )
    {
        updateRenderState();
    }
}

void GGameOption::updateRenderState()
{
    switch ( mFillMode )
    {
    case FillPoint:
        D9DEVICE->GetDvc()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_POINT );
        break;
    case FillSolid:
        D9DEVICE->GetDvc()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );
        break;
    case FillWire:
        D9DEVICE->GetDvc()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
        break;
    default:
        break;
    }
}

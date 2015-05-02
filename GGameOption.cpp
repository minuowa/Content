#include "GGameDemoHeader.h"
#include "GGameOption.h"
#include "GEnumTypesStrings.h"
#include "GD9Device.h"
#include "Content.h"

GGameOption::GGameOption ( void )
    : RenderBloom ( false )
{
    FillMode = FillSolid;
    Content::Device.mOnResetDevice += this;
}


GGameOption::~GGameOption ( void )
{
}

void GGameOption::registerAllProperty()
{
    //__super::registerAllProperty();
    __RegisterPropertyEnum ( FillMode );
    __RegisterProperty ( RenderBloom );
}

void GGameOption::onPropertyChangeEnd ( void* cur )
{
    __super::onPropertyChangeEnd ( cur );

    if ( &FillMode == cur )
    {
        updateRenderState();
    }
}

void GGameOption::updateRenderState()
{
    switch ( FillMode )
    {
    case FillPoint:
        Content::Device.getD9Device()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_POINT );
        break;
    case FillSolid:
        Content::Device.getD9Device()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );
        break;
    case FillWire:
        Content::Device.getD9Device()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
        break;
    default:
        break;
    }
}

void GGameOption::onCallBack ( const CXDelegate& delegate, CXEventArgs* )
{
    if ( delegate ==  Content::Device.mOnResetDevice )
    {
        updateRenderState();
    }
}

void GGameOption::destroy()
{
	Content::Device.mOnResetDevice -= this;
}

bool GGameOption::init()
{
	updateRenderState();
	return true;
}

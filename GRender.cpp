#include "GGameDemoHeader.h"
#include "GRender.h"
#include "Content.h"
#include "GD9Device.h"


GRender::GRender ( void )
{
    {
        GRenderState& state = mRenderState[GRenderTech_Primite];
        state.add ( GSatteType_Render, D3DRS_LIGHTING, FALSE );

		state.add ( GSatteType_Render, D3DRS_ALPHATESTENABLE, FALSE );

		state.add ( GSatteType_Render, D3DRS_ALPHABLENDENABLE, TRUE );
		state.add ( GStateType_Texture0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE  );
		state.add ( GStateType_Texture0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		state.add ( GStateType_Texture0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

		//state.add ( GSatteType_Render, D3DRS_COLORVERTEX , TRUE );
        //state.add ( GSatteType_Render, D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
        //state.add ( GSatteType_Render, D3DRS_ALPHABLENDENABLE, FALSE );
        //state.add ( GSatteType_Render, D3DRS_ALPHATESTENABLE, FALSE );
        state.add ( GStateType_Texture0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        state.add ( GStateType_Texture0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
        state.add ( GStateType_Texture0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
    }

    {
        GRenderState& state = mRenderState[GRenderTech_PrimiteWithPictureAlpha];
        state.add ( GSatteType_Render, D3DRS_LIGHTING, FALSE );
		state.add ( GSatteType_Render, D3DRS_ALPHATESTENABLE, FALSE );

        state.add ( GSatteType_Render, D3DRS_ALPHABLENDENABLE, TRUE );
		state.add ( GStateType_Texture0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE  );
		state.add ( GStateType_Texture0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		state.add ( GStateType_Texture0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );

        state.add ( GSatteType_Render, D3DRS_BLENDOP, D3DBLENDOP_ADD );
		state.add ( GSatteType_Render, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		state.add ( GSatteType_Render, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        state.add ( GStateType_Texture0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        state.add ( GStateType_Texture0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		state.add ( GStateType_Texture0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    }

	{
		GRenderState& state = mRenderState[GRenderTech_PrimiteWithPictureAlphaFromTextureColorFromDiffuse];
		state.add ( GSatteType_Render, D3DRS_LIGHTING, FALSE );

		state.add ( GSatteType_Render, D3DRS_ALPHABLENDENABLE, TRUE );
		state.add ( GStateType_Texture0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		state.add ( GStateType_Texture0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );

		state.add ( GSatteType_Render, D3DRS_BLENDOP, D3DBLENDOP_ADD );
		state.add ( GSatteType_Render, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		state.add ( GSatteType_Render, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		state.add ( GStateType_Texture0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		state.add ( GStateType_Texture0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		state.add ( GStateType_Texture0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	}
}


GRender::~GRender ( void )
{
}

void GRender::setTech ( GRenderTech type )
{
    if ( type != GRenderTech_Count )
        mRenderState[type].set();
}

void GRenderState::set()
{
for ( auto i: mData )
    {
        switch ( i->mState )
        {
        case GSatteType_Render:
        {
            Content::Device.getD9Device()->SetRenderState ( D3DRENDERSTATETYPE ( i->mType ), i->mValue );
        }
        break;
        case GStateType_Texture0:
        case GStateType_Texture1:
        {
            Content::Device.getD9Device()->SetTextureStageState (
                i->mState - GStateType_Texture0, D3DTEXTURESTAGESTATETYPE ( i->mType ), i->mValue );
        }
        break;
        default:
            break;
        }
    }
}

GRenderState::GRenderState()
{

}


void GRenderState::add ( GStateType stateType, u32 type, u32 value )
{
    GRenderData* data = new GRenderData ( );
    data->mState = stateType;
    data->mType = type;
    data->mValue = value;
    mData.push_back ( data );
}

GRenderState::~GRenderState()
{
    dSafeDeleteVector ( mData );
}

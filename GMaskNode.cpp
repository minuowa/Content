#include "GGameDemoHeader.h"
#include "GMaskNode.h"
#include "Content.h"
#include "GD9Device.h"


GMaskNode::GMaskNode ( void )
    : mMaskTexture ( nullptr )
{
}


GMaskNode::~GMaskNode ( void )
{
}

void GMaskNode::setMaskTexture ( const char* texture )
{
    mMaskTexture = Content::TextureMgr.getResource ( texture );
}

bool GMaskNode::render()
{
    IDirect3DDevice9* g_pd3dDevice = Content::Device.getD9Device();
    g_pd3dDevice->SetRenderState ( D3DRS_ZENABLE, true );
    g_pd3dDevice->SetRenderState ( D3DRS_ZFUNC, D3DCMP_LESS );
    g_pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE, TRUE );

    //ÉèÖÃAlpha²âÊÔ
    //g_pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, true );
    //g_pd3dDevice->SetRenderState ( D3DRS_ALPHAREF, 0x00000FF );
    //g_pd3dDevice->SetRenderState ( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

    g_pd3dDevice->SetRenderState ( D3DRS_STENCILENABLE, TRUE );
    g_pd3dDevice->SetRenderState ( D3DRS_STENCILREF, 255 );
    g_pd3dDevice->SetRenderState ( D3DRS_STENCILMASK, 255 );
    g_pd3dDevice->SetRenderState ( D3DRS_STENCILFUNC, D3DCMP_ALWAYS );
    g_pd3dDevice->SetRenderState ( D3DRS_STENCILWRITEMASK, 0xFFFFffff );
    g_pd3dDevice->SetRenderState ( D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE );
    if ( mMaskTexture )
        mMaskTexture->set ( 0 );
    mIB.set();
    mVB.set();
    if ( mIB.isValid() && mVB.isValid() )
        mGraph.draw();
    //g_pd3dDevice->SetRenderState ( D3DRS_ALPHAFUNC, D3DCMP_LESSEQUAL );

    //g_pd3dDevice->SetRenderState ( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
    //g_pd3dDevice->SetRenderState ( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

    if ( mTextureProp )
        mTextureProp->set ( 0 );
    mIB.set();
    mVB.set();
    if ( mIB.isValid() && mVB.isValid() )
        mGraph.draw();

    //mMaskTexture2->set ( 0 );
    //if ( mTextureProp )
    //    mTextureProp->set ( 0 );
    //mIB.set();
    //mVB.set();
    //if ( mIB.isValid() && mVB.isValid() )
    //    mGraph.draw();


    g_pd3dDevice->SetRenderState ( D3DRS_ZENABLE, FALSE );
    g_pd3dDevice->SetRenderState ( D3DRS_ALPHATESTENABLE, FALSE );
	g_pd3dDevice->SetRenderState ( D3DRS_STENCILENABLE, FALSE );
	return true;
}

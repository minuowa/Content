#include "GGameDemoHeader.h"

#include "XText.h"
#include "GD9Device.h"
//#include "GXNode.h"


//CXText gText;


CXText::CXText( )
{
	mCurrentFont = NULL;

}
bool CXText::Init()
{
	int mFontHeight[16] = {8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72};


	HDC hDC = GetDC( NULL );
	int nLogPixelsY = GetDeviceCaps( hDC, LOGPIXELSY );
	ReleaseDC( NULL, hDC );
	int nHeight;

	for ( int i = 0; i < 16; i++ )
	{
		nHeight = -1 * mFontHeight[i] * nLogPixelsY / 72;
		D3DXCreateFontA( D9DEVICE->GetDvc(),
			nHeight,
			0,
			FW_BOLD,
			1,
			FALSE,
			GB2312_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			"楷体",
			&mFonts[i]
		);
	}

	nHeight = -1 * 18 * nLogPixelsY / 72;
	HRESULT Hr = D3DXCreateFontA( D9DEVICE->GetDvc(),
		nHeight,
		0,
		FW_BOLD,
		1,
		FALSE,
		GB2312_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH ,
		"宋体",
		&mDefaultFont
		);

	CHECK_RESULT_BOOL(Hr);

	mDefaultFont = mFonts[7]; //默认18号字体
	Hr = D3DXCreateSprite( D9DEVICE->GetDvc(), &mFontSprite );

	CHECK_RESULT_BOOL(Hr);

	return true;
}
CXText::~CXText( void )
{
	//RELEASE()
}

void CXText::SetCurrentFontSize( nFontSize fs )
{
	//	int mFontHeight[16]={
	//8,9,10,11,
	//12,14,16,18,
	//20,22,24,26,
	//28,36,48,72
	//				};

	LnowFontSize = fs;

	switch ( LnowFontSize )
	{
	case fs8:
		mCurrentFont = mFonts[0];
		break;

	case fs9:
		mCurrentFont = mFonts[1];
		break;

	case fs10:
		mCurrentFont = mFonts[2];
		break;

	case fs11:
		mCurrentFont = mFonts[3];
		break;

	case fs12:
		mCurrentFont = mFonts[4];
		break;

	case fs14:
		mCurrentFont = mFonts[5];
		break;

	case fs16:
		mCurrentFont = mFonts[6];
		break;

	case fs18:
		mCurrentFont = mFonts[7];
		break;

	case fs20:
		mCurrentFont = mFonts[8];
		break;

	case fs22:
		mCurrentFont = mFonts[9];
		break;

	case fs24:
		mCurrentFont = mFonts[10];
		break;

	case fs26:
		mCurrentFont = mFonts[11];
		break;

	case fs28:
		mCurrentFont = mFonts[12];
		break;

	case fs36:
		mCurrentFont = mFonts[13];
		break;

	case fs48:
		mCurrentFont = mFonts[14];
		break;

	case fs72:
		mCurrentFont = mFonts[15];
		break;

	default:
		mCurrentFont = mDefaultFont;
		break;

	}

}



int CXText::DrawTextDefault( char* sText, D3DVECTOR pos )
{
	mFontSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT );

	RECT r = {pos.x, pos.y, pos.x + 300, pos.y + 200};

	mDefaultFont->DrawTextA(
		mFontSprite,
		sText,
		strlen( sText ),
		&r,
		DT_VCENTER,
		D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.8f )
		);

	mFontSprite->End();

	return 1;
}



int CXText::DrawTextInRect( char* sText, RECT *r, DWORD Color, OBJTYPE ot, TextPos tp )
{
	if ( !mCurrentFont )
	{
		mCurrentFont = mDefaultFont;
	};

	D3DXMATRIXA16 World;

	RECT rCal = {0, 0, 0, 0};

	int iH = mCurrentFont->DrawTextA(
		mFontSprite,
		sText,
		strlen( sText ),
		&rCal,
		DT_TOP | DT_LEFT | DT_CALCRECT ,
		//|DT_WORDBREAK
		( D3DXCOLOR )Color
		);

	//if (tp==tpLeft)
	//{
	//    D3DXMatrixTranslation(&World,r->left,r->top,0);
	//} else
	if ( tp == tpRight )
	{
		r->left = r->right - rCal.right;
	}
	else if ( tp == tpCenter )
	{
		r->left = r->left + ( ( r->right - r->left ) - ( rCal.right - rCal.left ) ) / 2;
	}

	D3DXMatrixTranslation( &World, r->left, r->top, 0 );

	if ( ot == otUI )
	{
		mFontSprite->Begin( D3DXSPRITE_ALPHABLEND );

		mFontSprite->SetTransform( &World );

	}
	else
	{
		D3DXMATRIX MatrixView, Rota;
		D3DXMatrixRotationYawPitchRoll( &Rota, 0, D3DX_PI, 0 );
		World = Rota * World;
		D9DEVICE->GetDvc()->GetTransform( D3DTS_VIEW, &MatrixView );
		mFontSprite->SetTransform( &World );
		mFontSprite->SetWorldViewLH( NULL, &MatrixView );
		mFontSprite->Begin(
			D3DXSPRITE_BILLBOARD |
			D3DXSPRITE_SORT_DEPTH_BACKTOFRONT |
			D3DXSPRITE_ALPHABLEND
			);

	}

	mCurrentFont->DrawTextA(
		mFontSprite,
		sText,
		strlen( sText ),
		&rCal,
		DT_TOP | DT_LEFT ,
		//|DT_NOCLIP|DT_WORDBREAK
		( D3DXCOLOR )Color
		);

	mFontSprite->End();

	return iH;

}

int CXText::DrawTextByPosColOt( char* sText, D3DVECTOR Pos, DWORD Color, OBJTYPE ot , bool Center )
{

	if ( !mCurrentFont )
	{
		mCurrentFont = mDefaultFont;
	};

	D3DXMATRIXA16 World;

	D3DXMatrixTranslation( &World, Pos.x, Pos.y, Pos.z );


	if ( ot == otUI )
	{
		mFontSprite->Begin( D3DXSPRITE_ALPHABLEND );
		mFontSprite->SetTransform( &World );

	}	else if ( ot == otBillBoard )
	{

		D3DXMATRIX MatrixView, Rota;
		D3DXMatrixRotationYawPitchRoll( &Rota, 0, D3DX_PI, 0 );
		World = Rota * World;
		D9DEVICE->GetDvc()->GetTransform( D3DTS_VIEW, &MatrixView );
		mFontSprite->SetTransform( &World );
		mFontSprite->SetWorldViewLH( NULL, &MatrixView );
		mFontSprite->Begin( D3DXSPRITE_BILLBOARD | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT |	D3DXSPRITE_ALPHABLEND );

	} else if ( ot == otWorld )
	{
		D3DXMATRIX MatrixView, Rota;
		D3DXVECTOR3 Pos( 0, 100, -100 ), Look( 0, 100, 1 ), Up( 0, 1, 0 );
		D3DXMatrixRotationYawPitchRoll( &Rota, 0, D3DX_PI, 0 );
		World = Rota * World;

		mFontSprite->SetTransform( &World );
		D9DEVICE->GetDvc()->GetTransform( D3DTS_VIEW, &MatrixView );
		Pos = -1 * D3DXVECTOR3( &MatrixView._41 );
		Look = Pos + D3DXVECTOR3( 0, 0, 1 );
		D3DXMatrixLookAtLH( &MatrixView, &Pos, &Look, &Up );
		mFontSprite->SetWorldViewLH( NULL, &MatrixView );
		mFontSprite->Begin( D3DXSPRITE_BILLBOARD | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT | D3DXSPRITE_ALPHABLEND );

	}

	RECT aR = {0, 0, 0, 0};

	int iH = mCurrentFont->DrawTextA( mFontSprite, sText, strlen( sText ), &aR, DT_TOP | DT_LEFT | DT_WORDBREAK | DT_CALCRECT , ( D3DXCOLOR )Color );
	aR.left += -aR.right / 2;
	mCurrentFont->DrawTextA( mFontSprite, sText, strlen( sText ), &aR, DT_TOP | DT_LEFT | DT_NOCLIP | DT_WORDBREAK , ( D3DXCOLOR )Color );

	mFontSprite->End();

	return iH;
}

void CXText::DrawPic( LPDIRECT3DTEXTURE9 Pic, D3DVECTOR Pos, bool IsCenter, OBJTYPE ot, bool bAlphaBlend , D3DCOLOR dwColor )
{
	D3DXMATRIXA16 World;
	D3DXMATRIX MatrixScale, MatrixView, Rota;
	D3DXVECTOR3 ePos( 0, 100, -100 ), eLook( 0, 100, 1 ), eUp( 0, 1, 0 );

	D3DXMatrixScaling( &MatrixScale, 1, 1, 1 );
	D3DXMatrixTranslation( &World, Pos.x, Pos.y, Pos.z );
	World = MatrixScale * World;

	switch ( ot )
	{
	case otUI:
		mFontSprite->Begin( D3DXSPRITE_ALPHABLEND );

		if ( bAlphaBlend )
		{
			D9DEVICE->GetDvc()->SetRenderState( D3DRS_ZWRITEENABLE, false );
			D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			D9DEVICE->GetDvc()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );		//D3DBLEND_SRCALPHA
			D9DEVICE->GetDvc()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		}

		mFontSprite->SetTransform( &World );
		break;

	case otWorld:
		D3DXMatrixRotationYawPitchRoll( &Rota, 0, D3DX_PI, 0 );
		World = Rota * World;
		mFontSprite->SetTransform( &World );
		D9DEVICE->GetDvc()->GetTransform( D3DTS_VIEW, &MatrixView );
		ePos = -1 * D3DXVECTOR3( &MatrixView._41 );
		eLook = ePos + D3DXVECTOR3( 0, 0, 1 );
		D3DXMatrixLookAtLH( &MatrixView, &ePos, &eLook, &eUp );
		mFontSprite->SetWorldViewLH( NULL, &MatrixView );
		mFontSprite->Begin( D3DXSPRITE_BILLBOARD | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT | D3DXSPRITE_ALPHABLEND );

		break;

	case otBillBoard:

		D3DXMatrixRotationYawPitchRoll( &Rota, 0, D3DX_PI, 0 );
		World = Rota * World;
		D9DEVICE->GetDvc()->GetTransform( D3DTS_VIEW, &MatrixView );
		mFontSprite->SetTransform( &World );
		mFontSprite->SetWorldViewLH( NULL, &MatrixView );
		mFontSprite->Begin( D3DXSPRITE_BILLBOARD | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT | D3DXSPRITE_ALPHABLEND );

		break;

	}

	int w, h;
	D3DSURFACE_DESC DESC;
	Pic->GetLevelDesc( 0, &DESC );
	w = DESC.Width;
	h = DESC.Height;

	if ( IsCenter )
	{
		D3DXVECTOR3 Center( w / 2.0f, h / 2.0f, 0 );
		mFontSprite->Draw( Pic, NULL, &Center, NULL, 0xFFFFFFFF & dwColor );
	}
	else if ( !IsCenter )
	{
		mFontSprite->Draw( Pic, NULL, NULL, NULL, 0xFFFFFFFF & dwColor );
	}

	mFontSprite->End();
	D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	D9DEVICE->GetDvc()->SetRenderState( D3DRS_ZWRITEENABLE, true );
}



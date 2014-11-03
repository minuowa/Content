#include "GGameDemoHeader.h"
#include "GHeightMap.h"


GxHeightMap::GxHeightMap( float Width, float Length,float MinHeight,float MaxHeight, const char* FileName )
{
	Datas=nullptr;
	Width = 0;
	Height = 0;
	RanageX = 0.0f;
	RanageZ = 0.0f;
	MinHeight = 0.0f;
	MaxHeight = 0.0f;

	//Texture Text = GxTexturePool.GetTexture(FileName);
	//if (Text != null)
	//{
	//	this.FileName = FileName;
	//	SurfaceDescription SD = Text.GetLevelDescription(0);
	//	if (SD.Format == Format.A8R8G8B8)
	//	{
	//		this.Width = SD.Width;
	//		this.Height = SD.Height;
	//		RanageX = Width;
	//		RanageZ = Length;
	//		this.MinHeight = MinHeight;
	//		this.MaxHeight = MaxHeight;
	//		Datas = (int[])Text.LockRectangle(typeof(int), 0, LockFlags.Discard, SD.Width * SD.Height);
	//		Text.UnlockRectangle(0);

	//	}

	//}
}

float GxHeightMap::GetHeight( float X, float Z )
{
	//ZIndex越大越靠近高度图上边
	int XIndex = ( int ) ( ( X + RanageX / 2.0f ) / RanageX * ( Width - 1 ) );
	int ZIndex = ( int ) ( ( Z + RanageZ / 2.0f ) / RanageZ * ( Height - 1 ) );
	return ( 0xff & Datas[XIndex + Width * ( Height - 1 - ZIndex )] ) * ( MaxHeight - MinHeight ) / 255.0f;
}

void GxHeightMap::SetHeight( float X, float Z, float Y )
{
	byte b = ( byte ) ( Y / ( float ) ( MaxHeight - MinHeight ) * 255.0f );
	if ( b > 255 )
	{
		b = 255;
	}
	int XIndex = ( int ) ( ( X + RanageX / 2.0f ) / RanageX * ( Width - 1 ) );
	int ZIndex = ( int ) ( ( Z + RanageZ / 2.0f ) / RanageZ * ( Height - 1 ) );
	//Datas[XIndex + Width * ( Height - 1 - ZIndex )] = Color.FromArgb ( 255, b, b, b ).ToArgb();
}

void GxHeightMap::Save()
{
	//Bitmap HeightMap = new Bitmap ( Width, Height, PixelFormat.Format32bppArgb );
	//for ( int i = 0; i < Height; i++ )
	//{
	//	for ( int j = 0; j < Width; j++ )
	//		HeightMap.SetPixel ( i, j, Color.FromArgb ( Datas[j * Width + i] ) );
	//}
	//HeightMap.Save ( FileName );
}

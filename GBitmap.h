#pragma once
class GBitmap
{
public:
	GBitmap(const char* file);
	~GBitmap(void);

	CXColor GetPixel(IN INT x,
		IN INT y);
	CXColor SetPixel(IN INT x,
		IN INT y);
public:
	int Width;
};


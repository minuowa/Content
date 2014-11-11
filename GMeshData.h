#pragma once
#include "GTexture.h"
class GGraphIndexBuffer
{
public:
	void Set();
protected:
	IDirect3DIndexBuffer9* mD9IndexBuffer;
};
class GGraphVertexBuffer
{
public:
	void Set();	
protected:
	IDirect3DVertexBuffer9*	mD9VertexBuffer;
	DWORD					mFVF;

	WORD					mRenderVertexCount;
	WORD					mDataSize;
public:
	inline DWORD GetRenderVertexCount() const
	{
		return mRenderVertexCount;
	}
	inline void	SetRenderVertexCount(WORD cnt) 
	{
		mRenderVertexCount = cnt;
	}
	WORD DataSize() const 
	{ 
		return mDataSize; 
	}
	void DataSize(WORD val) { mDataSize = val; }
	inline 	DWORD FVF() const 
	{ 
		return mFVF;
	}
	void FVF(DWORD val) 
	{ 
		mFVF = val;
	}
};

class GMetrialData
{
public:
	static D3DMATERIAL9 mDefaultWhite;
public:
	GMetrialData(void);
	void	setTexture(const char* fileName);
	void	setMetiral(const D3DMATERIAL9& d9matrial);
	void	set();
private:
	GTexture*			mTexture;
	D3DMATERIAL9		mMat;

};
typedef CXDynaArray<GMetrialData*>	GRenderDataArr;

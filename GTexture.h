#pragma once
#include "GResourceManager.h"
class GTexture
{
public:
	GTexture();
	~GTexture();

	bool createFromFile ( const char* name );

	inline IDirect3DTexture9* getTexture();

	void destory();
public:
	GString			mFileName;
protected:
	IDirect3DTexture9*	mD3DTexture;
};

inline IDirect3DTexture9* GTexture::getTexture()
{
	return mD3DTexture;
}
typedef CXSingleton<GResourceManager<GTexture>> GTextureManager;

#define TextureMgr	GTextureManager::getInstance()
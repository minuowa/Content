#pragma once

#include "XList.h"
//#include "EasyList.h"
#include "GResourceManager.h"
//������Mesh�Ƕ�Ӧ��
class GTexture
{
public:
	GTexture();
	~GTexture();

	bool createFromFile ( const char* name );

	inline IDirect3DTexture9* getTexture()
	{
		return mD3DTexture;
	}
public:
	GString			FileName;
protected:
	IDirect3DTexture9*	mD3DTexture;
};
typedef CXSingleton<GResourceManager<GTexture>> GTextureManager;

#define TextureMgr	GTextureManager::GetSingletonPtr()
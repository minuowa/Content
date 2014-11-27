#pragma once
#include "GResourceManager.h"

class GEffect: public CXCallBack
{
public:
	CXDelegate mDelegateRender;
	CXDelegate mDelegateSetPara;
	CXDelegate mDelegateOnReset;
public:
	GEffect();
	~GEffect();
	bool createFromFile ( const char* name );
	void draw();
	void setParams();
	bool recreate();
	inline ID3DXEffect* getD3DEffect();
	void destory()
	{
		delete this;
	}
public:
	GString	mFileName;
public:
	ID3DXEffect *mD3DEffect;
};
inline ID3DXEffect* GEffect::getD3DEffect()
{
    return mD3DEffect;
}
class GEffectManager:public GResourceManager<GEffect>,public CXCallBack
{
public:
	CXDelegate mDelegateRecreate;
public:
	GEffectManager();
	~GEffectManager();
	ID3DXEffectPool*& getPool();
	virtual void onCallBack ( const CXDelegate& , CXEventArgs*  )override;
	void clear();
	bool recreate();
private:
	ID3DXEffectPool* mEffectPool;
	CXMap<GString,GEffect*> mPointerMap;
};

#define EffectMgr	CXSingleton<GEffectManager>::getInstance()
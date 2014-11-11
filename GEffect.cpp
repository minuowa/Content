#include "GGameDemoHeader.h"
#include "GEffect.h"
#include <string>
#include "GD9Device.h"

void GEffect::setParams()
{
    //mD3DEffect->BeginParameterBlock();
    mDelegateSetPara.trigger();
    //mD3DEffect->EndParameterBlock();
}

bool GEffect::createFromFile ( const char* name )
{
    mFileName = name;
    HRESULT hr = E_FAIL;

    ID3DXBuffer *pErrBuffer = NULL;

    //创建顶点声明对象

    hr = D3DXCreateEffectFromFileA (
             D9DEVICE->GetDvc(),
             name,
             NULL,
             NULL,
             D3DXSHADER_DEBUG/*|D3DXSHADER_USE_LEGACY_D3DX9_31_DLL|D3DXFX_NOT_CLONEABLE*/,
             EffectMgr->getPool(),
             &mD3DEffect,
             &pErrBuffer
         );

    if ( pErrBuffer != NULL )
    {
        std::string sError;
        sError.assign ( ( char* ) ( char* ) pErrBuffer->GetBufferPointer() );
        MessageBoxA ( NULL, ( char* ) pErrBuffer->GetBufferPointer(), 0, 0 );
    }
    return mD3DEffect != nullptr;
}

void GEffect::draw()
{
    u32 passCount;
    mD3DEffect->Begin ( &passCount, 0 );
    for ( u32 i = 0; i < passCount; ++i )
    {
        mD3DEffect->BeginPass ( i );
        mDelegateRender.trigger();
        mD3DEffect->EndPass();
    }
    mD3DEffect->End();
}

GEffectManager::GEffectManager()
{
    mEffectPool = nullptr;
    D9DEVICE->mOnLostDevice += this;
    D9DEVICE->mOnResetDevice += this;
    recreate();
}

void GEffectManager::onCallBack ( const CXDelegate& delgate )
{
    if ( delgate == D9DEVICE->mOnLostDevice )
    {
        clear();
    }
    else if ( delgate == D9DEVICE->mOnResetDevice )
    {
        recreate();
    }
}

GEffectManager::~GEffectManager()
{
    D9DEVICE->mOnLostDevice -= this;
    D9DEVICE->mOnResetDevice -= this;
    clear();
}

void GEffectManager::clear()
{
    dSafeRelease ( mEffectPool );
for ( auto & p: mResourceMap )
    {
        dSafeRelease ( p.second->mD3DEffect );
    }
}

bool GEffectManager::recreate()
{
    if ( mEffectPool == nullptr )
    {
        CHECK_RESULT_BOOL ( D3DXCreateEffectPool ( &mEffectPool ) );
    }
for ( auto & p: mResourceMap )
    {
        GEffect* effect = p.second;
        CXASSERT_RETURN_FALSE ( effect->recreate() );
		effect->mDelegateOnReset.trigger();
    }
    return true;
}

ID3DXEffectPool*& GEffectManager::getPool()
{
    return mEffectPool;
}


GEffect::GEffect()
    : mD3DEffect ( nullptr )
{

}

GEffect::~GEffect()
{
    dSafeRelease ( mD3DEffect );
}

bool GEffect::recreate()
{
    dSafeRelease ( mD3DEffect );
    return createFromFile ( mFileName.c_str() );
}



#ifndef GGameDemoHeader_h__
#define GGameDemoHeader_h__

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <process.h>

#include <assert.h>

#include "GPlatform.h"

#include "G3DEngine.h"

#include "XBase.h"
#include "common.h"

struct D3DXVECTOR3;
struct D3DXPLANE;
struct D3DXQUATERNION;

typedef CXDynaArray<uString> CharStringArr;



typedef void* GHandle;
extern const char* fiGetMediaPath();

int luaRegistAll();

#endif // GGameDemoHeader_h__
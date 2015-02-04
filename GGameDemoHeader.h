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
typedef CXDynaArray<GString> CharStringArr;

#include "common.h"


typedef void* GHandle;
extern const char* fiGetMediaPath();

int luaRegistAll();

#endif // GGameDemoHeader_h__
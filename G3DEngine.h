#ifndef G3DEngine_h__
#define G3DEngine_h__

//Direct9����ͷ�ļ������ļ�
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3d9types.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif

#endif // G3DEngine_h__

#ifndef common_h__
#define common_h__

#ifndef OUT
#define OUT
#endif

#include "G3DEngine.h"
#include "GPlatform.h"



//定义常量

#define NORMALMATRIX D3DXMATRIX(1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)//单位矩阵

#define ZEROVECTOR3 D3DXVECTOR3(0.0f,0.0f,0.0f)		//零向量

#define ZEROFLOAT 0.0f								//float型零值

#define ERR_INT -1						//返回值为整形的函数，出错时返回的值
#define TRUE_INT 1						//返回值为整形的函数，正确时的返回值

#define ERR_DEBUG   0xffffffff			//调试用的，相当于一个错误值hr

//常用带的函数宏

#define SAFERELEASE(p) if(p!=NULL) { p->Release(); p=NULL;}

#define SAFED_ELETE(p) if(p!=NULL) { delete p; p=NULL;}

#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p)=NULL; } }

#define JUMP_HEIGHT	60.0f							//物体跳起后能达到的高度
#define JUMP_TIME	1.0f							//物体跳起后在空中停留时间的1/2

#define GRAVITY (2*JUMP_HEIGHT/JUMP_TIME/JUMP_TIME)	//重力加速度

#define LOD_MAX_LEVEL 5							//最高LOD等级

#define WINDOW_WIDTH 1024						//窗口宽度
#define WINDOW_HEIGHT 700						//窗口高度
#define dDegreeToAngle(d) ((d)*D3DX_PI/180.0f)
#define dAgngleToDegree(a) ((a)*180/D3DX_PI)

/** 为V3赋值 **/
void dVector ( OUT D3DVECTOR& v, float x, float y, float z );

/** 输出错误时Debug信息 **/
void dDebugMsgBox ( HRESULT hr, const char *StrDebug );

/** 给Mesh设置法线 **/
ID3DXMesh *dSetMeshNormal ( ID3DXMesh *pMesh, IDirect3DDevice9 *DVC );

/** 判断字符串是否存在，是否是空值 **/
bool dIsStrEmpty ( const char * str );

/** 判断文件是否存在 **/
bool dIsFileExist ( const char * sFileName );

void dMakeColor ( D3DCOLORVALUE& out, byte a, byte r, byte g, byte b );

void dGetYawPitchRollFromMatrix ( const D3DXMATRIX& mat, float& roll, float& pitch, float& yaw );
void dGetTranslateFromMatrix ( D3DXVECTOR3& out, const D3DXMATRIX* mat );
void dMatrixInverse ( D3DXMATRIX& mat );
void dMatrixTranslateVec3 ( D3DXVECTOR3& outin, const D3DXMATRIX& mat );

float dVector3Length ( const D3DXVECTOR3& v );
float dVector3Dot ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 );
void dVector3Normalize ( D3DXVECTOR3& v );
void dVector3Cross ( D3DXVECTOR3& out, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 );
D3DXVECTOR3 dVector3NormalizeSlow ( const D3DXVECTOR3& v );
void dGetVector3Ease ( D3DXVECTOR3& out, D3DXVECTOR3* start, D3DXVECTOR3* end, DWORD elpaseTime, DWORD totalTime );

void dGetMatixFromQuation ( D3DXMATRIX&out, D3DXQUATERNION& in );
#endif // common_h__
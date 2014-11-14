#ifndef StdAfx_h__
#define StdAfx_h__
//--------------------------------------------------------------------------------------------------
//Windows����ͷ�ļ������ļ�
#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <Windows.h>
#include <WinBase.h>
#include <tchar.h>
#include <MMSystem.h>
#include <process.h>
#include <shlwapi.h>
#include <vector>
#include <D3D9Types.h>

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"Imm32.lib")
#pragma comment(lib,"Kernel32.lib")


#include <assert.h>

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

#include "XBase.h"
typedef CXDynaArray<GString> CharStringArr;

//////////////////////////�����ļ���Ҫ�ĺ�////////////////////////////////////

//���峣��

#define NORMALMATRIX D3DXMATRIX(1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)//��λ����

#define ZEROVECTOR3 D3DXVECTOR3(0.0f,0.0f,0.0f)		//������

#define ZEROFLOAT 0.0f								//float����ֵ

#define ERR_INT -1						//����ֵΪ���εĺ���������ʱ���ص�ֵ
#define TRUE_INT 1						//����ֵΪ���εĺ�������ȷʱ�ķ���ֵ

#define ERR_DEBUG   0xffffffff			//�����õģ��൱��һ������ֵhr

//���ô��ĺ�����

#define RAND rand()

#define SAFERELEASE(p) if(p!=NULL) { p->Release(); p=NULL;}

#define SAFED_ELETE(p) if(p!=NULL) { delete p; p=NULL;}

#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p)=NULL; } }

inline void toggle ( bool b )
{
    b = !b;
}

//��Ϸ�ж������

#define Default_Force_Height 1.0f				//����Ĭ���ڵ�ͼ�ϵĸ߶�

#define STATIC_MESH_MAX_NUM_ON_MAP 10					//��ͼ�Ͼ�̬ģ���������

#define MAP_MAX_NUM	999								//��ͼ���������
//#define STATICMESHCOUNT							//��Դ��ģ������

#define MESH_PATH    "..\\Data\\res\\StaticMesh\\"				//ģ���ļ���·��

#define FILE_NAME_LENGTH 256						//�ļ�������󳤶�
//��Ծ�߶�




//Ϊ��ʹ������Ծʱ�����ڿ���ͣ����ʱ������Ծ�����в�ʹ���������ٶ�

#define JUMP_HEIGHT	60.0f							//����������ܴﵽ�ĸ߶�
#define JUMP_TIME	1.0f							//����������ڿ���ͣ��ʱ���1/2

#define GRAVITY (2*JUMP_HEIGHT/JUMP_TIME/JUMP_TIME)	//�������ٶ�

#define LOD_MAX_LEVEL 5							//���LOD�ȼ�

#define WINDOW_WIDTH 1024						//���ڿ��
#define WINDOW_HEIGHT 700						//���ڸ߶�

#define Max_Eye_Distance 30000						//�ܿ�������Զ����

#define LIGHT_NUM 1								//��Դ����

#define SEA_DEPTH_FACTOR 0.18f						//��ͼ��߶ȶ���,ռ�ܸ߶ȵı���



//�����ʼ�ٶ�
#define DEFAULT_SPEED_MOVE 100.0f
#define DEAULT_SPEED_TRUN 1.5f


#define MAX_QUAKE_COUNT 8							//��ˮ��Դ��������

//�ṹ���ö������

enum TextPos
{
    tpCenter,
    tpLeft,
    tpRight,
};

//ģ�͵����ͣ�����ģ�͵Ĵ�����ʽ
enum eMeshType
{
    mtMap,	//��ͼ���͵ģ����ͼ������

    mtXFile,	//��X�ļ��м��ص�ģ��

};

struct MapObjInfo
{
    int nObjId;		//����ID
    float x;		//�����Xֵ
    float z;		//�����Zֵ

    float fBlockHeight;
    D3DXVECTOR3 vMin;
    D3DXVECTOR3 vMax;
};

enum eForceType
{
    ftUpAlways,			//�Ϸ������ǣ�0,1,0��
    ftUpWithMap,		//�Ϸ�������洹ֱ�����ͼ���ı�
};
//////////////////////////�����ļ����õĺ���////////////////////////////////////

D3DVECTOR Vector ( float x, float y, float z );

//�������ʱDebug��Ϣ
void DebugMsgBox ( HRESULT hr, const char *StrDebug );

//��Mesh���÷���
ID3DXMesh *dSetMeshNormal ( ID3DXMesh *pMesh, IDirect3DDevice9 *DVC );

//��ȡ���ַ�������
int GetSubCharCount ( const char* Sourc, const char* Dev );

//���ļ������л�ȡ·��
void  GetFilePath ( char* sFileName, char* path );

//����·�����ļ�������ȡ�ļ�ȫ��
void GetFileName ( char *sPathName, char* sFileName );

//�ж��ַ����Ƿ���ڣ��Ƿ��ǿ�ֵ
bool IsStrEmpty ( const char * str );

//����ָ����Χ�����������
int RandIntEx ( int nMin, int nMax );

//����ָ����Χ�ĸ��������
float RandFloatEx ( float fMin, float fMax );

//�������������������
void PreRandom();

//�ж��ļ��Ƿ����
bool IsFileExist ( const char * sFileName );

//��һ���ַ���ת����D3DXVECTOR3
void StrToVector3 ( const char *str , D3DXVECTOR3& v );	//��һ������ά�������ַ�����ֵȡ����

//�жϵ��Ƿ���һ������������
bool IsPointInRect ( POINT pt, RECT *rc );

//��ȡ��Χ��뾶
bool GetBoundRadius ( ID3DXMesh *pMesh, float *pfRadiusOut );
inline void getVector3Ease ( D3DXVECTOR3& out, D3DXVECTOR3* start, D3DXVECTOR3* end, DWORD elpaseTime, DWORD totalTime )
{
    float e = elpaseTime * 0.001f;
    float t = totalTime * 0.001f;
    out.x = gEaser.getEase ( start->x, end->x, e, t );
    out.y = gEaser.getEase ( start->y, end->y, e, t );
    out.z = gEaser.getEase ( start->z, end->z, e, t );
}
inline void dGetTranslateFromMatrix ( D3DXVECTOR3& out, const D3DXMATRIX* mat )
{
    out.x = mat->_41;
    out.y = mat->_42;
    out.z = mat->_43;
}
#define Color_Max 255
inline void dMakeColor ( D3DCOLORVALUE& out, byte a, byte r, byte g, byte b )
{
    out.a = ( float ) a / 255;
    out.r = ( float ) r / 255;
    out.g = ( float ) g / 255;
    out.b = ( float ) b / 255;
}
//inline void dMakeColor ( D3DCOLORVALUE& out, float a, float r, float g, float b )
//{
//    out.a = a;
//    out.r = r;
//    out.g = g;
//    out.b = b;
//}
inline void dVector3Cross ( D3DXVECTOR3& out, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 )
{
    D3DXVec3Cross ( &out, &v1, &v2 );
}
inline void dVector3Normalize ( D3DXVECTOR3& v )
{
    D3DXVec3Normalize ( &v, &v );
}
inline D3DXVECTOR3 dVector3NormalizeSlow ( const D3DXVECTOR3& v )
{
    D3DXVECTOR3 out;
    D3DXVec3Normalize ( &out, &v );
    return out;
}
inline void dMatrixInverse ( D3DXMATRIX& mat )
{
    D3DXMatrixInverse ( &mat, 0, &mat );
}
inline void dMatrixTranslateVec3 ( D3DXVECTOR3& outin, const D3DXMATRIX& mat )
{
    D3DXVec3TransformCoord ( &outin, &outin, &mat );
}
inline float dVector3Length ( const D3DXVECTOR3& v )
{
    return sqrt ( dSequare ( v.x ) + dSequare ( v.y ) + dSequare ( v.z ) );
}
inline float dVector3Dot ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 )
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline void dGetMatixFromQuation ( D3DXMATRIX&out, D3DXQUATERNION& in )
{
    float x2 = in.x * in.x;
    float y2 = in.y * in.y;
    float z2 = in.z * in.z;
    float xy = in.x * in.y;
    float xz = in.x * in.z;
    float yz = in.y * in.z;
    float wx = in.w * in.x;
    float wy = in.w * in.y;
    float wz = in.w * in.z;

    out._11 = 1.0f - 2.0f * ( y2 + z2 );
    out._12 = 2.0f * ( xy + wz );
    out._13 = 2.0f * ( xz - wy );
    out._14 = 0.0f;

    out._21 = 2.0f * ( xy - wz );
    out._22 = 1.0f - 2.0f * ( x2 - z2 );
    out._23 = 2.0f * ( yz + wx );
    out._24 = 0.0f;

    out._31 = 2.0f * ( xz + wy );
    out._32 = 2.0f * ( yz - wx );
    out._33 = 1.0f - 2.0f * ( x2 + y2 );
    out._34 = 0.0f;

    out._41 = 0.0f;
    out._42 = 0.0f;
    out._43 = 0.0f;
    out._44 = 1.0f;
}
#define FiDeclareSingleton( ClassName ) \
	private:\
	static ClassName* mInstance;\
	public:\
	static ClassName& GetSingleton()  { if( mInstance == 0 ) { static ClassName scn; mInstance = &scn; } return *mInstance; }\
	static ClassName* getInstance(){ if( mInstance == 0 ) { static ClassName scn; mInstance = &scn;} return mInstance;}

#define FiImplateSingleton( ClassName ) ClassName* ClassName::mInstance = 0 ;
#define FiMarkSingleton() \
	static bool bInit = false;\
	CXASSERT(!bInit && "not a singleton");\
	bInit = true;\
 
enum eHitType
{
    htNull,							//ʲô��û��ײ��
    htAutoMoveHitNoMap,				//û��ײ��
    htAutoMoveHitNoObj,				//û��ײ������
    htAutoMoveHitMap,				//ײ����ͼ��
    htAutoMoveHitObj,				//ײ��������
    //htAutoMoveHitObjAndMap,		//��ײ����

    htNotAutoMoveHitNoMap,			//û��ײ��
    htNotAutoMoveHitMap,			//ײ����ͼ
    htNotAutoMoveHitObj,			//ײ������
    htNotAutoMoveHitNoObj,			//û��ײ������

    htNotAutoMoveHitBelowMap,		//ײ�������������
    htNotAutoMoveHitNoBelowMap,		//û��ײ�����������

};


//�������ͣ�����ʱ������
enum eObjType
{
    Obj,
    WorldObj,
    VisObj,
    MeshObj,
    AnimMeshObj,
};

#define UpdateWithMap 1

#define OS_LIGHTON				0x0001			//�Ƿ񿪵ƹ�
#define OS_RENDER				0x0002			//�Ƿ���Ⱦ
#define OS_GETINPUT				0x0004			//�Ƿ���Բٿ�
#define OS_UPDATE				0x0008			//�Ƿ���Ը���
#define OS_BEHIT				0x0010			//�Ƿ񱻵����
#define OS_CHANGEWHENHIT		0x0020			//���ʱ�Ƿ����ı�
#define OS_CANBESELECT			0x0040			//�Ƿ���Ա�ѡ��
#define OS_BESELECTED			0x0080			//�Ƿ�ѡ����
#define OS_PLAYDONE				0x0100			//�ö����Ƿ񲥷����

#define OS_DEFAULT				( OS_RENDER || OS_UPDATE )		//Ĭ��״̬


enum eObjAnimState			//�����״̬
{
    oasNULL,
    oasStandBy,
    oasMoving,
    oasTurning,
    oasDead,
    oasAttack,
    oasBeAttack,
    oasRunAttack,
};



enum eObjParentType
{
    optByPosition,
    optByName,
};

struct ForceMapPara
{
    void *pMap;

    float fForceHeight;

    eForceType ft;

    ForceMapPara ( void *Map, float fHeight, eForceType ForceType )
    {
        pMap = Map;

        fForceHeight = fHeight;

        ft = ForceType;
    }

};

enum eMeshUsage
{
    muRender,
    muInsect,
};

struct MeshPara
{
public:

    float mfCellWidth;	    //��ͼ�и��ӿ��
    float mfMaxHeight;	    //��ͼ�����߶�
    int	  LnCellCount;	    //��ͼ�и�������
    GString mstrFileName;     //�����ļ����֣�X�ļ�����
    GString mstrHeightMap;    //�߶�ͼ�ļ�����

public:
    MeshPara() {};

    MeshPara ( float CellWidth, float MaxHight, int CellCount, const char *strFileName, const  char *strHeightMap )
    {
        mfCellWidth = CellWidth;
        mfMaxHeight = MaxHight;
        LnCellCount = CellCount;
        mstrFileName = strFileName;
        mstrHeightMap = strHeightMap;
    }
};

struct VertexXYZAndColor
{
    float x, y, z;
    D3DCOLOR color;
};
typedef void* GHandle;
extern const char* GetMediaPath();
//--------------------------------------------------------------------------------------------------
#endif // StdAfx_h__
#ifndef common_h__
#define common_h__

#ifndef OUT
#define OUT
#endif

#include "G3DEngine.h"
#include "GPlatform.h"



//���峣��

#define NORMALMATRIX D3DXMATRIX(1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)//��λ����

#define ZEROVECTOR3 D3DXVECTOR3(0.0f,0.0f,0.0f)		//������

#define ZEROFLOAT 0.0f								//float����ֵ

#define ERR_INT -1						//����ֵΪ���εĺ���������ʱ���ص�ֵ
#define TRUE_INT 1						//����ֵΪ���εĺ�������ȷʱ�ķ���ֵ

#define ERR_DEBUG   0xffffffff			//�����õģ��൱��һ������ֵhr

//���ô��ĺ�����

#define SAFERELEASE(p) if(p!=NULL) { p->Release(); p=NULL;}

#define SAFED_ELETE(p) if(p!=NULL) { delete p; p=NULL;}

#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p)=NULL; } }

#define JUMP_HEIGHT	60.0f							//����������ܴﵽ�ĸ߶�
#define JUMP_TIME	1.0f							//����������ڿ���ͣ��ʱ���1/2

#define GRAVITY (2*JUMP_HEIGHT/JUMP_TIME/JUMP_TIME)	//�������ٶ�

#define LOD_MAX_LEVEL 5							//���LOD�ȼ�

#define WINDOW_WIDTH 1024						//���ڿ��
#define WINDOW_HEIGHT 700						//���ڸ߶�
#define dDegreeToAngle(d) ((d)*D3DX_PI/180.0f)
#define dAgngleToDegree(a) ((a)*180/D3DX_PI)

/** ΪV3��ֵ **/
void dVector ( OUT D3DVECTOR& v, float x, float y, float z );

/** �������ʱDebug��Ϣ **/
void dDebugMsgBox ( HRESULT hr, const char *StrDebug );

/** ��Mesh���÷��� **/
ID3DXMesh *dSetMeshNormal ( ID3DXMesh *pMesh, IDirect3DDevice9 *DVC );

/** �ж��ַ����Ƿ���ڣ��Ƿ��ǿ�ֵ **/
bool dIsStrEmpty ( const char * str );

/** �ж��ļ��Ƿ���� **/
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
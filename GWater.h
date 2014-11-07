#pragma once

#include "GMeshBaseObj.h"


#define SEAPICNUM   64	//��ˮ����ͼƬ�ĸ���

#define FaceSwitchTime 40 //ÿ���л�5������

struct SeaVertex
{
	D3DXVECTOR3 vertex;	    //x,y,z
	float nx, ny, nz;	  //���ߣ�x��y��z
	float u, v;		   //��������
};

struct QuakePoint
{
	float mInitTime;	//��ʼʱ��
	float mAmplitude;	//���
	float mAngVelocity;	//���ٶȣ�=f*2*PI
	float mfX, mfZ;		//��Դ��λ��
public:
	float getPointEffect ( float _x, float _z, DWORD _t );
	void init ( float _x, float _z, float amplitude, float AngVelocity, DWORD _t );
};
class GQuakePointManager
{
public:
	GQuakePointManager();
	~GQuakePointManager();
	void destory();
	void addPoint ( QuakePoint* point );
	void recreate ( CXIndex cnt ,float angVelocity);
	void setRangeX ( float fmin, float fmax );
	void setRangeZ ( float fmin, float fmax );
	float getPointEffect ( float _x, float _z,DWORD  _t );
private:
	float mMinX;
	float mMaxX;
	float mMinZ;
	float mMaxZ;
	CXDynaArray<QuakePoint*> mPointList;
};
#define FVFSea D3DFVF_XYZ | D3DFVF_TEX1|D3DFVF_NORMAL

class GWater : public GMeshBaseObj
{
	DeclareEditorType ( GWater );
public:
	GWater ( void );
	~GWater ( void );

	virtual void clear() override;
	virtual bool recreate() override;
	virtual void update () override;

	void setParam ( const MeshPara& val );
	void setPointCount ( unsigned int cnt );
public:
	virtual void onPropertyChangeEnd ( void* cur );
private:
	int addQuakePoint ( float _x, float _z, float fAmplitude, float fAngVelocity );

	void recreateQuakeInfo();
	void recreateGraphInfo();
	void recreateMetrialInfo();

	virtual void registerAllProperty();

	virtual void onCallBack( const CXDelegate& ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual bool render();

private:
	float mCellWidth;	    //��ͼ�и��ӿ��
	float mMaxHeight;	    //��ͼ�����߶�
	int	  mCellCount;	    //��ͼ��ÿ�и�������
	/** @brief �𶯽��ٶ� **/
	float mQuakeAngleVelocity;	
	GString mTextureFile;    //�����ļ����֣�X�ļ�����

	LPDIRECT3DTEXTURE9 *mpFace;

	GQuakePointManager mQuakeManager;

	/** @brief ��Դ���� **/
	int mQuakeCount;
};

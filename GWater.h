#pragma once

#include "GMeshBaseObj.h"


#define SEAPICNUM   64	//海水纹理图片的个数

#define FaceSwitchTime 40 //每秒切换5个纹理

struct SeaVertex
{
	D3DXVECTOR3 vertex;	    //x,y,z
	float nx, ny, nz;	  //法线，x，y，z
	float u, v;		   //纹理坐标
};

struct QuakePoint
{
	float mInitTime;	//初始时间
	float mAmplitude;	//振幅
	float mAngVelocity;	//角速度，=f*2*PI
	float mfX, mfZ;		//振动源的位置
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
	float mCellWidth;	    //地图中格子宽度
	float mMaxHeight;	    //地图的最大高度
	int	  mCellCount;	    //地图中每行格子数量
	/** @brief 震动角速度 **/
	float mQuakeAngleVelocity;	
	GString mTextureFile;    //纹理文件名字，X文件名字

	LPDIRECT3DTEXTURE9 *mpFace;

	GQuakePointManager mQuakeManager;

	/** @brief 振动源数量 **/
	int mQuakeCount;
};

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
    float GetPointEffect ( float _x, float _z, float _t );
    void Init ( float _x, float _z, float amplitude, float AngVelocity, float _t );
};

#define FVFSea D3DFVF_XYZ | D3DFVF_TEX1|D3DFVF_NORMAL

class CSea : public GMeshBaseObj
{
public:
    CSea ( void );
    ~CSea ( void );

    virtual bool reCreate( );

	void update ();

    int AddQuakePoint ( float _x, float _z, float fAmplitude, float fAngVelocity );
public:
    MeshPara* getParam() const;
    void setParam ( const MeshPara& val );
private:
    float mfCellWidth;	    //地图中格子宽度
    float mfMaxHeight;	    //地图的最大高度
    int	  mCellCount;	    //地图中每行格子数量
    char* mstrFileName;    //纹理文件名字，X文件名字
    char* mstrHeightMap;   //高度图文件名字

    MeshPara*	mCreateParam;

    LPDIRECT3DTEXTURE9 *mpFace;

    QuakePoint mQuakList[MAX_QUAKE_COUNT];

    int mQuakeCount;

};

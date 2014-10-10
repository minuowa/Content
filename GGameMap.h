#pragma once
#include "GMeshBaseObj.h"


struct MAPVERTEX
{
    D3DXVECTOR3 vVertex;
    float nx, ny, nz;
    float u, v;
};




#define FVFMAP D3DFVF_XYZ | D3DFVF_TEX1|D3DFVF_NORMAL

class GGameMap :
    public GMeshBaseObj
{
    public:
        GGameMap( void );
        ~GGameMap( void );

		bool reCreate();

		bool render(  );

        void LoadFromFile( char *sConfigName );

        bool IsBlock( D3DXVECTOR3 vPos, int *pCounter );

    public:
        MapObjInfo mppStaticObj[1000];


        float mfCellWidth;	    //地图中格子宽度
        float mfMaxHeight;	    //地图的最大高度
        int	  LnCellCount;	    //地图中每行格子数量
        char* mstrFileName;    //纹理文件名字，X文件名字
        char* mstrHeightMap;   //高度图文件名字

    protected:

        int _nNumFace;			//材质数量

        ID3DXMesh *_pMesh;

        LPDIRECT3DTEXTURE9 *_pTexture;	//纹理组

        D3DMATERIAL9 *_pMat;		//材质组



    private:

        int AfterCreate();

		bool StrToMapObjInfo( const char *str, MapObjInfo *pObjInfo );
};

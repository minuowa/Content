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


        float mfCellWidth;	    //��ͼ�и��ӿ��
        float mfMaxHeight;	    //��ͼ�����߶�
        int	  LnCellCount;	    //��ͼ��ÿ�и�������
        char* mstrFileName;    //�����ļ����֣�X�ļ�����
        char* mstrHeightMap;   //�߶�ͼ�ļ�����

    protected:

        int _nNumFace;			//��������

        ID3DXMesh *_pMesh;

        LPDIRECT3DTEXTURE9 *_pTexture;	//������

        D3DMATERIAL9 *_pMat;		//������



    private:

        int AfterCreate();

		bool StrToMapObjInfo( const char *str, MapObjInfo *pObjInfo );
};

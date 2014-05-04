#pragma once

#include "XList.h"
//#include "EasyList.h"
//������Mesh�Ƕ�Ӧ��
class GTexture
{
public:
	GTexture();
	bool Create();
	inline IDirect3DTexture9* GetTexture()
	{
		return mD3DTexture;
	}
public:
	CharString			FileName;
protected:
	IDirect3DTexture9*	mD3DTexture;
};
//class GTextureNode
//{
//    public:
//        GTextureNode() {};
//        ~GTextureNode() {};
//    public:
//
//        CXDynaArray<IDirect3DTexture9*> mFaceArr;
//
//        LPDIRECT3DTEXTURE9 *ppTexture;
//
//        int LnID;	    //��Ӧ��ģ�͵�ID
//        //
//        //int LnIndex;    //��ģ���е�����
//
//        int LnCount;    //����ͼƬ�������������������Ӧ
//
//};
typedef CXMap<CharString,GTexture*> GTextureMap;
class GTextureBuffer
{
	CXDeclareSingleton(GTextureBuffer);
    public:
        GTextureBuffer( void );
        ~GTextureBuffer( void );
		GTexture* GetTexture(const char* fileName);
		IDirect3DTexture9* LoadFormFile(const char* fileName);
    public:
		GTextureMap		mTextureMap;
};
#define TextureMgr	GTextureBuffer::GetSingleton()
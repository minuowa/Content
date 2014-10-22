#pragma once

#include "GTexture.h"
#include "GMeshBaseObj.h"
#include "GMeshData.h"

struct MeshNodeForLod   //LODMeshµÄ½Úµã
{
    ID3DXMesh *pMesh[LOD_MAX_LEVEL];

    MeshNodeForLod()
    {
        ZeroMemory( pMesh, sizeof( pMesh ) );
    }

};

class GMeshBufferNode
{
    public:

        GMeshBufferNode();

        ~GMeshBufferNode();

        HRESULT MakeLod( DWORD* pAdj );

		bool draw();

		void Add(GMetrialData* data)
		{
			mRenderData.push_back(data);
		}
		WORD getSubCount() const { return mSubSetCount; }
		void setSubCount(WORD val) { mSubSetCount = val; }
		GMetrialData* getMaterial(CXIndex idx) const;
    public:
		ID3DXMesh*		mMesh;
		ID3DXMesh* getMesh() const { return mMesh; }
		void setMesh(ID3DXMesh* val) { mMesh = val; }
		WORD			mSubSetCount;
		GRenderDataArr	mRenderData;
		GString		mFileName;
};

typedef CXMap<GString,GMeshBufferNode*> GMeshBufferNodeMap;
class GMeshManager
{
        CXDeclareSingleton( GMeshManager );
    public:
        GMeshManager( void );
        ~GMeshManager( void );
        bool Init();
		GMeshBufferNode*	QueryCreate(const char* fileName);
public:
	GMeshBufferNode*	CreateFormFile( const char* fileName );
        GMeshBufferNodeMap	mMeshMap;
};
#define MeshMgr GMeshManager::GetSingleton()
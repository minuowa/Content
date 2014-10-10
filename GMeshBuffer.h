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

        HRESULT LoadFromFile( int nID );

        HRESULT MakeLod( DWORD* pAdj );

		bool Render();

		void Add(GMetrialData* data)
		{
			mRenderData.push_back(data);
		}
		WORD SubSetCount() const { return mSubSetCount; }
		void SubSetCount(WORD val) { mSubSetCount = val; }
    public:
		ID3DXMesh*		mMesh;
		ID3DXMesh* Mesh() const { return mMesh; }
		void Mesh(ID3DXMesh* val) { mMesh = val; }
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
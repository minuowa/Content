#pragma once
#include "GBound.h"
#include "GRenderObject.h"

class GMeshBufferNode;
class GMeshBaseObj :
    public GRenderObject
{
	DeclareEditorType(GMeshBaseObj);
    public:
        GMeshBaseObj( void );
        ~GMeshBaseObj( void );

		virtual void registerAllProperty();

		bool render();

        ID3DXMesh *GetMeshByUsage( eMeshUsage mu );

        bool Pick( POINT pt );

        bool CheckIntersect(
            D3DXVECTOR4 vPos,	//世界坐标系中的点
            D3DXVECTOR4 vDir,	//世界坐标系中的向量
            bool bInsectInfo	//是否需要碰撞信息
        );
		ID3DXMesh* ResetVectorMesh();
protected:
        //渲染所需参数
        GMeshBufferNode* mMeshBufferNode;
		ID3DXMesh*	mpMeshForVector;
};

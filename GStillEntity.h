#pragma once
#include "GBound.h"
#include "GRenderEntity.h"
enum eMeshUsage
{
	eMeshUsageRender,
	eMeshUsageInsect,
};
class GMeshBufferNode;
class GStillEntity :
	public GRenderEntity
{
	DeclareEditorType(GStillEntity);
public:
	GStillEntity( void );
	~GStillEntity( void );

	virtual void registerAllProperty();

	bool render();

	ID3DXMesh *getMeshByUsage( eMeshUsage mu );

	bool pick( const POINT& pt );

	bool checkIntersect(const D3DXVECTOR4& vPos, /*世界坐标系中的点 */ const D3DXVECTOR4& vDir, /*世界坐标系中的向量 */ bool bInsectInfo /*是 裥枰鲎残畔?*/ );

	/** @brief 生成碰撞mesh **/
	ID3DXMesh* recreateInsectMesh();
protected:
	//渲染所需参数
	GMeshBufferNode* mMeshBufferNode;
	ID3DXMesh*	mMeshForInsect;
};

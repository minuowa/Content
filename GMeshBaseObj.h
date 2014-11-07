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

	ID3DXMesh *getMeshByUsage( eMeshUsage mu );

	bool pick( const POINT& pt );

	bool checkIntersect(const D3DXVECTOR4& vPos, /*��������ϵ�еĵ� */ const D3DXVECTOR4& vDir, /*��������ϵ�е����� */ bool bInsectInfo /*�� ���Ҫ��ײ���?*/ );

	/** @brief ������ײmesh **/
	ID3DXMesh* recreateInsectMesh();
protected:
	//��Ⱦ�������
	GMeshBufferNode* mMeshBufferNode;
	ID3DXMesh*	mMeshForInsect;
};

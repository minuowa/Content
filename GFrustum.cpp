#include "GGameDemoHeader.h"
#include "GFrustum.h"
#include "GCamera.h"
#include "GTerrainCommon.h"


GFrustum::GFrustum ( void )
{
}


GFrustum::~GFrustum ( void )
{
}

void GFrustum::update ( GCamera* camera )
{
	float fFar = camera->getFar();
	float fNear = camera->getNear();
	float y = ( float ) ( ( fFar - fNear ) * tanf ( camera->getFov() / 2.0 ) );

	float x = y * camera->getAspect();

	//顶点示意图
	//1.........2
	//4.........3
	Points[0] = D3DXVECTOR3 ( 0, 0, 0 );
	Points[1] = D3DXVECTOR3 ( -x, y, fFar );
	Points[2] = D3DXVECTOR3 ( x, y, fFar );
	Points[3] = D3DXVECTOR3 ( x, -y, fFar );
	Points[4] = D3DXVECTOR3 ( -x, -y, fFar );
	//Points[0] = new Vector3(0, 0, camera.Near);
	//Points[1] = new Vector3(-x, y, camera.Far );
	//Points[2] = new Vector3(x, y, camera.Far );
	//Points[3] = new Vector3(x, -y, camera.Far);
	//Points[4] = new Vector3(-x, -y, camera.Far);

	//将本地坐标转换到世界坐标系中
	//将世界坐标系系中点转换到物体本体坐标系中

	mCenter = D3DXVECTOR3 (
		0, 0, 0.5f * ( fFar - fNear )
		);
	mRadius = dVector3Length(Points[1] - mCenter);
}

bool GFrustum::isInFrustum ( GCubeBound* bound )
{
	return dVector3Length ( mCenter - bound->mCenter ) < ( mRadius + bound->mRadius );
}

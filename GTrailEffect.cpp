#include "GGameDemoHeader.h"
#include "GTrailEffect.h"
#include "GAnimMeshObj.h"

GTrailEffect::GTrailEffect ( void )
	: mLastTranslate ( 0, 0, 0 )
	, mLastDir ( 0, 0, 0 )
{
}

GTrailEffect::~GTrailEffect ( void )
{
}

void GTrailEffect::Update()
{
	if ( mLinker1 && mLinker2 )
	{
		D3DXVECTOR3 v1 ( mLinker1->mTransform->_41, mLinker1->mTransform->_42, mLinker1->mTransform->_43 );
		D3DXVECTOR3 v2 ( mLinker2->mTransform->_41, mLinker2->mTransform->_42, mLinker2->mTransform->_43 );

		//D3DXVECTOR3 vL1(mLastMat1._41,mLastMat1._42,mLastMat1._43);
		//D3DXVECTOR3 vL2(mLastMat2._41,mLastMat2._42,mLastMat2._43);

		D3DXVECTOR3 curTranslate ( v1 );
		D3DXVECTOR3 curDir ( v2 - v1 );

		//D3DXMATRIX curWorld;
		//D3DXMatrixIdentity(&curWorld,&curWorld);

		//if (v1!=vL1 && v2!=vL2)
		if ( curTranslate != mLastTranslate || curDir != mLastDir )
		{
			//Generate(v1,v2);
			//mLastMat1 = v1;
			//mLastMat2 = v2;
		}
	}
}

void GTrailEffect::Init ( CChar* linkerName1, CChar* linkerName2 )
{
	mLinkerName1 = linkerName1;
	mLinkerName2 = linkerName2;
}

void GTrailEffect::Attach ( GAnimMeshObj* anim )
{
	CXASSERT_RETURN ( anim );
	GAnimationResource* res = anim->GetResource();
	CXASSERT ( res );
	mLinker1 = res->GetBoneInfo ( mLinkerName1 );
	mLinker2 = res->GetBoneInfo ( mLinkerName2 );

	CXASSERT_RETURN ( mLinker1 && mLinker2 );
}

int G_TrailEffectCount = 10;
void GTrailEffect::Generate ( D3DXMATRIX* v1, D3DXMATRIX*v2 )
{
	assert ( mLinker1 && mLinker2 );

	D3DXQUATERNION q1, q2, qRes;
	//MatrixToQuaternion(v1,q1);
	//MatrixToQuaternion(v2,q2);

	for ( int i = 0; i < G_TrailEffectCount; ++i )
	{
		D3DXQuaternionSlerp ( &qRes, &q1, &q2, i * 1.0f / G_TrailEffectCount );

	}

}

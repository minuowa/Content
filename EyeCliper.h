#pragma once



class CEyeCliper
{
	/********************************************************************

	1,���۾��������֮��Ķ�����Բ����޳������Կ��Լ�Ϊһ����׶

	2��ֻ������Զ����ĸ����㣬�������㷨�ĸ��Ӷ�

	********************************************************************/

public:

	CEyeCliper(void);

	~CEyeCliper(void);

	CEyeCliper(float fn,float angle, float wh);

	D3DXVECTOR3 mVertex[5];

	D3DXPLANE mPlane[5];

public:

	bool IsInEye(D3DXVECTOR3 vPos);

	void Update( D3DXMATRIX *pMat );

};

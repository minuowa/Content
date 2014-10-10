#ifndef GCuller_h__
#define GCuller_h__
//-------------------------------------------------------------------------
/**
	@brief
		1,���۾��������֮��Ķ�����Բ����޳������Կ��Լ�Ϊһ����׶
		2��ֻ������Զ����ĸ����㣬�������㷨�ĸ��Ӷ�
	@author nnboy,9/10/2014  16:49
*/
class GCuller
{
public:
    GCuller ( void );

    ~GCuller ( void );

    GCuller ( float fn, float angle, float wh );

    D3DXVECTOR3 mVertex[5];

    D3DXPLANE mPlane[5];

public:

    bool IsInEye ( D3DXVECTOR3 vPos );

    void Update ( D3DXMATRIX *pMat );

};
#endif // GCuller_h__

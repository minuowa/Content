#ifndef GCuller_h__
#define GCuller_h__
//-------------------------------------------------------------------------
/**
	@brief
		1,在眼睛和最近面之间的对象可以不做剔除，所以可以简化为一个视锥
		2，只计算最远面的四个顶点，减少了算法的复杂度
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

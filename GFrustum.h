#pragma once
class GCamera;
struct GCubeBound;
class GFrustum
{
public:
	GFrustum(void);
	~GFrustum(void);

	void init(GCamera* camera);

	D3DXVECTOR3 mCenter;
	float mRadius;
protected:
	bool isInFrustum(GCubeBound* bound);

	D3DXPLANE Plane[5];
	D3DXVECTOR3 Points[5];

};


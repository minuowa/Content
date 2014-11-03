#pragma once
class GCamera;
struct GCubeBound;
class GFrustum
{
public:
	GFrustum(void);
	~GFrustum(void);

	void update(GCamera* camera);
	bool isInFrustum(GCubeBound* bound);
protected:
	D3DXPLANE Plane[5];
	D3DXVECTOR3 Points[5];
	D3DXVECTOR3 CameraPos;

	D3DXVECTOR3 mCenter;
	float mRadius;
};


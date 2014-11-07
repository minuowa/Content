#ifndef GHeightMap_h__
#define GHeightMap_h__

class GHeightMap
{
public:
	GHeightMap ( float Width, float Length, float MinHeight, float MaxHeight, const char* FileName );
	~GHeightMap();
	bool load();
	float GetHeight ( float X, float Z );
	void SetHeight ( float X, float Z, float Y );
	void Save();
protected:
	DWORD*	mDatas;
	int		mWidth;
	int		mHeight ;
	float	mRanageX;
	float	mRanageZ;
	float	mMinHeight;
	float	mMaxHeight;
	GString mFileName;
};

#endif // GHeightMap_h__

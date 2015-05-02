#pragma once
class GTexture;
enum eTerrainBrushType
{
	eTerrainBrushType_A,
	eTerrainBrushType_R,
	eTerrainBrushType_G,
	eTerrainBrushType_B,
	eTerrainBrushType_Count,
};
class GTerrainBrush
{
public:
	GTerrainBrush(void);
	~GTerrainBrush(void);
	void setXMLFile(const char* name);
	bool recreate();
	void clear();
	inline GTexture* getTexture(eTerrainBrushType type) const;
private:
	uString mXMLFile;
	GTexture* mTexture[eTerrainBrushType_Count];
};
inline GTexture* GTerrainBrush::getTexture ( eTerrainBrushType type ) const
{
	return mTexture[type];
}

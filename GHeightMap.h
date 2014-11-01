namespace GameEditor
{
class GxHeightMap
{
public:
    GxHeightMap ( float Width, float Length, float MinHeight, float MaxHeight, const char* FileName );

    float GetHeight ( float X, float Z );
    void SetHeight ( float X, float Z, float Y );
    void Save();
protected:
	int*	Datas;
	int		Width;
	int		Height ;
	float	RanageX;
	float	RanageZ;
	float	MinHeight;
	float	MaxHeight;
	GString FileName;
};
}


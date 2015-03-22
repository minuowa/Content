#pragma once


enum OBJTYPE
{
    otEye, otWorld, otBillBoard, otUI
};

enum TextPos
{
	tpCenter,
	tpLeft,
	tpRight,
};

enum eFontSize
{
    fs8, fs9, fs10, fs11, fs12, fs14, fs16, fs18, fs20, fs22, fs24, fs26, fs28, fs36, fs48, fs72,
    //字体大小枚举{8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,72};
};
class GText: public CXCallBack
{
public:
    GText ( void );
    ~GText ( void );

	CXDelegate mDelegateOnDrawTextBegin;

    bool init();
    void clear();
    bool recreate();
    void addText ( GString* str );
    void removeText ( GString* str );
    void drawText();
    void setCurrentFontSize ( eFontSize fs );
	ID3DXSprite* getSprite() const;
    int DrawTextDefault ( char* sText, D3DVECTOR pos );
    //2D中使用,其坐标原点为屏幕左上角

    int DrawTextInRect ( char* sText, RECT *r, DWORD Color, OBJTYPE ot, TextPos tp );

    //3D中使用，其坐标系为物体本地坐标系
    int DrawTextByPosColOt ( char* sText, D3DVECTOR Pos, DWORD Color, OBJTYPE ot, bool Center ); //center表示是否居中显示，Pos.x,Pos.y为中心坐标

    void DrawPic (
        LPDIRECT3DTEXTURE9 Pic,
        D3DVECTOR Pos,
        bool IsCenter,  //是否以Pos为中心绘制纹理
        OBJTYPE ot,
        bool bAlphaBlend,//是否与当前纹理混合
        D3DCOLOR dwColor //纹理掩盖色
    );

	virtual void onCallBack ( const CXDelegate& , CXEventArgs*  )override;

    //2D3D都可使用

protected:
    CXDynaArray<GString*> mTextArray;
private:
    ID3DXFont *mFonts[16];
    ID3DXFont *mCurrentFont;

    eFontSize	mFontSize;
    LPD3DXSPRITE mFontSprite;
};

inline ID3DXSprite* GText::getSprite() const
{
	return mFontSprite;
}

#define TextMgr CXSingleton<GText>::getInstance()

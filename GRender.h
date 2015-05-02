#pragma once
enum GRenderTech
{
    GRenderTech_Primite,
	GRenderTech_PrimiteWithPictureAlpha,
	GRenderTech_PrimiteWithPictureAlphaFromTextureColorFromDiffuse,
    GRenderTech_Count,
};
enum GStateType
{
    GSatteType_Render,
    GStateType_Texture0,
    GStateType_Texture1,
    GStateType_Count,
};
struct GRenderData
{
    GStateType mState;
    u32 mType;
    u32 mValue;
};
class GRenderState
{
public:
    GRenderState();
    ~GRenderState();
    void add ( GStateType stateType, u32 type, u32 value );
    void set();
protected:
    CXDynaArray<GRenderData*> mData;
};
class GRender
{
public:
    GRender ( void );
    ~GRender ( void );

    void setTech ( GRenderTech type );

private:
    GRenderState mRenderState[GRenderTech_Count];
};


#pragma once

class GD9Device
{
public:
    static const int LIGHT_NUM = 8;

    GD9Device ( void );
    ~GD9Device ( void );

private:

    IDirect3DDevice9 *mD9Device;

public:

    HWND mHwnd;

    int mWidth;	    //渲染区域宽度
    int mHeight;	    //渲染区域高度

    D3DLIGHT9 mLight[LIGHT_NUM];

    D3DCAPS9 mCaps;

    IDirect3D9 *mD3D9;
public:
    int GetScreenWidth() const;
    int GetScreenHeight() const;

    bool Init ( HWND hWnd ); //初始化3D设备

    bool BeginRender();	    //开始绘制

    void EndRender();	    //结束绘制

    void SetLight();	    //设置初始灯光

    void setBackColor ( DWORD color );
	DWORD getBackColor();

    void OpenLight ( int nIndex, bool bOpen, bool bUseMatrialColor = false ); //开启或者关闭指定的光源

    void OpenlightEx ( int nIndex, bool bOpen, float fDiffuseIntensity, bool bUseMatrialColor = false );

    void openAllLight ( bool bOpen, bool bUseMatrialColor = false ); //开启所有光源

    void OpenAllLightEx ( bool bOpen, float fDiffuseIntensity, float fAmbientIntensity, float fSpecularIntensity, bool bUseMatrialColor = false ); //设置指定参数光源，并开启

    void openAlphaBlend ( bool bOpen );	//开启Alpha混合

    void ResetRenderState();

    void BeginMirrorRender();

    void OpenUIAlpha ( bool bTransparent );

    bool VertexShaderSurport() const;

	bool beginRenderUI();

    inline IDirect3DDevice9 *GetDvc() const;
    inline int TestDevice();
    void OnDeviceLost();
    void OnResize ( int w, int h );

	void renderFirstGraph(bool useTexture);
protected:
    bool ResetDevice ( int w, int h );
    DWORD mBackColor;
public:
    CXDelegate mOnLostDevice;
    CXDelegate mOnResetDevice;
};
typedef CXSingleton<GD9Device> GSingletonD9Device;
#define  D9DEVICE	GSingletonD9Device::getInstance()

inline int GD9Device::TestDevice()
{
    return mD9Device->TestCooperativeLevel();
}

inline IDirect3DDevice9 * GD9Device::GetDvc() const
{
    return mD9Device;
}

#pragma once

class GD9Device
{
public:
	GD9Device ( void );
    ~GD9Device ( void );

private:

    IDirect3DDevice9 *mD9Device;

public:

    HWND mhWnd;

    int mWidth;	    //��Ⱦ������
    int mHeight;	    //��Ⱦ����߶�

    D3DLIGHT9 mLight[LIGHT_NUM];

    D3DCAPS9 mCaps;

	IDirect3D9 *mD3D9;
public:
    int GetScreenWidth() const;
    int GetScreenHeight() const;

    bool Init ( HWND hWnd ); //��ʼ��3D�豸

    bool BeginRender();	    //��ʼ����

    void EndRender();	    //��������

    void SetLight();	    //���ó�ʼ�ƹ�

    void OpenLight ( int nIndex, bool bOpen, bool bUseMatrialColor = false ); //�������߹ر�ָ���Ĺ�Դ

    void OpenlightEx ( int nIndex, bool bOpen, float fDiffuseIntensity, bool bUseMatrialColor = false );

    void OpenAllLight ( bool bOpen, bool bUseMatrialColor = false ); //�������й�Դ

    void OpenAllLightEx ( bool bOpen, float fDiffuseIntensity, float fAmbientIntensity, float fSpecularIntensity, bool bUseMatrialColor = false ); //����ָ��������Դ��������

    void OpenAlphaBlend ( bool bOpen );	//����Alpha���

    void ResetRenderState();

    void BeginMirrorRender();

    void OpenUIAlpha ( bool bTransparent );

    bool VertexShaderSurport() const;

    IDirect3DDevice9 *GetDvc() const
    {
        return mD9Device;
    }

    void OnDeviceLost();
	void OnResize(int w,int h);
protected:
	bool ResetDevice(int w,int h);
};
typedef CXSingleton<GD9Device> GSingletonD9Device;
#define  D9DEVICE	GSingletonD9Device::GetSingletonPtr()

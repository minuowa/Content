#include "GGameDemoHeader.h"
#include "GD9Device.h"

GD9Device::GD9Device ( void )
{
    mD9Device = NULL;
    mD3D9 = 0;
    mBackColor = 0;
    mHwnd = 0;
}

GD9Device::~GD9Device ( void )
{
    dSafeRelease ( mD9Device );
    dSafeRelease ( mD3D9 );
}

bool GD9Device::init ( HWND hWnd )
{
    mHwnd = hWnd;
    //��̨�������Ϳͻ�����Сһ�£�����ͼ��ʧ��
    RECT rcClient;
    GetClientRect ( hWnd, &rcClient );
    mHeight = rcClient.bottom - rcClient.top;
    mWidth = rcClient.right - rcClient.left;

    return ResetDevice ( mWidth, mHeight );
}

bool GD9Device::BeginRender()
{
    if ( mD9Device == NULL )
        return false;


    //��һ��ָ������ɫ����ӿڣ������ӿ��е�һ���������������Ȼ����ģ�建��

    //RECT rc={300,300,500,330};
    //D3DRECT d3dRc[4];
    //d3dRc[0].x1=0;
    //d3dRc[0].y1=0;
    //d3dRc[0].x2=300;
    //d3dRc[0].y2=700;

    //d3dRc[1].x1=500;
    //d3dRc[1].y1=0;
    //d3dRc[1].x2=1008;
    //d3dRc[1].y2=700;

    //d3dRc[2].x1=300;
    //d3dRc[2].y1=0;
    //d3dRc[2].x2=500;
    //d3dRc[2].y2=330;

    //d3dRc[3].x1=300;
    //d3dRc[3].y1=330;
    //d3dRc[3].x2=500;
    //d3dRc[3].y2=700;

    if ( FAILED ( mD9Device->Clear ( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, mBackColor, 1.0f, 0 ) ) )
    {
        assert ( 0 && "Clear device failed!" );
        return false;
    }

    //mD9Device->Clear(2,&d3dRc[0],D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0x99999999,1.0f,0);

    if ( FAILED ( mD9Device->BeginScene() ) )
    {
        return false;
    }

    return true;
}

void GD9Device::EndRender()
{
    if ( mD9Device != NULL )
    {
        mD9Device->EndScene();
    }

    if ( D3DERR_DEVICELOST == mD9Device->Present ( 0, 0, mHwnd, 0 ) )
    {
        CXASSERT ( 0 );
    }
}
#define DIFFUSE_INTENSITY 1.0f//1.33f			//�������ǿ��
#define AMBIENT_INTENSIYT 1.0f					//������ǿ�� 0.3f
#define SPECULAR_INTENSITY 1.0f					//�����ǿ��

void GD9Device::SetLight()
{
    dMemoryZero ( mLight, sizeof ( mLight ) );
    for ( int i = 0; i < LIGHT_NUM; i++ )
    {
        mLight[i].Type = D3DLIGHT_DIRECTIONAL;

        mLight[i].Diffuse.a = DIFFUSE_INTENSITY;
        mLight[i].Diffuse.r = DIFFUSE_INTENSITY;
        mLight[i].Diffuse.g = DIFFUSE_INTENSITY;
        mLight[i].Diffuse.b = DIFFUSE_INTENSITY;

        mLight[i].Ambient.a = AMBIENT_INTENSIYT;
        mLight[i].Ambient.r = AMBIENT_INTENSIYT;
        mLight[i].Ambient.g = AMBIENT_INTENSIYT;
        mLight[i].Ambient.b = AMBIENT_INTENSIYT;

        mLight[i].Specular.a = SPECULAR_INTENSITY;
        mLight[i].Specular.r = SPECULAR_INTENSITY;
        mLight[i].Specular.g = SPECULAR_INTENSITY;
        mLight[i].Specular.b = SPECULAR_INTENSITY;
    }

    dVector ( mLight[0].Direction, 1.0f, -0.5f , 0.0f );

    //mLight[0].Direction=Vector(-3.0f,-2.0f,-1.732f);
    //mLight[1].Direction=Vector(3.0f,-2.0f,-1.732f);
    //mLight[2].Direction=Vector(0.0f,-2.0f,3.464f);
    //mLight[3].Direction=Vector(0.0f,4.0f,0.0f);
    //mLight[3].Direction=Vector(0.0f,-4.0f,0.0f);

    if ( mD9Device != NULL )
    {
        for ( int i = 0; i < LIGHT_NUM; i++ )
        {
            mD9Device->SetLight ( i, &mLight[i] );
        }
    }

}



void GD9Device::OpenLight ( int nIndex, bool bOpen, bool bUseMatrialColor/*=false*/ )
{
    mD9Device->SetRenderState ( D3DRS_LIGHTING, bOpen );
    mD9Device->LightEnable ( nIndex, bOpen );
    mD9Device->SetRenderState ( D3DRS_AMBIENT, 0xff4a4a4a );
    mD9Device->SetRenderState ( D3DRS_SPECULARENABLE, bOpen );

    //����������ɫΪ��һ��������������ɫΪ�ڶ�����
    mD9Device->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

    if ( bUseMatrialColor )
    {
        mD9Device->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTSS_COLORARG2 );
    }
    else
    {
        if ( bOpen )
        {
            mD9Device->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE ); //���˹��գ�ʹ�����߻����ɫ
        }
        else
        {
            mD9Device->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTSS_COLORARG1 ); //û�����գ�ֻʹ��������ɫ
        }
    }
}



void GD9Device::openAllLight ( bool bOpen, bool bUseMatrialColor/*=false*/ )
{
    if ( bOpen )
    {
        for ( int i = 0; i < LIGHT_NUM; i++ )
        {
            OpenLight ( i, bOpen, bUseMatrialColor );
        }
    }
    else
    {
        mD9Device->SetRenderState ( D3DRS_LIGHTING, false );
    }
}



void GD9Device::OpenAllLightEx ( bool bOpen, float fDiffuseIntensity, float fAmbientIntensity, float fSpecularIntensity, bool bUseMatrialColor/*=false*/ )
{
    mD9Device->SetRenderState ( D3DRS_LIGHTING, bOpen );

    if ( bOpen )
    {

        for ( int i = 0; i < LIGHT_NUM; i++ )
        {
            if ( fDiffuseIntensity != ZEROFLOAT )
            {
                mLight[i].Diffuse.r = fDiffuseIntensity;
                mLight[i].Diffuse.g = fDiffuseIntensity;
                mLight[i].Diffuse.b = fDiffuseIntensity;

                mD9Device->SetLight ( i, &mLight[i] );
                OpenLight ( i, bOpen, bUseMatrialColor );
            }

        }

    }
}

void GD9Device::openAlphaBlend ( bool bOpen )
{
    mD9Device->SetRenderState ( D3DRS_ALPHABLENDENABLE, bOpen );

    if ( bOpen )
    {
        mD9Device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        mD9Device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
        mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
        mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

        ////��ɫ��ϣ���͸��
        //mD9Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
        //mD9Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
    }
}


void GD9Device::ResetRenderState()
{
    //�رձ����ѡ,ʹ���涼�ܿ���,Ĭ��˳ʱ�뷽��ķ���Ϊ������ʹ�������жϹ���
    mD9Device->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );

    //������Ȼ���
    mD9Device->SetRenderState ( D3DRS_ZENABLE, true );
    mD9Device->SetRenderState ( D3DRS_ZWRITEENABLE, true );

    //������ز�����ȫ�������
    mD9Device->SetRenderState ( D3DRS_MULTISAMPLEANTIALIAS, true );

    //���ø��޵���ɫģʽ
    mD9Device->SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );


    //��С���Ŵ����ģʽ�²�������������˲���
    mD9Device->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    mD9Device->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    //mD9Device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

    //���ģʽ
    //mD9Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT);//�����
    //Content::Device.SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID ); //��������
    //Content::Device.SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);//�߿����

    //�ر�Alpha���
    mD9Device->SetRenderState ( D3DRS_ALPHABLENDENABLE, false );

    //�رչ���
    mD9Device->SetRenderState ( D3DRS_LIGHTING, false );

    //�رջ�����
    mD9Device->SetRenderState ( D3DRS_AMBIENT, 0xFFFFFFFF );

    //�رվ��淴��
    mD9Device->SetRenderState ( D3DRS_SPECULARENABLE, true );

    //������������Ѱַ��ʽΪѭ��Ѱַ
    mD9Device->SetSamplerState ( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
    mD9Device->SetSamplerState ( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
    //mD9Device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);//������������任
    //mD9Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);//ʹ�ö�������������

    D3DMATERIAL9 matr;
    dMemoryZero ( &matr, sizeof ( D3DMATERIAL9 ) );
    mD9Device->SetMaterial ( &matr );
    //�ر���������任
    D3DXMATRIX mat;
    D3DXMatrixIdentity ( &mat );
    mD9Device->SetTransform ( D3DTS_TEXTURE0, &mat );

    //�ö�����ɫ��Ϊ����
    //mD9Device->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_COLOR1);
}


void GD9Device::OpenlightEx ( int nIndex, bool bOpen, float fDiffuseIntensity, bool bUseMatrialColor/*=false*/ )
{
    if ( fDiffuseIntensity != ZEROFLOAT )
    {
        mLight[nIndex].Diffuse.r = fDiffuseIntensity;
        mLight[nIndex].Diffuse.g = fDiffuseIntensity;
        mLight[nIndex].Diffuse.b = fDiffuseIntensity;
    }

    mD9Device->SetLight ( nIndex, &mLight[nIndex] );

    OpenLight ( nIndex, bOpen, bUseMatrialColor );
}

void GD9Device::BeginMirrorRender()
{
    mD9Device->Clear ( 0, 0, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x000000000, 1.0f, 0 );

    mD9Device->SetRenderState ( D3DRS_STENCILENABLE, true );

    mD9Device->SetRenderState ( D3DRS_STENCILREF, 0x1 );

    mD9Device->SetRenderState ( D3DRS_STENCILWRITEMASK, 0xffffffff );

}

void GD9Device::OpenUIAlpha ( bool bTransparent )
{
    //����Alpha���

    mD9Device->SetRenderState ( D3DRS_ALPHABLENDENABLE, bTransparent );

    if ( bTransparent )
    {

        mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
        mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );


        //����Alpha��Ϸ����ͻ��ϵ��

        mD9Device->SetRenderState ( D3DRS_BLENDOP, D3DBLENDOP_ADD ); //Ĭ������
        mD9Device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        mD9Device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    }
}

bool GD9Device::VertexShaderSurport() const
{
    return mCaps.VertexShaderVersion >= D3DVS_VERSION ( 2, 0 );
}

int GD9Device::GetScreenWidth() const
{
    return mWidth;
}

int GD9Device::GetScreenHeight() const
{
    return mHeight;
}

void GD9Device::OnDeviceLost()
{
    mOnLostDevice.trigger();
}

void GD9Device::OnResize ( int w, int h )
{
    if ( mD9Device != nullptr )
    {
        if ( w > 0 && h > 0 )
        {
            mOnLostDevice.trigger();
            ResetDevice ( w, h );
            mOnResetDevice.trigger();
        }
    }
}

bool GD9Device::ResetDevice ( int w, int h )
{
    if ( mHwnd == 0 )
    {
        return false;
    }
    //������
    HRESULT hr = 0;

    //Ĭ���Կ�����ʾģʽ
    D3DDISPLAYMODE d9DisplayMode;

    //�豸���ܲ���

    //���ز���֧�ֶ�
    DWORD dwMultiSampleQulity = 0;

    //���㴦��ʽ
    DWORD dwBehavior;

    if ( !mD3D9 )
    {
        mD3D9 = Direct3DCreate9 ( D3D_SDK_VERSION );

        CXASSERT_RETURN_FALSE ( mD3D9 );
    }

    //����豸��ʾģʽ
    mD3D9->GetAdapterDisplayMode ( D3DADAPTER_DEFAULT, &d9DisplayMode );

    //D3DVS_VERSION()
    //����豸���ܣ��Ƿ�֧��Ӳ�����㴦��͹���
    //����豸֧�ֶ���ת���͹��գ���Ӳ���������������ģ��
    mD3D9->GetDeviceCaps ( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &mCaps );

    if ( mCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
        dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    //�����ز���֧�ֶ�
    mD3D9->CheckDeviceMultiSampleType (
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d9DisplayMode.Format,
        TRUE, D3DMULTISAMPLE_8_SAMPLES, &dwMultiSampleQulity
    );
    mWidth = w;
    mHeight = h;
    D3DPRESENT_PARAMETERS d3dp;
    dMemoryZero ( &d3dp, sizeof ( D3DPRESENT_PARAMETERS ) );

    d3dp.AutoDepthStencilFormat = D3DFMT_D24S8;	//��Ȼ���24λ��ģ�建��8λ
    d3dp.BackBufferCount = 1;						//��̨������һ��
    d3dp.BackBufferFormat = d9DisplayMode.Format;	//��̨���������ظ�ʽ��Ӧ�ú�ǰ̨��ͬ��
    d3dp.BackBufferHeight = mHeight;				//��̨�������߶�
    d3dp.BackBufferWidth = mWidth;				//��̨���������
    d3dp.EnableAutoDepthStencil = TRUE;			//����ģ�建�����Ȼ���
    d3dp.Flags = 0;								//Ĭ��Ϊ0
    d3dp.FullScreen_RefreshRateInHz = 0;			//������ʾʱ��������Ϊ0
    d3dp.hDeviceWindow = mHwnd;
    d3dp.MultiSampleQuality = dwMultiSampleQulity - 1;
    d3dp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;		//SwapEffect��ΪD3DSWAPEFFECT_DISCARDʱ���ò�����������ΪD3DMULTISAMPLE_NONE
    d3dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    //d3dp.SwapEffect=D3DSWAPEFFECT_COPY;
    d3dp.Windowed = TRUE;

    if ( mD9Device )
    {
        CXASSERT_RESULT_FALSE ( mD9Device->Reset ( &d3dp ) );
    }
    else
    {
        hr = mD3D9->CreateDevice (
                 D3DADAPTER_DEFAULT,
                 //D3DDEVTYPE_REF,
                 D3DDEVTYPE_HAL,
                 mHwnd,
                 dwBehavior | D3DCREATE_MULTITHREADED,
                 &d3dp,
                 &mD9Device
             );

        DWORD dwi = GetLastError();

        if ( FAILED ( hr ) )
        {
            return false;
        }
    }
    ResetRenderState();
    SetLight();
    return true;
}

void GD9Device::setBackColor ( DWORD color )
{
    mBackColor = color;
}

bool GD9Device::beginRenderUI()
{
    mD9Device->SetRenderState ( D3DRS_LIGHTING, FALSE );
    mD9Device->SetRenderState ( D3DRS_FOGENABLE, FALSE );
    mD9Device->SetRenderState ( D3DRS_ZENABLE, D3DZB_FALSE );
    mD9Device->SetRenderState ( D3DRS_ALPHATESTENABLE, FALSE );
    mD9Device->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
    mD9Device->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );
    mD9Device->SetRenderState ( D3DRS_SCISSORTESTENABLE, TRUE );
    mD9Device->SetRenderState ( D3DRS_ZWRITEENABLE, FALSE );

    // setup texture addressing settings
    mD9Device->SetSamplerState ( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    mD9Device->SetSamplerState ( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

    // setup colour calculations
    mD9Device->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    //Content::Device.SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );

    // setup alpha calculations
    mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    //Content::Device.SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

    // setup filtering
    mD9Device->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    mD9Device->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    // disable texture stages we do not need.
    mD9Device->SetTextureStageState ( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );

    // setup scene alpha blending
    mD9Device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );

    return true;
}

void GD9Device::renderFirstGraph ( bool useTexture )
{
    //Content::Device.SetRenderState ( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
    //Content::Device.SetSamplerState ( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
    //Content::Device.SetSamplerState ( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

    mD9Device->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );

    mD9Device->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_COLOROP, useTexture ? D3DTOP_MODULATE : D3DTOP_SELECTARG2 );

    mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    mD9Device->SetTextureStageState ( 0, D3DTSS_ALPHAOP, useTexture ? D3DTOP_MODULATE : D3DTOP_SELECTARG2 );
}

DWORD GD9Device::getBackColor()
{
    return mBackColor;
}





#pragma once


typedef D3DXHANDLE(*SetPara)(ID3DXEffect *);

D3DXHANDLE SetEffectParaments(ID3DXEffect *pEffect);

class GEffect
{
public:
	GEffect(void);
	~GEffect(void);
	bool createFromFile ( const char* name );

	inline ID3DXEffect* getTexture()
	{
		return mD3DEffect;
	}
public:
	GString			FileName;
protected:
	D3DXHANDLE SetEffectPara(SetPara FunSetPara);

public:

	ID3DXEffect *mD3DEffect;

	static ID3DXEffectPool *mEffectPool;

	D3DXHANDLE m_hParaBlock;

	D3DXHANDLE m_hWorldViewProj;

	D3DXHANDLE m_Tech;

	D3DXHANDLE m_hUseMaterialOnly;

	D3DXHANDLE m_hTexture;

	D3DXHANDLE m_hMtrlAmbient;

	D3DXHANDLE m_hMtrlDiffuse;

	D3DXHANDLE m_hWorld;

	D3DXHANDLE m_hOpenLight;


	IDirect3DVertexDeclaration9 *m_pVDel;

public:

};

extern GEffect gEffect;
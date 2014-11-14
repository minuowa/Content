#pragma once
class GMatrix
{
public:
    GMatrix();
    inline GMatrix ( D3DXMATRIX& rhs );
    inline GMatrix operator* ( GMatrix& rhs );
    inline GMatrix& operator= ( D3DXMATRIX& rhs );
    inline void product ( GMatrix& out, const GMatrix& rhs ) const;
public:
    D3DXVECTOR3 mRight;
    float _14;
    D3DXVECTOR3 mUpon;
    float _24;
    D3DXVECTOR3 mDir;
    float _34;
    D3DXVECTOR3 mTranslate;
    float _44;
};

inline GMatrix::GMatrix ( D3DXMATRIX& rhs )
{
    dMemoryCopy ( this, &rhs, sizeof ( *this ) );
}

inline GMatrix GMatrix::operator* ( GMatrix& rhs )
{
    D3DXMATRIX matT;
    D3DXMatrixMultiply ( &matT, ( D3DXMATRIX* ) this, ( D3DXMATRIX* ) &rhs );
    return GMatrix ( matT );
}
inline void GMatrix::product ( GMatrix& out, const GMatrix& rhs ) const
{
    D3DXMatrixMultiply ( ( D3DXMATRIX* ) &out, ( D3DXMATRIX* ) this, ( const D3DXMATRIX* ) &rhs );
}

inline GMatrix& GMatrix::operator= ( D3DXMATRIX& rhs )
{
    dMemoryCopy ( this, &rhs, sizeof ( *this ) );
    return *this;
}

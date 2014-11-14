#include "GGameDemoHeader.h"
#include "GMatrix.h"


GMatrix::GMatrix ( void )
    : mRight ( 1, 0, 0 )
    , mUpon ( 0, 1, 0 )
    , mDir ( 0, 0, 1 )
    , mTranslate ( 0, 0, 0 )
    , _14 ( 0 )
    , _24 ( 0 )
    , _34 ( 0 )
    , _44 ( 1 )
{
}




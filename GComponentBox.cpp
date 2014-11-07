#include "GGameDemoHeader.h"
#include "GComponentBox.h"


GComponentBox::GComponentBox ( void )
	: mXDim ( 1 )
	, mYDim ( 1 )
	, mZDim ( 1 )
{
}


GComponentBox::~GComponentBox ( void )
{
}

void GComponentBox::registerAllProperty()
{
	__RegisterProperty ( mXDim );
	__RegisterProperty ( mYDim );
	__RegisterProperty ( mZDim );
}

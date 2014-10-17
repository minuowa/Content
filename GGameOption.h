#ifndef GGameOption_h__
#define GGameOption_h__
#include "GObject.h"
#include "GEnums.h"
//-------------------------------------------------------------------------
/**
	@brief 
	@author nnboy,14/10/2014  11:50
*/
class GGameOption:public GObject
{
public:
	GGameOption(void);
	~GGameOption(void);

	virtual void registerAllProperty();

	virtual void onPropertyChangeEnd( void* cur );

	void updateRenderState();

protected:
	FillMode mFillMode;
};

typedef CXSingleton<GGameOption> GGameOptionSingleton;

#define GameOption GGameOptionSingleton::GetSingletonPtr() 

#endif // GGameOption_h__

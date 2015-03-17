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

	bool init();

	virtual void registerAllProperty();

	virtual void onPropertyChangeEnd( void* cur );

	void updateRenderState();

	virtual void onCallBack ( const CXDelegate& , CXEventArgs*  )override;

	void destroy();

protected:
	eFillMode FillMode;
	bool RenderBloom;
};

#endif // GGameOption_h__

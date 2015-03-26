#pragma once
class GPlugin: public CXCallBack
{
public:
    GPlugin ( void );
    ~GPlugin ( void );
    virtual const char* getPluginName() = 0;
    virtual GPlugin* createPlugin() = 0;
};

#define DeclarePlugin(className) \
	virtual const char* getPluginName()\
	{\
		return #className;\
	}\
	virtual GPlugin* createPlugin()\
	{\
		return new className();\
	}

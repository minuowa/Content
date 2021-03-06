#pragma once
#include "GObject.h"
#include "GFactory.h"
enum eComponentType
{
	eComponentType_Trans,
	eComponentType_Mesh,
	eComponentType_Ani,
	eComponentType_Effect,
	eComponentType_Script,
	eComponentType_Box,
	eComponentType_Particles,
	eComponentType_Count,
};
class  GNode;
class GComponentInterface :
	public GObject
{
public:
	GComponentInterface ( void );
	virtual ~GComponentInterface ( void );
	void setOwner(GNode* target);
	GNode* getOwner() const;
protected:
	eComponentType	mComponentType;
	GNode*	mOwner;
	bool	mCanDetach;
public:
	inline eComponentType getType()
	{
		return mComponentType;
	}
	inline const char* getComponentName()
	{
		return categoryName();
	}
	inline bool canDetach() const
	{
		return mCanDetach;
	}
	inline void setCanDetach ( bool can )
	{
		mCanDetach = can;
	}
};
template<eComponentType TYPE>
class GComponentBase: public GComponentInterface
{
public:
	enum{ComponentType=TYPE,};
	virtual void set()
	{
	};
	GComponentBase()
	{
		mComponentType = TYPE;
	}
};


class GComponentOwner
{
public:
	GComponentOwner();
	~GComponentOwner();

	GComponentInterface* getComponent ( const char* name ) const;
	GComponentInterface* getComponent ( eComponentType type ) const;

	GComponentInterface* attachComponent ( eComponentType type);
	GComponentInterface* attachComponent ( const char* name);

	void detachComponent ( const char* name );
	void detachComponent ( eComponentType type );
protected:

	GComponentInterface* mCompoents[eComponentType_Count];
};



#define DeclareComponentCreator(className) \
	 public:\
	 static GComponentInterface* createComponent() \
{\
	return new className;\
}

#define DeclareComponentType(className)\
	DeclareCategoryName(className)\
	DeclareComponentCreator(className)


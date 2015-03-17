#pragma once
#include "GComponentInterface.h"

class GComponentFactory
{
public:
	typedef GComponentInterface* ( *ObjCrator ) ();

	struct ComponentCreator
	{
		int mType;
		ObjCrator mCreator;
	};

	typedef CXMap<GString, ComponentCreator*> ComponentCreatorMap;
	~GComponentFactory();
public:
	void registerCreator ( const char* typeName, int type, ObjCrator creator );
	GComponentInterface* create ( const char* typeName );
	ComponentCreator* getCreator ( const char* typeName ) const;
	ComponentCreator* getCreator ( eComponentType type ) const;

private:
	ComponentCreatorMap mComponentCreatorMap;
public:
	const ComponentCreatorMap& getCreators() const
	{
		return mComponentCreatorMap;
	}
};


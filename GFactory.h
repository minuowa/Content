#pragma once
template<typename T>
class GFactory
{
public:
	typedef T* ( *ObjCrator ) ();
	typedef CXMap<GString, ObjCrator> ObjCreatorMap;

	//GFactory ( void );
	virtual   ~GFactory ( void )
	{
	}
	T*	create ( const char* typeName )
	{
		ObjCrator creator = getCreator ( typeName );
		CXASSERT_RETURN_FALSE ( creator );
		return creator();
	}
	void registerCreator ( const char* typeName, ObjCrator creator )
	{
		if ( mCreatorMap.find ( typeName ) == mCreatorMap.end() )
		{
			mCreatorMap.Insert ( typeName, creator );
		}
		else
		{
			CXASSERT ( 0 );
		}
	}
	ObjCrator getCreator ( const char* typeName )
	{
		ObjCrator creator = 0;
		CXASSERT_RETURN_FALSE ( mCreatorMap.Get ( typeName, creator ) );
		return creator;
	}
	const ObjCreatorMap& getCreators() const
	{
		return mCreatorMap;
	}
protected:
	ObjCreatorMap	mCreatorMap;
};

#define __RegisterCreator(className)\
	registerCreator(#className,className::CreateNode);


#define DeclareNodeCreator(className) \
	public:\
	static GNode* CreateNode() \
{\
	return new className();\
}

#define DeclareEditorType(className) \
	DeclareCategoryName(className)\
	DeclareNodeCreator(className)\


#pragma once
#include "gcomponentinterface.h"
class GComponentMesh :
	public GComponentBase<eComponentType_Mesh>
{
	DeclareComponentType(GComponentMesh);
public:
	GComponentMesh(void);
	~GComponentMesh(void);

	virtual void registerAllProperty();
	virtual void onPropertyChange(void* pre,void* changed) override;
protected:
	uString mMeshFile;
public:
	inline const uString& meshFile() const 
	{
		return mMeshFile; 
	}
	inline void meshFile(const char* val) 
	{ 
		mMeshFile = val;
		registerAll();
	}
};


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
	virtual void OnPropertyChange(void* pre,void* changed);
protected:
	GString mMeshFile;
public:
	inline const GString& MeshFile() const 
	{
		return mMeshFile; 
	}
	inline void MeshFile(const char* val) 
	{ 
		mMeshFile = val;
		registerAll();
	}
};


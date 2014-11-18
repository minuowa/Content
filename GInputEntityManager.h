#pragma once
class GNode;
class GInputEntityManager
{
public:
	GInputEntityManager(void);
	~GInputEntityManager(void);

	void addInputObj(GNode* node);
	void getInput(u32 frameTimeMS);
	void remove(u32 objID);
private:
	CXDynaArray<u32> mInputObjIDList;
	CXDynaArray<u32> mFreeIDList;
};

#define InputEntityMgr CXSingleton<GInputEntityManager>::getInstance()

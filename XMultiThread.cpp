#include "GGameDemoHeader.h"
#include "XMultiThread.h"
#include <winbase.h>

CXMultiThread::CXMultiThread(void)
{
	mGHandle=NULL;

	mdwThredId=0;
}

CXMultiThread::~CXMultiThread(void)
{
	if (mGHandle!=NULL)
	{
		CloseHandle(mGHandle);
	}

	mGHandle=NULL;

	mdwThredId=0;
}


bool CXMultiThread::Init( LPTHREAD_START_ROUTINE MainFun,LPVOID pPara,bool bRunWhenCreate )
{
	GHandle GHandle=NULL;

	if (bRunWhenCreate)
	{
		GHandle=CreateThread(NULL,NULL,MainFun,pPara,0,&mdwThredId);

	}
	else
	{
		GHandle=CreateThread(NULL,NULL,MainFun,pPara,CREATE_SUSPENDED,&mdwThredId);
	}

	if (GHandle!=NULL)
	{
		mGHandle=GHandle;

		return true;
	}

	return false;

}

void CXMultiThread::Resume()
{
	if (mGHandle!=NULL)
	{
		ResumeThread(mGHandle);
	}
}

void CXMultiThread::Suspend()
{
	if (mGHandle!=NULL)
	{
		SuspendThread(mGHandle);
	}
}

void CXMultiThread::Terminate()
{
	if (mGHandle!=NULL)
	{
		TerminateThread(mGHandle,1);
	}

}

//bool CXMultiThread::Init( UINT WINAPI MainFun,LPVOID pPara,bool bRunWhenCreate )
//{
//	_beginthreadex(NULL,NULL,&MainFun,pPara,0,&mdwThredId);
//}

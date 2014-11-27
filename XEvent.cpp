#include "GGameDemoHeader.h"
#include "XEvent.h"

GXEvent gEvent;

GXEvent::GXEvent(void)
{
	mGHandle=NULL;
}

GXEvent::~GXEvent(void)
{
}



GHandle GXEvent::Create( char *sEventName )
{
	GHandle GHandle=OpenEventA(EVENT_ALL_ACCESS,FALSE,sEventName);

	if (GHandle==NULL)
	{
		GHandle=CreateEventA(NULL,TRUE,TRUE,sEventName);
	}

	if (GHandle!=NULL)
	{
		mGHandle=GHandle;
	}

	return mGHandle;
}

void GXEvent::SetUsed()
{
	if (mGHandle!=NULL)
	{
		ResetEvent(mGHandle);
	}
}

void GXEvent::SetNoUse()
{
	if (mGHandle!=NULL)
	{
		SetEvent(mGHandle);
	}
}

void GXEvent::WaitForUse( DWORD dwWaitTime )
{
	if (mGHandle!=NULL)
	{
		WaitForSingleObject(mGHandle,dwWaitTime);
	}
}

#pragma once



class GXEvent
{
public:
	GXEvent ( void );

	~GXEvent ( void );

public:

	GHandle Create ( char *sEventName );

	void SetUsed();

	void SetNoUse();

	void WaitForUse ( DWORD dwWaitTime );

	GHandle mGHandle;
};

extern GXEvent gEvent;
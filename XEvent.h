#pragma once



class CXEvent
{
public:
	CXEvent ( void );

	~CXEvent ( void );

public:

	GHandle Create ( char *sEventName );

	void SetUsed();

	void SetNoUse();

	void WaitForUse ( DWORD dwWaitTime );

	GHandle mGHandle;
};

extern CXEvent gEvent;